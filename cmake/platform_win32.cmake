## ========================================================
##
## FILE:			/cmake/platform_win32.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Rex Engine WIN32 platform information
##
## LAST EDITED:		January 2nd, 2023
##
## ========================================================

add_library(rex_platform STATIC "platform/win32.cpp")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "I86")

	set(REX_ARCH_LIBS "win386")

endif()
