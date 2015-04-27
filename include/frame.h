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

#ifndef _FRAME_
#define _FRAME_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include "normalize.h"
#include "mathtools.h"
#include "meshtools.h"
#include "primitives.h"
#include "texture.h"
#include "tools.h"
#include "vbObject.h"

#ifndef M__PI
#define M__PI   3.1415926535898
#endif
#ifndef M__PI_2
#define M__PI_2 1.5707963267949
#endif
#ifndef M__2_PI
#define M__2_PI 6.2831853071796
#endif


static std::string g_sProgramName( "BallsInABox \"Computer Graphics\" 2005" );
static std::string g_sAuthor("Alexandru Duliu (duliu@in.tum.de) & Markus Bucher (bucherm@in.tum.de)");

/*
 * VARIABLE declarations
 */
static int g_iWinWidth   = 512;    // initial width of viewport.
static int g_iWinHeight  = 512;    // initial height of viewport.
static GLfloat g_fAspect = (float)g_iWinWidth/(float)g_iWinHeight;   // initial aspect ratio of viewport.
static GLfloat g_fBigSphereRadius = 0.5f;


/*
 * MOUSE HANDLING
 */
static int g_imx,g_imy;              
static int g_iLeftMouseFlag=0,g_iRigthMouseFlag=0;
static GLfloat g_fRotX, g_fRotY, g_fRotZ, g_fElevation, g_fAzimuth; 
static GLfloat g_fTransZ=-3.5; 
static float g_fpRotMatrix[16];
static float g_fAtmosphereSize1, g_fAtmosphereSize2, g_fAtmosphereTransZ, g_fAtmosphereElevation, g_fAtmosphereAzimuth;

// ligthing 
static bool g_bToggleLighting = false;
static GLfloat g_pfLightPos[4] = {0.0f,1.5f,0.0f,1.0f};
static GLfloat g_pfCurrLightPos[4] = {0.0f,1.5f,0.0f,1.0f};
static GLfloat g_pfLightSpecular[4]		= {1.0f,1.0f,1.0f,1.0f};
static GLfloat g_pfLightBlack[4]		= {0.0f,0.0f,0.0f,0.0f};
static GLfloat g_pfLightDiffuse[4]		= {0.7f,0.7f,1.0f,1.0f};
static GLfloat g_pfLigthAmbient[4]		= {0.3f,0.3f,0.3f,1.0f};

