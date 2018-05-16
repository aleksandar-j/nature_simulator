
#include "object_basic_plant.h"

#include "helper_functions.h"

// 2 BYTES will be used like this
// 0000 0000 - 0000 0111     (1 - already used for something)

// First 3 bits - size of the plant

bool basic_plant_procreate(game_state* game, int x, int y, object* current_object) 
{

    int new_x, new_y;
    get_random_surrounding_xy(game, x, y, &new_x, &new_y);

    if (is_free_xy(game, new_x, new_y)) {

        int64_t NEW_SIZE = BASIC_PLANT_SIZE_0;
        int64_t NEW_COLOR = *current_object & BASIC_PLANT_COLOR_BITS;
        int64_t NEW_AGE = 0;

        // Mutating our current color to make the new one
        const int64_t max_mutation = 10;
        int64_t mutation_intensity = rand() % max_mutation;

        int64_t increment = mutation_intensity << BASIC_PLANT_COLOR_BITS_PADDING;
        if (rand() % 2) {
            increment = -increment;
        }

        int red_green_or_blue = rand() % 3;
        if (red_green_or_blue == 0) {
            mutation_intensity *= ((NEW_COLOR >> BASIC_PLANT_COLOR_BITS_PADDING) & 0xFF) - 0xFF;
            NEW_COLOR += increment;
        } else if (red_green_or_blue == 1) {
            mutation_intensity *= ((NEW_COLOR >> BASIC_PLANT_COLOR_BITS_PADDING >> 8) & 0xFF) - 0xFF;
            NEW_COLOR += increment << 8;
        } else {
            mutation_intensity *= ((NEW_COLOR >> BASIC_PLANT_COLOR_BITS_PADDING >> 16) & 0xFF) - 0xFF;
            NEW_COLOR += increment << 16;
        }

        if ((NEW_COLOR & (~BASIC_PLANT_COLOR_BITS)) != 0) {
            NEW_COLOR = *current_object & BASIC_PLANT_COLOR_BITS;
        }

        // Determining the lifespan of a plant based on its color
        NEW_AGE = ((int64_t)((double)mutation_intensity / (double)max_mutation * (double)(BASIC_PLANT_AGE_MAX >> BASIC_PLANT_AGE_BITS_PADDING))) << BASIC_PLANT_AGE_BITS_PADDING;

        if (NEW_AGE >= BASIC_PLANT_AGE_MAX) {
            return false;
        }

        // Placing the plant
        place_id_xy(game, new_x, new_y,
            BASIC_PLANT_ID | NEW_SIZE | NEW_COLOR | NEW_AGE | MOVED);

        return true;
    }

    return false;

}

void basic_plant_live(game_state* game, size_t game_board_index)
{
    int x, y;
    memindex_to_xy(game_board_index, &x, &y);

    object* current_object = &(game->board[game_board_index]);

    if ((*current_object & BASIC_PLANT_AGE_BITS) == BASIC_PLANT_AGE_MAX) {
        *current_object = EMPTY_BOARD_SLOT;
        return;
    } else {
        *current_object += 1LL << BASIC_PLANT_AGE_BITS_PADDING;
    }

    size_t plant_size = *current_object & BASIC_PLANT_SIZE_BITS;
    if (plant_size >= BASIC_PLANT_SIZE_2) {
        if (basic_plant_procreate(game, x, y, current_object)) {
            *current_object -= BASIC_PLANT_SIZE_1 * 2;
        } else if (plant_size < BASIC_PLANT_SIZE_4) {
            *current_object += BASIC_PLANT_SIZE_1;
        }
    } else {
        *current_object += BASIC_PLANT_SIZE_1;
    }
}