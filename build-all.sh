## ========================================================
##
## FILE:			/build-all.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (All targets)
##
## LAST EDITED:		December 31st, 2022
##
## ========================================================

#!/bin/bash

## DOS, DJGPP
source ./build-dos32-djgpp.sh
cd ..

## DOS, Watcom
source ./build-dos32-watcom.sh
cd ..

## SDL, GCC
source ./build-sdl-gcc.sh
cd ..

## SDL, MINGW
source ./build-sdl-mingw.sh
cd ..

## Win32, Watcom
source ./build-win32-watcom.sh
cd ..

## Linux32, Watcom
source ./build-linux32-watcom.sh
cd ..
