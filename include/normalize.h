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
 * program  : Normalization Function                           *
 * author   : Alexandru Duliu, Markus Bucher                   *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de, bucherm@in.tum.de               *
 *                                                             *
 ***************************************************************/

#ifndef _NORMALIZE_
#define _NORMALIZE_

/** 
 * Berectnet die Triangle- und die VertexNormalen
 * vertices	= Anzahl der Vertices
 * triangles 	= Anzahl der Triangles
 * vertex	= Array von Vertices
 * triangle	= Array von Triangles
 * normalVer	= Array der VertexNormalen
**/
void calculateNormals(int vertices,int triangles,float vertex[],unsigned int triangle[],float normalVer[]);


#endif
