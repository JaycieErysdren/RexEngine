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
## LAST EDITED:		January 2nd, 2023
##
## ========================================================

#!/bin/bash

## Null
source ./build-null.sh
cd ..

## DOS, DJGPP
source ./build-dos32-djgpp.sh
cd ..

## DOS, Watcom
source ./build-dos32-watcom.sh
cd ..

## SDL, GCC
source ./build-sdl-gcc.sh
cd ..

##
## commenting out these targets because they're a bit of a pain
##

## SDL, MINGW
source ./build-sdl-mingw.sh
cd ..

## Win32, Watcom
source ./build-win32-watcom.sh
cd ..

## Linux32, Watcom
source ./build-linux32-watcom.sh
cd ..
