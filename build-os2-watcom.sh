## ========================================================
##
## FILE:			/build-os2-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (OS/2, Watcom)
##
## LAST EDITED:		December 31st, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-os2-watcom ] 
then
	mkdir cmake-build-os2-watcom
fi

cd cmake-build-os2-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=OS2 -D REX_TARGET=OS2 -D REX_COMPILER=WATCOM ..
wmake
