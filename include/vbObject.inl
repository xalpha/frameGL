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

inline void VBObject::draw()
{
	if(vertexCount != 0)
	{
		glEnableClientState( GL_VERTEX_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);
		glVertexPointer( vertexSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(normalCount != 0)
	{
        glEnableClientState( GL_NORMAL_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals);
		glNormalPointer( GL_FLOAT, 0, (char*)NULL);
	}

	if(colorCount != 0)
	{
		glEnableClientState( GL_COLOR_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors);
		glColorPointer( colorSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(texCoordCount != 0)
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboTexCoords);
		glTexCoordPointer( texCoordSize, GL_FLOAT, 0, (char*)NULL);
	}

	if(indexCount != 0)
	{
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vboIndices);
		glDrawElements( primitivesType, indexCount*indexSize, GL_UNSIGNED_INT, (char*)NULL);
	}
	else
	{
		// we assume that because there are no indices, only GL_POINTS are to be renderd
		glDrawArrays( GL_POINTS, 0, vertexCount);
	}

	glDisableClientState( GL_VERTEX_ARRAY);
	glDisableClientState( GL_NORMAL_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY);
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}


inline void VBObject::updateVertices(float *v)
{
	vertices = v;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(float)*vertexSize, vertices, vertexBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


inline void VBObject::updateColors(float *c)
{
	colors = c;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, colorCount*sizeof(float)*colorSize, colors, colorBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}
