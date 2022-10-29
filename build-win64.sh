## ========================================================
##
## FILE:			/build-win64.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine WIN64 compile script.
##
## LAST EDITED:		October 28th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-win64 ] 
then
	mkdir cmake-build-win64
fi

cd cmake-build-win64
cmake -DBRENDER_BUILD_EXAMPLES=OFF -DCMAKE_TOOLCHAIN_FILE=../source/cmake/mingw-w64-x86_64.cmake ../source
make
