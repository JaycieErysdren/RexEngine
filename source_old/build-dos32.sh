## ========================================================
##
## FILE:			/build-dos32.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine DOS32 compile script.
##
## LAST EDITED:		November 18th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-dos32 ] 
then
	mkdir cmake-build-dos32
fi

cd cmake-build-dos32
cmake -D CMAKE_C_COMPILER=i586-pc-msdosdjgpp-gcc ../source2
make
