#pragma once

// Variables to be used by other drawing functions (maybe dangerious?)
extern int game_box_width;
extern int game_box_height;

extern void* game_box_rgb_buffer;

extern int one_box_dimensions;

//
struct screen_buffer {
    void* memory;

    int width;
    int height;

    int bpp;
};

// Pictures we will be using (simplifies shit)
struct rgb_square_picture {
    void* rgb_memory;
    size_t rgb_memory_size;
};