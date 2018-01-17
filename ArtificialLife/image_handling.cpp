
#include "image_handling.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"

void png_load(std::string path, rgb_square_picture* out_pict, size_t desired_dimensions)
{
    int width, height, bpp;

    void* mem = stbi_load(path.c_str(), &width, &height, &bpp, 0);

    unsigned char* new_mem = new unsigned char[desired_dimensions*desired_dimensions*bpp + 1];
    stbir_resize_uint8((stbi_uc*)mem, width, height, 0,
                (stbi_uc*)new_mem, desired_dimensions, desired_dimensions, 0, bpp);

    int* final_mem = new int[desired_dimensions*desired_dimensions]();

    for (size_t i = 0, j = 0; i < desired_dimensions*desired_dimensions*bpp; i += bpp, j++) {
        // Alpha
        if (bpp >= 4) {
            final_mem[j] += new_mem[i + 3];
            final_mem[j] <<= 8;
        }

        // Red
        final_mem[j] += new_mem[i];
        final_mem[j] <<= 8;

        // Green
        final_mem[j] += new_mem[i + 1];
        final_mem[j] <<= 8;

        // Blue
        final_mem[j] += new_mem[i + 2];
    }

    delete out_pict->rgb_memory;
    out_pict->rgb_memory = final_mem;

    delete mem;
    delete new_mem;
}

void png_resize(rgb_square_picture* in_out_pict, int percentage)
{
    if (percentage >= 100 || percentage <= 0) {
        return;
    }

    // TODO: Handle this
    int bpp = 4;

    int org_image_dimensions = sqrt(in_out_pict->rgb_memory_size);
    int new_image_dimensions = (double)org_image_dimensions * ((double)percentage / 100.0);

    if (org_image_dimensions == new_image_dimensions) {
        return;
    }

    unsigned char* new_mem = new unsigned char[new_image_dimensions*new_image_dimensions*bpp + 1];
    stbir_resize_uint8((stbi_uc*)in_out_pict->rgb_memory, org_image_dimensions, org_image_dimensions, 0,
                        (stbi_uc*)new_mem, new_image_dimensions, new_image_dimensions, 0, bpp);

    int* final_mem = new int[new_image_dimensions*new_image_dimensions*bpp + 1];

    for (size_t i = 0, j = 0; i < new_image_dimensions*new_image_dimensions*bpp; i += bpp, j++) {
        // Alpha
        if (bpp >= 4) {
            final_mem[j] += new_mem[i + 3];
            final_mem[j] <<= 8;
        }

        // Red
        final_mem[j] += new_mem[i + 2];
        final_mem[j] <<= 8;

        // Green
        final_mem[j] += new_mem[i + 1];
        final_mem[j] <<= 8;

        // Blue
        final_mem[j] += new_mem[i];
    }

    memset(in_out_pict->rgb_memory, 0, in_out_pict->rgb_memory_size*bpp);

    int margin = (org_image_dimensions - new_image_dimensions) / 2;

    unsigned char* dest_mem = (unsigned char*)in_out_pict->rgb_memory;
    unsigned char* source_mem = (unsigned char*)final_mem;

    size_t copy_here = org_image_dimensions*margin*bpp + margin*bpp;
    size_t copy_from_here = 0;

    while (copy_from_here < new_image_dimensions*new_image_dimensions*bpp) {
        memcpy_s(dest_mem + copy_here, org_image_dimensions*bpp + 1,
               source_mem + copy_from_here, new_image_dimensions*bpp);

        copy_here += org_image_dimensions*bpp;
        copy_from_here += new_image_dimensions*bpp;
    }

    delete final_mem;
    delete new_mem;
}
