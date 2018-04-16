#pragma once

#include "game_state.h"

// Unique ID
#define BASIC_PLANT_ID 0xa0

// Picture filename
#define BASIC_PLANT_PICTURE "basic_plant.png"

// Size bits
#define BASIC_PLANT_SIZE_BITS_PADDING (ID_BITS_COUNT)
#define BASIC_PLANT_SIZE_BITS_COUNT (4)
#define BASIC_PLANT_SIZE_BITS (0xF << BASIC_PLANT_SIZE_BITS_PADDING)

#define BASIC_PLANT_SIZE_0 0 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_1 1 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_2 2 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_3 3 << ID_BITS_COUNT
#define BASIC_PLANT_SIZE_4 4 << ID_BITS_COUNT

// Color bits
#define BASIC_PLANT_COLOR_BITS_PADDING (ID_BITS_COUNT + BASIC_PLANT_SIZE_BITS_COUNT) 
#define BASIC_PLANT_COLOR_BITS_COUNT (24)
#define BASIC_PLANT_COLOR_BITS (0xFFFFFF << BASIC_PLANT_COLOR_BITS_PADDING)

void basic_plant_live(game_state* game, size_t game_board_index);
