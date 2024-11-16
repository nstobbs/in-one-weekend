// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5, -1), 100));

    camera cam;
    cam.aspectRatio = 16.0 / 16.0;
    cam.imagePlaneWidth = 1000;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    //cam.render(world);
    std::cout << "Start parallelRender Func..." << std::endl;
    cam.parallelRender(world);
}