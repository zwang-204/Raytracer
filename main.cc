#include "pbrt.h"
#include "spectrum.h"
#include "hittable.h"
#include "material.h"
#include "pdf.h"
#include "color.h"

#include "paramset.h"
//#include "box.h"
#include "primitive.h"
#include "camera.h"
#include "bvh.h"
#include "sphere.h"

#include "stats.h"
#include "parallel.h"

#include <iostream>

using namespace pbrt;

Spectrum ray_color(
    const Ray& r,
    const Spectrum& background, 
    const shared_ptr<Primitive>& world,
    int depth
){
    SurfaceInteraction si;

    if (depth <= 0)
        return Spectrum(0.0);
    if (!world->Intersect(r, &si)) 
        return background;
    scatter_record srec;
    Spectrum emitted = si.primitive->GetMaterial()->emitted(r, si, si.uv[0], si.uv[1], si.p);
    if(!si.primitive->GetMaterial()->scatter(r, si, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
            * ray_color(srec.specular_ray, background, world, depth-1);
    }
    
    // auto light_ptr = make_shared<hittable_pdf>(lights, si.p);
    // mixture_pdf p(light_ptr, srec.pdf_ptr);

    Ray scattered = Ray(si.p, Random_in_unit_sphere());
    // auto pdf_val = p.value(scattered.direction());

    return emitted + srec.attenuation * si.primitive->GetMaterial()->scatter(r, si, srec)
                            * ray_color(scattered, background, world, depth-1);
}

// hittable_list cornell_box() {
//     hittable_list objects;

//     float redCol[3] = {.65, .05, .05};
//     Spectrum redSpec(0.0);
//     float whiteCol[3] = {.73, .73, .73};
//     Spectrum whiteSpec(0.0);
//     float greenCol[3] = {.12, .45, .15};
//     Spectrum greenSpec(0.0);
//     float lightCol[3] = {15, 15, 15};
//     Spectrum lightSpec(0.0);

//     auto red = make_shared<lambertian>(redSpec.FromRGB(redCol));
//     auto white = make_shared<lambertian>(whiteSpec.FromRGB(whiteCol));
//     auto green = make_shared<lambertian>(greenSpec.FromRGB(greenCol));
//     auto light = make_shared<diffuse_light>(lightSpec.FromRGB(lightCol));

//     objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
//     objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
//     objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
//     objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
//     objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
//     objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
    
//     shared_ptr<hittable> box1 = make_shared<box>(Point3f(0,0,0), Point3f(165,330,165), white);
//     box1 = make_shared<rotate_y>(box1, 15);
//     box1 = make_shared<translate>(box1, Vector3f(265,0,295));
//     objects.add(box1);

//     shared_ptr<hittable> box2 = make_shared<box>(Point3f(0,0,0), Point3f(165,165,165), white);
//     box2 = make_shared<rotate_y>(box2, -18);
//     box2 = make_shared<translate>(box2, Vector3f(130,0,65));
//     objects.add(box2);
//     //auto glass = make_shared<dielectric>(1.5);
//     //objects.add(make_shared<cylinder>(90, point3(190,90,190),0, 90, 2*pi, red));

//     return objects;
// }

std::shared_ptr<Primitive> test_sphere() {

    std::vector<std::shared_ptr<Primitive>> objects;
    std::shared_ptr<Primitive> accel;
    ParamSet paramSet;

    float lightCol[3] = {15, 15, 15};
    Spectrum lightSpec(0.0);

    Transform ObjectToWorld;
    Transform WorldToObject;

    std::shared_ptr<Material> light = make_shared<diffuse_light>(lightSpec.FromRGB(lightCol));
    std::shared_ptr<Shape> sphere = CreateSphereShape(&ObjectToWorld, &WorldToObject, false, paramSet);
    std::shared_ptr<GeometricPrimitive> light_sphere = make_shared<GeometricPrimitive>(sphere, light);
    objects.push_back(light_sphere);
    std::shared_ptr<Primitive> bvh = CreateBVHAccelerator(objects, paramSet);
    return objects.at(0);
}

int main(){

    ParallelInit();
    InitProfiler();

    // Image
    auto aspect_ratio = 1.0;
    int image_width = 200;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 30;
    const int max_depth = 5;

    // World
    // hittable_list world = cornell_box();
    std::shared_ptr<Primitive> world = test_sphere();
    // size_t start = 0;
    // size_t end = world.objects.size() -1 ;
    // auto world_bvh = bvh_node(world.objects, start, end);
    
    // Camera

    Point3f lookfrom(278, 278, -800);
    Point3f lookat(278,278,0);
    Vector3f vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    Spectrum background(0.0);

    camera cam(lookfrom, lookat, vup, 40.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    
    // Render
    std::cout<<"P3/n"<< image_width << ' ' << image_height << "\n255\n";
    
    for(int j = image_height-1; j>=0; j--){
        std::cerr << "\rScanlines remaining: "<< j <<' '<<std::flush;
        for(int i = 0; i<image_width; i++){
            Spectrum pixel_color(0.0);
            for (int s = 0; s < samples_per_pixel; ++s){
                auto u = (i + pbrt::random_float()) / (image_width - 1);
                auto v = (j + pbrt::random_float()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr<<"\nDone.\n";
}