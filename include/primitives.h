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
 * program  : primitives                                       *
 * author   : Alexandru Duliu, Markus Bucher                   *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de, bucherm@in.tum.de               *
 *                                                             *
 ***************************************************************/

#ifndef _PRIMITIVES_
#define _PRIMITIVES_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>


#ifndef M__PI
#define M__PI   3.1415926535898
#endif
#ifndef M__PI_2
#define M__PI_2 1.5707963267949
#endif
#ifndef M__2_PI
#define M__2_PI 6.2831853071796
#endif

/*
 * BUILD A SPHERE. Sphere is centered at c with radius r.
 * LOD specifies the level of detail, i.e. the number of quads is LOD*LOD/2.
 * Sphere does not actually draw a sphere, but records it in a so called
 * "display-list". Display-lists are used to cache vertices and offer
 * great rendering speed, but only for static geometry. Sphere returns
 * an unsigned integer that serves as a handle to access the geometry. See
 * functions initialize() and displayfunc() for more details.
 * texMirr gibt an, ob die Texturkoordinate r von 0-1 (texMirr=0) oder von 0-2(texMirr=1)
 * gehen soll. Dies ist notwendig um GL_MIRRORED_REPEAT anzuwenden.
 */
GLuint sphere( float texMirr, float r, float cx,float cy,float cz, unsigned int LODi, float iStart=-M__PI_2, float iEnd=M__PI_2);

#endif

