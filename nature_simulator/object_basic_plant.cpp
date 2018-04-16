
#include "object_basic_plant.h"

#include "helper_functions.h"

// 2 BYTES will be used like this
// 0000 0000 - 0000 0111     (1 - already used for something)

// First 3 bits - size of the plant

void basic_plant_live(game_state* game, size_t game_board_index)
{
    int x, y;
    memindex_to_xy(game_board_index, &x, &y);

    object* current_guy = &(game->board[game_board_index]);

    size_t checking_bits = *current_guy & BASIC_PLANT_SIZE_BITS;
    if (checking_bits >= BASIC_PLANT_SIZE_2) {
        int new_x, new_y;
        get_random_surrounding_xy(game, x, y, &new_x, &new_y);

        int placed = place_id_if_free_xy(game, new_x, new_y, 
                            BASIC_PLANT_ID | BASIC_PLANT_SIZE_0 | MOVED);
        if (placed) {
            *current_guy -= BASIC_PLANT_SIZE_1 * 2;
        } else if (checking_bits < BASIC_PLANT_SIZE_4) {
            *current_guy += BASIC_PLANT_SIZE_1;
        }
    } else {
        *current_guy += BASIC_PLANT_SIZE_1;
    }

    // TODO: Make this more advanced
    *current_guy = (*current_guy & ~BASIC_PLANT_COLOR_BITS) | ((rand() % 0xFFFFFF) << BASIC_PLANT_COLOR_BITS_PADDING);
}
