#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "color.h"
#include "vec3.h"

class hit_record;

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation,
                          ray& scattered ) const
    {
        return false;
    }
};

class lambertian : public material
{
public:
    lambertian( const color& albedo ) : albedo( albedo ) {}

    bool scatter( const ray& r_in, const hit_record& rec, color& attentuation,
                  ray& scattered ) const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        if ( scatter_direction.near_zero() ) {
            scatter_direction = rec.normal;
        }

        scattered    = ray( rec.p, scatter_direction );
        attentuation = albedo;

        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal( const color& albedo, double fuzz )
        : albedo( albedo ), fuzz( fuzz < 1 ? fuzz : 1 ) {}

    bool scatter( const ray& r_in, const hit_record& rec, color& attenuation,
                  ray& scattered ) const override
    {
        vec3 reflected = reflect( r_in.direction(), rec.normal );
        reflected = unit_vector( reflected ) + ( fuzz * random_unit_vector() );

        scattered   = ray( rec.p, reflected );
        attenuation = albedo;

        return ( dot( scattered.direction(), rec.normal ) > 0 );
    }

private:
    color  albedo;
    double fuzz;
};

class dielectric : public material
{
public:
    dielectric( double refraction_index ) : refraction_index( refraction_index ) {}

    bool scatter( const ray& r_in, const hit_record& rec, color& attenuation,
                  ray& scattered ) const override
    {
        attenuation = color( 1.0, 1.0, 1.0 );
        double r_i = rec.front_face ? ( 1.0 / refraction_index ) : refraction_index;

        vec3 unit_direction = unit_vector( r_in.direction() );

        double cos_theta = std::fmin( dot( -unit_direction, rec.normal ), 1.0 );
        double sin_theta = std::sqrt( 1.0 - cos_theta*cos_theta );

        bool cannot_refract = r_i * sin_theta  > 1.0;
        vec3 direction = cannot_refract
            ? reflect( unit_direction, rec.normal )
            : refract( unit_direction, rec.normal, r_i );

        scattered = ray( rec.p, direction );

        return true;
    }

private:
    double refraction_index;
};

#endif
