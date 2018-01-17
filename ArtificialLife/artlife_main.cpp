
#include "artlife_main.h"

#include "game_state_update.h"
#include "game_box_render.h"

#include "helper_functions.h"

#include "all_objects.h"

#include <Windows.h>

game_state game;

// Globals
unsigned int BOXES_COUNT_WIDTH = 120;
unsigned int BOXES_COUNT_HEIGHT = 100;
int GAME_SPEED = 1;

// Locals but needed here
int old_win_size_stamp = 0;
int old_boxes_count_stamp = 0;

LARGE_INTEGER COUNTER_TICKS_PER_SECOND;

bool disasters_on = true;

// Local functions
bool check_and_handle_resize(screen_buffer* screen_buffer);

void render(int* running, int* updated, 
    screen_buffer* screen_buffer, input* game_keyboard_mouse)
{
    while (*running) {
        LARGE_INTEGER clock_1;
        QueryPerformanceCounter(&clock_1);
        
        // Handle input
        {
            static int plus_minus_function = 0;

            if (game_keyboard_mouse->Keyboard.W) {
                plus_minus_function = 1;
                game_keyboard_mouse->Keyboard.W = 0;
            }

            if (game_keyboard_mouse->Keyboard.P) {
                // TODO: Remove this
                game.board[BOXES_COUNT_WIDTH + 1 + 1] = BASIC_PLANT_ID |MOVED;
                game_keyboard_mouse->Keyboard.P = 0;
            }

            if (game_keyboard_mouse->Keyboard.S) {
                plus_minus_function = 0;
                game_keyboard_mouse->Keyboard.S = 0;
            }

            if (game_keyboard_mouse->Keyboard.H) {
                plus_minus_function = 2;
                game_keyboard_mouse->Keyboard.H = 0;
            }

            if (game_keyboard_mouse->Keyboard.Z) {
                disasters_on = (disasters_on) ? false : true;
                game_keyboard_mouse->Keyboard.Z = 0;
            }

            if (game_keyboard_mouse->Keyboard.C) {
                // TODO: Remove this
                game.board[BOXES_COUNT_WIDTH + 1 + 1] = BASIC_HERBIVORE_ID | MOVED;
                game_keyboard_mouse->Keyboard.C = 0;
            }

            if (game_keyboard_mouse->Keyboard.PlusSign || game_keyboard_mouse->Keyboard.MinusSign) {
                switch (plus_minus_function) {
                    // Set GAME_SPEED by using + and -
                    case (0):
                    {
                        GAME_SPEED += game_keyboard_mouse->Keyboard.PlusSign;
                        GAME_SPEED -= game_keyboard_mouse->Keyboard.MinusSign;
                        if (GAME_SPEED <= 1) {
                            GAME_SPEED = 1;
                        }
                    } break;
                    // Set BOXES_COUNT_WIDTH by using + and -
                    case (1):
                    {
                        BOXES_COUNT_WIDTH += game_keyboard_mouse->Keyboard.PlusSign;
                        BOXES_COUNT_WIDTH -= game_keyboard_mouse->Keyboard.MinusSign;
                        if (BOXES_COUNT_WIDTH <= 3) {
                            BOXES_COUNT_WIDTH = 3;
                        }
                    } break;
                    // Set BOXES_COUNT_HEIGHT by using + and -
                    case (2):
                    {
                        BOXES_COUNT_HEIGHT += game_keyboard_mouse->Keyboard.PlusSign;
                        BOXES_COUNT_HEIGHT -= game_keyboard_mouse->Keyboard.MinusSign;
                        if (BOXES_COUNT_HEIGHT <= 3) {
                            BOXES_COUNT_HEIGHT = 3;
                        }
                    } break;
                }

                game_keyboard_mouse->Keyboard.PlusSign = 0;
                game_keyboard_mouse->Keyboard.MinusSign = 0;
            }
        }

        bool resized = check_and_handle_resize(screen_buffer);

        // Main part of the loop
        {
            // Disasters are killing...
            if (disasters_on) {
                int disasters_count = (int)sqrt(BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT) / 2;
                while (disasters_count--) {
                    int rand_x = rand() % (BOXES_COUNT_WIDTH - 2) + 1;
                    int rand_y = rand() % (BOXES_COUNT_HEIGHT - 2) + 1;

                    game.board[xy_to_memindex(rand_x, rand_y)] = NATURAL_DISASTER_ID;
                }
            }

            // Update the positions of every object
            game_state_update(&game);

            // Draw final image 
            draw_box(screen_buffer, &game, resized);

            *updated = 1;
        }
        
        LARGE_INTEGER clock_2;
        QueryPerformanceCounter(&clock_2);

        double seconds_passed = (double)(clock_2.QuadPart - clock_1.QuadPart) / (double)COUNTER_TICKS_PER_SECOND.QuadPart;
        int64_t miliseconds_passed = (int64_t)(seconds_passed * 1000.0);

        int64_t sleep_this_much = (1000 / GAME_SPEED) - miliseconds_passed;

        if (sleep_this_much > 0) {
            Sleep(sleep_this_much);
        }
    }
}

