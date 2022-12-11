## ========================================================
##
## FILE:			/build-dos32-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Duke4Ever
##
## LICENSE:			TBD
##
## DESCRIPTION:		Bash compile script (Watcom)
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
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS ..
wmake
