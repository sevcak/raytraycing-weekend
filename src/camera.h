#ifndef CAMERA_H
#define CAMERA_H

#include "stb_include.h"

#include "rtweekend.h"
#include "hittable.h"
#include "vec3.h"


class camera
{
public:
    double aspect_ratio = 1.0;
    int    image_width  = 100;

    int render( const hittable& world )
    {
        initialize();

        std::vector<unsigned char> pixels(image_width * image_height * 3);

        for ( int i = 0; i < image_height; ++i ) {
            std::clog << "\rScanlines remaining: "
                      << ( image_height - i ) << ' ' << std::flush;

            for ( int j = 0; j < image_width; ++j ) {
                auto pixel_center = pixel_0_0_location
                                    + ( j * pixel_delta_u )
                                    + ( i * pixel_delta_v );
                auto ray_direction = pixel_center - center;
                ray r( center, ray_direction );

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

private:
    int    image_height;
    point3 center;
    point3 pixel_0_0_location;
    vec3   pixel_delta_u;
    vec3   pixel_delta_v;


    void initialize( void )
    {
        image_height = int( image_width / aspect_ratio );
        image_height = image_height < 1 ? 1 : image_height;

        center = point3( 0, 0, 0 );

        auto focal_length    = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width  = viewport_height * ( double( image_width ) / image_height );

        auto viewport_u = vec3( viewport_width, 0, 0 );
        auto viewport_v = vec3( 0, -viewport_height, 0 );

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center
                                   - vec3( 0, 0, focal_length )
                                   - viewport_u/2
                                   - viewport_v/2;
        pixel_0_0_location = viewport_upper_left
                             + ( 0.5 * ( pixel_delta_u + pixel_delta_v ) );
    }

    color ray_color( const ray& r, const hittable& world ) const
    {
        hit_record rec;

        if ( world.hit( r, interval( 0, infinity ), rec ) ) {
            return 0.5 * ( rec.normal + color( 1, 1, 1 ) );
        }


        vec3 unit_direction = unit_vector( r.direction() );
        auto a = 0.5 * ( unit_direction.y() + 1.0 );

        return ( 1.0 - a ) * color( 1.0, 1.0, 1.0 ) + ( a * color( 0.5, 0.7, 1.0 ) );
    }
};

#endif
