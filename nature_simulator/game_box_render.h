#pragma once

#include "rendering_defines.h"

#include "game_state.h"

// Main function that takes main screen buffer, fits the boxes...
//    This function calls other draws
//    it will also recieve all objects on screen
void draw_box(screen_buffer* ScreenBuffer, game_state* game, bool resized);
