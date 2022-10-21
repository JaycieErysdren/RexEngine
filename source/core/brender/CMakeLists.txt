## ========================================================
##
## FILE:			/source/core/brender/CMakeLists.txt
##
## AUTHORS:			Jaycie Ewald
##
## PROJECT:			Rex Engine
##
## LICENSE:			ACSL 1.4
##
## DESCRIPTION:		BRender CMake project definition.
##
## LAST EDITED:		October 21st, 2022
##
## ========================================================

##
## BRender directories
##

set(BRENDER_DIR ${CMAKE_SOURCE_DIR}/external/CrocDE-BRender/brender)
set(BRENDER_FMT_DIR ${BRENDER_DIR}/fmt)
set(BRENDER_FW_DIR ${BRENDER_DIR}/fw)
set(BRENDER_GLREND_DIR ${BRENDER_DIR}/glrend)
set(BRENDER_HOST_DIR ${BRENDER_DIR}/host)
set(BRENDER_INC_DIR ${BRENDER_DIR}/inc)
set(BRENDER_MATH_DIR ${BRENDER_DIR}/math)
set(BRENDER_NULLDEV_DIR ${BRENDER_DIR}/nulldev)
set(BRENDER_PIXELMAP_DIR ${BRENDER_DIR}/pixelmap)
set(BRENDER_STD_DIR ${BRENDER_DIR}/std)
set(BRENDER_V1DB_DIR ${BRENDER_DIR}/v1db)

##
## Global definitions
##

## Global included folders
include_directories(${BRENDER_INC_DIR})
include_directories(${BRENDER_FW_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

## Global compile definitions
add_compile_definitions(INTERFACE
	STATIC=static
	BASED_FIXED=0
	BASED_FLOAT=1
	PARANOID=1
	EVAL=0
)

##
## BRENDER_INC
## Include Directories
##

## BRENDER_INC: Add custom Perl command
add_custom_command(
	OUTPUT pretok.h pretok.c toktype.c
	COMMAND "${PERL_EXECUTABLE}" "${BRENDER_INC_DIR}/tokgen.pl" "${BRENDER_INC_DIR}/pretok.tok"
	MAIN_DEPENDENCY "${BRENDER_INC_DIR}/pretok.tok"
	DEPENDS "${BRENDER_INC_DIR}/tokgen.pl"
)

## BRENDER_INC: Generated token files
set(BRENDER_INC_TOKEN_FILES
	pretok.h
	pretok.c
	toktype.c
	${BRENDER_INC_DIR}/pretok.tok
	${BRENDER_INC_DIR}/tokgen.pl
)

## BRENDER_INC: Specify header files only
source_group("Token" FILES ${BRENDER_INC_TOKEN_FILES})
set_source_files_properties(${BRENDER_INC_TOKEN_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

## BRENDER_INC: Source files
set(BRENDER_INC_FILES
	${BRENDER_INC_DIR}/actor.h
	${BRENDER_INC_DIR}/angles.h
	${BRENDER_INC_DIR}/assoca_p.h
	${BRENDER_INC_DIR}/assocarr.h
	${BRENDER_INC_DIR}/brdiag.h
	${BRENDER_INC_DIR}/brender.h
	${BRENDER_INC_DIR}/brerror.h
	${BRENDER_INC_DIR}/brfile.h
	${BRENDER_INC_DIR}/brfont.h
	${BRENDER_INC_DIR}/brlimits.h
	${BRENDER_INC_DIR}/brlog.h
	${BRENDER_INC_DIR}/brmem.h
	${BRENDER_INC_DIR}/brv1db.h
	${BRENDER_INC_DIR}/brvector.h
	${BRENDER_INC_DIR}/camera.h
	${BRENDER_INC_DIR}/colour.h
	${BRENDER_INC_DIR}/compend.h
	${BRENDER_INC_DIR}/compiler.h
	${BRENDER_INC_DIR}/custsupt.h
	${BRENDER_INC_DIR}/dmyifix.h
	${BRENDER_INC_DIR}/fixed.h
	${BRENDER_INC_DIR}/fw_p.h
	${BRENDER_INC_DIR}/light.h
	${BRENDER_INC_DIR}/material.h
	${BRENDER_INC_DIR}/math_p.h
	${BRENDER_INC_DIR}/matrix.h
	${BRENDER_INC_DIR}/model.h
	${BRENDER_INC_DIR}/order.h
	${BRENDER_INC_DIR}/pixelmap.h
	${BRENDER_INC_DIR}/pm_p.h
	${BRENDER_INC_DIR}/pool.h
	${BRENDER_INC_DIR}/quat.h
	${BRENDER_INC_DIR}/render.h
	${BRENDER_INC_DIR}/scalar.h
	${BRENDER_INC_DIR}/syscn_p.h
	${BRENDER_INC_DIR}/token.h
	${BRENDER_INC_DIR}/transfrm.h
	${BRENDER_INC_DIR}/v1db_p.h
	${BRENDER_INC_DIR}/vecfns.h
	${BRENDER_INC_DIR}/inc-dummy.c
)

## BRENDER_INC: Generate static library
add_library(brender_inc STATIC
    ${BRENDER_INC_TOKEN_FILES}
    ${BRENDER_INC_FILES}
)
