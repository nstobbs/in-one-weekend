// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#include "vec3.h"
#include "color.h"
#include <iostream>

int main()
{
    // Image
    int imagePlaneHeight = 256;
    int imagePlaneWidth = 256;

    // Render
    std::cout << "P3\n" << imagePlaneWidth << ' ' << imagePlaneHeight << "\n255\n";
    for (int y = 0; y < imagePlaneHeight; y++)
    {
        std::clog << "\rScanlines Left: " << (imagePlaneHeight - y) << ' ' << std::flush;
        for (int x = 0; x < imagePlaneWidth; x++)
        {
            auto pixelColor = color(float(x)/(imagePlaneWidth-1), float(y)/(imagePlaneHeight-1), 0);
            writeColor(std::cout, pixelColor);
        };
    };

    std::clog << "\rDone.                 \n";
}