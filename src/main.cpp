#include <cmath>

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"

int main( void )
{
    hittable_list world;

    auto material_ground = make_shared<lambertian>( color( 0.5, 0.5, 0.5 ) );
    world.add( make_shared<sphere>( point3( 0, -1000, 0 ), 1000, material_ground ) );

    for ( int a = -11; a < 11; ++a ) {
        for ( int b = -11; b < 11; ++b ) {
            auto choose_mat = random_double();
            point3 center( a + ( 0.9 * random_double() ),
                           0.2,
                           b + ( 0.9 * random_double() ) );

            if ( ( center - point3( 4, 0.2, 0 ) ).length() > 0.9 ) {
                shared_ptr<material> sphere_material;

                if ( choose_mat < 0.8 ) {
                    /* Diffuse */
                    auto albedo     = color::random() * color::random();
                    sphere_material = make_shared<lambertian>( albedo );
                } else if ( choose_mat < 0.95 ) {
                    /* Metal */
                    auto albedo     = color::random( 0.5, 1 );
                    auto fuzz       = random_double( 0, 0.5 );
                    sphere_material = make_shared<metal>( albedo, fuzz );
                } else {
                    /* Glass */
                    sphere_material = make_shared<dielectric>( 1.5 );
                }

                world.add( make_shared<sphere>( center, 0.2, sphere_material ) );
            }
        }
    }

    auto material_1 = make_shared<dielectric>( 1.5 );
    world.add( make_shared<sphere>( point3( 0, 1, 0 ), 1.0, material_1 ) );

    auto material_2 = make_shared<lambertian>( color( 0.4, 0.2, 0.1 ) );
    world.add( make_shared<sphere>( point3( -4, 1, 0 ), 1.0, material_2 ) );

    auto material_3 = make_shared<metal>( color( 0.7, 0.6, 0.5 ), 0.0 );
    world.add( make_shared<sphere>( point3( 4, 1, 0 ), 1.0, material_3 ) );

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.v_fov     = 20;
    cam.look_from = point3( 13, 2, 3);
    cam.look_at   = point3(  0, 0, 0);
    cam.v_up      = vec3( 0, 1, 0);

    cam.defocus_angle  =  0.6;
    cam.focus_distance = 10.0;

    cam.render(world);
}
