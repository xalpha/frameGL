////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of FrameGL                                               //
//                                                                            //
// Copyright (C) 2005 Alexandru Duliu, Markus Bucher                          //
//                                                                            //
// FrameGL is free software; you can redistribute it and/or                   //
// modify it under the terms of the GNU  General Public License               //
// as published by the Free Software Foundation; either version 3             //
// of the License, or (at your option) any later version.                     //
//                                                                            //
// FrameGL is distributed in the hope that it will be useful,                 //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with FrameGL. If not, see <http://www.gnu.org/licenses/>.            //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////

/***************************************************************
 *                                                             *
 * program  : Texture Handling                                 *
 * author   : Alexandru Duliu, Markus Bucher                   *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de, bucherm@in.tum.de               *
 *                                                             *
 ***************************************************************/

#ifndef _TEXTURE_
#define _TEXTURE_


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "bmp.h"

/*
 Loads a texture from file "filepath" and sets TexParameters.
 Returns the newly generated texture id.
*/
GLuint loadTexture(const std::string&, GLenum format);

GLuint genCellular(int density, int size, GLfloat color1[4], GLfloat color2[4]);

GLuint genCheckerboard(int density, int size, GLfloat color1[4], GLfloat color2[4]);

GLuint genColorMap(bool gradient, double level, int size);

GLuint genExpSprite(int size,float factor);

GLuint genWhiteCircle(int size);


#endif

