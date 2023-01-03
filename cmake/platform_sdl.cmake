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
## LAST EDITED:		January 2nd, 2023
##
## ========================================================

find_package(SDL2 REQUIRED)

add_library(rex_platform STATIC "platform/sdl.cpp")

target_include_directories(rex_platform PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(rex_platform PUBLIC ${SDL2_LIBRARIES})