// Mirror
static GLfloat g_fMirrorX[16] = {-1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
static bool g_bMirror=false;
static bool g_bStencil=true;

// mirrored light
static GLfloat g_pfLightPos1[4] = {-4.0f,1.5f,0.0f,1.0f};
static GLfloat g_pfLightSpecular1[4]		= {0.3f,0.3f,0.3f,1.0f};

// other switches
static bool g_bWireframe=false;   // toggles between wireframe and solid
static bool g_bFullScreen=false;
static int g_iSafeWinWidth,	g_iSafeWinHeight, g_iPosX, g_iPosY;
static bool g_bFogMode = false;

// Shadows
static bool g_bToggleShadows= false;
static int g_iShadowMapSize = 512;
GLuint g_iShadowMapTex;
//GLuint g_iShadowMapCubeMapTex;
static float g_pfBiasMatrix[16] = {	0.5f,0.0f,0.0f,0.0f,
									0.0f,0.5f,0.0f,0.0f,
									0.0f,0.0f,0.5f,0.0f,
									0.5f,0.5f,0.5f,1.0f };
static float g_pfLightProjection[16] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
static float g_pfLightModelview[16]  = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
static float g_pfCameraProjection[16]= {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
static float g_pfCameraModelview[16] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
static float g_pfTextureMatrix[16]   = {1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
static GLfloat g_pfShadowSpecular[4]		= {0.0f,0.0f,0.0f,1.0f};
static GLfloat g_pfShadowDiffuse[4]		= {0.3f,0.3f,0.3f,1.0f};
static GLfloat g_pfShadowAmbient[4]		= {0.1f,0.1f,0.1f,1.0f};
static float g_fParam1=2.0;
static float g_fParam2=0.5;


// Particles
VBObject particles;
static bool g_bToggleParticles=false;
static bool g_bParticleAging=true;
static bool g_bParticleMovement=true;
static float g_pfParticleOriginSource[3] = {0.8f,0.5f,-0.9f};
static float g_pfParticleOrigin[3] = {0.8f,0.5f,-0.9f};
static float g_pfOriginMatrix[16]= {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
static int g_iParticleEmitter = 1;
static int g_iParticleGravityMode=0;
static float *g_pfParticlePositions;		// x, y, z
static float *g_pfParticleAttributes;		// vx, vy, vz, life, time of last remaining life update
static float *g_pfParticleStartAttributes;	// speeds with which particles are initialized, starting life
static float *g_pfParticleColors;			// red, green, blue, alpha
static int g_iParticleCount=2500;
static float g_fParticleSpeed=0.5f;
static float g_fParticleLife=5.0f;
static float g_fParticleSize=8.0f;
static float g_fParticleGravity=6.82;
static float g_fImpactSpeedDissipation=0.75;
static bool g_bToggleHose=false;

// fade text
static float g_fFadeValue = 0;
static char g_pcLastOutput[200];

/*
 * VARIOUS HANDLES FOR DISPLAY-LISTS AND TEXTURES
 */
GLuint g_ipSmallSphere[4];     
GLuint bigSphere;
int g_iBigSphereLOD = 128;
float smallColor[4][4]={{1.0f,0.0f,1.0f,1.0f},
                        {1.0f,1.0f,0.0f,1.0f},
                        {0.0f,1.0f,0.2f,1.0f},
                        {0.0f,1.0f,1.0f,1.0f}};

float g_ipSmallSphereGeo[4][6] = {	{0.0f, 0.3f, 1.0f, 0.0f, 0.0f,64.0f},
                                    {0.0f, 0.2f, 0.8f, 0.5f,-0.9f,64.0f},
                                    {0.0f, 0.1f,-0.8f,-0.6f,-0.9f,64.0f},
                                    {0.0f, 0.2f, 0.8f,-0.7f, 0.8f,64.0f} };
					
//Model
static bool g_bBunny=false;
float *g_fpBunnyVertex;
int g_iBunnyVertexSize;
GLuint *g_ipBunnyIndex;
int g_iBunnyIndexSize;
float *g_fpBunnyVertexNormals;

//BufferObjects
VBObject bunny;

// Fur
bool g_bToggleFur=false;
float *g_fpFurVertex;
int g_iFurVertexSize;
GLuint *g_ipFurIndex;
int g_iFurIndexSize;
float *g_fpFurNormals;
int g_iFurTexCoordSize;
float *g_fpFurTexCoords;
static int g_iFurLayers=20;
static float g_fFurHeight=0.05;
static float g_fFurAlpha=1;
VBObject *fur;

// Glass Bunny
bool g_bToggleGlassBunny=false;
VBObject glassBunny;
float *g_fpGlassBunnyVertex;
GLuint *g_ipGlassBunnyIndex;
float *g_fpGlassBunnyVertexNormals;
float g_fGlassBunnyParameter = -0.33;


//Textures
static bool g_bToggleTexture = false;
GLuint g_iBoxBack;
GLuint g_iBoxFront;
GLuint g_iBoxLeft;
GLuint g_iBoxRight;
GLuint g_iBoxTop;
GLuint g_iBoxBottom;
GLuint g_iSphereEarth;
GLuint g_ipSphereSmall[4];
GLuint g_iParticleSprite;
GLuint g_iAtmosphere;
GLuint g_iFur;
GLuint g_ipFurLayers[6];


//TestQuad
static bool g_bToggleTestQuad=false;

//CubeMap
GLuint g_iCubeMap;
static char g_cCubeMapMode='1';
static bool g_bToggleCubeMap = false;
static int g_iCubeMapDetail=256;
static bool g_bCubeMapOffset=false;
static float g_fCubeMapOffset=0;
static float g_fCubeMapOffsetX=0;
static float g_fCubeMapOffsetY=0;
static float g_fCubeMapOffsetZ=0;

// Animation
static bool g_bAnimateSpheres;

static float g_fRotationAngles1[] = {	0.0,1.0,1.0,1.0,
					0.0,0.0,1.0,0.0,
					0.0,0.0,0.0,-1.0,
					0.0,1.0,0.0,-0.5	};
static float g_fRotationAngles2[] = {	0.0,0.0,1.0,0.0,
					0.0,0.0,1.0,0.0,
					0.0,0.0,1.0,0.0,
					0.0,0.0,1.0,0.0	};
static float g_bAnimateLights;
static float g_pfLightRotationAngle[4] = {0.0f,1.0f,1.0f,1.0f};
static double g_dAnimFPS;
static double g_dAnimLastTime;
static double g_dAnimDeltaTime=1.0;
static double g_dTempMatrix[16];

// regular methodes
void initParticles();
void updateParticlePositions();
void updateShadowMap();
GLuint sphere( float texMirr, float r, float cx,float cy,float cz, unsigned int LODi, float iStart, float iEnd);
void centerBunny( float width, float height, float depth);
void initOldFur();
void initExtensions();
bool fileExists(char* filename);
void initialize();
void renderCubeMapMirror(int direction);
void renderCubeMap();
void switchParticleEmitter();
void renderParticles();
void renderShadows(bool bigSphere, bool smallSpheres,bool lights, bool mirror, bool isCubeMap, bool shadows, bool particles);
void printHelp(void);

// inline methodes
inline void invertCullFaceMode();
inline void displayOutput();
inline void setupWireframe();
inline void setupLighting();
inline void setupUserViewProj();
inline void setupCubeMapViewProj();
inline void renderFog();
inline void renderMirror();
inline void renderWalls();
inline void renderBunny();
inline void renderGlow();
inline void renderFur();
inline void renderSmallSpheres();
inline void renderSpheres(bool mirror, bool isCubeMap);
inline void updateAtmosphere();
inline void renderAtmosphere(bool mirror);
inline void updateAtmosphere();
inline void renderLights();
inline void renderScene(bool bigSphere, bool smallSpheres, bool lights, bool mirror, bool isCubeMap, bool shadows, bool particles);
inline void displayfunc();
inline void idlefunc(void);
inline void reshapefunc(int iWidth,int iHeight);
inline void mousefunc(int button,int state,int x,int y);
inline void motionfunc(int x,int y);
inline void keyboardfunc(unsigned char key,int x,int y);
inline void specialfunc(int key,int x,int y);


#include "frame.inl"

#endif

