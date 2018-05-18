
#include "game_state_update.h"

#include "all_objects.h"

#include <vector>
#include <algorithm>

void game_state_update(game_state* game_state)
{   
    std::vector<size_t> board_positions(BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT);
    for (size_t i = 0; i < BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT; i++) {
        board_positions[i] = i;
    }
    std::random_shuffle(board_positions.begin(), board_positions.end());

    for (auto i : board_positions) {
        if (game_state->board[i] != EMPTY_BOARD_SLOT) {
            // There is something on this piece

            if (game_state->board[i] & MOVED) {
                // It already had its chance...

                // If it's a disaster that had its chance, kill it
                if (GET_ID_BITS(game_state->board[i]) == NATURAL_DISASTER_ID) {
                    game_state->board[i] = EMPTY_BOARD_SLOT;
                }

                // Remove the moved tag
                game_state->board[i] &= ~MOVED;
                continue;
            }

            switch (GET_ID_BITS(game_state->board[i])) {
                case (IMPASSABLE_MOUNTAIN_ID): {
                    // You're a mountain, you don't do things :)...
                } break;

                case (NATURAL_DISASTER_ID):
                {
                    // Disasters are created in main loop, they just die here
                } break;

                case (BASIC_PLANT_ID):
                {
                    basic_plant_live(game_state, i);
                } break;

                case (BASIC_HERBIVORE_ID): 
                {
                    basic_herbivore_live(game_state, i);
                } break;

                default: {
                    // We don't know anything about this piece, as
                    // far as we are concerned, this is a rock
                } break;
            }

            game_state->board[i] |= MOVED;

        }

    }

}
