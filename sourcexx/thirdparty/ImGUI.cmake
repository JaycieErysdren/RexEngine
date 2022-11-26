## ========================================================
##
## FILE:			/sourcexx/thirdparty/ImGUI.cmake
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		ImGUI CMake project definition.
##
## LAST EDITED:		November 26th, 2022
##
## ========================================================

##
## Project definitions
##

## Define project
project(ImGUI LANGUAGES CXX)

## Define directories
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/ImGUI)

## Define sources
set(IMGUI_SRC
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
)

## Define library
add_library(ImGUI ${IMGUI_SRC})

## Define include directories
target_include_directories(ImGUI PUBLIC
	${IMGUI_DIR}/core/inc
)
