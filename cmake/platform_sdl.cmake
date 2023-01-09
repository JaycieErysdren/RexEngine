## ========================================================
##
## FILE:			/cmake/platform_sdl.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Rex Engine SDL platform information
##
## LAST EDITED:		January 8th, 2023
##
## ========================================================

## Find packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

## Add platform library
add_library(rex_platform STATIC "platform/sdl.cpp")

## SDL2
target_link_libraries(rex_platform PUBLIC SDL2::SDL2)

## OpenGL
target_link_libraries(rex_platform PUBLIC OpenGL::GL)
target_link_libraries(rex_platform PUBLIC OpenGL::GLU)

target_link_libraries(rex_platform PUBLIC usb-1.0)

## GLFW
target_link_libraries(rex_platform PUBLIC glfw)
