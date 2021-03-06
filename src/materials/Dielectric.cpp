#include "Dielectric.hpp"

Dielectric::Dielectric(float ri) : ref_idx(ri) {}

bool Dielectric::scatter(const Ray &r_in, const HitRecord &rec, Color3 &attenuation, Ray &scattered) const
{
    attenuation = Color3(1.0, 1.0, 1.0);
    float refractive_div = rec.front_face ? (1.0 / ref_idx) : ref_idx;

    Vector3 unit_direction = normalize(r_in.direction());

    float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if ((refractive_div * sin_theta > 1.0) || (random_float() < schlick(cos_theta, refractive_div)))
    {
        Vector3 reflected = Vector3::reflect(unit_direction, rec.normal);
        scattered = Ray(rec.p, reflected, r_in.time());
        return true;
    }

    Vector3 refracted = Vector3::refract(unit_direction, rec.normal, refractive_div);
    scattered = Ray(rec.p, refracted, r_in.time());
    return true;
}