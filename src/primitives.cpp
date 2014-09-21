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

#include "primitives.h"

GLuint sphere( float texMirr, float r, float cx,float cy,float cz, unsigned int LODi, float iStart, float iEnd) {

    int list;
    unsigned int i, j;
    float angle = iStart;
    float oldStart[3];
    float cos_angle_tmp, sin_angle_tmp;
    float step = M__PI / LODi;
    float **temp;
    temp = new float* [LODi];  //saves the Last circles Vertizes (for Resource saving ;-)
    for( i=0; i<LODi; i++) {
            temp[i] = new float [3];
              temp[i][0] = cx;
              temp[i][1] = cy - r;
              temp[i][2] = cz;
    }

    list = glGenLists(1);
    glNewList( list, GL_COMPILE_AND_EXECUTE);

    // Bottom to Top...
   for( i=1; i<=LODi; i++) {
        angle += step;
        sin_angle_tmp = sin(angle);
        cos_angle_tmp = cos(angle);
        glBegin( GL_QUAD_STRIP);
        oldStart[0] = temp[0][0];
        oldStart[1] = temp[0][1];
        oldStart[2] = temp[0][2];

        // ...and around
        for( j=0; j<LODi; j++) {
            //schreibe Vertex, Normale & Texturkoord. unten
            glTexCoord2f( (LODi-j)*pow((float)2.0, texMirr)/(float)LODi, (i-1)/(float)LODi);
            glNormal3f( (temp[j][0]-cx)/r, (temp[j][1]-cy)/r, (temp[j][2]-cz)/r);
            glVertex3f( temp[j][0], temp[j][1], temp[j][2]);
            //berechne Vertex oben
            temp[j][0] = cx + r*cos_angle_tmp*cos(j*M__2_PI/LODi);
            temp[j][1] = cy + r*sin_angle_tmp;
            temp[j][2] = cz + r*cos_angle_tmp*sin(j*M__2_PI/LODi);
            //schreibe Vertex & Normale oben
            glTexCoord2f( (LODi-j)*pow((float)2.0, texMirr)/(float)LODi, i/(float)LODi);
            glNormal3f( (temp[j][0]-cx)/r, (temp[j][1]-cy)/r, (temp[j][2]-cz)/r);
            glVertex3f( temp[j][0], temp[j][1], temp[j][2]);
        }
        // The Strip-Connection ;-) (cos(2*PI)=1)
        glTexCoord2f( 0.0, (i-1)/(float)LODi);
        glNormal3f( (oldStart[0]-cx)/r, (oldStart[1]-cy)/r, (oldStart[2]-cz)/r);
        glVertex3f( oldStart[0], oldStart[1], oldStart[2]);

        glTexCoord2f( 0.0, i/(float)LODi);
        glNormal3f( (temp[0][0]-cx)/r, (temp[0][1]-cy)/r, (temp[0][2]-cz)/r);
        glVertex3f( temp[0][0], temp[0][1], temp[0][2]);

        glEnd();
    }
    glEndList();

    //Clean up
    for( i=0; i<LODi; i++)
        delete [] temp[i];
    delete [] temp;

    return list;
}
