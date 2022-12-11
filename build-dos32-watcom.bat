:: ========================================================
::
:: FILE:			/build-dos32-watcom.bat
::
:: AUTHORS:			Jaycie Ewald
::
:: PROJECT:			Rex Engine
::
:: LICENSE:			ACSL v1.4
::
:: DESCRIPTION:		Batch compile script (DOS32, Watcom)
::
:: LAST EDITED:		December 11th, 2022
::
:: ========================================================

if not exist cmake-build-dos32-watcom\ (
	md cmake-build-dos32-watcom
)

cd cmake-build-dos32-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS -D REX_TARGET=DOS -D REX_COMPILER=WATCOM ..
wmake
