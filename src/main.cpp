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

    auto groundMaterial = make_shared<diffuse>(color(0.5,0.5,0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto chooseMat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());
            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphereMaterial;

                if (chooseMat < 0.8)
                {
                    auto albedo = color::random() * color::random();
                    sphereMaterial = make_shared<diffuse>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95){
                    auto albedo = color::random();
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                } else {
                    sphereMaterial = make_shared<glass>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<glass>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = make_shared<diffuse>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4,1,0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4,1,0), 1.0, material3));

    camera cam;
    cam.aspectRatio = 16.0 / 9.0;
    cam.imagePlaneWidth = 1200;
    cam.samplesPerPixel = 500;
    cam.maxDepth = 50;
    
    cam.viewFov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    if (USE_THREADING)
    {
        cam.parallelRender(world);
    } else {
        cam.render(world);
    };
}