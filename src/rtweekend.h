#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <memory>

using std::make_shared;
using std::shared_ptr;


/* Common headers */

#include "constants.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "interval.h"


/* Utility functions */

inline double degrees_to_radians( double degrees )
{
    return degrees * pi / 180.0;
}

#endif