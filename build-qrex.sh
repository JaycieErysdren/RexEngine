## ========================================================
##
## FILE:			/build-qrex.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile script for Q-Rex
##
## LAST EDITED:		December 27th, 2022
##
## ========================================================

#!/bin/bash

cd ./source/qrex/
fteqcc64 progs.src
cd ../../
