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

#include <stdio.h>

#include <tools.h>

#ifdef _WIN32
# include <sys/timeb.h>
# include <windows.h>
#else
# include <sys/time.h>
# include <sys/times.h>
# include <sys/types.h>
# include <unistd.h>
#endif


double timerClock()
{
#ifdef _WIN32
    return 0.001*double(GetTickCount());
#else
    struct timeval ct; // current time
    gettimeofday( &ct, 0 );
    return double(ct.tv_sec) + double(ct.tv_usec)/1000000.0;
#endif
}


void timerStart()
{
    // start time
    g_dTimerStart = timerClock();
    g_dTimerFrame = g_dTimerStart;
    g_dTimerFPS = g_dTimerStart;
}

double timerFrameStart()
{
    g_dTimerFrame = timerClock();
    return g_dTimerFrame;
}

void timerFrameEnd(const char *progName)
{
    // how long has it been since last measuring the fps
    double elapsed = timerClock() - g_dTimerFPS;
    if( elapsed >= 1.0 )
    {
        // awesome (most overused word in the universe), show FPS
        char text[255];
        sprintf(text,"%s @ %.2f fps",progName,g_iTimerFPSCount);
        glutSetWindowTitle(text);

        // reset FPS counter
        g_dTimerFPS = timerClock();
        g_iTimerFPSCount = 0;
    }
    else
        g_iTimerFPSCount += 1.0;
}


GLuint genMirrorTexture(int texSize)
{
    printf("Generating mirror texture...");

    GLuint tex;
    GLubyte *buffer = new GLubyte[texSize*texSize*4];
    GLubyte temp;

    for (int x = 0; x<texSize; x++) {
        for (int y = 0; y<texSize; y++)
        {
            temp = rand()%30+225;

            buffer[(x*texSize+y)*4+0] = temp;
            buffer[(x*texSize+y)*4+1] = temp;
            buffer[(x*texSize+y)*4+2] = temp;
            buffer[(x*texSize+y)*4+3]	= 128;
        }
    }

    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,texSize,texSize,0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    delete[] buffer;
    printf(" done.\n");
    return tex;
}
