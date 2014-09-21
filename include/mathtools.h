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
 * program  : math functions                                   *
 * author   : Alexandru Duliu                                  *
 * version  : 1.0                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/


inline void MatrixVectorProd3(float mat[16], float vec[3])
{
    float result[3];
    result[0]=mat[0]*vec[0]+mat[4]*vec[1]+mat[8]*vec[2];
    result[1]=mat[1]*vec[0]+mat[5]*vec[1]+mat[9]*vec[2];
    result[2]=mat[2]*vec[0]+mat[6]*vec[1]+mat[10]*vec[2];

    vec[0] = result[0];
    vec[1] = result[1];
    vec[2] = result[2];
}


inline void MatrixMatrixProd(float matA[16], float matB[16])
{
    float result[16];
    for (int x = 0;x<16;x+=4)
        for (int y = 0;y<4;y++)
            result[x+y] = matB[0+y]*matA[0+x]+matB[4+y]*matA[1+x]+matB[8+y]*matA[2+x]+matB[12+y]*matA[3+x];

    for (int i=0;i<16;i++) matA[i] = result[i];
}
