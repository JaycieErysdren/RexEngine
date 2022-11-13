## ========================================================
##
## FILE:			/build-dos32-djgpp.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine DOS32 compile script (DJGPP).
##
## LAST EDITED:		November 13th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-dos32-djgpp ] 
then
	mkdir cmake-build-dos32-djgpp
fi

cd cmake-build-dos32-djgpp
cmake -D CMAKE_C_COMPILER_ID=DJGPP -D CMAKE_C_COMPILER=i586-pc-msdosdjgpp-gcc -D CMAKE_SYSTEM_NAME=DOS ..
make
