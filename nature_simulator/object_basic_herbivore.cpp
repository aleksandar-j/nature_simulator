
#include "object_basic_herbivore.h"

#include "object_basic_plant.h"
#include "helper_functions.h"
#include "object_impassable_mountain.h"

// 2 BYTES will be used like this
// 000000000000-11-11     (1 - already used for something)

// First 2 bits - size of the animal (only two states)
// Next 2 bits - how many days of starvation

#define EATEN_BIT_PAD (1 << ID_BITS_COUNT << 2)
#define EATEN_BITS (EATEN_BIT_PAD)

//  | (EATEN_BIT_PAD << 1)

void basic_herbivore_live(game_state* game, size_t memindex)
{
    int x, y; 
    int new_x, new_y;
    memindex_to_xy(memindex, &x, &y);

    game->board[memindex] |= MOVED;

    // Try to eat something
    bool eaten = false;

    if (get_random_surrounding_object_with_id_xy(game, x, y, &new_x, &new_y, BASIC_PLANT_ID)) {
        move_xy(game, x, y, new_x, new_y);
        x = new_x; y = new_y; memindex = xy_to_memindex(x, y);
        eaten = true;
    
        if (bits_set(game->board[memindex], BASIC_HERBIVORE_SIZE_1) && (rand() % 4 == 0)) {
            get_random_surrounding_xy(game, x, y, &new_x, &new_y);

            if (get_object_id_at_xy(game, new_x, new_y) == EMPTY_BOARD_SLOT) {
                place_id_xy(game, new_x, new_y, BASIC_HERBIVORE_ID | MOVED | BASIC_HERBIVORE_SIZE_0);
            }
        }
    } else {
        get_random_surrounding_xy(game, x, y, &new_x, &new_y);
    }

    if (eaten == false) {
        if ((game->board[memindex] & EATEN_BITS) == EATEN_BITS) {
            // We die
            game->board[memindex] = EMPTY_BOARD_SLOT;
        }
        game->board[memindex] += EATEN_BIT_PAD;
    } else {
        game->board[memindex] &= ~EATEN_BITS;
        game->board[memindex] |= BASIC_HERBIVORE_SIZE_1;
    }
}
