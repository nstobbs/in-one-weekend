#ifndef CAMERA_H
#define CAMERA_H

// openEXR
#include <ImfRgbaFile.h>
#include <ImfArray.h>

// OneTBB
#include <tbb/parallel_for.h>

#include "hittable.h"

class camera
{
    public:
        double aspectRatio = 1.0;
        int imagePlaneWidth = 100;
        int samplesPerPixel = 10;
        int maxDepth = 10;

        void parallelRender(const hittable& world)
        {
            std::cout << "Starting..." << std::endl;
            initialize();
            Imf::Array2D<Imf::Rgba> frame(imagePlaneWidth, imagePlaneHeight);

            tbb::parallel_for(0, imagePlaneHeight, [&](int y){
                tbb::parallel_for(0, imagePlaneWidth, [&](int x){
                    calPixelColor(x, y, world, frame);
                });
            });

            std::cout << "Writing Out..." << std::endl;
            writeToOpenEXR(frame, imagePlaneWidth, imagePlaneHeight, "output.exr");
        };

        void render(const hittable& world)
        {
            initialize();

            //std::cout << "P3\n" << imagePlaneWidth << ' ' << imagePlaneHeight << "\n255\n";
            Imf::Array2D<Imf::Rgba> frame(imagePlaneWidth, imagePlaneHeight);
            Imf::Array2D<Imf::Rgba> testFrame(imagePlaneWidth, imagePlaneHeight);

            for (int y = 0; y < imagePlaneHeight; y++)
            {
                std::clog << "\rScanlines Left: " << (imagePlaneHeight - y) << ' ' << std::flush;
                for (int x = 0; x < imagePlaneWidth; x++)
                {
                    //std::cout << "On Pixel:" << x << ' ' << y << std::endl;
                    color pixelColor (0,0,0);
                    for (int sampleID = 0; sampleID < samplesPerPixel; sampleID++)
                    {
                        ray r = getRay(x, y);
                        pixelColor += rayColor(r, maxDepth, world);
                    }
                    auto finalPixel = pixelSampleScale * pixelColor;
                    //std::cout << "Writing pixel to pos: " << x << ' ' << y << std::endl;
                    frame[x][y] = Imf::Rgba(half(finalPixel.x()), half(finalPixel.y()), half(finalPixel.z()), 1.0); 
                    testFrame[x][y] = Imf::Rgba(x / (imagePlaneWidth-1.0f), y / (imagePlaneHeight-1.0f), 0.0);
                    //std::clog << "testFrame: " << testFrame.r() << 
                    //writeColor(std::cout, finalPixel);
                };
            };

            writeToOpenEXR(frame, imagePlaneWidth, imagePlaneHeight, "output.exr");
            writeToOpenEXR(testFrame, imagePlaneWidth, imagePlaneHeight, "test.exr");
            std::clog << "\rDone.                 \n";
        }
    private:
        int imagePlaneHeight;
        double pixelSampleScale;
        point3 cameraCenter;
        point3 pixel_00_loc;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize()
        {
            imagePlaneHeight = int(imagePlaneWidth / aspectRatio);
            imagePlaneHeight = (imagePlaneHeight < 1) ? 1 : imagePlaneHeight;

            std::cout << "Height: " << imagePlaneHeight<< std::endl;
            std::cout << "Width: " << imagePlaneWidth << std::endl;
            std::cout << "aspectRatio: " << aspectRatio << std::endl;

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

        color rayColor(const ray& r, int maxDepth, const hittable& world)
        {
            if (maxDepth <= 0)
            {
                return color(0,0,0);
            }
            hitRecord rec;
            if(world.hit(r, interval(0, infinity), rec))
            {
                vec3 direction = rec.normal + randomOnHemisphere(rec.normal);
                return 0.5 * rayColor(ray(rec.p, direction), maxDepth-1,world);
            }

            vec3 unitDirection = unitVector(r.direction());
            auto a = 0.5*(unitDirection.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        };

        void calPixelColor(int x, int y, const hittable& world, Imf::Array2D<Imf::Rgba>& frame)
        {
            color pixelColor (0,0,0);
            for (int sampleID = 0; sampleID < samplesPerPixel; sampleID++)
            {
                ray r = getRay(x, y);
                pixelColor += rayColor(r, maxDepth, world);
            }
            auto finalPixel = pixelSampleScale * pixelColor;
            frame[y][x] = Imf::Rgba(half(finalPixel.x()), half(finalPixel.y()), half(finalPixel.z()), 0.0);
        };
};

#endif