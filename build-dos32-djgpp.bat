:: ========================================================
::
:: FILE:			/build-dos32-djgpp.bat
::
:: AUTHORS:			Jaycie Ewald
::
:: PROJECT:			Rex Engine
::
:: LICENSE:			ACSL v1.4
::
:: DESCRIPTION:		Batch compile script (DOS32, DJGPP)
::
:: LAST EDITED:		December 11th, 2022
::
:: ========================================================

if not exist cmake-build-dos32-djgpp\ (
	md cmake-build-dos32-djgpp
)

cd cmake-build-dos32-djgpp
cmake -D CMAKE_TOOLCHAIN_FILE=../cmake/i586-pc-msdosdjgpp.cmake -D REX_TARGET=DOS -D REX_COMPILER=DJGPP ..
make
