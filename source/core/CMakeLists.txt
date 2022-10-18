## ========================================================
##
## FILE:			/source/core/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Core CMake project definition.
##
## LAST EDITED:		October 18th, 2022
##
## ========================================================

set(REX_SRC
	formats/pak.c
	io/devices.c
	logic/startup.c
	logic/shutdown.c
	rendering/fonts.c
	rendering/palettes.c
	rendering/window.c
	utilities/failure.c
)

add_library(rex ${REX_SRC})
