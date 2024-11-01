// https://raytracing.github.io/books/RayTracingInOneWeekend.html

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
        for (int x = 0; x < imagePlaneWidth; x++)
        {
            auto red = float(x) / (imagePlaneWidth - 1);
            auto green = float(y) / (imagePlaneHeight - 1);
            auto blue = 0.0f;

            int iRed = int(255.999 * red);
            int iGreen = int(255.999 * green);
            int iBlue = int(255.999 * blue);

            std::cout << iRed << ' ' << iGreen << ' ' << iBlue << '\n';
        };
    };
}