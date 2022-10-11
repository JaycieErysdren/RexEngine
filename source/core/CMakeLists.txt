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
## LAST EDITED:		October 10th, 2022
##
## ========================================================

set(REX_SRC
	io/devices.c
	logic/startup.c
	logic/shutdown.c
	rendering/window.c
	utilities/failure.c
)

add_library(rex ${REX_SRC})

set_target_properties(rex
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../build"
)
