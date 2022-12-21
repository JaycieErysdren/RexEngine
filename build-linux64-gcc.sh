## ========================================================
##
## FILE:			/build-linux64-gcc.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Linux64, GCC)
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-linux64-gcc ] 
then
	mkdir cmake-build-linux64-gcc
fi

cd cmake-build-linux64-gcc
cmake -D REX_TARGET=LINUX -D REX_COMPILER=GCC ..
make
