## ========================================================
##
## FILE:			/cmake/rex_determinator.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Rex Engine platform & compiler determinator
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

##
## Target & host Platforms:
## 0: Linux
## 1: Windows
## 2: DOS
##

##
## Compilers:
## 0: GCC
## 1: MINGW
## 2: DJGPP
## 3: WATCOM
##

## Linux target
if(REX_TARGET STREQUAL "LINUX")

	add_compile_definitions("REX_TARGET=0")

	## GCC
	if(REX_COMPILER STREQUAL "GCC")

		add_compile_definitions("REX_COMPILER=0")

	endif()

endif()

## Windows target
if(REX_TARGET STREQUAL "WINDOWS")

	add_compile_definitions("REX_TARGET=1")

	## MINGW
	if(REX_COMPILER STREQUAL "MINGW")

		add_compile_definitions("REX_COMPILER=1")

	endif()

endif()

## DOS target
if(REX_TARGET STREQUAL "DOS")

	add_compile_definitions("REX_TARGET=2")

	## DJGPP
	if(REX_COMPILER STREQUAL "DJGPP")

		add_compile_definitions("REX_COMPILER=2")
		add_compile_options("-Wno-attributes")

	endif()

	## Open Watcom
	if(REX_COMPILER STREQUAL "WATCOM")

		add_compile_definitions("REX_COMPILER=3")

	endif()

endif()
