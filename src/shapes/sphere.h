#ifndef SHAPES_SPHERE_H
#define SHAPES_SPHERE_H

// shapes/sphere.h
#include "shape.h"
#include "geometry.h"
#include "paramset.h"
#include "sampling.h"

namespace pbrt {

class Sphere : public Shape {
  public:
    // Sphere Public Methods
    Sphere(const Transform *ObjectToWorld, const Transform *WorldToObject,
           bool reverseOrientation, float radius, float zMin, float zMax,
           float phiMax)
        : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
          radius(radius),
          zMin(Clamp(std::min(zMin, zMax), -radius, radius)),
          zMax(Clamp(std::max(zMin, zMax), -radius, radius)),
          thetaMin(std::acos(Clamp(std::min(zMin, zMax) / radius, -1, 1))),
          thetaMax(std::acos(Clamp(std::max(zMin, zMax) / radius, -1, 1))),
          phiMax(Radians(Clamp(phiMax, 0, 360))) {}
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    float Area() const;
    Interaction Sample(const Point2f &u, float *pdf) const;
    Interaction Sample(const Interaction &ref, const Point2f &u,
                       float *pdf) const;
    float Pdf(const Interaction &ref, const Vector3f &wi) const;
    float SolidAngle(const Point3f &p, int nSamples) const;

  private:
    // Sphere Private Data
    const float radius;
    const float zMin, zMax;
    const float thetaMin, thetaMax, phiMax;
};

std::shared_ptr<Shape> CreateSphereShape(const Transform *o2w,
                                         const Transform *w2o,
                                         bool reverseOrientation,
                                         const ParamSet &params);

}

#endif