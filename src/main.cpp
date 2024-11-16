// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    hittable_list world;

    auto materialGround = make_shared<diffuse>(color(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<diffuse>(color(0.1, 0.2, 0.5));
    auto materialLeft = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
    auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0,-100.5, -1), 100, materialGround));
    world.add(make_shared<sphere>(point3(0,0,-1.2), 0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1.0,0,-1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(1.0,0,-1.0), 0.5, materialRight));

    camera cam;
    cam.aspectRatio = 16.0 / 16.0;
    cam.imagePlaneWidth = 1000;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    //cam.render(world);
    cam.parallelRender(world);
}