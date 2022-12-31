## ========================================================
##
## FILE:			/build-linux32-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Linux32, Watcom)
##
## LAST EDITED:		December 31st, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-linux32-watcom ] 
then
	mkdir cmake-build-linux32-watcom
fi

cd cmake-build-linux32-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=Linux -D REX_TARGET=LINUX32 -D REX_COMPILER=WATCOM ..
wmake
