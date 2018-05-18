#pragma once

#include "game_state.h"

// Unique ID
#define BASIC_PLANT_ID 0xA0

// Picture filename
#define BASIC_PLANT_PICTURE "basic_plant.png"

// Size bits
#define BASIC_PLANT_SIZE_BITS_PADDING (ID_BITS_COUNT)
#define BASIC_PLANT_SIZE_BITS_COUNT (4LL)
#define BASIC_PLANT_SIZE_BITS (0xFLL << BASIC_PLANT_SIZE_BITS_PADDING)

#define BASIC_PLANT_SIZE_MAX (4LL)

inline object GET_BASIC_PLANT_SIZE_BITS(const object plant) {
    return (plant & BASIC_PLANT_SIZE_BITS);
}
inline object GET_BASIC_PLANT_SIZE(const object plant) {
    return (GET_BASIC_PLANT_SIZE_BITS(plant) >> BASIC_PLANT_SIZE_BITS_PADDING);
}

inline void INC_BASIC_PLANT_SIZE(object& plant) {
    if (GET_BASIC_PLANT_SIZE(plant) < BASIC_PLANT_SIZE_MAX) {
        plant += 1LL << BASIC_PLANT_SIZE_BITS_PADDING;
    }
}
inline void DEC_BASIC_PLANT_SIZE(object& plant) {
    if (GET_BASIC_PLANT_SIZE(plant) > 0) {
        plant -= 1LL << BASIC_PLANT_SIZE_BITS_PADDING;
    }
}

inline void SET_BASIC_PLANT_SIZE(object& plant, const object new_size) {
    plant &= ~BASIC_PLANT_SIZE_BITS | ((new_size << BASIC_PLANT_SIZE_BITS_PADDING) & BASIC_PLANT_SIZE_BITS);
}

// Color bits
#define BASIC_PLANT_COLOR_BITS_PADDING (BASIC_PLANT_SIZE_BITS_PADDING + BASIC_PLANT_SIZE_BITS_COUNT) 
#define BASIC_PLANT_COLOR_BITS_COUNT (24LL)
#define BASIC_PLANT_COLOR_BITS (0xFFFFFFLL << BASIC_PLANT_COLOR_BITS_PADDING)

inline object GET_BASIC_PLANT_COLOR_BITS(const object plant) {
    return (plant & BASIC_PLANT_COLOR_BITS);
}
inline object GET_BASIC_PLANT_COLOR(const object plant) {
    return (GET_BASIC_PLANT_COLOR_BITS(plant) >> BASIC_PLANT_COLOR_BITS_PADDING);
}

// Age bits
#define BASIC_PLANT_AGE_BITS_PADDING (BASIC_PLANT_COLOR_BITS_PADDING + BASIC_PLANT_COLOR_BITS_COUNT) 
#define BASIC_PLANT_AGE_BITS_COUNT (5LL)
#define BASIC_PLANT_AGE_BITS (0b11111LL << BASIC_PLANT_AGE_BITS_PADDING)

#define BASIC_PLANT_AGE_MAX (31LL)
#define BASIC_PLANT_AGE_CHANCE_TO_DIE_AT_MAX (4LL)
#define BASIC_PLANT_AGE_CHANCE_TO_DIE_AT_ANY (4LL * BASIC_PLANT_AGE_CHANCE_TO_DIE_AT_MAX)

inline object GET_BASIC_PLANT_AGE_BITS(const object plant) {
    return (plant & BASIC_PLANT_AGE_BITS);
}
inline object GET_BASIC_PLANT_AGE(const object plant) {
    return (GET_BASIC_PLANT_AGE_BITS(plant) >> BASIC_PLANT_AGE_BITS_PADDING);
}

inline void INC_BASIC_PLANT_AGE(object& plant) {
    if (GET_BASIC_PLANT_AGE(plant) < BASIC_PLANT_AGE_MAX) {
        plant += 1LL << BASIC_PLANT_AGE_BITS_PADDING;
    }
}

// Public functions
void basic_plant_live(game_state* game, size_t memindex);
