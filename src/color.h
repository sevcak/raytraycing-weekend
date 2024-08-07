#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
#include <vector>

using color = vec3;

inline void write_color( std::vector<unsigned char>& pixels,
                         size_t pixel_index, const color& pixel_color )
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    /* Translate [0, 1] to [0, 255] */
    int r_byte = int( 255.999 * r );
    int g_byte = int( 255.999 * g );
    int b_byte = int( 255.999 * b );

    pixels[pixel_index]     = r_byte;
    pixels[pixel_index + 1] = g_byte;
    pixels[pixel_index + 2] = b_byte;
}

#endif
