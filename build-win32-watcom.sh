## ========================================================
##
## FILE:			/build-win32-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Win32, Watcom)
##
## LAST EDITED:		December 31st, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-win32-watcom ] 
then
	mkdir cmake-build-win32-watcom
fi

cd cmake-build-win32-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=Windows -D REX_TARGET=WIN32 -D REX_COMPILER=WATCOM ..
wmake
