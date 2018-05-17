
#include "game_box_render.h"

#include "game_box_render_drawing.h"
#include "image_handling.h"
#include "all_objects.h"
#include "helper_functions.h"
#include <memory.h>

#include <Windows.h>

// Window size and drawing memory
int game_width;
int game_height;

int game_box_width;
int game_box_height;

int game_box_width_margin;
int game_box_height_margin;

void* game_box_rgb_buffer;

// Resources and resource values
int one_box_dimensions;

rgb_square_picture basic_herbivore_0;
rgb_square_picture basic_herbivore_1;

rgb_square_picture basic_plant_0;
rgb_square_picture basic_plant_1;
rgb_square_picture basic_plant_2;
rgb_square_picture basic_plant_3;
rgb_square_picture basic_plant_4;

rgb_square_picture impassable_mountain;

rgb_square_picture natural_disaster;

// Local functions only we need to call
void load_resources(void);

void resize_box(screen_buffer* screen_buffer);
void draw_game(game_state* game);

//-------------------------------------------------------------------------
void draw_box(screen_buffer* screen_buffer, game_state* game, bool resized)
{
    /*
        *
        * We create or update our global vaiables, we allocate memory, etc.
        *
    */

    if (resized) {
        // Update global variables...
        resize_box(screen_buffer);

        // Load all resources as rgb_images and resize them 
        load_resources();
    }
    

    // We clean out the previos buffer data
    memset(game_box_rgb_buffer, 0, game_box_width*game_box_height*sizeof(int));


    /*
        *
        * We render the game out
        *
    */

    draw_game(game);


    /*
        *
        * We clean up memory, we copy any image buffers...
        *   We did the work, now we reap the prize!
        *
    */

    // Copying game buffer back into main buffer
    int copy_from_here = 0;
    int copy_here = (game_box_height_margin / 2) * screen_buffer->width +
                            (game_box_width_margin / 2);
    
    for (int i = 0; i < game_box_height; i++) {
        memcpy_s(&(((int*)screen_buffer->memory)[copy_here]), 
                                               (game_box_width + 1) * sizeof(int),
                 &(((int*)game_box_rgb_buffer)[copy_from_here]), 
                                                game_box_width * sizeof(int));
        
        copy_from_here += game_box_width;
        copy_here += screen_buffer->width;
    }
}

void draw_game(game_state* game)
{
    int x = 0;
    int y = BOXES_COUNT_HEIGHT - 1;
    
    for (size_t i = 0; i < BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT; i++) {
        if (game->board[i] != 0) {
            // There is something on this piece

            memindex_to_xy(i, &x, &y);

            switch (GET_ID_BITS(game->board[i])) {
                case (IMPASSABLE_MOUNTAIN_ID):
                {
                    draw_icon(&impassable_mountain, x, y);
                } break;

                case (NATURAL_DISASTER_ID):
                {
                    draw_icon(&natural_disaster, x, y);
                } break;

                case (BASIC_PLANT_ID):
                {
                    size_t plant_size = game->board[i] & BASIC_PLANT_SIZE_BITS;

                    rgb_square_picture picture;

                    if (plant_size == BASIC_PLANT_SIZE_4) {
                        picture.rgb_memory_size = basic_plant_4.rgb_memory_size;
                        
                        picture.rgb_memory = new int32_t[basic_plant_4.rgb_memory_size];
                        memcpy(picture.rgb_memory, basic_plant_4.rgb_memory, basic_plant_4.rgb_memory_size * 4);

                    } else if (plant_size == BASIC_PLANT_SIZE_3) {
                        picture.rgb_memory_size = basic_plant_3.rgb_memory_size;
                        
                        picture.rgb_memory = new int32_t[basic_plant_3.rgb_memory_size];
                        memcpy(picture.rgb_memory, basic_plant_3.rgb_memory, basic_plant_3.rgb_memory_size * 4);

                    } else if (plant_size == BASIC_PLANT_SIZE_2) {
                        picture.rgb_memory_size = basic_plant_2.rgb_memory_size;
                        
                        picture.rgb_memory = new int32_t[basic_plant_2.rgb_memory_size];
                        memcpy(picture.rgb_memory, basic_plant_2.rgb_memory, basic_plant_2.rgb_memory_size * 4);

                    } else if (plant_size == BASIC_PLANT_SIZE_1) {
                        picture.rgb_memory_size = basic_plant_1.rgb_memory_size;

                        picture.rgb_memory = new int32_t[basic_plant_1.rgb_memory_size];
                        memcpy(picture.rgb_memory, basic_plant_1.rgb_memory, basic_plant_1.rgb_memory_size * 4);

                    } else {
                        picture.rgb_memory_size = basic_plant_0.rgb_memory_size;
                        
                        picture.rgb_memory = new int32_t[basic_plant_0.rgb_memory_size];
                        memcpy(picture.rgb_memory, basic_plant_0.rgb_memory, basic_plant_0.rgb_memory_size * 4);

                    }

                    int32_t desired_color = (int32_t)(game->board[i] >> BASIC_PLANT_COLOR_BITS_PADDING);

                    for (size_t j = 0; j < picture.rgb_memory_size; j++) {
                        if ((((int32_t*)picture.rgb_memory)[j] & 0xFFFFFF) == 0xFFFFFF) {
                            ((int32_t*)picture.rgb_memory)[j] = desired_color;
                        }
                    }

                    draw_icon(&picture, x, y);

                    delete picture.rgb_memory;
                } break;

                case (BASIC_HERBIVORE_ID):
                {
                    if (game->board[i] & BASIC_HERBIVORE_SIZE_1) {
                        draw_icon(&basic_herbivore_1, x, y);
                    } else {
                        draw_icon(&basic_herbivore_0, x, y);
                    }
                } break;
                
                default:
                {
                    // We don't know anything about this piece, as
                    // far as we are concerned, this is a rock
                } break;
            }

        }

    }

}

