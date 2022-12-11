## ========================================================
##
## FILE:			/build-dos32-djgpp.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Duke4Ever
##
## LICENSE:			TBD
##
## DESCRIPTION:		Bash compile script (DOS32, DJGPP)
##
## LAST EDITED:		November 29th, 2022
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
