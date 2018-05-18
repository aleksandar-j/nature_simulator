#pragma once

#include "game_state.h"

//-------------------------------------------------------------------------------------
// Unique identifier
#define BASIC_HERBIVORE_ID 0xa1

// Image filename
#define BASIC_HERBIVORE_PICTURE "basic_herbivore.png"

//-------------------------------------------------------------------------------------
// Procreation rates
#define BASIC_HERBIVORE_PROCREATE_CHANCE (3LL)
#define BASIC_HERBIVORE_PROCREATE_MIN_AGE (2LL)
#define BASIC_HERBIVORE_PROCREATE_MAX_HUNGER (2LL)

//-------------------------------------------------------------------------------------
// Size bits
#define BASIC_HERBIVORE_SIZE_BITS_PADDING (ID_BITS_COUNT)
#define BASIC_HERBIVORE_SIZE_BITS_COUNT (2LL)
#define BASIC_HERBIVORE_SIZE_BITS (0b11 << BASIC_HERBIVORE_SIZE_BITS_PADDING)

#define BASIC_HERBIVORE_SIZE_MAX (1LL)

inline object GET_BASIC_HERBIVORE_SIZE_BITS(const object herbivore) { 
    return (herbivore & BASIC_HERBIVORE_SIZE_BITS); 
}
inline object GET_BASIC_HERBIVORE_SIZE(const object herbivore) { 
    return (GET_BASIC_HERBIVORE_SIZE_BITS(herbivore) >> BASIC_HERBIVORE_SIZE_BITS_PADDING); 
}

inline void INC_BASIC_HERBIVORE_SIZE_BITS(object& herbivore) {
    if (GET_BASIC_HERBIVORE_SIZE(herbivore) < BASIC_HERBIVORE_SIZE_MAX) {
        herbivore += 1LL << BASIC_HERBIVORE_SIZE_BITS_PADDING;
    }
}

// Hunger bits
#define BASIC_HERBIVORE_HUNGER_BITS_PADDING (BASIC_HERBIVORE_SIZE_BITS_PADDING + BASIC_HERBIVORE_SIZE_BITS_COUNT)
#define BASIC_HERBIVORE_HUNGER_BITS_COUNT (2LL)
#define BASIC_HERBIVORE_HUNGER_BITS (0b11LL << BASIC_HERBIVORE_HUNGER_BITS_PADDING)

#define BASIC_HERBIVORE_HUNGER_MAX (3LL)

inline object GET_BASIC_HERBIVORE_HUNGER_BITS(const object herbivore) {
    return (herbivore & BASIC_HERBIVORE_HUNGER_BITS);
}
inline object GET_BASIC_HERBIVORE_HUNGER(const object herbivore) {
    return (GET_BASIC_HERBIVORE_HUNGER_BITS(herbivore) >> BASIC_HERBIVORE_HUNGER_BITS_PADDING);
}

inline void INC_BASIC_HERBIVORE_HUNGER_BITS(object& herbivore) {
    if (GET_BASIC_HERBIVORE_HUNGER(herbivore) < BASIC_HERBIVORE_HUNGER_MAX) {
        herbivore += 1LL << BASIC_HERBIVORE_HUNGER_BITS_PADDING;
    }
}

inline void SET_BASIC_HERBIVORE_HUNGER(object& herbivore, const object new_hunger) {
    herbivore &= ~BASIC_HERBIVORE_HUNGER_BITS | ((new_hunger << BASIC_HERBIVORE_HUNGER_BITS_PADDING) & BASIC_HERBIVORE_HUNGER_BITS);
}

// Age bits
#define BASIC_HERBIVORE_AGE_BITS_PADDING (BASIC_HERBIVORE_HUNGER_BITS_PADDING + BASIC_HERBIVORE_HUNGER_BITS_COUNT) 
#define BASIC_HERBIVORE_AGE_BITS_COUNT (3LL)
#define BASIC_HERBIVORE_AGE_BITS (0b111LL << BASIC_HERBIVORE_AGE_BITS_PADDING)

#define BASIC_HERBIVORE_AGE_MAX (7LL)
#define BASIC_HERBIVORE_AGE_CHANCE_TO_DIE_AT_MAX (3LL)
#define BASIC_HERBIVORE_AGE_CHANCE_TO_DIE_AT_ANY (3LL * BASIC_HERBIVORE_AGE_CHANCE_TO_DIE_AT_MAX)

inline object GET_BASIC_HERBIVORE_AGE_BITS(const object herbivore) {
    return (herbivore & BASIC_HERBIVORE_AGE_BITS);
}
inline object GET_BASIC_HERBIVORE_AGE(const object herbivore) {
    return (GET_BASIC_HERBIVORE_AGE_BITS(herbivore) >> BASIC_HERBIVORE_AGE_BITS_PADDING);
}

inline void INC_BASIC_HERBIVORE_AGE(object& herbivore) {
    if (GET_BASIC_HERBIVORE_AGE(herbivore) < BASIC_HERBIVORE_AGE_MAX) {
        herbivore += 1LL << BASIC_HERBIVORE_AGE_BITS_PADDING;
    }
}

// Global functions
void basic_herbivore_live(game_state* game, size_t memindex);
