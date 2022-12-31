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
## LAST EDITED:		December 30th, 2022
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
