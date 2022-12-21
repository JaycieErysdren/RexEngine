## ========================================================
##
## FILE:			/build-dos32-djgpp.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (DOS32, DJGPP)
##
## LAST EDITED:		December 19th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-dos32-djgpp ] 
then
	mkdir cmake-build-dos32-djgpp
fi

cd cmake-build-dos32-djgpp
cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/i586-pc-msdosdjgpp.cmake -D REX_TARGET=DOS -D REX_COMPILER=DJGPP ..
make
