## ========================================================
##
## FILE:			/build-amd64.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		Rex Engine AMD64 compile script.
##
## LAST EDITED:		October 28th, 2022
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-amd64 ] 
then
	mkdir cmake-build-amd64
fi

cd cmake-build-amd64
cmake ../source
make
