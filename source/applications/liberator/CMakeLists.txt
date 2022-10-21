## ========================================================
##
## FILE:			/source/applications/liberator/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Liberator CMake project definition.
##
## LAST EDITED:		October 21st, 2022
##
## ========================================================

project(Liberator)

set(LIBERATOR_SRC
	liberator.c
)

add_executable(liberator ${LIBERATOR_SRC})
