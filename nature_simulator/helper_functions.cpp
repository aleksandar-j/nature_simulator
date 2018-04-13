
#include "helper_functions.h"

void move_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y)
{
    int org_i = xy_to_memindex(org_x, org_y);    
    int dest_i = xy_to_memindex(dest_x, dest_y);

    game->board[dest_i] = game->board[org_i];
    game->board[org_i] = 0;
}

void move_if_free_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y)
{
    if (get_object_id_at_xy(game, dest_x, dest_y) == EMPTY_BOARD_SLOT) {
        move_xy(game, org_x, org_y, dest_x, dest_y);
    }
}

void move_memindex(game_state* game, size_t org_index, size_t dest_index)
{
    game->board[dest_index] = game->board[org_index];
    game->board[org_index] = 0;
}

void move_if_free_memindex(game_state* game, size_t org_index, size_t dest_index)
{
    if (get_object_id_at_memindex(game, dest_index) == EMPTY_BOARD_SLOT) {
        move_memindex(game, org_index, dest_index);
    }
}

void copy_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y)
{
    int org_i = xy_to_memindex(org_x, org_y);
    int dest_i = xy_to_memindex(dest_x, dest_y);

    game->board[dest_i] = game->board[org_i];
}

void copy_if_free_xy(game_state* game, int org_x, int org_y, int dest_x, int dest_y)
{
    if (get_object_id_at_xy(game, dest_x, dest_y) == EMPTY_BOARD_SLOT) {
        copy_xy(game, org_x, org_y, dest_x, dest_y);
    }
}

void place_id_xy(game_state* game, int x, int y, object ID)
{
    game->board[xy_to_memindex(x, y)] = ID;
}

bool place_id_if_free_xy(game_state* game, int x, int y, object ID)
{
    if (get_object_id_at_xy(game, x, y) == EMPTY_BOARD_SLOT) {
        place_id_xy(game, x, y, ID);
        return true;
    }
    return false;
}

void get_surrounding_objects_xy(game_state* game, int x, int y, 
                        board_piece* objects_arr, size_t* objects_count)
{
    if (!is_valid_xy(x, y) || objects_arr == NULL) {
        return;
    }

    *objects_count = 0;

    size_t org_memindex = xy_to_memindex(x, y);

    x--; y++;
    // Outer layer iterates over Y's
    for (int i = 0; i < 3; i++, x -= 3, y--) {
        size_t memindex = xy_to_memindex(x, y);

        // This inner layer changes X every time
        for (int j = 0; j < 3; j++, x++) {

            // If we are out of the board, have an empty slot, or have our own spot...
            if (!is_valid_memindex(memindex + j) ||
                        game->board[memindex + j] == EMPTY_BOARD_SLOT ||
                        (memindex + j) == org_memindex) {
                continue;
            }
            
            objects_arr[*objects_count].piece_data = game->board[memindex + j];
            objects_arr[*objects_count].x = x;
            objects_arr[*objects_count].y = y;

            (*objects_count)++;
        }
    }
}

void get_random_surrounding_xy(game_state* game, int org_x, int org_y, int* rand_x, int* rand_y)
{
    if (!is_valid_xy(org_x, org_y)) {
        return;
    }

    *rand_x = org_x;
    *rand_y = org_y;

    do {
        *rand_x += rand() % 3 - 1;
        *rand_y += rand() % 3 - 1;
    } while (!is_valid_xy(*rand_x, *rand_y) || (*rand_x == org_x && *rand_y == org_y));
}

bool get_random_surrounding_object_with_id_xy(game_state* game,
    int org_x, int org_y, int* dest_x, int* dest_y,
    object ID)
{
    board_piece* objects = new board_piece[8];
    size_t objects_count;
    get_surrounding_objects_xy(game, org_x, org_y, objects, &objects_count);

    board_piece** objects_with_correct_id = new board_piece*[objects_count];
    size_t objects_wci_index = 0;
    for (size_t i = 0; i < objects_count; i++) {
        if (get_object_id_at_xy(game, objects[i].x, objects[i].y) == ID) {
            objects_with_correct_id[objects_wci_index++] = &objects[i];
        }
    }

    if (objects_wci_index != 0) {
        size_t rand_choice = rand() % objects_wci_index;
        *dest_x = objects_with_correct_id[rand_choice]->x;
        *dest_y = objects_with_correct_id[rand_choice]->y;

        return true;
    } else {
        return false;
    }
}

bool bits_set(object data, object bits)
{
    if ((data & bits) == bits) {
        return true;
    }
    return false;
}

void memindex_to_xy(size_t memindex, int* x, int* y)
{
    if (!is_valid_memindex(memindex)) {
        // TODO: Assert here
    }

    *x = memindex % BOXES_COUNT_WIDTH;
    *y = BOXES_COUNT_HEIGHT - ((memindex - *x) / BOXES_COUNT_WIDTH) - 1;
}

size_t xy_to_memindex(int x, int y)
{
    if (!is_valid_xy(x, y)) {
        // TODO: Assert here
    }

    return (((BOXES_COUNT_HEIGHT - 1) - y) * BOXES_COUNT_WIDTH) + (x);
}

inline bool is_valid_xy(int x, int y)
{
    if (x < 0 || y < 0 || x >= (int)BOXES_COUNT_WIDTH || y >= (int)BOXES_COUNT_HEIGHT) {
        return false;
    }
    return true;
}

bool is_valid_memindex(size_t memindex)
{
    if (memindex >= (unsigned)(BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT) || memindex < 0) {
        return false;
    }
    return true;
}

object get_object_id_at_memindex(game_state* game, size_t memindex)
{
    return game->board[memindex] & ID_BITS;
}

object get_object_id_at_xy(game_state* game, int x, int y)
{
    return game->board[xy_to_memindex(x, y)] & ID_BITS;
}
