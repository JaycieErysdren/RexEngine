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
## LAST EDITED:		January 5th, 2023
##
## ========================================================

## Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)

## Add platform library
add_library(rex_platform STATIC "platform/sdl.cpp")

## SDL2
target_include_directories(rex_platform PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(rex_platform PUBLIC ${SDL2_LIBRARIES})

## SDL2_image
target_include_directories(rex_platform PUBLIC ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(rex_platform PUBLIC ${SDL2_IMAGE_LIBRARIES})

## OpenGL
target_include_directories(rex_platform PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(rex_platform PUBLIC ${OPENGL_LIBRARIES})

## GLFW
target_link_libraries(rex_platform PUBLIC glfw)
