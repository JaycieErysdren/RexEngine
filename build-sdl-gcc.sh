## ========================================================
##
## FILE:			/build-sdl-gcc.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (SDL, GCC)
##
## LAST EDITED:		December 30th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-sdl-gcc ] 
then
	mkdir cmake-build-sdl-gcc
fi

cd cmake-build-sdl-gcc
cmake -D REX_TARGET=SDL -D REX_COMPILER=GCC ..
make
