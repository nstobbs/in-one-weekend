#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

using color = vec3;

void writeColor(std::ostream& out, const color& pixelColor)
{
    auto red = pixelColor.x();
    auto green = pixelColor.y();
    auto blue = pixelColor.z();

    int redByte = int(255.999 * red);
    int greenByte = int(255.999 * green);
    int blueByte = int(255.999 * blue);

    out << redByte << ' ' << greenByte << ' ' << blueByte << '\n';
}

#endif