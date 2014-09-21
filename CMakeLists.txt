##############################################################################
#                                                                            #
# This file is part of FrameGL                                               #
#                                                                            #
# Copyright (C) 2006 Alexandru Duliu, Markus Bucher                          #
#                                                                            #
# FrameGL is free software; you can redistribute it and/or                   #
# modify it under the terms of the GNU  General Public License               #
# as published by the Free Software Foundation; either version 3             #
# of the License, or (at your option) any later version.                     #
#                                                                            #
# FrameGL is distributed in the hope that it will be useful,                 #
# but WITHOUT ANY WARRANTY; without even the implied warranty of             #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
# GNU General Public License for more details.                               #
#                                                                            #
# You should have received a copy of the GNU General Public License          #
# along with FrameGL. If not, see <http://www.gnu.org/licenses/>.            #
#                                                                            #
##############################################################################


cmake_minimum_required(VERSION 2.8)
project( FrameGL CXX )

# add module paths
list( APPEND CMAKE_MODULE_PATH
    ${CMAKE_INSTALL_PREFIX}/share
    ${CMAKE_CURRENT_LIST_DIR}/cmake
    $ENV{HOME}/.local/share )

# check if windows
if( WIN32 )
    add_definitions( -D_WIN32 )
endif()

# find dependencies
find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( GLUT REQUIRED )

# set the target
set( FrameGL_Target frameGL )

# set the header files
set( FrameGL_INC 
    include/bmp.h
    include/frame.h
    include/frame.inl
    include/mathtools.h
    include/meshtools.h
    include/normalize.h
    include/primitives.h
    include/texture.h
    include/tools.h
    include/vbObject.h
    include/vbObject.inl )
set( FrameGL_SRC 
    src/bmp.cpp
    src/frame.cpp
    src/meshtools.cpp
    src/normalize.cpp
    src/primitives.cpp
    src/texture.cpp
    src/tools.cpp
    src/vbObject.cpp )

# set include directories
set( FrameGL_INCLUDE_DIRS
    include
    extern/cimg
    $ENV{HOME}/.local/include
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_PATH}
    ${CMAKE_CURRENT_BINARY_DIR} )

# set link libraries
set( FrameGL_LINK_LIBS
    ${OPENGL_LIBRARIES}
    ${GLUT_glut_LIBRARY}
    ${GLEW_LIBRARY} )

# set compile definitions
set( FrameGL_COMPILE_DEFINITIONS _USE_MATH_DEFINES )

# set include paths
include_directories( ${FrameGL_INCLUDE_DIRS} )

# add the exec
add_executable( ${FrameGL_Target} ${FrameGL_SRC} ${FrameGL_INC} )

# set link libraries
target_link_libraries( ${FrameGL_Target} ${FrameGL_LINK_LIBS} )

