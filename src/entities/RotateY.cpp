#include "RotateY.hpp"

RotateY::RotateY(std::shared_ptr<Entity> p, float angle) : _ptr(p)
{
    auto radians = degrees_to_radians(angle);
    _sin_theta = sin(radians);
    _cos_theta = cos(radians);
    _hasbox = _ptr->bounding_box(0, 1, _bbox);

    Vector3 min(infinity, infinity, infinity);
    Vector3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                auto x = i * _bbox.max().x() + (1 - i) * _bbox.min().x();
                auto y = j * _bbox.max().y() + (1 - j) * _bbox.min().y();
                auto z = k * _bbox.max().z() + (1 - k) * _bbox.min().z();

                auto newx = _cos_theta * x + _sin_theta * z;
                auto newz = -_sin_theta * x + _cos_theta * z;

                Vector3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++)
                {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    _bbox = AABB(min, max);
}

bool RotateY::hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const
{
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = _cos_theta * r.origin()[0] - _sin_theta * r.origin()[2];
    origin[2] = _sin_theta * r.origin()[0] + _cos_theta * r.origin()[2];

    direction[0] = _cos_theta * r.direction()[0] - _sin_theta * r.direction()[2];
    direction[2] = _sin_theta * r.direction()[0] + _cos_theta * r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

    if (!_ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = _cos_theta * rec.p[0] + _sin_theta * rec.p[2];
    p[2] = -_sin_theta * rec.p[0] + _cos_theta * rec.p[2];

    normal[0] = _cos_theta * rec.normal[0] + _sin_theta * rec.normal[2];
    normal[2] = -_sin_theta * rec.normal[0] + _cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

bool RotateY::bounding_box(float t0, float t1, AABB &output_box) const
{
    output_box = _bbox;
    return _hasbox;
}