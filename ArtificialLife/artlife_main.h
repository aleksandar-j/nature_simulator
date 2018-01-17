#pragma once

#include "rendering_defines.h"

// 
struct input {
    // Mouse
    struct {
        bool LeftClicked;
        bool RightClicked;

        float XPos;
        float YPos;
    } Mouse;

    // Keyboard
    struct {
        // Letters
        int Q;
        int W;
        int P;
        int A;
        int S;
        int H;
        int Z;
        int C;

        // Symbols
        int PlusSign;
        int MinusSign;
    } Keyboard;
};

// Main artlife function, we only need image for now but we may
//     implement sound later
void render(int* running, int* updated, 
    screen_buffer* ScreenBuffer, input* KeyboardMouse);