
#include "game_box_render_drawing.h"

#include "game_state.h"
#include <memory>
#include <math.h>

void draw_icon(rgb_square_picture* Picture, int box_x, int box_y)
{
    if (box_x >= BOXES_COUNT_WIDTH || box_y >= BOXES_COUNT_HEIGHT || 
        Picture->rgb_memory == nullptr || Picture->rgb_memory_size == 0) {
        // TODO: Assert this
    }
    // TODO: We assume that icon size is the correct size...

    int copy_from_here = 0;
    // We skip (BOXES_COUNT_HEIGHT - box_y - 1) many boxes from the top
    //     and (box_x) boxes from the left
    int copy_here = ((BOXES_COUNT_HEIGHT - box_y - 1) * one_box_dimensions) * game_box_width + 
                        ((box_x) * one_box_dimensions);

    for (int i = 0; i < one_box_dimensions; i++) {
        memcpy_s(&(((int*)game_box_rgb_buffer)[copy_here]),
                                        (game_box_width + 1) * sizeof(int),
                 &(((int*)Picture->rgb_memory)[copy_from_here]),
                                         one_box_dimensions * sizeof(int));

        copy_from_here += one_box_dimensions;
        copy_here += game_box_width;
    }
}
