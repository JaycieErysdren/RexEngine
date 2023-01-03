## ========================================================
##
## FILE:			/build-null.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script (Null)
##
## LAST EDITED:		January 2nd, 2023
##
## ========================================================

#!/bin/bash

if [ ! -d cmake-build-null ] 
then
	mkdir cmake-build-null
fi

cd cmake-build-null
cmake ..
make
