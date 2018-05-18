
#include "object_basic_herbivore.h"

#include "object_basic_plant.h"
#include "helper_functions.h"

void basic_herbivore_live(game_state* game, size_t memindex)
{
    object* current_object = &(game->board[memindex]);

    int x, y; 
    memindex_to_xy(memindex, &x, &y);

    int new_x = x, new_y = y;

    // Check whether we should die
    if (GET_BASIC_HERBIVORE_AGE(*current_object) == BASIC_HERBIVORE_AGE_MAX) {
        if (rand() % BASIC_HERBIVORE_AGE_CHANCE_TO_DIE_AT_MAX == 0) {
            *current_object = EMPTY_BOARD_SLOT;
            return;
        }
    } else {
        if (rand() % BASIC_HERBIVORE_AGE_CHANCE_TO_DIE_AT_ANY == 0) {
            *current_object = EMPTY_BOARD_SLOT;
            return;
        }
    }

    // Try to eat something
    bool eaten = false;

    if (get_random_surrounding_object_with_id_xy(game, x, y, &new_x, &new_y, BASIC_PLANT_ID)) {
        move_xy(game, x, y, new_x, new_y);
        x = new_x; y = new_y; memindex = xy_to_memindex(x, y); current_object = &(game->board[memindex]);
        eaten = true;
    } else if (get_random_surrounding_object_with_id_xy(game, x, y, &new_x, &new_y, EMPTY_BOARD_SLOT)) {
        move_xy(game, x, y, new_x, new_y);
        x = new_x; y = new_y; memindex = xy_to_memindex(x, y); current_object = &(game->board[memindex]);
    }

    // Try to produce offspring
    if (GET_BASIC_HERBIVORE_AGE(*current_object) >= BASIC_HERBIVORE_PROCREATE_MIN_AGE && 
        GET_BASIC_HERBIVORE_HUNGER(*current_object) <= BASIC_HERBIVORE_PROCREATE_MAX_HUNGER)
    { 
        if (rand() % BASIC_HERBIVORE_PROCREATE_CHANCE == 0) {
            if (get_random_surrounding_object_with_id_xy(game, x, y, &new_x, &new_y, EMPTY_BOARD_SLOT)) {
                place_id_xy(game, new_x, new_y, BASIC_HERBIVORE_ID | MOVED);
            } else if (get_random_surrounding_object_with_id_xy(game, x, y, &new_x, &new_y, BASIC_PLANT_ID)) {
                if (rand() % 2 == 0) {
                    place_id_xy(game, new_x, new_y, BASIC_HERBIVORE_ID | MOVED);
                }
            }
        }
    }

    // Update size and hunger based on whether we ate this turn
    if (eaten == false) {
        if (GET_BASIC_HERBIVORE_HUNGER(*current_object) == BASIC_HERBIVORE_HUNGER_MAX) {
            *current_object = EMPTY_BOARD_SLOT;
        } else {
            INC_BASIC_HERBIVORE_HUNGER_BITS(*current_object);
        }
    } else {
        SET_BASIC_HERBIVORE_HUNGER(*current_object, 0);
        INC_BASIC_HERBIVORE_SIZE_BITS(*current_object);
    }

    INC_BASIC_HERBIVORE_AGE(*current_object);

    *current_object |= MOVED;
}
