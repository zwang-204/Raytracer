// textures/checkerboard.cpp*
#include "textures/checkerboard.h"

namespace pbrt {

// CheckerboardTexture Method Definitions
Texture<float> *CreateCheckerboardFloatTexture(const Transform &tex2world,
                                               const TextureParams &tp) {
    int dim = tp.FindInt("dimension", 2);
    if (dim != 2 && dim != 3) {
        Error("%d dimensional checkerboard texture not supported", dim);
        return nullptr;
    }
    std::shared_ptr<Texture<float>> tex1 = tp.GetFloatTexture("tex1", 1.f);
    std::shared_ptr<Texture<float>> tex2 = tp.GetFloatTexture("tex2", 0.f);
    if (dim == 2) {
        // Initialize 2D texture mapping _map_ from _tp_
        std::unique_ptr<TextureMapping2D> map;
        std::string type = tp.FindString("mapping", "uv");
        if (type == "uv") {
            float su = tp.FindFloat("uscale", 1.);
            float sv = tp.FindFloat("vscale", 1.);
            float du = tp.FindFloat("udelta", 0.);
            float dv = tp.FindFloat("vdelta", 0.);
            map.reset(new UVMapping2D(su, sv, du, dv));
        } else if (type == "spherical")
            map.reset(new SphericalMapping2D(Inverse(tex2world)));
        else if (type == "cylindrical")
            map.reset(new CylindricalMapping2D(Inverse(tex2world)));
        else if (type == "planar")
            map.reset(new PlanarMapping2D(
                tp.FindVector3f("v1", Vector3f(1, 0, 0)),
                tp.FindVector3f("v2", Vector3f(0, 1, 0)),
                tp.FindFloat("udelta", 0.f), tp.FindFloat("vdelta", 0.f)));
        else {
            Error("2D texture mapping \"%s\" unknown", type.c_str());
            map.reset(new UVMapping2D);
        }

        // Compute _aaMethod_ for _CheckerboardTexture_
        std::string aa = tp.FindString("aamode", "closedform");
        AAMethod aaMethod;
        if (aa == "none")
            aaMethod = AAMethod::None;
        else if (aa == "closedform")
            aaMethod = AAMethod::ClosedForm;
        else {
            Warning(
                "Antialiasing mode \"%s\" not understood by "
                "Checkerboard2DTexture; using \"closedform\"",
                aa.c_str());
            aaMethod = AAMethod::ClosedForm;
        }
        return new Checkerboard2DTexture<float>(std::move(map), tex1, tex2,
                                                aaMethod);
    } else {
        // Initialize 3D texture mapping _map_ from _tp_
        std::unique_ptr<TextureMapping3D> map(new IdentityMapping3D(tex2world));
        return new Checkerboard3DTexture<float>(std::move(map), tex1, tex2);
    }
}

Texture<Spectrum> *CreateCheckerboardSpectrumTexture(const Transform &tex2world,
                                                     const TextureParams &tp) {
    int dim = tp.FindInt("dimension", 2);
    if (dim != 2 && dim != 3) {
        Error("%d dimensional checkerboard texture not supported", dim);
        return nullptr;
    }
    std::shared_ptr<Texture<Spectrum>> tex1 =
        tp.GetSpectrumTexture("tex1", 1.f);
    std::shared_ptr<Texture<Spectrum>> tex2 =
        tp.GetSpectrumTexture("tex2", 0.f);
    if (dim == 2) {
        // Initialize 2D texture mapping _map_ from _tp_
        std::unique_ptr<TextureMapping2D> map;
        std::string type = tp.FindString("mapping", "uv");
        if (type == "uv") {
            float su = tp.FindFloat("uscale", 1.);
            float sv = tp.FindFloat("vscale", 1.);
            float du = tp.FindFloat("udelta", 0.);
            float dv = tp.FindFloat("vdelta", 0.);
            map.reset(new UVMapping2D(su, sv, du, dv));
        } else if (type == "spherical")
            map.reset(new SphericalMapping2D(Inverse(tex2world)));
        else if (type == "cylindrical")
            map.reset(new CylindricalMapping2D(Inverse(tex2world)));
        else if (type == "planar")
            map.reset(new PlanarMapping2D(
                tp.FindVector3f("v1", Vector3f(1, 0, 0)),
                tp.FindVector3f("v2", Vector3f(0, 1, 0)),
                tp.FindFloat("udelta", 0.f), tp.FindFloat("vdelta", 0.f)));
        else {
            Error("2D texture mapping \"%s\" unknown", type.c_str());
            map.reset(new UVMapping2D);
        }

        // Compute _aaMethod_ for _CheckerboardTexture_
        std::string aa = tp.FindString("aamode", "closedform");
        AAMethod aaMethod;
        if (aa == "none")
            aaMethod = AAMethod::None;
        else if (aa == "closedform")
            aaMethod = AAMethod::ClosedForm;
        else {
            Warning(
                "Antialiasing mode \"%s\" not understood by "
                "Checkerboard2DTexture; using \"closedform\"",
                aa.c_str());
            aaMethod = AAMethod::ClosedForm;
        }
        return new Checkerboard2DTexture<Spectrum>(std::move(map), tex1, tex2,
                                                   aaMethod);
    } else {
        // Initialize 3D texture mapping _map_ from _tp_
        std::unique_ptr<TextureMapping3D> map(new IdentityMapping3D(tex2world));
        return new Checkerboard3DTexture<Spectrum>(std::move(map), tex1, tex2);
    }
}

}  // namespace pbrt
