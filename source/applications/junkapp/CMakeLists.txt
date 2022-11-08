## ========================================================
##
## FILE:			/source/applications/junkapp/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		JunkApp CMake project definition.
##
## LAST EDITED:		November 8th, 2022
##
## ========================================================

project(JunkApp)

set(JUNKAPP_SRC
	junkapp.c
)

add_executable(junkapp ${JUNKAPP_SRC})
