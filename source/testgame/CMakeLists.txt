## ========================================================
##
## FILE:			/source/testgame/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		TestGame CMake project definition.
##
## LAST EDITED:		October 20th, 2022
##
## ========================================================

set(TESTGAME_SRC
	testgame.c
)

add_executable(testgame ${TESTGAME_SRC})

target_link_libraries(testgame rex)

