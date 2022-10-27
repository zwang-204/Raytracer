#ifndef AARECT_H
#define AARECT_H

#include "pbrt.h"

#include "hittable.h"

namespace pbrt{

class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(float _x0, float _x1, float _y0, float _y1, float _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Bounds3f& output_box) const override {
            output_box = Bounds3f(Point3f(x0, y0, k-0.0001), Point3f(x1, y1, k+0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float x0, x1, y0, y1, k;
};

class xz_rect : public hittable {
    public:
        xz_rect() {}

        xz_rect(float _x0, float _x1, float _z0, float _z1, float _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Bounds3f& output_box) const override {
            output_box = Bounds3f(Point3f(x0, k-0.0001, z0), Point3f(x1, k+0.0001, z1));
            return true;
        }

        virtual float pdf_value(const Point3f& origin, const Vector3f& v) const override {
            hit_record rec;
            if (!this->hit(Ray(origin, v), 0.001, Infinity, rec))
                return 0;
            
            auto area = (x1-x0)*(z1-z0);
            auto distance_squared = rec.t * rec.t * v.LengthSquared();
            auto cosine = fabs(Dot(v, rec.normal) / v.Length());

            return distance_squared / (cosine * area);
        }

        virtual Vector3f random(const Point3f& origin) const override {
            auto random_point = Point3f(random_float(x0, x1), k, random_float(z0, z1));
            return random_point - origin;
        }

    public:
        shared_ptr<material> mp;
        float x0, x1, z0, z1, k;
};

class yz_rect : public hittable {
    public:
        yz_rect() {}

        yz_rect(float _y0, float _y1, float _z0, float _z1, float _k,
            shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
        
        virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(Bounds3f& output_box) const override {
            output_box = Bounds3f(Point3f(k-0.0001, y0, z0), Point3f(k+0.0001, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float y0, y1, z0, z1, k;
};

bool xy_rect::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    auto t = (k-r.origin().z) / r.direction().z;
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x + t*r.direction().x;
    auto y = r.origin().y + t*r.direction().y;
    if(x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

bool xz_rect::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    auto t = (k-r.origin().y) / r.direction().y;
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x + t*r.direction().x;
    auto z = r.origin().z + t*r.direction().z;
    if(x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vector3f(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

bool yz_rect::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    auto t = (k-r.origin().x) / r.direction().x;
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y + t*r.direction().y;
    auto z = r.origin().z + t*r.direction().z;
    if(y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outward_normal = Vector3f(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r(t);
    return true;
}

}

#endif