## ========================================================
##
## FILE:			/build-dos32-watcom.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine DOS32 compile script (Open Watcom v2).
##
## LAST EDITED:		November 13th, 2022
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
