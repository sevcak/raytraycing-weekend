#include <iostream>
#include <vector>

#include "stb_include.h"
#include "color.h"
// #include "vec3.h"

int main( void )
{
    int image_width  = 256;
    int image_height = 256;

    std::vector<unsigned char> pixels(image_width * image_height * 3);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for ( int i = 0; i < image_height; ++i ) {
        std::clog << "\rScanlines remaining: "
                  << ( image_height - i ) << ' ' << std::flush;

        for ( int j = 0; j < image_width; ++j ) {
            size_t pixel_index = ( i * image_width + j ) * 3;
            auto   pixel_color = color( double( j ) / ( image_width  - 1 ),
                                        double( i ) / ( image_height - 1 ),
                                        0.0 );
            write_color( pixels, pixel_index, pixel_color );
        }
    }

    // Write the image to a file
    const char* filename = "image.png";
    int result = stbi_write_png( filename,
                                 image_width, image_height,
                                 3, pixels.data(), image_width * 3 );
    if (result == 0) {
        std::cerr << "Error writing PNG file." << std::endl;
        return 1;
    }

    std::clog << "\rDone. Image saved as " << filename << "\n";
    return 0;
}
