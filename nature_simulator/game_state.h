#pragma once

// Percent of the screen width used for game of life box
#define GAME_BOX_SCREEN_PERCENT 100

// How many small 'boxes' for objects will there be in the game
//    we don't include the border mountains
extern unsigned int BOXES_COUNT_WIDTH;
extern unsigned int BOXES_COUNT_HEIGHT;

// We can change the size that each piece occupies
#include <stdint.h>
typedef int64_t object;

struct game_state {
    // 1 << 31 -- On for already moved pieces
    // First BYTE -- Identify the piece
    // Second 2 BYTES -- Piece specific
    object* board;
};

// Here you will find ID
#define ID_BITS_PADDING (0LL)
#define ID_BITS_COUNT (8LL)
#define ID_BITS (0xFFLL)

inline object GET_ID_BITS(const object piece) { 
    return (piece & ID_BITS);
}

// Flag which says that the object already had it's turn
#define MOVED (1LL << (sizeof(object)*8 - 1))

// Object for which we don't have .h file
#define EMPTY_BOARD_SLOT (0LL)
