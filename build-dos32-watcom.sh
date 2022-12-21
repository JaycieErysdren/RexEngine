## ========================================================
##
## FILE:			/build-dos32-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (DOS32, Watcom)
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-dos32-watcom ] 
then
	mkdir cmake-build-dos32-watcom
fi

cd cmake-build-dos32-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS -D REX_TARGET=DOS -D REX_COMPILER=WATCOM ..
wmake
