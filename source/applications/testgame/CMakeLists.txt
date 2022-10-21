## ========================================================
##
## FILE:			/source/applications/testgame/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		TestGame CMake project definition.
##
## LAST EDITED:		October 21st, 2022
##
## ========================================================

project(TestGame)

set(TESTGAME_SRC
	testgame.c
)

add_executable(testgame ${TESTGAME_SRC})
