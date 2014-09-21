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
 * version  : 1.0                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#ifndef _BMP_
#define _BMP_

#include <stdio.h>
#include <stdlib.h>	
#include <math.h>

#define cimg_display 0
#include <CImg.h>

class Bmp
{

	public:

	int width;
	int height;
	int colorCount;
	unsigned char *data;

    cimg_library::CImg<unsigned char> image;



	Bmp();
	Bmp(char *fileName);
	//Bmp(int width, int height, char format, char *data);

	/*
	unsigned int getWidth();
	void setWidth(int widthX);

	unsigned int getHeight();
	void setHeight(int heightX);

	unsigned int getColorCount();
	void setColorCount(int colorCountX);

	char* getData();
	void setData(char *data);
	*/

    //bool writeBmp(char *fileName);
	bool readBmp(char *fileName);
};


#endif
