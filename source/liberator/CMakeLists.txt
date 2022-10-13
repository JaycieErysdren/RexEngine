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
## LAST EDITED:		October 12th, 2022
##
## ========================================================

set(LIBERATOR_SRC
	main.c
)

add_executable(liberator ${LIBERATOR_SRC})

target_link_libraries(liberator rex)

