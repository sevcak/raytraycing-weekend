#ifndef CAMERA_H
#define CAMERA_H

#include "stb_include.h"

#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include "hittable.h"

class camera
{
public:
    double aspect_ratio      = 1.0;
    int    image_width       = 100;
    int    samples_per_pixel = 10;
    int    max_depth         = 10;

    int render( const hittable& world )
    {
        initialize();

        std::vector<unsigned char> pixels(image_width * image_height * 3);

        for ( int i = 0; i < image_height; ++i ) {
            std::clog << "\rScanlines remaining: "
                      << ( image_height - i ) << ' ' << std::flush;

            for ( int j = 0; j < image_width; ++j ) {
                color pixel_color( 0, 0, 0 );

                for ( int sample = 0; sample < samples_per_pixel; ++sample ) {
                    ray r = get_ray( j, i );
                    pixel_color += ray_color( r, max_depth, world );
                }

                size_t pixel_index = ( i * image_width + j ) * 3;

                write_color( pixels, pixel_index, pixel_color * pixel_samples_scale );
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
    double pixel_samples_scale;
    point3 center;
    point3 pixel_0_0_location;
    vec3   pixel_delta_u;
    vec3   pixel_delta_v;


    void initialize( void )
    {
        image_height = int( image_width / aspect_ratio );
        image_height = image_height < 1 ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

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

    /*
     * Constract a camera ray directed from the origin at a randomly sampled point around
     * the pixel location «i», «j».
     */
    ray get_ray( int i, int j ) const
    {
        auto offset       = sample_square();
        auto pixel_sample = pixel_0_0_location
                            + ( ( i + offset.x() ) * pixel_delta_u )
                            + ( ( j + offset.y() ) * pixel_delta_v );

        auto ray_origin    = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray( ray_origin, ray_direction );
    }

    /*
     * Return the vector to a random point in the [-0.5, -0.5]-[0.5, 0.5] unit square.
     */
    vec3 sample_square( void ) const
    {
        return vec3( random_double() - 0.5, random_double() - 0.5, 0 );
    }

    color ray_color( const ray& r, int depth, const hittable& world ) const
    {
        if ( depth <= 0 ) { return color( 0, 0, 0 ); }

        hit_record rec;

        if ( world.hit( r, interval( 0.001, infinity ), rec ) ) {
            vec3 direction = rec.normal + random_unit_vector();
            return 0.5 * ( ray_color( ray( rec.p, direction ), depth - 1, world ) );
        }


        vec3 unit_direction = unit_vector( r.direction() );
        auto a = 0.5 * ( unit_direction.y() + 1.0 );

        return ( 1.0 - a ) * color( 1.0, 1.0, 1.0 ) + ( a * color( 0.5, 0.7, 1.0 ) );
    }
};

#endif
