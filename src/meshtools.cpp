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
 * program  : mesh functions                                   *
 * author   : Alexandru Duliu                                  *
 * version  : 1.0                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#include <fstream>
#include <sstream>
#include <vector>


void readMesh( const std::string& filename,
               float** vertices, int& vertexSize,
               unsigned int** indices, int& indexSize )
{
    // try to load the model
    std::ifstream file;
    file.open( filename.c_str() );
    std::vector<float> vs;
    std::vector<unsigned int> is;

    // check if it was loaded
    if( file.is_open() )
    {
        while( !file.eof() )
        {
            // get the next line
            std::string line;
            std::getline( file, line );
            std::stringstream lineSS;
            lineSS<< line;
            lineSS.seekg( 1 );
            float tempV;
            float tempI;

            // interpret line
            switch( line[0] )
            {
                case 'v' : while( !lineSS.eof() ){ lineSS >> tempV; vs.push_back(tempV); } break;
                case 'f' : while( !lineSS.eof() ){ lineSS >> tempI; is.push_back(tempI); } break;
                default : break;
            }
        }

        // vertices
        if( *vertices != 0 )
            delete [] (*vertices);
        vertexSize = vs.size();
        *vertices = new float[vs.size()];
        for( int v=0; v<vs.size(); v++ )
            (*vertices)[v] = vs[v];

        // indices
        if( *indices != 0 )
            delete [] (*indices);
        indexSize = is.size();
        *indices = new unsigned int[is.size()];
        for( int i=0; i<is.size(); i++ )
            (*indices)[i] = is[i]-1;
    }
    else
    {
        vertexSize = 0;
        indexSize = 0;
    }
}
