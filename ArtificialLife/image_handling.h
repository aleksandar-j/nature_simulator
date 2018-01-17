#pragma once

#include "rendering_defines.h"

#include <string>

void png_load(std::string path, rgb_square_picture* out_pict, size_t desired_dimensions);

// Not true resizing, we just make it smaller
void png_resize(rgb_square_picture* in_out_pict, int percentage);
