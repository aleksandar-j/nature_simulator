
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

        if (is_free_xy(game, new_x, new_y)) {
            // Mutating our current color to make the new one
            int64_t new_color;
            do {
                new_color = *current_guy & BASIC_PLANT_COLOR_BITS;
                int64_t increment = 5 << BASIC_PLANT_COLOR_BITS_PADDING;
                if (rand() % 2) {
                    increment = -increment;
                }
                int red_green_or_blue = rand() % 3;
                if (red_green_or_blue == 0) {
                    new_color += increment;
                } else if (red_green_or_blue == 1) {
                    new_color += increment << 8;
                } else {
                    new_color += increment << 16;
                }
            } while ((new_color & (~BASIC_PLANT_COLOR_BITS)) != 0);

            place_id_xy(game, new_x, new_y,
                BASIC_PLANT_ID | BASIC_PLANT_SIZE_0 | MOVED | new_color);

            *current_guy -= BASIC_PLANT_SIZE_1 * 2;
        } else if (checking_bits < BASIC_PLANT_SIZE_4) {
            *current_guy += BASIC_PLANT_SIZE_1;
        }
    } else {
        *current_guy += BASIC_PLANT_SIZE_1;
    }
}