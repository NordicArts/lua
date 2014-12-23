# Copyright (C) 2007-2013 LuaDist.
# Created by Peter Drahoš, Peter Kapec
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
# Please note that the package source code is licensed under its own license.

# CMake Version
cmake_minimum_required(VERSION 2.8)

# Project Name
project(lua)

# Add Dist Make
include(${PROJECT_SOURCE_DIR}/cmake/dist.cmake)

# Config
set(LUA_PATH "LUA_PATH" CACHE STRING "Enviroment variable, package.path")
set(LUA_CPATH "LUA_CPATH" CACHE STRING "Enviroment variable, package.cpath")
set(LUA_INIT "LUA_INIT" CACHE STRING "Enviroment variable, init")
set(LUA_IDSIZE 60 CACHE NUMBER "Max size of description")

# Options
option(LUA_ANSI "Only ANSI" OFF)
option(LUA_USE_RELATIVE_LOADLIB "Use local loadlib.c" ON)
option(LUA_COMPAT_ALL "Backwards compatible" ON)

#2DO: LUAI_* and LUAL_* settings, for now defaults are used.
set ( LUA_DIRSEP "/" )
set ( LUA_MODULE_SUFFIX ${CMAKE_SHARED_MODULE_SUFFIX} )
set ( LUA_LDIR ${INSTALL_LMOD} )
set ( LUA_CDIR ${INSTALL_CMOD} )

if ( LUA_USE_RELATIVE_LOADLIB )
  # This will set up relative paths to lib
  string ( REGEX REPLACE "[^!/]+" ".." LUA_DIR "!/${INSTALL_BIN}/" )
else ( )
  # Direct path to installation
  set ( LUA_DIR ${CMAKE_INSTALL_PREFIX} CACHE STRING "Destination from which modules will be resolved. See INSTALL_LMOD and INSTALL_CMOD." )
endif ( )

set ( LUA_PATH_DEFAULT "./?.lua;${LUA_DIR}${LUA_LDIR}/?.lua;${LUA_DIR}${LUA_LDIR}/?/init.lua" )
set ( LUA_CPATH_DEFAULT "./?${LUA_MODULE_SUFFIX};${LUA_DIR}${LUA_CDIR}/?${LUA_MODULE_SUFFIX};${LUA_DIR}${LUA_CDIR}/loadall${LUA_MODULE_SUFFIX}" )

