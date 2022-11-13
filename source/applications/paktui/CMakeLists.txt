## ========================================================
##
## FILE:			/source/applications/paktui/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		PAKTUI CMake project definition.
##
## LAST EDITED:		November 12th, 2022
##
## ========================================================

project(PAKTUI)

set(PAKTUI_SRC
	paktui.c
)

add_executable(paktui ${PAKTUI_SRC})
