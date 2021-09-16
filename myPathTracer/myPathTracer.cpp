// /* 
//  * Below is a simple Monte-Carlo Path Tracer w/ explicit lights in C++
//  *
//  * Resources used:  Ray-tracing in one weekend series, 
//  *                  CG Principles and Practice (3rd ed.)
//  *                  learncpp.com, smallpt, smallpaint
//  */

#include "camera.h"
#include "geometry.h"
#include "image.h"
#include "material.h"
#include "primitives.h"
#include "scene.h"

#include <iostream>
#include <chrono>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;

typedef std::chrono::high_resolution_clock Clock;

Vec3 trace(Ray &r, Scene &scene, int depth) {
    Vec3 color{0,0,0};
    if (depth <= 0) { return color; }
    float t{};
    shared_ptr<Primitive> p;
    // Russian Roulette for early termination
    float rr = 1.0;
    if (depth <= 15) {
        const float rrStop = 0.1f;
        if (randf() <= rrStop) {
            return color;
        }
        rr = 1.0f / (1.0f - rrStop);
    }

    if (scene.intersect(r, t, p)) {
        Point3 hp = r.getPointAtDistance(t);
        Vec3 norm = p->normalAt(hp);
        Vec3 normal = (norm.dot(r.direction) < 0) ? norm : (norm * -1);
        auto mat = p->getMaterial();
        Vec3 attenuation;
        if (mat->scatter(r, hp, normal, t, attenuation)) {
            return mat->emission + attenuation * trace(r, scene, depth--) * rr;
        }
    }
    color = Vec3(0.75, 0.75, 0.75);
    return color;
}
int main()
{
    // pathtracing properties
    int depth = 10;
    int samples = 4096;

    // image properties
    int w = 1080;
    int h = 540;
    std::string image_name = "images/my_image.png";
    Image img(w, h, image_name);

    // camera properties - lookfrom, lookat, up, w, h, fovy
    Point3 lookfrom(0,0,0);
    Point3 lookat(0,0,-1);
    Vec3   up(0, 1, 0);
    float fovy{24};
    Camera cam(lookfrom, lookat, up, w, h, fovy);

    // Scene
    Scene scene;
    // materials - params are albedo, emission, roughness, Type
    auto dif =   make_shared<Material>(Vec3(0.75f, 0.75f, 0.75f), Vec3(), 0,     DIFFUSE); 
    auto dif2 =  make_shared<Material>(Vec3(0.25f, 0.75f, 0.55f), Vec3(), 0,     DIFFUSE);
    auto spec =  make_shared<Material>(Vec3(0.85f, 0.85f, 0.85f), Vec3(), 0.35f, SPECULAR);
    auto spec2 = make_shared<Material>(Vec3(0.85f, 0.6f, 0.3f),   Vec3(), 0.55f, SPECULAR);
    auto refr =  make_shared<Material>(Vec3(),                    Vec3(), 0,     REFRACTIVE);
    auto emit =  make_shared<Material>(Vec3(), Vec3(50, 50, 50),          0,     DIFFUSE); // light

    // a sphere has position, radius, emission, and type 
    scene.add(make_shared<Sphere>(Point3(0.0, 0.0, -6), 0.55,  spec));      // sphere in middle
    scene.add(make_shared<Sphere>(Point3(-1.0, -0.5, -6), 0.2, dif2));      // left
    scene.add(make_shared<Sphere>(Point3(1.0, -0.5, -6), 0.2,  refr));      // right
    scene.add(make_shared<Sphere>(Point3(0.0, -0.5, -5.0), 0.2, spec2));    // forward
    scene.add(make_shared<Sphere>(Point3(0.0, -4.5, -6), 4.0,   dif));      // "ground"
    scene.add(make_shared<Sphere>(Point3(0.9f, 0.5f, -6), 0.3,  emit));     // light source

    auto start_time = Clock::now();
    // loop through each pixel in the image 
    for (int j = h-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < w; ++i) {
            Vec3 color{};
            // loop to generate samples
            for (int s = 0; s < samples; ++s) {
               Ray primary_ray = cam.rayThroughPixel(i, j);
               color += trace(primary_ray, scene, depth);
            }
            color /= float(samples);
            // write color to image
            img.writeToPixel(i, j, color);
        }
    }
    img.saveImage();
    auto end_time = Clock::now();

    std::cout << '\n' << image_name << " successfully saved." << '\n';
    std::cout << "The render took " 
              << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count()
              << " seconds.\n";
}