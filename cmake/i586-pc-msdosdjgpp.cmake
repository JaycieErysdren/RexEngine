## ========================================================
##
## FILE:			/cmake/i586-pc-msdosdjgpp.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL v1.4
##
## DESCRIPTION:		CMake toolchain file for DJGPP
##
## LAST EDITED:		December 19th, 2022
##
## ========================================================

set(CMAKE_SYSTEM_NAME DOS)
set(TOOLCHAIN_PREFIX i586-pc-msdosdjgpp)
set(DJGPP TRUE)

# cross compilers to use for C, C++ and Fortran
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)

# target environment on the build host system
set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# modify default behavior of FIND_XXX() commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
