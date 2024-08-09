#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <memory>

using std::make_shared;
using std::shared_ptr;


/* Common headers */

#include "constants.h"

/* Utility functions */

inline double degrees_to_radians( double degrees )
{
    return degrees * pi / 180.0;
}

inline double random_double( void )
{
    return std::rand() / ( RAND_MAX + 1.0 );
}

inline double random_double( double min, double max )
{
    return min + ( ( max - min ) * random_double() );
}

#endif
