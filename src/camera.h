#ifndef CAMERA_H
#define CAMERA_H

#include "stb_include.h"

#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio      = 1.0;
    int    image_width       = 100;
    int    samples_per_pixel = 10;
    int    max_depth         = 10;

    double v_fov     = 90;
    point3 look_from = point3( 0, 0,  0 );
    point3 look_at   = point3( 0, 0, -1 );
    vec3   v_up      = vec3( 0, 1, 0 );

    double defocus_angle  = 0;
    double focus_distance = 10;

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

        const char* filename = "image.png";
        int result = stbi_write_png( filename,
                                     image_width, image_height,
                                     3, pixels.data(), image_width * 3 );
        if ( result == 0 ) {
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
    vec3   pixel_delta_u;       /* Offset to pixel to the right */
    vec3   pixel_delta_v;       /* Offset to pixel below */
    vec3   u, v, w;             /* Camera frame basis vectors */
    vec3   defocus_disk_u;
    vec3   defocus_disk_v;


    void initialize( void )
    {
        image_height = int( image_width / aspect_ratio );
        image_height = image_height < 1 ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = look_from;

        auto theta           = degrees_to_radians( v_fov );
        auto h               = std::tan( theta/2 );
        auto viewport_height = 2 * h * focus_distance;
        auto viewport_width  = viewport_height * ( double( image_width ) / image_height );

        w = unit_vector( look_from - look_at );
        u = unit_vector( cross( v_up, w ) );
        v = cross( w, u );

        auto viewport_u = viewport_width  *  u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center
                                   - ( focus_distance * w )
                                   - viewport_u/2
                                   - viewport_v/2;
        pixel_0_0_location = viewport_upper_left
                             + ( 0.5 * ( pixel_delta_u + pixel_delta_v ) );

        auto defocus_radius =
            focus_distance * std::tan( degrees_to_radians( defocus_angle/2 ) );
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    /*
     * Constract a camera ray directed from the defocus disk,
     * directed a randomly sampled point around the pixel location «i», «j».
     */
    ray get_ray( int i, int j ) const
    {
        auto offset       = sample_square();
        auto pixel_sample = pixel_0_0_location
                            + ( ( i + offset.x() ) * pixel_delta_u )
                            + ( ( j + offset.y() ) * pixel_delta_v );

        auto ray_origin    = defocus_angle <= 0 ? center : defocus_disk_sample();
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

    point3 defocus_disk_sample() const
    {
        auto p = random_in_unit_disk();

        return center + ( p[0] * defocus_disk_u ) + ( p[1] * defocus_disk_v );
    }

    color ray_color( const ray& r, int depth, const hittable& world ) const
    {
        if ( depth <= 0 ) { return color( 0, 0, 0 ); }

        hit_record rec;

        if ( world.hit( r, interval( 0.001, infinity ), rec ) ) {
            ray   scattered;
            color attenuation;

            if ( rec.mat->scatter( r, rec, attenuation, scattered ) ) {
                return attenuation * ray_color( scattered, depth - 1, world );
            }

            return color( 0, 0, 0 );
        }


        vec3 unit_direction = unit_vector( r.direction() );
        auto a = 0.5 * ( unit_direction.y() + 1.0 );

        return ( 1.0 - a ) * color( 1.0, 1.0, 1.0 ) + ( a * color( 0.5, 0.7, 1.0 ) );
    }
};

#endif