void resize_box(screen_buffer* screen_buffer)
{
    // Game width and height after removing side panel
    game_width = screen_buffer->width * GAME_BOX_SCREEN_PERCENT / 100;
    game_height = screen_buffer->height;

    // Game box width and height after subtracting margins
    game_box_width_margin = game_width % BOXES_COUNT_WIDTH;
    game_box_height_margin = game_height % BOXES_COUNT_HEIGHT;

    game_box_width = game_width - game_box_width_margin;
    game_box_height = game_height - game_box_height_margin;

    // Since we want every icon to be a box, we need to make sure that
    //    game box width and height are the same multiple of boxes count
    int multiple_width = game_box_width / BOXES_COUNT_WIDTH;
    int multiple_height = game_box_height / BOXES_COUNT_HEIGHT;

    if (multiple_width > multiple_height) {
        game_box_width -= BOXES_COUNT_WIDTH * (multiple_width - multiple_height);
    } else if (multiple_width < multiple_height) {
        game_box_height -= BOXES_COUNT_HEIGHT * (multiple_height - multiple_width);
    }

    game_box_width_margin = game_width - game_box_width;
    game_box_height_margin = game_height - game_box_height;

    // We calculate box (width and height are the same) dimensions
    one_box_dimensions = game_box_width / BOXES_COUNT_WIDTH;

    // We allocate the memory for the game box drawing
    delete game_box_rgb_buffer;
    game_box_rgb_buffer = new int[game_box_width * game_box_height]();

}

void load_resources(void)
{
    LPSTR temp_path = new char[256];
    GetCurrentDirectory(256, temp_path);

    std::string base_path = temp_path;
    base_path += "\\icons\\";

    //
    delete impassable_mountain.rgb_memory;

    impassable_mountain.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    impassable_mountain.rgb_memory = new int[impassable_mountain.rgb_memory_size];

    std::string impassable_mountain_icon_path = base_path + IMPASSABLE_MOUNTAIN_PICTURE;
    png_load(impassable_mountain_icon_path, &impassable_mountain, one_box_dimensions);

    //
    delete natural_disaster.rgb_memory;

    natural_disaster.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    natural_disaster.rgb_memory = new int[natural_disaster.rgb_memory_size];

    std::string natural_disaster_icon_path = base_path + NATURAL_DISASTER_PICTURE;
    png_load(natural_disaster_icon_path, &natural_disaster, one_box_dimensions);

    //
    delete basic_plant_0.rgb_memory;

    basic_plant_0.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_plant_0.rgb_memory = new int[basic_plant_0.rgb_memory_size];

    std::string basic_plant_0_icon_path = base_path + BASIC_PLANT_PICTURE;
    png_load(basic_plant_0_icon_path, &basic_plant_0, one_box_dimensions);

    png_resize(&basic_plant_0, 20);

    //
    delete basic_plant_1.rgb_memory;

    basic_plant_1.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_plant_1.rgb_memory = new int[basic_plant_1.rgb_memory_size];

    std::string basic_plant_1_icon_path = base_path + BASIC_PLANT_PICTURE;
    png_load(basic_plant_1_icon_path, &basic_plant_1, one_box_dimensions);

    png_resize(&basic_plant_1, 40);

    //
    delete basic_plant_2.rgb_memory;

    basic_plant_2.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_plant_2.rgb_memory = new int[basic_plant_2.rgb_memory_size];

    std::string basic_plant_2_icon_path = base_path + BASIC_PLANT_PICTURE;
    png_load(basic_plant_2_icon_path, &basic_plant_2, one_box_dimensions);

    png_resize(&basic_plant_2, 60);

    //
    delete basic_plant_3.rgb_memory;

    basic_plant_3.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_plant_3.rgb_memory = new int[basic_plant_3.rgb_memory_size];

    std::string basic_plant_3_icon_path = base_path + BASIC_PLANT_PICTURE;
    png_load(basic_plant_3_icon_path, &basic_plant_3, one_box_dimensions);

    png_resize(&basic_plant_3, 80);

    //
    delete basic_plant_4.rgb_memory;

    basic_plant_4.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_plant_4.rgb_memory = new int[basic_plant_4.rgb_memory_size];

    std::string basic_plant_4_icon_path = base_path + BASIC_PLANT_PICTURE;
    png_load(basic_plant_4_icon_path, &basic_plant_4, one_box_dimensions);

    //
    delete basic_herbivore_0.rgb_memory;

    basic_herbivore_0.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_herbivore_0.rgb_memory = new int[basic_herbivore_0.rgb_memory_size];

    std::string basic_herbivore_0_icon_path = base_path + BASIC_HERBIVORE_PICTURE;
    png_load(basic_herbivore_0_icon_path, &basic_herbivore_0, one_box_dimensions);

    png_resize(&basic_herbivore_0, 50);

    //
    delete basic_herbivore_1.rgb_memory;

    basic_herbivore_1.rgb_memory_size = one_box_dimensions * one_box_dimensions;
    basic_herbivore_1.rgb_memory = new int[basic_herbivore_1.rgb_memory_size];

    std::string basic_herbivore_1_icon_path = base_path + BASIC_HERBIVORE_PICTURE;
    png_load(basic_herbivore_1_icon_path, &basic_herbivore_1, one_box_dimensions);

}
