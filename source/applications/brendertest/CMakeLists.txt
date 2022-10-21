## ========================================================
##
## FILE:			/source/applications/brendertest/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		BRenderTest CMake project definition.
##
## LAST EDITED:		October 21st, 2022
##
## ========================================================

project(BRenderTest)

set(BRENDERTEST_SRC
	brendertest.c
)

add_executable(brendertest ${BRENDERTEST_SRC})
