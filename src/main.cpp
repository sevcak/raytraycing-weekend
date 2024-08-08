#include <cmath>
#include <iostream>
#include <vector>

#include "stb_include.h"

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color( const ray& r, const hittable& world )
{
    hit_record rec;
    if ( world.hit( r, interval( 0, infinity ), rec ) ) {
        return 0.5 * ( rec.normal + color( 1, 1, 1 ) );
    }

    vec3 unit_direction = unit_vector( r.direction() );
    auto a = 0.5 * ( unit_direction.y() + 1.0 );
    return ( ( 1.0 - a ) * color( 1.0, 1.0, 1.0 ) ) + ( a * color( 0.5, 0.7, 1.0 ) );
}

int main( void )
{
    auto aspect_ratio = 16.0 / 9.0;
    int  image_width  = 400;
    int  image_height = int( image_width / aspect_ratio );
    image_height = image_height < 1 ? 1 : image_height;

    hittable_list world;
    world.add( make_shared<sphere>( point3( 0,      0, -1 ), 0.5 ) );
    world.add( make_shared<sphere>( point3( 0, -100.5, -1 ), 100 ) );

    auto focal_length    = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width  = viewport_height * ( double( image_width ) / image_height );
    auto camera_center   = point3( 0, 0, 0 );

    /* Horizontal and vertical viewport edge vectors */
    auto viewport_u = vec3( viewport_width, 0, 0 );
    auto viewport_v = vec3( 0, -viewport_height, 0 );

    /* Horizontal and vertical pixel-to=pixel delta vectors */
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    auto viewport_upper_left = camera_center
                               - vec3( 0, 0, focal_length )
                               - viewport_u/2
                               - viewport_v/2;

    auto pixel_0_0_location = viewport_upper_left
                              + ( 0.5 * ( pixel_delta_u + pixel_delta_v ) );

    std::vector<unsigned char> pixels(image_width * image_height * 3);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for ( int i = 0; i < image_height; ++i ) {
        std::clog << "\rScanlines remaining: "
                  << ( image_height - i ) << ' ' << std::flush;

        for ( int j = 0; j < image_width; ++j ) {
            auto pixel_center = pixel_0_0_location
                                + ( j * pixel_delta_u )
                                + ( i * pixel_delta_v );
            auto ray_direction = pixel_center - camera_center;
            ray r( camera_center, ray_direction );

            size_t pixel_index = ( i * image_width + j ) * 3;
            auto   pixel_color = ray_color( r, world );

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
