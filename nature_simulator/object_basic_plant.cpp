
#include "object_basic_plant.h"

#include "helper_functions.h"

bool basic_plant_reproduce(game_state* game, int x, int y, object* current_object) 
{

    int new_x, new_y;
    get_random_surrounding_xy(game, x, y, &new_x, &new_y);

    if (is_free_xy(game, new_x, new_y)) {

        object NEW_SIZE = 0;
        object NEW_COLOR = GET_BASIC_PLANT_COLOR_BITS(*current_object);
        object NEW_AGE = 0;

        // Mutating our current color to make the new one
        const object max_mutation = 10;
        object mutation_intensity = rand() % max_mutation;

        object increment = mutation_intensity << BASIC_PLANT_COLOR_BITS_PADDING;
        if (rand() % 2 == 0) {
            increment = -increment;
        }

        /* If we get 0 every shift will be 0 and we will edit red,
           if we get 1 every shift will be 8 and we will edit green... */
        int red_green_or_blue = rand() % 3;
        mutation_intensity *= ((NEW_COLOR >> BASIC_PLANT_COLOR_BITS_PADDING >> (8 * red_green_or_blue)) & 0xFF) - 0xFF;
        NEW_COLOR += increment << (8 * red_green_or_blue);

        if ((NEW_COLOR & (~BASIC_PLANT_COLOR_BITS)) != 0) {
            NEW_COLOR = *current_object & BASIC_PLANT_COLOR_BITS;
        }

        // Determining the lifespan of a plant based on its color
        NEW_AGE = ((object)((double)mutation_intensity / (double)max_mutation * (double)(BASIC_PLANT_AGE_MAX >> BASIC_PLANT_AGE_BITS_PADDING))) << BASIC_PLANT_AGE_BITS_PADDING;

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

void basic_plant_live(game_state* game, size_t memindex)
{
    object* current_object = &(game->board[memindex]);

    int x, y;
    memindex_to_xy(memindex, &x, &y);

    if (GET_BASIC_PLANT_AGE(*current_object) == BASIC_PLANT_AGE_MAX) {
        if (rand() % BASIC_PLANT_AGE_CHANCE_TO_DIE_AT_MAX == 0) {
            *current_object = EMPTY_BOARD_SLOT;
            return;
        }
    } else {
        if (rand() % BASIC_PLANT_AGE_CHANCE_TO_DIE_AT_ANY == 0) {
            *current_object = EMPTY_BOARD_SLOT;
            return;
        }
    }

    // Spread and create new plants
    size_t plant_size = GET_BASIC_PLANT_SIZE(*current_object);
    if (plant_size >= 2) {
        if (basic_plant_reproduce(game, x, y, current_object)) {
            SET_BASIC_PLANT_SIZE(*current_object, GET_BASIC_PLANT_SIZE(*current_object) - 3);
        }
    }

    INC_BASIC_PLANT_SIZE(*current_object);
    INC_BASIC_PLANT_AGE(*current_object);

    *current_object |= MOVED;
}