## ========================================================
##
## FILE:			/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Duke4Ever
##
## LICENSE:			TBD
##
## DESCRIPTION:		Duke4Ever CMake project definition
##
## LAST EDITED:		December 1st, 2022
##
## ========================================================

##
## Project Definitions
##

## CMake version gate
cmake_minimum_required(VERSION 3.13)

## Project
project(Duke4Ever LANGUAGES C CXX)

## Build type
set(CMAKE_BUILD_TYPE Release)

##
## Project Compile Options
##

add_compile_options("-O3")

##
## Project Thirdparty Libraries
##

if(ENABLE_RVR)

set(RVR_DIR ${CMAKE_SOURCE_DIR}/source/thirdparty/rvr)
set(ANARCH_DIR ${RVR_DIR}/anarch)

add_library(rvr STATIC ${RVR_DIR}/RvR_all.c)
add_library(rvr_dos STATIC ${RVR_DIR}/backend/RvR_backend_dos.cpp)

aux_source_directory(${ANARCH_DIR} ANARCH_SRC) 
add_executable(anarch ${ANARCH_SRC})
target_include_directories(anarch PUBLIC ${RVR_DIR})
target_link_libraries(anarch PUBLIC rvr rvr_dos)

endif()

##
## Project Libraries
##

set(REX3D_CORE_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/core/console.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/palette.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/picture.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/core/utils.cpp
)

set(REX3D_HWDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/hwdrv/dos.cpp
)

set(REX3D_MATH_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/math/math_i.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/math_x.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/matrix.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/sintable.cpp
	${CMAKE_SOURCE_DIR}/source/rex3d/math/sintable2.cpp
)

set(REX3D_THRDPRTY_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/thrdprty/rcl.cpp
)

set(REX3D_VIDDRV_SRC
	${CMAKE_SOURCE_DIR}/source/rex3d/viddrv/vga.cpp
)

add_library(rex3d STATIC
	${REX3D_CORE_SRC}
	${REX3D_HWDRV_SRC}
	${REX3D_MATH_SRC}
	${REX3D_THRDPRTY_SRC}
	${REX3D_VIDDRV_SRC}
)

target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(rex3d PUBLIC ${CMAKE_SOURCE_DIR}/source/rex3d)

##
## Project Applications
##

set(DUKE4EVER_SRC
	${CMAKE_SOURCE_DIR}/source/duke4evr/main.cpp
)

add_executable(duke4evr ${DUKE4EVER_SRC})
target_link_libraries(duke4evr rex3d)
target_include_directories(duke4evr PUBLIC ${CMAKE_SOURCE_DIR}/source)
set_target_properties(duke4evr
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/builddos"
)
