## ========================================================
##
## FILE:			/build-all.sh
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		Bash compile testing script (DOS)
##
## LAST EDITED:		December 11th, 2022
##
## ========================================================

#!/bin/bash

##
## DJGPP
##

printf "Testing DJGPP build...\n"

if [ -d cmake-build-dos32-djgpp ] 
then
    printf "DJGPP build folder already exists, removing...\n"
	rm -r cmake-build-dos32-djgpp
fi

printf "Running DJGPP build...\n"
source ./build-dos32-djgpp.sh
cd ..
printf "DJGPP build finished.\n"

##
## Watcom
##

printf "Testing Watcom build...\n"

if [ -d cmake-build-dos32-watcom ] 
then
    printf "Watcom build folder already exists, removing...\n"
	rm -r cmake-build-dos32-watcom
fi

printf "Running Watcom build...\n"
source ./build-dos32-watcom.sh
cd ..
printf "Watcom build finished.\n"
