#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <vector>

using color = vec3;

inline double linear_to_gamma( double linear_component )
{
    if ( linear_component > 0 ) {
        return std::sqrt( linear_component );
    }

    return 0;
}

inline void write_color( std::vector<unsigned char>& pixels,
                         size_t pixel_index, const color& pixel_color )
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma( r );
    g = linear_to_gamma( g );
    b = linear_to_gamma( b );

    /* Translate [0, 1] to [0, 255] */
    static const interval intensity( 0.000, 0.999 );
    int r_byte = int( 256 * intensity.clamp( r ) );
    int g_byte = int( 256 * intensity.clamp( g ) );
    int b_byte = int( 256 * intensity.clamp( b ) );

    pixels[pixel_index]     = r_byte;
    pixels[pixel_index + 1] = g_byte;
    pixels[pixel_index + 2] = b_byte;
}

#endif
