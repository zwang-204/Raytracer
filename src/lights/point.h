#ifndef LIGHTS_POINT_H
#define LIGHTS_POINT_H

// lights/point.h*
#include "pbrt.h"
#include "light.h"
#include "shape.h"
#include "paramset.h"

namespace pbrt {

// PointLight Declarations
class PointLight : public Light {
  public:
    // PointLight Public Methods
    PointLight(const Transform &LightToWorld,
               const MediumInterface &mediumInterface, const Spectrum &I)
        : Light((int)LightFlags::DeltaPosition, LightToWorld, mediumInterface),
          pLight(LightToWorld(Point3f(0, 0, 0))),
          I(I) {}
    Spectrum Sample_Li(const Interaction &ref, const Point2f &u, Vector3f *wi,
                       float *pdf, VisibilityTester *vis) const;
    Spectrum Power() const;
    float Pdf_Li(const Interaction &, const Vector3f &) const;
    Spectrum Sample_Le(const Point2f &u1, const Point2f &u2, float time,
                       Ray *ray, Normal3f *nLight, float *pdfPos,
                       float *pdfDir) const;
    void Pdf_Le(const Ray &, const Normal3f &, float *pdfPos,
                float *pdfDir) const;

  private:
    // PointLight Private Data
    const Point3f pLight;
    const Spectrum I;
};

std::shared_ptr<PointLight> CreatePointLight(const Transform &light2world,
                                             const Medium *medium,
                                             const ParamSet &paramSet);

}  // namespace pbrt

#endif  // PBRT_LIGHTS_POINT_H
