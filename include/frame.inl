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
 * author   : Alexandru Duliu, Markus Bucher                   *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

inline void invertCullFaceMode() {
	int iCFMode;
	glGetIntegerv(GL_CULL_FACE_MODE,&iCFMode);
	if (iCFMode != GL_BACK)
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT);
}

inline void displayOutput() {
  if (g_fFadeValue <= 0) return;
  int iLen     = (int)strlen(g_pcLastOutput),i;
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glLoadIdentity();
  glColor4f(1.0f,1.0f,1.0f,g_fFadeValue--/255.0);
  glRasterPos3f(iLen*-(35.0/g_iWinWidth),0,-4);  
  for(i=0;i<iLen;i++) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,g_pcLastOutput[i]);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}
inline void setupWireframe() {

	if( g_bWireframe){
		glDisable( GL_CULL_FACE);
		//glLineWidth(3);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode( GL_FRONT, GL_LINE);
		glEnable( GL_CULL_FACE);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	}

	//glFlush();
}

inline void setupLighting() {

	if( g_bToggleLighting) {
		glEnable( GL_LIGHTING);
		glEnable( GL_LIGHT0);
		if(g_bMirror)
		{
			glEnable( GL_LIGHT1);
		}	
		else	
		{
			glDisable( GL_LIGHT1);
		}
	}
	else {
		glDisable( GL_LIGHT0);
		if(g_bMirror)
		{
			glDisable( GL_LIGHT1);
		}
		glDisable( GL_LIGHTING);
	}
}


inline void setupUserViewProj() {
	glViewport(0,0,g_iWinWidth,g_iWinHeight);	// and set new dimensions
    	glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
    	glLoadIdentity();
	gluPerspective(60.0,g_fAspect,0.002,20.0);  
    	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();				// Overwrites the current matrix (MODELVIEW) with 4x4 identity.
}


inline void setupCubeMapViewProj() {
	glViewport(0, 0, g_iCubeMapDetail, g_iCubeMapDetail);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
	gluPerspective(90.0, 1.0, 0.001, 20.0);  
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();				// Overwrites the current matrix (MODELVIEW) with 4x4 identity.
}



inline void renderFog()
{
	// Just for Fun :-)
	if(g_bFogMode)
	{
		glEnable(GL_FOG);
		{
			GLfloat fogColor[4] = {0.75, 0.75, 0.75, 0.5};
			//GLfloat fogColor[4] = {0.0f, 0.0f, 0.0f, 0.5f};

			glFogi (GL_FOG_MODE, GL_EXP2);
			glFogfv (GL_FOG_COLOR, fogColor);
			glFogf (GL_FOG_DENSITY, 0.15);
			glHint (GL_FOG_HINT, GL_DONT_CARE);
			glFogf (GL_FOG_START, 1.0);
			glFogf (GL_FOG_END, 5.0);
		}
	}
	else
	{
		glDisable(GL_FOG);
	}
}