if ( WIN32 AND NOT CYGWIN )
  # Windows systems
  option ( LUA_WIN "Windows specific build." ON )
  option ( LUA_BUILD_WLUA "Build wLua interpretter without console output." ON )
  option ( LUA_BUILD_AS_DLL "Build Lua library as Dll." ${BUILD_SHARED_LIBS} )
  
  # Paths (Double escapes ne  option needed)
  set ( LUA_DIRSEP "\\\\" )
  string ( REPLACE " /" ${LUA_DIRSEP} LUA_DIR "${LUA_DIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_LDIR "${LUA_LDIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_CDIR "${LUA_CDIR}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_PATH_DEFAULT "${LUA_PATH_DEFAULT}" )
  string ( REPLACE "/" ${LUA_DIRSEP} LUA_CPATH_DEFAULT "${LUA_CPATH_DEFAULT}" )
else ( )
  # Posix systems (incl. Cygwin)
  option ( LUA_USE_POSIX "Use POSIX features." ON )
  option ( LUA_USE_DLOPEN "Use dynamic linker to load modules." ON )
  option ( LUA_USE_MKSTEMP "Use mkstep." ON )
  option ( LUA_USE_ISATTY "Use tty." ON )
  option ( LUA_USE_POPEN "Use popen." ON )
  option ( LUA_USE_ULONGJMP "Use ulongjmp" ON )
  option ( LUA_USE_GMTIME_R "Use GTIME_R" ON )
  # Apple and Linux specific
  if ( LINUX OR APPLE )
    option ( LUA_USE_STRTODHEX "Assume 'strtod' handles hexa formats" ON )
    option ( LUA_USE_AFORMAT "Assume 'printf' handles 'aA' specifiers" ON )
    option ( LUA_USE_LONGLONG "Assume support for long long" ON )
  endif ( )
endif ( )

## SETUP
# Optional libraries
find_package ( Readline )
if ( READLINE_FOUND )
  option ( LUA_USE_READLINE "Use readline in the Lua CLI." ON )
endif ( )

find_package ( Curses )
if ( CURSES_FOUND )
  option ( LUA_USE_CURSES "Use curses in the Lua CLI." ON )
endif ( )

# Setup needed variables and libraries
if ( LUA_USE_POSIX )
  # On POSIX Lua links to standard math library "m"
  list ( APPEND LIBS m )
endif ( )

if ( LUA_USE_DLOPEN )
  # Link to dynamic linker library "dl"
	find_library ( DL_LIBRARY NAMES dl )
	if ( DL_LIBRARY )
  	list ( APPEND LIBS ${DL_LIBRARY} )
	endif ( )
endif ( )

if ( LUA_USE_READLINE )
  # Add readline
  include_directories ( ${READLINE_INCLUDE_DIR} )
  list ( APPEND LIBS ${READLINE_LIBRARY} )
endif ( )

if ( LUA_USE_CURSES )
  # Add curses
  include_directories ( ${CURSES_INCLUDE_DIR} )
  list ( APPEND LIBS ${CURSES_LIBRARY} )
endif ( )

# Var
set(PSD ${PROJECT_SOURCE_DIR})
set(PSDS ${PROJECT_SOURCE_DIR}/lua/src)
set(PSDI ${PROJECT_SOURCE_DIR}/lua/include)

link_directories(${PSD})
include_directories(${PSD})

## SOURCES
# Generate luaconf.h
configure_file ( ${PSDI}/luaconf.h.in ${PSDI}/luaconf.h )

# Headers
include_directories(${PSDI})
link_directories(${PSDI})

# C Files
include_directories(${PSDS})
link_directories(${PSDS})

# Sources
set(SRC_CORE ${PSDS}/lapi.c)
list(APPEND SRC_CORE ${PSDS}/lcode.c)
list(APPEND SRC_CORE ${PSDS}/lctype.c)
list(APPEND SRC_CORE ${PSDS}/ldebug.c)
list(APPEND SRC_CORE ${PSDS}/ldo.c)
list(APPEND SRC_CORE ${PSDS}/ldump.c)
list(APPEND SRC_CORE ${PSDS}/lfunc.c)
list(APPEND SRC_CORE ${PSDS}/lgc.c)
list(APPEND SRC_CORE ${PSDS}/llex.c)
list(APPEND SRC_CORE ${PSDS}/lmem.c)
list(APPEND SRC_CORE ${PSDS}/lobject.c)
list(APPEND SRC_CORE ${PSDS}/lopcodes.c)
list(APPEND SRC_CORE ${PSDS}/lparser.c)
list(APPEND SRC_CORE ${PSDS}/lstate.c)
list(APPEND SRC_CORE ${PSDS}/lstring.c)
list(APPEND SRC_CORE ${PSDS}/ltable.c)
list(APPEND SRC_CORE ${PSDS}/ltm.c)
list(APPEND SRC_CORE ${PSDS}/lundump.c)
list(APPEND SRC_CORE ${PSDS}/lvm.c)
list(APPEND SRC_CORE ${PSDS}/lzio.c)

set(SRC_LIB ${PSDS}/lauxlib.c)
list(APPEND SRC_LIB ${PSDS}/lbaselib.c)
list(APPEND SRC_LIB ${PSDS}/lbitlib.c)
list(APPEND SRC_LIB ${PSDS}/lcorolib.c)
list(APPEND SRC_LIB ${PSDS}/ldblib.c)
list(APPEND SRC_LIB ${PSDS}/liolib.c)
list(APPEND SRC_LIB ${PSDS}/lmathlib.c)
list(APPEND SRC_LIB ${PSDS}/loslib.c)
list(APPEND SRC_LIB ${PSDS}/lstrlib.c)
list(APPEND SRC_LIB ${PSDS}/ltablib.c)
list(APPEND SRC_LIB ${PSDS}/linit.c)

set ( SRC_LUA ${PSDS}/lua.c )

set ( SRC_LUAC ${PSDS}/luac.c )

if ( LUA_USE_RELATIVE_LOADLIB )
  # Use modified loadlib
  list ( APPEND SRC_LIB ${PSDS}/loadlib_rel.c )
else ( )
  list ( APPEND SRC_LIB ${PSDS}/loadlib.c )
endif ( )

## BUILD
# Create lua library
add_library ( liblua SHARED ${SRC_CORE} ${SRC_LIB} ${LUA_DLL_RC} ${LUA_DEF} )
target_link_libraries ( liblua ${LIBS} )
set_target_properties ( liblua PROPERTIES OUTPUT_NAME lua CLEAN_DIRECT_OUTPUT 1 )
if ( LUA_BUILD_AS_DLL )
  set_target_properties ( liblua PROPERTIES COMPILE_DEFINITIONS LUA_BUILD_AS_DLL )
endif ()

SET(LUA_INCLUDES ${PROJECT_SOURCE_DIR}/lua/include)

