#pragma once

#include "game_state.h"

#define BASIC_HERBIVORE_ID 0xa1

#define BASIC_HERBIVORE_PICTURE "basic_herbivore.png"

#define BASIC_HERBIVORE_SIZE_BITS 0b11 << ID_BITS_COUNT

#define BASIC_HERBIVORE_SIZE_0 0 << ID_BITS_COUNT
#define BASIC_HERBIVORE_SIZE_1 1 << ID_BITS_COUNT

void basic_herbivore_live(game_state* game, size_t memindex);
