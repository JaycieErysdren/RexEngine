:: ========================================================
::
:: FILE:			/build-dos32-watcom.bat
::
:: AUTHORS:			Jaycie Ewald
::
:: PROJECT:			Duke4Ever
::
:: LICENSE:			TBD
::
:: DESCRIPTION:		Batch compile script (Watcom)
::
:: LAST EDITED:		December 11th, 2022
::
:: ========================================================

if not exist cmake-build-dos32-watcom\ (
	md cmake-build-dos32-watcom
)

cd cmake-build-dos32-watcom
cmake -G "Watcom WMake" -D CMAKE_SYSTEM_NAME=DOS ..
wmake
