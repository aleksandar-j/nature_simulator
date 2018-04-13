#pragma once

#include "game_state.h"

#define BASIC_PLANT_ID 0xa0

#define BASIC_PLANT_PICTURE "basic_plant.png"

#define BASIC_PLANT_SIZE_BITS (0b111 << ID_BITS_COUNT)
#define BASIC_PLANT_SIZE_BITS_COUNT (3)

#define BASIC_PLANT_SIZE_0 0 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_1 1 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_2 2 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_3 3 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_4 4 << ID_BITS_COUNT

void basic_plant_live(game_state* game, size_t game_board_index);
