#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera
{
    public:
        double aspectRatio = 1.0;
        int imagePlaneWidth = 100;
        int samplesPerPixel = 1; //TODO increasing this value bricks the renders

        void render(const hittable& world)
        {
            initialize();

            std::cout << "P3\n" << imagePlaneWidth << ' ' << imagePlaneHeight << "\n255\n";

            for (int y = 0; y < imagePlaneHeight; y++)
            {
                std::clog << "\rScanlines Left: " << (imagePlaneHeight - y) << ' ' << std::flush;
                for (int x = 0; x < imagePlaneWidth; x++)
                {
                    color pixelColor (0,0,0);
                    for (int sampleID = 0; sampleID < samplesPerPixel; sampleID++)
                    {
                        ray r = getRay(x, y);
                        pixelColor += rayColor(r, world);
                    }
                    writeColor(std::cout, pixelSampleScale * pixelColor);
                };
            };

            std::clog << "\rDone.                 \n";
        }
    private:
        int imagePlaneHeight;
        int pixelSampleScale;
        point3 cameraCenter;
        point3 pixel_00_loc;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize()
        {
            imagePlaneHeight = int(imagePlaneWidth / aspectRatio);
            imagePlaneHeight = (imagePlaneHeight < 1) ? 1 : imagePlaneHeight;

            pixelSampleScale = 1.0 / samplesPerPixel;

            cameraCenter = point3 (0,0,0);

            auto focalLength = 1.0;
            auto viewportHeight = 2.0;
            auto viewportWidth = viewportHeight * (float(imagePlaneWidth)/imagePlaneHeight);

            auto viewportU = vec3(viewportWidth, 0, 0);
            auto viewportV = vec3(0, -viewportHeight, 0);

            pixelDeltaU = viewportU / imagePlaneWidth;
            pixelDeltaV = viewportV / imagePlaneHeight;

            auto viewportUpperLeft = cameraCenter - vec3(0,0, focalLength) - viewportU/2 - viewportV/2;
            pixel_00_loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

        };

        ray getRay(int x, int y) const
        {
            auto offset = sampleSquare();
            auto pixelSample = pixel_00_loc + ((x + offset.x()) * pixelDeltaU) + ((y + offset.y()) * pixelDeltaV);
            auto rayOrigin = cameraCenter;
            auto rayDirection = pixelSample - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        vec3 sampleSquare() const
        {
            return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
        }

        color rayColor(const ray& r, const hittable& world)
        {
            hitRecord rec;
            if(world.hit(r, interval(0, infinity), rec))
            {
                return 0.5 * (rec.normal + color(1,1,1));
            }

            vec3 unitDirection = unitVector(r.direction());
            auto a = 0.5*(unitDirection.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        };
};

#endif