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
 * program  : tools                                            *
 * author   : Alexandru Duliu                                  *
 * version  : 1.0                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>


// timer
static double g_dTimerStart;
static double g_dTimerFrame;
static double g_dTimerFPS;
static double g_iTimerFPSCount;

double timerClock();
void timerStart();
double timerFrameStart();
void timerFrameEnd(const char *progName);


// mirror texture
GLuint genMirrorTexture(int texSize);

#endif
