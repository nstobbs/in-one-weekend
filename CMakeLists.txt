cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
project(inOneWeekend)

find_package(OpenEXR REQUIRED)

add_executable(inOneWeekend src/main.cpp
                src/vec3.h
                src/color.h
                src/hittable.h
                src/hittable_list.h
                src/rtweekend.h
                src/sphere.h
                src/interval.h
                src/camera.h )

target_link_libraries(inOneWeekend OpenEXR::OpenEXR)