#pragma once

#include "game_state.h"
#include <random>

// Given data, returns whether the bits in 'bits' are also on in data
bool bits_set(object data, object bits);


// Resolves memory index to X, Y coordinates
void memindex_to_xy(size_t memindex, int* x, int* y);

// Resolves X, Y coordinates to the memory of the image
size_t xy_to_memindex(int x, int y);

// Does it fall within the bounds of our box
bool is_valid_xy(int x, int y);

// Does it fall within the bounds of our array
bool is_valid_memindex(size_t memindex);

// Get object ID of the specified memindex
object get_object_id_at_memindex(game_state* game, size_t memindex);

// Get object ID of the specified x, y spot
object get_object_id_at_xy(game_state* game, int x, int y);

// Checks whether the position contains EMPTY_BOARD_SLOT (which is 0)
bool is_free_xy(game_state* game, int x, int y);


// Moves object from X, Y to another X, Y. Does not ask questions.
void move_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y);

// Moves object from X, Y to another X, Y. Asks questions.
void move_if_free_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y);

// Moves object from X, Y to another X, Y. Does not ask questions.
void move_memindex(game_state* game, size_t org_index, size_t dest_index);

// Moves object from X, Y to another X, Y. Asks questions.
void move_if_free_memindex(game_state* game, size_t org_index, size_t dest_index);

// Moves object from X, Y to another X, Y. Does not ask questions.
void copy_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y);

// Moves object from X, Y to another X, Y. Asks questions.
void copy_if_free_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y);

// Places an ID on the selected X, Y coordinates
void place_id_xy(game_state* game, int x, int y, object ID);

// Places an ID on the selected X, Y coordinates, if free
bool place_id_if_free_xy(game_state* game, int x, int y, object ID);


struct board_piece {
    object piece_data;
    int x, y;
};

// Gets all objects next to the X, Y selected; objects_arr must be allocated with 8
void get_surrounding_objects_xy(game_state* game, int x, int y, 
                        board_piece* objects_arr, size_t* objects_count);

// Gets random surrounding square
void get_random_surrounding_xy(game_state* game, 
                int org_x, int org_y, int* rand_x, int* rand_y);

// Gets random surrounding square
bool get_random_surrounding_object_with_id_xy(game_state* game,
                            int org_x, int org_y, int* dest_x, int* dest_y, 
                                                object ID);