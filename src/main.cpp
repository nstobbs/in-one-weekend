// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

bool USE_THREADING = true;

int main()
{
    hittable_list world;

    auto materialGround = make_shared<diffuse>(color(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<diffuse>(color(0.1, 0.2, 0.5));
    auto materialLeft = make_shared<glass>(1.5);
    auto materialBubble = make_shared<glass>(1.00 / 1.50);
    auto materialRight = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3(0,-100.5, -1), 100, materialGround));
    world.add(make_shared<sphere>(point3(0,0,-1.2), 0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1.0,0,-1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, materialBubble));
    world.add(make_shared<sphere>(point3(1.0,0,-1.0), 0.5, materialRight));

    camera cam;
    cam.aspectRatio = 16.0 / 16.0;
    cam.imagePlaneWidth = 500;
    cam.samplesPerPixel = 500;
    cam.maxDepth = 250;
    
    cam.viewFov = 20;
    cam.lookFrom = point3(-2, 2, 1);
    cam.lookAt = point3(0,0, -1);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 10.0;
    cam.focusDist = 3.4;

    if (USE_THREADING)
    {
        cam.parallelRender(world);
    } else {
        cam.render(world);
    };
}