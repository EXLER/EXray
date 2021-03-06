#include "AABB.hpp"

AABB::AABB() {}
AABB::AABB(const Vector3 &a, const Vector3 &b) : _min(a), _max(b) {}

Vector3 AABB::min() const
{
    return _min;
}
Vector3 AABB::max() const
{
    return _max;
}

bool AABB::hit(const Ray &r, float tmin, float tmax) const
{
    for (int a = 0; a < 3; a++)
    {
        auto invD = 1.0f / r.direction()[a];
        auto t0 = (min()[a] - r.origin()[a]) * invD;
        auto t1 = (max()[a] - r.origin()[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = fmin(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin)
            return false;
    }
    return true;
}

AABB AABB::surrounding_box(AABB box0, AABB box1)
{
    Vector3 small(fmin(box0.min().x(), box1.min().x()),
                  fmin(box0.min().y(), box1.min().y()),
                  fmin(box0.min().z(), box1.min().z()));

    Vector3 big(fmax(box0.max().x(), box1.max().x()),
                fmax(box0.max().y(), box1.max().y()),
                fmax(box0.max().z(), box1.max().z()));

    return AABB(small, big);
}