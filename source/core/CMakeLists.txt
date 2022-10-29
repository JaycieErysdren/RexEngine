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
## LAST EDITED:		October 29th, 2022
##
## ========================================================

set(REX_FORMATS_SRC
	formats/formats.c
	formats/lev.c
	formats/mdl.c
	formats/pak.c
	formats/tmf.c
	formats/wal.c
)

set(REX_IO_SRC
	io/devices.c
	io/files.c
	io/resources.c
	io/tinyfd.c
)

set(REX_LOGIC_SRC
	logic/math.c
	logic/startup.c
	logic/shutdown.c
)

set(REX_PHYSICS_SRC
	physics/physics.c
)

set(REX_RENDERING_SRC
	rendering/brender.c
	rendering/nuklear.c
	rendering/palettes.c
	rendering/window.c
)

set(REX_UTILITIES_SRC
	utilities/endian.c
	utilities/failure.c
)

add_library(RexEngine ${REX_FORMATS_SRC} ${REX_IO_SRC} ${REX_LOGIC_SRC} ${REX_PHYSICS_SRC} ${REX_RENDERING_SRC} ${REX_UTILITIES_SRC})
