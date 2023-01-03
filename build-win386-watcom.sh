## ========================================================
##
## FILE:			/build-win386-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Win386, Watcom)
##
## LAST EDITED:		January 2nd, 2023
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-win386-watcom ] 
then
	mkdir cmake-build-win386-watcom
fi

cd cmake-build-win386-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=Windows -D REX_TARGET=WIN386 -D REX_COMPILER=WATCOM ..
wmake
