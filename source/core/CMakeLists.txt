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
## LAST EDITED:		October 21st, 2022
##
## ========================================================

set(REX_ENGINE_SRC
	formats/formats.c
	formats/lev.c
	formats/mdl.c
	formats/pak.c
	formats/tmf.c
	io/devices.c
	io/files.c
	io/resources.c
	logic/math.c
	logic/startup.c
	logic/shutdown.c
	rendering/brender.c
	rendering/fonts.c
	rendering/palettes.c
	rendering/window.c
	utilities/endian.c
	utilities/failure.c
)

add_library(RexEngine ${REX_ENGINE_SRC})
