## ========================================================
##
## FILE:			/build-functional-targets.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Functional targets)
##
## LAST EDITED:		January 8th, 2023
##
## ========================================================

#!/bin/bash

## Null
source ./build-null.sh
cd ..

## DOS, DJGPP
source ./build-dos32-djgpp.sh
cd ..

## SDL, GCC
source ./build-sdl-gcc.sh
cd ..
