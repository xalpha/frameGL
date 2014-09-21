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
 * program  : VBObject Class                                   *
 * author   : Alexandru Duliu                                  *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#ifndef _VBOBJECT_
#define _VBOBJECT_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

class VBObject
{
	int primitivesType;	//type of primitives which are indexed. Must be: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, or GL_POLYGON

	// vertices
	float *vertices;	// array of vertices (default float)
	int vertexCount;	// number ob vertices
	int vertexSize;		// number of coordinates per vertex. Must be 2, 3, or 4.
	int vertexBufferUsage;	// The usage argument is a key value for helping the VBO memory manager fully optimize your buffers. (static, dynamic, etc.)
	GLuint vboVertices;	// vertex buffer object identifier

	// indices
	unsigned int *indices;	// array of indices (default unsigned int)
	int indexCount;			// number of indices
	int indexSize;			// number of indices per primitive. Must be: GL_POINTS, GL_LINES, GL_TRIANGLES or GL_QUADS
	int indexBufferUsage;	// The usage argument is a key value for helping the VBO memory manager fully optimize your buffers. (static, dynamic, etc.)
	GLuint vboIndices;		// index buffer object identifier

	//	normals
	float *normals;		// array of normals (default float)
	int normalCount;	// numver of normals
	int normalBufferUsage;	// The usage argument is a key value for helping the VBO memory manager fully optimize your buffers. (static, dynamic, etc.)
	GLuint vboNormals;	// normal buffer object identifier
	
	//	colors
	float *colors;		// array of colors (default float)
	int colorCount;		// number of colors
	int colorSize;		// number of components per color. Must be 3 or 4
	int colorBufferUsage;	// The usage argument is a key value for helping the VBO memory manager fully optimize your buffers. (static, dynamic, etc.)
	GLuint vboColors;	// color buffer object identifier

	// texture coordinates
    float *texCoords;	// array of texture coordinates
	int texCoordCount;	// number of texture coordinates
	int texCoordSize;	//	number of coordinates per array element. Must be 1, 2, 3 or 4.	
	int texCoordBufferUsage;	// The usage argument is a key value for helping the VBO memory manager fully optimize your buffers. (static, dynamic, etc.)
	GLuint vboTexCoords;	// texture coordinates buffer object identifier
	

public:

	VBObject();

	~VBObject();

	void initVertices(float *v, int vc, int vs, int vbu, int pt);

	inline void updateVertices(float *v);

	void initIndices(unsigned int *i, int ic, int ibu, int pt);

	void initNormals(float *n, int nc, int nbu);

	void initColors(float *c, int cc, int cs, int cbu);

	inline void updateColors(float *c);

	void initTexCoords(float *t, int tc, int ts, int tbu);

	void initialize(float *v, unsigned int *i, float *n, int vc, int ic, int nc);

	inline void draw();

};

#include "vbObject.inl"

#endif

