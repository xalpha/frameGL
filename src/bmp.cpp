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
 * program  : BMP IO                                           *
 * author   : Alexandru Duliu                                  *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>


Bmp::Bmp(){ data=0; }

Bmp::Bmp(const std::string& fileName)
{
    data=0;
	readBmp(fileName);
}

/*
Bmp::Bmp(int widthX, int heightX, char format, char *dataX)
{
	width = widthX;
	height= heightX;
	data  = dataX;
}
*/

/*
unsigned int Bmp::getWidth()	{	return width;	}
void Bmp::setWidth(int widthX)	{	width = widthX;	}

unsigned int Bmp::getHeight()	{	return height;	}
void Bmp::setHeight(int heightX)	{	height = heightX;	}

unsigned int Bmp::getColorCount()	{	return colorCount;	}
void Bmp::setColorCount(int colorCountX)	{	colorCount = colorCountX;	}

char* Bmp::getData()	{	return data;	}
void Bmp::setData(char *dataX)	{	data = dataX;	}
*/



bool Bmp::readBmp( const std::string& fileName)
{
    // load image
    image.load( fileName.c_str() );
    image.mirror('y');

    // ensure RGB
    if( image.spectrum() == 1 )
    {
        image.append( image.get_channel(0), 'c' );
        image.append( image.get_channel(0), 'c' );
    }
    printf("%d\n",image.spectrum());

    // get the image info
    colorCount = image.spectrum();
    width = image.width();
    height= image.height();

    // ingterlace the buffer
    if( data != 0 )
        delete [] data;
	data = new unsigned char[width*height*colorCount];
    cimg_forXYC( image, x,y,c )
    {
        data[y*(height*colorCount) + x*colorCount + c] = image(x,y,0,c);
    }

	return false;
}
