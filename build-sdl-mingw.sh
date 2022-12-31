## ========================================================
##
## FILE:			/build-sdl-mingw.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (SDL, MINGW)
##
## LAST EDITED:		December 31st, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-sdl-mingw ] 
then
	mkdir cmake-build-sdl-mingw
fi

cd cmake-build-sdl-mingw
cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/mingw-w64-x86_64.cmake -D REX_TARGET=SDL -D REX_COMPILER=MINGW ..
make
