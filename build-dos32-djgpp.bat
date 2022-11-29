:: ========================================================
::
:: FILE:			/build-dos32-djgpp.bat
::
:: AUTHORS:			Jaycie Ewald
::
:: PROJECT:			Duke4Ever
::
:: LICENSE:			TBD
::
:: DESCRIPTION:		Batch compile script (DJGPP)
::
:: LAST EDITED:		November 29th, 2022
::
:: ========================================================

if not exist cmake-build-dos32-djgpp\ (
	md cmake-build-dos32-djgpp
)

cd cmake-build-dos32-djgpp
cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/i586-pc-msdosdjgpp.cmake ..
make
