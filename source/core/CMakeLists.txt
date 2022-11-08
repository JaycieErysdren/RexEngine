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
## LAST EDITED:		November 7th, 2022
##
## ========================================================

## Main engine module
set(REX_ENGINE_SRC
	engine/bootstrap.c
	engine/device_io.c
	engine/endian.c
	engine/error.c
	engine/file_io.c
	engine/log.c
	engine/math.c
	engine/resfile.c
)

## Formats module
set(REX_FORMATS_SRC
	formats/lev.c
	formats/mdl.c
	formats/pak.c
	formats/tmf.c
	formats/wal.c
)

## LIB I/O module
set(REX_LIBIO_SRC
	lib_io/brender.c
	lib_io/nuklear.c
	lib_io/tinyfd.c
)

## Physics module
set(REX_PHYSICS_SRC
	physics/physics.c
)

## Rendering module
set(REX_RENDERING_SRC
	rendering/palettes.c
	rendering/window.c
)

## Add Rex Engine library
add_library(RexEngine
	${REX_ENGINE_SRC}
	${REX_FORMATS_SRC}
	${REX_LIBIO_SRC}
	${REX_PHYSICS_SRC}
	${REX_RENDERING_SRC}
	${REX_WORLD_SRC}
)