bool check_and_handle_resize(screen_buffer* screen_buffer) 
{
    bool resized = false;

    int OLD_BOXES_COUNT_WIDTH = 0;
    int OLD_BOXES_COUNT_HEIGHT = 0;

    // Check if the user resized
    int new_win_size_stamp = (screen_buffer->width << 16) | (screen_buffer->height);
    int new_boxes_count_stamp = (BOXES_COUNT_WIDTH << 16) | (BOXES_COUNT_HEIGHT);

    if (new_win_size_stamp != old_win_size_stamp || new_boxes_count_stamp != old_boxes_count_stamp) {
        resized = true;

        OLD_BOXES_COUNT_WIDTH = old_boxes_count_stamp >> 16;
        OLD_BOXES_COUNT_HEIGHT = (int16_t)old_boxes_count_stamp;

        old_win_size_stamp = new_win_size_stamp;
        old_boxes_count_stamp = new_boxes_count_stamp;
    }

    // Check if something changed
    if (resized) {
        // See the counter granularity        
        QueryPerformanceFrequency(&COUNTER_TICKS_PER_SECOND);

        // Clear the memory
        memset(screen_buffer->memory, 0, screen_buffer->width*screen_buffer->height*screen_buffer->bpp);

        // Allocate board
        object* new_board = new object[BOXES_COUNT_WIDTH*BOXES_COUNT_HEIGHT]();

        if (game.board != NULL) {

            size_t old_memindex = 0;
            size_t new_memindex = 0;

            for (int y = OLD_BOXES_COUNT_HEIGHT - 1; y >= 0; y--) {
                if (y > (BOXES_COUNT_HEIGHT - 1)) {
                    continue;
                }

                for (int x = 0; x < OLD_BOXES_COUNT_WIDTH && x < BOXES_COUNT_WIDTH; x++) {
                    if (x > (BOXES_COUNT_WIDTH - 1)) {
                        break;
                    }

                    old_memindex = (((OLD_BOXES_COUNT_HEIGHT - 1) - y) * OLD_BOXES_COUNT_WIDTH) + (x);
                    new_memindex = (((BOXES_COUNT_HEIGHT - 1) - y) * BOXES_COUNT_WIDTH) + (x);

                    if (game.board[old_memindex] != IMPASSABLE_MOUNTAIN_ID) {
                        new_board[new_memindex] = game.board[old_memindex];
                    }
                }
            }
        }

        game.board = new_board;

        // Hardcode Edges
        for (size_t i = 0, j = xy_to_memindex(0, 0); i < BOXES_COUNT_WIDTH; i++, j++) {
            game.board[i] = IMPASSABLE_MOUNTAIN_ID;
            game.board[j] = IMPASSABLE_MOUNTAIN_ID;
        }
        for (int y = 0; y < (int)BOXES_COUNT_HEIGHT; y++) {
            game.board[xy_to_memindex(0, y)] = IMPASSABLE_MOUNTAIN_ID;
            game.board[xy_to_memindex(BOXES_COUNT_WIDTH - 1, y)] = IMPASSABLE_MOUNTAIN_ID;
        }
    }

    return resized;
}
