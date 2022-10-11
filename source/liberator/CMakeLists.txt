## ========================================================
##
## FILE:			/source/liberator/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Liberator CMake project definition.
##
## LAST EDITED:		October 10th, 2022
##
## ========================================================

set(LIBERATOR_SRC
	main.c
)

add_executable(liberator ${LIBERATOR_SRC})

set_target_properties(liberator
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
)
