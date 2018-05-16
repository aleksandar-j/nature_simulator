
#include "game_state_update.h"

#include "all_objects.h"

#include <vector>
#include <algorithm>

void game_state_update(game_state* game_state)
{
    // Clean up old zappers
    for (size_t i = 0; i < BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT; i++) {
        if ((game_state->board[i] & 0xFF) == NATURAL_DISASTER_ID) {
            if (game_state->board[i] & MOVED) {
                game_state->board[i] = EMPTY_BOARD_SLOT;
            }
        }
    }
    
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

                // Remove the moved tag
                game_state->board[i] ^= MOVED;
                continue;
            }

            switch (game_state->board[i] & 0xFF) {
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
