#ifndef ONB_H
#define ONB_H

#include "pbrt.h"
#include "geometry.h"

namespace pbrt{

class onb {
    public:
        onb() {}

        inline Vector3f operator[](int i) const { return axis[i]; }

        Vector3f u() const { return axis[0]; }
        Vector3f v() const { return axis[1]; }
        Vector3f w() const { return axis[2]; }

        Vector3f local(double a, double b, double c) const {
            return a*u() + b*v() + c*w();
        }

        Vector3f local(const Vector3f& a) const {
            return a.x*u() + a.y*v() + a.z*w();
        }

        void build_from_w(const Vector3f&);
    
    public:
        Vector3f axis[3];
};

inline void onb::build_from_w(const Vector3f& n) {
    axis[2] = Normalize(n);
    Vector3f a = (fabs(w().x) > 0.9) ? Vector3f(0,1,0) : Vector3f(1,0,0);
    axis[1] = Normalize(Cross(w(), a));
    axis[0] = Cross(w(), v());
}

}

#endif