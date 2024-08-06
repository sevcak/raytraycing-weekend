#include <iostream>
#include <vector>
#include "stb_include.h"

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
            auto r = double( i ) / ( image_width  - 1 );
            auto g = double( j ) / ( image_height - 1 );
            auto b = 0.0;

            int ir = int( 255.999 * r );
            int ig = int( 255.999 * g );
            int ib = int( 255.999 * b );

            size_t pixel_index = ( i * image_width + j ) * 3;
            pixels[pixel_index] = ir;
            pixels[pixel_index + 1] = ig;
            pixels[pixel_index + 2] = ib;
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