inline void renderMirror() {

	glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfShadowSpecular);
	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	
	// LEFT
	//glDisable( GL_CULL_FACE);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	GLboolean alphaTest=false;
	glGetBooleanv(GL_ALPHA_TEST,&alphaTest);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	if( g_bToggleTexture) glBindTexture( GL_TEXTURE_2D, g_iBoxLeft);
	glBegin(GL_QUADS);
	        glColor4f(0.55,0.55,0.55, 0.25); // add brown color to the mirror
		glNormal3f( 1.0f, 0.0f, 0.0f); 
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(-2.0f,-1.5f,-2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(-2.0f,+1.5f,-2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(-2.0f,+1.5f,+2.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(-2.0f,-1.5f,+2.0f);
    	glEnd();

	glDisable(GL_BLEND);
	if(alphaTest)	glEnable(GL_ALPHA_TEST);

	glEnable( GL_CULL_FACE);

	glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfLightSpecular);
	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}

inline void renderWalls() {

	
	glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfShadowSpecular);
	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	
        // BOTTOM
	glBindTexture( GL_TEXTURE_2D, g_iBoxBottom);
	glBegin(GL_QUADS);
		if( g_bToggleTexture) glColor3f( 1.0, 1.0, 1.0);
	        else glColor3f(0.8,0.2,0.2);
        	glNormal3f( 0.0f, 1.0f, 0.0f); // needs only to be set if it changes.
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(-2.0f,-1.5f,-2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(-2.0f,-1.5f,+2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(+2.0f,-1.5f,+2.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(+2.0f,-1.5f,-2.0f);	
	glEnd();

        // TOP
	glBindTexture( GL_TEXTURE_2D, g_iBoxTop);
	glBegin(GL_QUADS);
		if( g_bToggleTexture) glColor3f( 1.0, 1.0, 1.0);
		else glColor3f(0.5,0.5,0.8);
	        glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(-2.0f,+1.5f,-2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,-2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,+2.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(-2.0f,+1.5f,+2.0f);
    	glEnd();


        // FRONT
	glBindTexture( GL_TEXTURE_2D, g_iBoxFront);
	glBegin(GL_QUADS);
		if( g_bToggleTexture) glColor3f( 1.0, 1.0, 1.0);
		else glColor3f(0.8,1.0,1.0);
        	glNormal3f( 0.0f, 0.0f, 1.0f); 
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(-2.0f,-1.5f,-2.0f);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(+2.0f,-1.5f,-2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,-2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(-2.0f,+1.5f,-2.0f);
	glEnd();


	// BACK
	glBindTexture( GL_TEXTURE_2D, g_iBoxBack);
	glBegin(GL_QUADS);
		if( g_bToggleTexture) glColor3f( 1.0, 1.0, 1.0);
		else glColor3f(1.0,0.2,1.0);
		glNormal3f( 0.0f, 0.0f, -1.0f); 
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(-2.0f,+1.5f,+2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,+2.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(+2.0f,-1.5f,+2.0f);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(-2.0f,-1.5f,+2.0f);
		
	glEnd();

	


	// RIGHT
	glBindTexture( GL_TEXTURE_2D, g_iBoxRight);
	glBegin(GL_QUADS);
		if( g_bToggleTexture) glColor3f( 1.0, 1.0, 1.0);
		else glColor3f(1,1,1);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(+2.0f,-1.5f,-2.0f);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(+2.0f,-1.5f,+2.0f);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,+2.0f);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(+2.0f,+1.5f,-2.0f);
	glEnd();
		
	glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfLightSpecular);
	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	
}

inline void renderBunny() {

	
	if( g_bToggleTexture) glBindTexture( GL_TEXTURE_2D, 0);
	if(g_bToggleCubeMap)
	{
		glColor3f(1.0,1.0,1.0);
		if( !g_bToggleGlassBunny )	bunny.draw(); 
		else
		{
			glDisable(GL_LIGHTING);
			glassBunny.draw();
			if(g_bToggleLighting)	glEnable(GL_LIGHTING);
		}
	}
	else	
	{
		glColor3f(0.803921568, 0.396078431, 0.0);
		bunny.draw(); 
	}
	
	

}

inline void renderGlow()
{
	
	glDisable(GL_LIGHTING);
	
	glBindTexture( GL_TEXTURE_2D, 0);

	glColor4f(0.803921568, 0.396078431, 0.0, g_fFurAlpha/(float)g_iFurLayers);


	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	for(int i=0; i < g_iFurLayers; i++)
	{
		fur[i].draw();

	}
	glDisable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	if(g_bToggleLighting) glEnable(GL_LIGHTING);

	glPopMatrix();
}

inline void renderFur()
{
	
	glDisable(GL_LIGHTING);
	
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	for(int i=0; i < g_iFurLayers; i++)
	{
		glColor4f( (float)(i+1)/(float)g_iFurLayers,(float)(i+1)/(float)g_iFurLayers,(float)(i+1)/(float)g_iFurLayers, pow(1-((float)i/(float)g_iFurLayers),2) );
		glBindTexture( GL_TEXTURE_2D, g_ipFurLayers[i*6/g_iFurLayers]);
		fur[i].draw();

	}
	glDisable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	if(g_bToggleLighting) glEnable(GL_LIGHTING);
	glBindTexture( GL_TEXTURE_2D, 0);

	glPopMatrix();
}

inline void renderSmallSpheres() {
	
		
	for( int i=0; i<4; i++) 
	{
		if( g_bToggleTexture) {
			//glActiveTexture(GL_TEXTURE0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glBindTexture( GL_TEXTURE_2D, g_ipSphereSmall[i]);
		}
		else glColor4fv( smallColor[i]);
	
		int j=i*4;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glRotatef(g_fRotationAngles1[j+0],g_fRotationAngles1[j+1],g_fRotationAngles1[j+2],g_fRotationAngles1[j+3]);
		glRotatef(g_fRotationAngles2[j+0],g_fRotationAngles2[j+1],g_fRotationAngles2[j+2],g_fRotationAngles2[j+3]);

		glCallList(g_ipSmallSphere[i]);		
		glPopMatrix();
	}
}

inline void renderSpheres(bool mirror, bool isCubeMap) {

	if( g_bWireframe)	glDisable(GL_CULL_FACE);
	else	glEnable(GL_CULL_FACE);

	glColor3f( 0.5, 0.5, 0.5);
	if( g_bToggleTexture) {
		if( g_bToggleCubeMap) {
			glDisable( GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE1);
			glEnable( GL_TEXTURE_CUBE_MAP);
			glBindTexture( GL_TEXTURE_CUBE_MAP, g_iCubeMap);
			glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			// Transform the Texture Matrix
			
			if(mirror && !isCubeMap)
			{
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glRotatef(-g_fAzimuth,0,1,0);	// rotate azimuth
				glRotatef(-g_fElevation,1,0,0);	// rotate elevation
				glMatrixMode(GL_MODELVIEW);
			}
			else if(!mirror && !isCubeMap)
			{
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glMultMatrixf(g_fMirrorX);
				glRotatef(-g_fAzimuth,0,1,0);	// rotate azimuth
				glMatrixMode(GL_MODELVIEW);
			}
			else if(!mirror && isCubeMap)
			{
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glMultMatrixf(g_fMirrorX);
				glRotatef(-g_fAzimuth,0,1,0);	// rotate azimuth
				glRotatef(-g_fElevation+180,1,0,0);	// rotate elevation
				glMatrixMode(GL_MODELVIEW);
			}
			
			glEnable( GL_TEXTURE_GEN_S);
			glEnable( GL_TEXTURE_GEN_T);
			glEnable( GL_TEXTURE_GEN_R);
		}
		else if(!g_bBunny){
			glBindTexture( GL_TEXTURE_2D, g_iSphereEarth);
		}
		
	}

	// render
	if(g_bBunny)
	{
		renderBunny();	
	}
	else
	{
		glColor3f( 1.0, 1.0, 1.0);
		glCallList( bigSphere);

		
	}

	// if no more textures, bug out!
	if( !g_bToggleTexture ) return;

	// clean up cube map
	if( g_bToggleCubeMap) 
	{
		// Restore the Texture Matrix
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		// Disable CubeMapping
		glDisable( GL_TEXTURE_GEN_S);
		glDisable( GL_TEXTURE_GEN_T);
		glDisable( GL_TEXTURE_GEN_R);
		glDisable( GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE0);
		glEnable( GL_TEXTURE_2D);
	}
}

inline void updateAtmosphere()
{
	float alpha = 90 - g_fAzimuth;
	alpha   = fmod((double)alpha,360.0);
	float b = g_fTransZ * cos (g_fElevation * M__PI / 180.0);
	float a2 = b*b + 16 + 8*b*cos(M__PI * alpha / 180.0);
	g_fAtmosphereAzimuth = acos((a2 - b*b + 16)/(8 * sqrt(a2))) * (180.0/M__PI);
	g_fAtmosphereAzimuth = 90 - g_fAtmosphereAzimuth;
	if((alpha < 0 && alpha > -180) || (alpha > 180 && alpha < 360))	g_fAtmosphereAzimuth = 180 - g_fAtmosphereAzimuth;
	g_fAtmosphereElevation = atan ( sin(g_fElevation * M__PI / 180.0)*g_fTransZ / sqrt(a2)) * (180.0/M__PI);
	g_fAtmosphereTransZ = sqrt (a2) / cos ( g_fAtmosphereElevation * (M__PI / 180.0) );

	// calculate size of the Atmosphere
	g_fAtmosphereSize1 = (( -g_fTransZ - g_fBigSphereRadius ) * tan( asin( g_fBigSphereRadius / -g_fTransZ ) ) ) * 1.35;
	g_fAtmosphereSize2 = (( g_fAtmosphereTransZ - g_fBigSphereRadius ) * tan( asin( g_fBigSphereRadius / -g_fAtmosphereTransZ ) ) ) * 1.35;
}

inline void renderAtmosphere(bool mirror)
{
	// render halo
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float size;

	if(!mirror)
	{
		glRotatef(g_fAtmosphereAzimuth,0,-1,0);	// rotate azimuth
		glRotatef(g_fAtmosphereElevation,1,0,0);	// rotate elevation
		size = g_fAtmosphereSize2;
	}
	else
	{
		glRotatef(g_fAzimuth,0,-1,0);	// rotate azimuth
		glRotatef(g_fElevation,-1,0,0);	// rotate elevation
		size = g_fAtmosphereSize1;
	}
	glTranslatef( 0, 0, g_fBigSphereRadius);

	glColor3f(0.5,0.5,1.0);
	glBindTexture( GL_TEXTURE_2D, g_iAtmosphere);

	//glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	//glDepthMask(0);

	
	glBegin(GL_QUADS);
		// Normal
		glNormal3f( 0.0f, 0.0f, 1.0f); 
		// Tex coords & vertex coords
		glTexCoord2f( 0.0f, 0.0f);
		glVertex3f(-size,-size,0);
		glTexCoord2f( 1.0f, 0.0f);
		glVertex3f(size,-size,0);
		glTexCoord2f( 1.0f, 1.0f);
		glVertex3f(size,size,0);
		glTexCoord2f( 0.0f, 1.0f);
		glVertex3f(-size,size,0);
	glEnd();

	//glDepthMask(1);
	//glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	if(g_bToggleLighting)	glEnable(GL_LIGHTING);
	glPopMatrix();
}

inline void renderLights()
{
	glPushMatrix();
	
	//Rotate Light
	glRotatef(g_pfLightRotationAngle[0],g_pfLightRotationAngle[1],g_pfLightRotationAngle[2],g_pfLightRotationAngle[3]);

	//Calculate Current Light Position (for Shadows)
	float *M = new float[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, M);
	for( int i=0; i<3; i++)	g_pfCurrLightPos[i] = g_pfLightPos[i];
	MatrixVectorProd3( M, g_pfCurrLightPos);	
	delete [] M;

	glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
	if(g_bMirror) glLightfv( GL_LIGHT1, GL_POSITION, g_pfLightPos1);

	/*
	// render a point for the Light 
	if(g_bToggleLighting) glDisable(GL_LIGHTING);
	glPointSize(3);
	
	glBegin(GL_POINTS);
		glVertex3f(g_pfLightPos[0],g_pfLightPos[1],g_pfLightPos[2]);
	glEnd();
	if(g_bToggleLighting) glEnable(GL_LIGHTING);
	*/
	if(g_bToggleLighting) glEnable(GL_LIGHTING);
	
	glPopMatrix();
}


inline void renderScene(bool bigSphere, bool smallSpheres, bool lights, bool mirror, bool isCubeMap, bool shadows, bool particles)
{
	if(lights)	renderLights();
	if(lights && shadows)	renderShadows(bigSphere,smallSpheres,lights,mirror,isCubeMap,false,false);
	else
	{
		renderWalls();
		if(smallSpheres)	renderSmallSpheres();	
		if(bigSphere)	renderSpheres(mirror,isCubeMap);
		if(mirror)	renderMirror();
	}
	if(particles)	
	{
		//if(isCubeMap)	glPointSize(g_fParticleSize/3.0f);
		//else	
		renderParticles();
	}
	if(!g_bBunny && !g_bToggleCubeMap && g_bToggleTexture && !(g_bToggleShadows && !shadows))	renderAtmosphere(mirror);
	if(g_bBunny && !g_bToggleCubeMap && g_bToggleTexture && !(g_bToggleShadows && !shadows))
	{
		if(g_bToggleFur)	renderFur();
		else	renderGlow();
	}
}

/*
 * RENDER ONE IMAGE 
 */
inline void displayfunc()
{
    // startup the frame
    g_dTimerFrame = timerFrameStart();

    // render cube map
	if( g_bToggleCubeMap) renderCubeMap();
	if( g_bToggleShadows) updateShadowMap();
	if( g_bToggleParticles) updateParticlePositions();
	
	// setup and clear the viewport
	setupUserViewProj();

	// Modelview Transformation
	if( g_bToggleTexture) glEnable( GL_TEXTURE_2D);
	else glDisable( GL_TEXTURE_2D);


	// Clear Buffer for new frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// render Mirror
	if(g_bMirror)
	{
		//glDisable(GL_BLEND);
		setupWireframe();
		glLoadIdentity();
		glMultMatrixf(g_fMirrorX);
		glTranslatef( 0, 0, g_fTransZ);	// Zoom
		glRotatef(g_fElevation,1,0,0);	// rotate elevation
		glRotatef(-g_fAzimuth,0,1,0);	// rotate azimuth
		glTranslatef(4.0f,0.0f,0.0f);
		if(!g_bToggleShadows)	glCullFace(GL_FRONT);

		// Stencil Buffer
		if(g_bStencil)
		{
			//Setup the Stencil Buffer
			glEnable(GL_STENCIL_TEST);
			glColorMask(false,false,false,false);
			glStencilFunc(GL_ALWAYS,1,1);
			glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
			glDisable(GL_DEPTH_TEST);

			//Draw the MirrorMaks on the Stencil Buffer
			glFrontFace(GL_CW);
			glBegin(GL_QUADS);
				glVertex3f(-2.0f,-1.5f,-2.0f);
				glVertex3f(-2.0f,+1.5f,-2.0f);
				glVertex3f(-2.0f,+1.5f,+2.0f);
				glVertex3f(-2.0f,-1.5f,+2.0f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(-2.0f,-1.5f,+2.0f);
				glVertex3f(-2.0f,+1.5f,+2.0f);
				glVertex3f(-2.0f,+1.5f,-2.0f);
				glVertex3f(-2.0f,-1.5f,-2.0f);
			glEnd();
			glFrontFace(GL_CCW);
			
			//Reconfigure the Stencil Buffer
			glEnable(GL_DEPTH_TEST);
			glColorMask(true,true,true,true);
			glStencilFunc(GL_EQUAL,1,1);
			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
		} 


		// Render Scene and adjust the Transformation Matrix of the CubeMap
		renderScene(true,true,g_bToggleLighting,false,false,g_bToggleShadows,g_bToggleParticles);

		if(g_bStencil)
		{
			// Disable the Stencil Test
			glDisable(GL_STENCIL_TEST);
		}
		if(!g_bToggleShadows)	glCullFace(GL_BACK);
	}

	glLoadIdentity();
	glTranslatef( 0, 0, g_fTransZ);	// Zoom
	glRotatef(g_fElevation,1,0,0);	// rotate elevation
	glRotatef(g_fAzimuth,0,1,0);	// rotate azimuth
	
	// render the scene
	renderScene(true,true,g_bToggleLighting,true,false,g_bToggleShadows,g_bToggleParticles);
		
	// display status messages
    //displayOutput();

	// ANIMATION
    g_dAnimDeltaTime=g_dTimerFrame-g_dAnimLastTime;
    g_dAnimLastTime=g_dTimerFrame;
	if(g_bAnimateLights)
	{
		g_pfLightRotationAngle[0]+=(float)(M__PI_2*(g_dAnimDeltaTime*10.0));
		g_pfLightRotationAngle[0]= fmod(g_pfLightRotationAngle[0], (float)360.0);
	}
	if(g_bAnimateSpheres)
	{
		for(int i=0;i<4;i++)	
		{
			g_fRotationAngles1[i*4]+=(float)(M__PI_2 *(double) (i+1)*(g_dAnimDeltaTime*10.0));
			g_fRotationAngles2[i*4]+=(float)(M__PI_2 *(double) (i+1)*(g_dAnimDeltaTime*10.0));
		}
	}
		
    timerFrameEnd(g_sProgramName.c_str());
    glutSwapBuffers();     // swap the front and back buffer to display the next frame.
}


/*
 * IDLE-FUNCTION. Called whenever nothing else is to do.
 */
inline void idlefunc(void) {
     glutPostRedisplay();    // signals GLUT to call the display function. 
     // Never call the display function directly, strange side-effects!
}


/*
 * RESHAPEFUNC. Called whenever the user resizes the window. 
 * width, height are the new dimensions of the viewport.
 */
inline void reshapefunc(int iWidth,int iHeight) {
	// update viewport dimensions
    	g_iWinWidth	=iWidth;
	g_iWinHeight	=iHeight;
    	g_fAspect	=(float)iWidth/(float)iHeight;
	
	// aplying the viewport parameters is done in the 
	// displayfunc so no further work has to be done here
    	glutPostRedisplay();
}


/*
 * MOUSEFUNC. Called whenever a mouse-button is pressed. 
 * button returns a token for the mouse-button, 
 * x and y are the mouse coordinates at that time.
 */
inline void mousefunc(int button,int state,int x,int y) {
    if (button==GLUT_LEFT_BUTTON)
        if (state==GLUT_DOWN) {
                g_iLeftMouseFlag=1;
                g_imx = x;
                g_imy = y;
        }
        else g_iLeftMouseFlag=0;
    else if (button==GLUT_RIGHT_BUTTON)
		if (state==GLUT_DOWN) {
            g_iRigthMouseFlag=1;
            g_imy = y; // Otherwise Zoom will jump
		}
        else g_iRigthMouseFlag=0;       
}


/*
 * MOTIONFUNC. Called whenever the mouse is moved.
 * x,y as in mousefunc.
 */
inline void motionfunc(int x,int y) {
   	
	if (g_iLeftMouseFlag>0) // Rotate
	{
		// new Mouse Movement
		float dx = (float) -(g_imx - x) / (float) 10;
		float dy = (float) -(g_imy - y) / (float) 10;
		
		// Update Azimuth & Elevation
		g_fAzimuth   += dx;
		g_fElevation += dy;
		
		// predevent big values
		g_fAzimuth   = fmod((double)g_fAzimuth,360.0);
		g_fElevation = fmod((double)g_fElevation,360.0);

		

		


		
		// Update global rotation so far
		//g_fRotX += rotX;
		//g_fRotX = (float)fmod((double)g_fRotX,360.0);
		//g_fRotY = (float)cos( ((double) g_fRotX * M__PI)/180.0) * g_fElevation;
		//g_fRotZ = (float)sin( ((double) g_fRotX * M__PI)/180.0) * g_fElevation;
		
		// Debug
		//printf("rotX= %f\n", g_fRotX);
		//printf("rotY= %f\n", g_fRotY);
		//printf("rotZ= %f\n", g_fRotZ);
		//printf("azimuth  = %f\n", g_fAzimuth);
		//printf("elevation= %f\n\n", g_fElevation);
		
		g_imx=x;
		g_imy=y;
		
	}
	else if (g_iRigthMouseFlag>0) // Zoom
	{
		// Calculate relativ zoom
		float  dist = (float)(g_imy - y)/(float)50;
		// make zoom logarithmic
		dist *= log((fabs((double)g_fTransZ/10.0)+1.0));
		// update globas zoom
		g_fTransZ  += dist;
		
		

		// Debug
		//printf("g_fTransZ= %f\n",g_fTransZ);
		
		// convert Azimuth and Elevation to radians
		//double azimuth   = (double) g_fAzimuth   * M__PI / (double)180;
		//double elevation = (double) g_fElevation * M__PI / (double)180;
		
		// project the position-vector on the three axis
		//float transX = dist*sin(-azimuth)*cos(elevation);
		//float transY = dist*sin(elevation);
		//float transZ = dist*cos( azimuth)*cos(elevation);
		
		// translate view
		//glTranslatef( transX, transY, transZ);
		
		g_imy = y;
	}

	if(g_bCubeMapOffset)
	{
		g_fCubeMapOffset=g_fBigSphereRadius*pow((1/(-g_fTransZ+0.5f)),1.5f*(-g_fTransZ-0.5f));
	
		// convert Azimuth and Elevation to radians
		double azimuth   = (double) g_fAzimuth   * M__PI / (double)180;
		double elevation = (double) g_fElevation * M__PI / (double)180;

		g_fCubeMapOffsetX= g_fCubeMapOffset*sin(azimuth);
		g_fCubeMapOffsetY= g_fCubeMapOffset*sin(elevation);
		g_fCubeMapOffsetZ= g_fCubeMapOffset*cos(azimuth);
	}

	// calculate Atmosphere orientation
	if(g_bToggleTexture)	updateAtmosphere();
	
	glutPostRedisplay();
}

/*
 * KEYBOARDFUNC. Called whenever a key was pressed. 
 * key specifies the key that-3.5+g was pressed,
 * x and y specify the mouse-cordinates at that time.
 */
inline void keyboardfunc(unsigned char key,int x,int y) {
 
    switch(key) {
		case 'q': case 'Q': case 27:
			
			// display list cleanup
			glDeleteLists( bigSphere, 1);
   			for (int i = 0;i<4;i++) {
				glDeleteLists( g_ipSmallSphere[i], 1);
			}

			// texture cleanup
			glDeleteTextures( 1, &g_iBoxBack);
			glDeleteTextures( 1, &g_iBoxFront);
			glDeleteTextures( 1, &g_iBoxLeft);
			glDeleteTextures( 1, &g_iBoxRight);
			glDeleteTextures( 1, &g_iBoxTop);
			glDeleteTextures( 1, &g_iBoxBottom);
			glDeleteTextures( 1, &g_iSphereEarth);
			glDeleteTextures( 4, g_ipSphereSmall);

			exit(0);
			break;  // never reached, but looks nice :)

		case 'a':
			g_bAnimateSpheres=!g_bAnimateSpheres;
			if (g_bAnimateSpheres) sprintf(g_pcLastOutput,"Sphere Animation is ENABLED\n"); else sprintf(g_pcLastOutput,"Sphere Animation is DISABLED\n"); 
			
			break;
			
		case 'A':
			if(g_bToggleLighting)
			{
				g_bAnimateLights=!g_bAnimateLights;
				if (g_bBunny) sprintf(g_pcLastOutput,"Lighting Animation is ENABLED\n"); else sprintf(g_pcLastOutput,"Lighting Animation is DISABLED\n"); 
			}
			break;

		case '1': 
			g_fParam1+=0.01;
			printf("Param1 = %f\t Param2 = %f\n",g_fParam1,g_fParam2);
			break;
			
		case '2': 
			g_fParam1-=0.01;
			printf("Param1 = %f\t Param2 = %f\n",g_fParam1,g_fParam2);
			break;
			
		case '3': 
			g_fParam2+=0.1;
			printf("Param1 = %f\t Param2 = %f\n",g_fParam1,g_fParam2);
			break;
			
		case '4': 
			g_fParam2-=0.1;
			printf("Param1 = %f\t Param2 = %f\n",g_fParam1,g_fParam2);
			break;
			
		case 'b': 
			g_bBunny=!g_bBunny;    // first, update internal var.
			if (g_bBunny) sprintf(g_pcLastOutput,"Bunny is ENABLED\n"); else sprintf(g_pcLastOutput,"Bunny is DISABLED\n"); 
			break;

		case 'B' :
			g_bToggleGlassBunny = !g_bToggleGlassBunny;
			if (g_bToggleGlassBunny) sprintf(g_pcLastOutput,"Glass Bunny is ENABLED\n"); else sprintf(g_pcLastOutput,"Glass Bunny is DISABLED\n"); 
			break;
			
		case 'r':
			//glTranslatef( 0.0, 0.0, g_fTransZ+3.5);
			g_fTransZ = -3.5;
			g_fAzimuth=(float)0;
			g_fElevation=(float)0;
			if(g_bCubeMapOffset)
			{
				g_fCubeMapOffset=g_fBigSphereRadius*pow((1/(-g_fTransZ+0.5f)),1.5f*(-g_fTransZ-0.5f));
					
				// convert Azimuth and Elevation to radians
				double azimuth   = (double) g_fAzimuth   * M__PI / (double)180;
				double elevation = (double) g_fElevation * M__PI / (double)180;

				g_fCubeMapOffsetX= g_fCubeMapOffset*sin(azimuth);
				g_fCubeMapOffsetY= g_fCubeMapOffset*sin(elevation);
				g_fCubeMapOffsetZ= g_fCubeMapOffset*cos(azimuth);
			}			
			sprintf(g_pcLastOutput,"RESET Viewpoint\n");
			break;

		case 'R' :
			g_fRotationAngles1[0] = 0.0f;
			g_fRotationAngles1[4] = 0.0f;
			g_fRotationAngles1[8] = 0.0f;
			g_fRotationAngles1[12] = 0.0f;

			g_fRotationAngles2[0] = 0.0f;
			g_fRotationAngles2[4] = 0.0f;
			g_fRotationAngles2[8] = 0.0f;
			g_fRotationAngles2[12] = 0.0f;

			g_pfLightRotationAngle[0] = 0.0f;

			if (g_bToggleCubeMap) sprintf(g_pcLastOutput,"Reflection Mapping is ENABLED\n"); else sprintf(g_pcLastOutput,"Reflection Mapping is DISABLED\n"); 
			
			break;

		case 'c' :
			if( g_bToggleTexture) {
				g_bToggleCubeMap=!g_bToggleCubeMap;
				if (g_bToggleCubeMap) sprintf(g_pcLastOutput,"Reflection Mapping is ENABLED\n"); else sprintf(g_pcLastOutput,"Reflection Mapping is DISABLED\n"); 
			}
			break;

		case 'o' :
			if( g_bToggleTexture) {
				g_bCubeMapOffset=!g_bCubeMapOffset;
				if (g_bCubeMapOffset) 
				{
					g_fCubeMapOffset=g_fBigSphereRadius*pow((1/(-g_fTransZ+0.5f)),1.5f*(-g_fTransZ-0.5f));
					
					// convert Azimuth and Elevation to radians
					double azimuth   = (double) g_fAzimuth   * M__PI / (double)180;
					double elevation = (double) g_fElevation * M__PI / (double)180;

					g_fCubeMapOffsetX= g_fCubeMapOffset*sin(azimuth);
					g_fCubeMapOffsetY= g_fCubeMapOffset*sin(elevation);
					g_fCubeMapOffsetZ= g_fCubeMapOffset*cos(azimuth);

					sprintf(g_pcLastOutput,"Reflection Mapping Offset Calculations are ENABLED\n"); 
				}
				else sprintf(g_pcLastOutput,"Reflection Mapping Offset Calculations are DISABLED\n");
			}
			break;
			
		case 'F': 
			g_bToggleFur=!g_bToggleFur;
			if (g_bFogMode) sprintf(g_pcLastOutput,"Bunny Fur is ENABLED\n"); else sprintf(g_pcLastOutput,"Bunny Fur is DISABLED\n"); 
			break;
		case 'f' :
			g_bFullScreen = !g_bFullScreen;
			if (g_bFullScreen) {
				g_iSafeWinWidth = g_iWinWidth;
				g_iSafeWinHeight = g_iWinHeight;
				g_iPosX = glutGet(GLUT_WINDOW_X);
				g_iPosY = glutGet(GLUT_WINDOW_Y);
				glutFullScreen();
				sprintf(g_pcLastOutput,"FULLSCREEN Mode\n");
			} else {
				glutPositionWindow(g_iPosX,g_iPosY);
				glutReshapeWindow(g_iSafeWinWidth, g_iSafeWinHeight);
				sprintf(g_pcLastOutput,"WINDOWED Mode\n");
			}
			break;
 
		case 'm':
			g_bMirror=!g_bMirror;
			if(!g_bMirror)g_bStencil=g_bMirror;
			if (g_bMirror) sprintf(g_pcLastOutput,"Mirror is ENABLED\n"); else sprintf(g_pcLastOutput,"Mirror is DISABLED\n"); 
			setupLighting();
			setupWireframe();
			break;

		case 'M': 
			if(!g_bMirror) break;
			g_bStencil=!g_bStencil;				
			if (g_bStencil) sprintf(g_pcLastOutput,"Stencil Buffer is ENABLED\n"); else sprintf(g_pcLastOutput,"Stencil Buffer is DISABLED\n"); 
			break;

		case 'p': 
			g_bToggleParticles=!g_bToggleParticles;
			if(!g_bToggleParticles)
			{
				g_bParticleAging=true;
				g_bParticleMovement=true;
			}
			if(g_bToggleParticles)	initParticles();				
			if (g_bToggleParticles) sprintf(g_pcLastOutput,"Particle System is ENABLED\n"); else sprintf(g_pcLastOutput,"Particle System is DISABLED\n"); 
			break;

		case 'P': 
			g_iParticleEmitter=(g_iParticleEmitter+1)%4;
			switchParticleEmitter();
			sprintf(g_pcLastOutput,"Particle Emitter #%i is ENABLED\n",g_iParticleEmitter);
			break;

		case 'g': 
			g_iParticleGravityMode=(g_iParticleGravityMode+1)%7;
			sprintf(g_pcLastOutput,"Particle Gravity Field %i is ENABLED\n",g_iParticleGravityMode);
			break;

		case 'G': 
			if(g_iParticleGravityMode==0)	g_iParticleGravityMode=6;
			else g_iParticleGravityMode=(g_iParticleGravityMode-1)%7;
			sprintf(g_pcLastOutput,"Particle Gravity Field %i is ENABLED\n",g_iParticleGravityMode);
			break;
	
		case 'h': case 'H': 
			g_bToggleHose=!g_bToggleHose;
            if(g_bToggleHose){	g_iParticleCount=50000;	initParticles();	}
			else{					g_iParticleCount=2500;	initParticles();	}
			if (g_bToggleHose) sprintf(g_pcLastOutput,"Water Hose is ENABLED\n"); else sprintf(g_pcLastOutput,"Water Hose is DISABLED\n"); 
			break;
		
		
		case 's':
			if( !g_bToggleLighting) break;  // Shadows without Lights does not make sense ;-)
			g_bToggleShadows = !g_bToggleShadows;
			if (g_bToggleShadows) sprintf(g_pcLastOutput,"Shadow is ENABLED\n"); else sprintf(g_pcLastOutput,"Shadow is DISABLED\n"); 
			break;

		case 'S': 
			{
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                int uiWidth  = viewport[2];
                int uiHeight = viewport[3];
                glPixelStorei(GL_UNPACK_ALIGNMENT,1);
                glPixelStorei(GL_PACK_ALIGNMENT,1);
                glReadBuffer(GL_FRONT);
                unsigned char *buffer = new unsigned char[uiWidth*uiHeight*3];
                glReadPixels(viewport[0],viewport[1],uiWidth,uiHeight,GL_RGB,GL_UNSIGNED_BYTE,buffer);
                cimg_library::CImg<unsigned char> image(uiWidth,uiHeight);
                // TODO

                glReadBuffer(GL_BACK);
                image.save_bmp("screenshot.bmp");
			}
			sprintf(g_pcLastOutput,"Frame Saved\n");
			break; 
		
		case 't':	case 'T': // This is where Texture is activated
			if(!g_bToggleTexture)
			{
				g_bToggleCubeMap=true;
			}
			else
			{
				g_bToggleCubeMap=false;
			}
			g_bToggleTexture = !g_bToggleTexture;
			
			if (g_bToggleTexture) sprintf(g_pcLastOutput,"Texture is ENABLED\n"); else { sprintf(g_pcLastOutput,"Texture is DISABLED\n"); g_bToggleCubeMap=false; }
			break; 
			
		case 'l':	case 'L': // This is where lighting is activated
			g_bToggleLighting = !g_bToggleLighting;
			if(!g_bToggleLighting){ g_bAnimateLights=false; g_bToggleShadows=false;	}
			if (g_bToggleLighting) sprintf(g_pcLastOutput,"Lighting is ENABLED\n"); else sprintf(g_pcLastOutput,"Lighting is DISABLED\n"); 
			setupLighting();
			break; 
			
		case 'v':	case 'V': 
			g_bToggleTestQuad = !g_bToggleTestQuad;
			if (g_bToggleTestQuad) sprintf(g_pcLastOutput,"Test Quad is ENABLED\n"); else sprintf(g_pcLastOutput,"Test Quad is DISABLED\n"); 
			break; 

		case 'w':	case 'W': // This is how the wireframe is activated
			g_bWireframe=!g_bWireframe;    // first, update internal var.
			if (g_bWireframe) sprintf(g_pcLastOutput,"Wireframe is ENABLED\n"); else sprintf(g_pcLastOutput,"Wireframe is DISABLED\n"); 
			setupWireframe();
			break;

		case 'x':
			if(g_bToggleParticles)	g_bParticleAging = !g_bParticleAging;
			else break;
			if (g_bToggleParticles) sprintf(g_pcLastOutput,"Particle Aging is ENABLED\n"); else sprintf(g_pcLastOutput,"Particle Aging is DISABLED\n"); 
			break;
				
		case 'X':
			if(g_bToggleParticles)	g_bParticleMovement = !g_bParticleMovement;
			else break;
			if (g_bParticleMovement) sprintf(g_pcLastOutput,"Particle Movement is ENABLED\n"); else sprintf(g_pcLastOutput,"Particle Movement is DISABLED\n"); 
			break;

		default:
			sprintf(g_pcLastOutput,"undefined key\n");
    }
	g_fFadeValue = 255;
    glutPostRedisplay();
}


/*
 * SPECIALFUNC. Analogous to keyboardfunc, but called whenever a "special key" was pressed.
 * special keys include UP,DOWN,LEFT,RIGHT,F1 .. F12 etc. See also the GLUT Manual.
 */
inline void specialfunc(int key,int x,int y) {
    
}

