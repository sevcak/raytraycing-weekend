#include <cmath>

#include "color.h"
#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"

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
    hittable_list world;

    auto mat_ground = make_shared<lambertian>( color( 0.8, 0.8, 0.0 ) );
    auto mat_center = make_shared<lambertian>( color( 0.1, 0.2, 0.5 ) );
    auto mat_left   = make_shared<metal>(      color( 0.8, 0.8, 0.8 ) );
    auto mat_right  = make_shared<metal>(      color( 0.8, 0.6, 0.2 ) );

    world.add( make_shared<sphere>( point3(  0.0, -100.6, -1.0 ), 100.0, mat_ground ) );
    world.add( make_shared<sphere>( point3(  0.0,    0.0, -1.2 ),   0.5, mat_center ) );
    world.add( make_shared<sphere>( point3( -1.0,    0.0, -1.0 ),   0.5, mat_left ) );
    world.add( make_shared<sphere>( point3(  1.0,    0.0, -1.0 ),   0.5, mat_right ) );

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    return cam.render( world );
}

