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
 * version  : 2.0                                              *
 * date     : 20.Okt.2005                                      *
 * e-mail   : duliu@in.tum.de, bucherm@in.tum.de               *
 *                                                             *
 ***************************************************************/
#include "time.h"

#include "frame.h"

void initParticles()
{
	glPointSize(g_fParticleSize);

	g_pfParticlePositions = new float[3*g_iParticleCount];
	g_pfParticleAttributes= new float[5*g_iParticleCount];
	g_pfParticleStartAttributes = new float[4*g_iParticleCount];
	g_pfParticleColors = new float[4*g_iParticleCount];
	
	int indexPos;
	int indexAtr;
	int indexCol;
	for(int i=0; i<g_iParticleCount;i++)
	{
		// update indexes
		indexPos = i*3;
		indexAtr = i*5;
		indexCol = i*4;

		// initialize particle positions
		g_pfParticlePositions[indexPos+0]=g_pfParticleOrigin[0];
		g_pfParticlePositions[indexPos+1]=g_pfParticleOrigin[1];
		g_pfParticlePositions[indexPos+2]=g_pfParticleOrigin[2];
		
		// initialize particle speeds
        float currentTime=float(clock())/float(CLOCKS_PER_SEC);
		float random1=((float)(rand()%g_iParticleCount)/(float)g_iParticleCount)*M__2_PI;
		float random2=((float)(rand()%g_iParticleCount)/(float)g_iParticleCount)*M__2_PI;
		//double random1=((double)i/(double)g_iParticleCount)*M__2_PI;
		//double random2=((double)i/(double)g_iParticleCount)*M__2_PI;

		// set initial velocity and life counters
		g_pfParticleAttributes[indexAtr+0]=g_fParticleSpeed*sin(random1);
		g_pfParticleAttributes[indexAtr+1]=g_fParticleSpeed*(cos(random1)*sin(random2));
		g_pfParticleAttributes[indexAtr+2]=g_fParticleSpeed*(cos(random1)*cos(random2));
		g_pfParticleAttributes[indexAtr+3]=(random2/M__2_PI)*g_fParticleLife;
		g_pfParticleAttributes[indexAtr+4]=currentTime;

		// copy initial velocity
		g_pfParticleStartAttributes[indexCol+0]=g_pfParticleAttributes[indexAtr+0];
		g_pfParticleStartAttributes[indexCol+1]=g_pfParticleAttributes[indexAtr+1];
		g_pfParticleStartAttributes[indexCol+2]=g_pfParticleAttributes[indexAtr+2];
		g_pfParticleStartAttributes[indexCol+3]=g_pfParticleAttributes[indexAtr+3];
		
		// set the colors
		g_pfParticleColors[indexCol+0]=(float)(rand()%255)/255.0;
		g_pfParticleColors[indexCol+1]=(float)(rand()%255)/255.0;
		g_pfParticleColors[indexCol+2]=(float)(rand()%255)/255.0;
		g_pfParticleColors[indexCol+3]=1.0f;	// particle Alpha
	}

	particles.initVertices(g_pfParticlePositions,g_iParticleCount,3,GL_DYNAMIC_DRAW_ARB,GL_POINTS);
	particles.initColors(g_pfParticleColors,g_iParticleCount,4,GL_DYNAMIC_DRAW_ARB);
}

void updateParticlePositions()
{
    // get time


	// update particle origin
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Transform the Matrix from the Object point of view
	glLoadIdentity();
	int j = g_iParticleEmitter*4;
	glRotatef(g_fRotationAngles1[j+0],g_fRotationAngles1[j+1],g_fRotationAngles1[j+2],g_fRotationAngles1[j+3]);
	glRotatef(g_fRotationAngles2[j+0],g_fRotationAngles2[j+1],g_fRotationAngles2[j+2],g_fRotationAngles2[j+3]);
	glTranslatef(g_pfParticleOriginSource[0],g_pfParticleOriginSource[1],g_pfParticleOriginSource[2]);

	// extract the transformed coordinates ;)
	glGetFloatv(GL_MODELVIEW_MATRIX,g_pfOriginMatrix);
	glPopMatrix();

	// Update the origin
	g_pfParticleOrigin[0]=g_pfOriginMatrix[12];
	g_pfParticleOrigin[1]=g_pfOriginMatrix[13];
	g_pfParticleOrigin[2]=g_pfOriginMatrix[14];

	float tLength, tLengthVelocity, rX, rY, rZ, temp1, temp2, temp3;
	float tX, tY, tZ, tVx, tVy, tVz, tStartVx, tStartVy, tStartVz, tLife, tLastLife;
	
	int indexPos;
	int indexAtr;
	int indexCol;
	for(int i=0;i<g_iParticleCount;i++)
	{
		// update indexes
		indexPos = i*3;
		indexAtr = i*5;
		indexCol = i*4;

		// copy positions
		tX = g_pfParticlePositions[indexPos];
		tY = g_pfParticlePositions[indexPos+1];
		tZ = g_pfParticlePositions[indexPos+2];
		tLength = sqrt(tX*tX + tY*tY + tZ*tZ);

		// copy Attributes
		tVx = g_pfParticleAttributes[indexAtr];
		tVy = g_pfParticleAttributes[indexAtr+1];
		tVz = g_pfParticleAttributes[indexAtr+2];
		tLife = g_pfParticleAttributes[indexAtr+3];
		tLastLife = g_pfParticleAttributes[indexAtr+4];
		tLengthVelocity = sqrt(tVx*tVx + tVy*tVy + tVz*tVz);

		// copy initial velocities
		tStartVx = g_pfParticleStartAttributes[indexCol+0];
		tStartVy = g_pfParticleStartAttributes[indexCol+1];
		tStartVz = g_pfParticleStartAttributes[indexCol+2];
		
		// update remaining Life
        float currentTime=float(clock())/float(CLOCKS_PER_SEC);
		float deltaTime=currentTime-tLastLife;
		if(g_bParticleAging)	tLife-=deltaTime;
		tLastLife=currentTime;
		g_pfParticleColors[indexCol+3]=1- ((g_fParticleLife-tLife)/g_fParticleLife)*((g_fParticleLife-tLife)/g_fParticleLife);
		
		// update particle Positions
		if(g_bParticleAging && tLife <= 0.0f )
		{
			// reinitialize particle positions
			tX=g_pfParticleOrigin[0];
			tY=g_pfParticleOrigin[1];
			tZ=g_pfParticleOrigin[2];

			// reinitialize speed multiplyers
			tVx=tStartVx;
			tVy=tStartVy;
			tVz=tStartVz;

			// reset its remaining life
			tLife=g_pfParticleStartAttributes[indexCol+3];
		}
		else if(g_bParticleMovement)
		{
			//printf("deltaTime=%f\n",deltaTime);
			// calculate gravity displacement
			switch(g_iParticleGravityMode)
			{
				// zero-g behaviour (default)
				case 0 :	break;

				// basic gravity
				case 1 :	tVy -= g_fParticleGravity * deltaTime;	break;

				// inverted grayity
				case 2 :	tVy += g_fParticleGravity * deltaTime;	break;

				// terestrial gravity
				case 3 :
							temp1 = g_fParticleGravity * deltaTime / tLength;
							tVx -= tX*temp1;
							tVy -= tY*temp1;
							tVz -= tZ*temp1;
							break;

				// highly damped vertical movement
				case 4 :	tVy = tStartVx * pow(tLife / g_fParticleLife, 7 * tLife);	break;

				// particles are falling simultaneously to the centers of the three axes
				case 5 :	
							// dampen the speeds
							temp1 = g_fParticleGravity * deltaTime * 0.328;
							if(tX > 0)	tVx -= temp1;
							else tVx += temp1;
							if(tY > 0)	tVy -= temp1;
							else tVy += temp1;
							if(tZ > 0)	tVz -= temp1;
							else tVz += temp1;

							// decrease there distance to the center
							tX*=0.995f;
							tY*=0.995f;
							tZ*=0.995f;
							break;
				
				// particle trail with "pixies"
				case 6 :	
							temp1 = tLife / g_fParticleLife;
							temp2 = 3 * tLife;
							tVx = tStartVx * pow(temp1, temp2);
							tVy = tStartVy * pow(temp1, temp2);
							tVz = tStartVz * pow(temp1, temp2);
							break;

				/*
				case 7 :	if(g_pfParticlePositions[indexPos+0]>g_pfParticleOrigin[0])	g_pfParticleAttributes[indexAtr+0]-=g_fParticleGravity*deltaTime;
							else g_pfParticleAttributes[indexAtr+0]+=g_fParticleGravity*deltaTime;
							if(g_pfParticlePositions[indexPos+1]>g_pfParticleOrigin[1])	g_pfParticleAttributes[indexAtr+1]-=g_fParticleGravity*deltaTime;
							else g_pfParticleAttributes[indexAtr+1]+=g_fParticleGravity*deltaTime;
							if(g_pfParticlePositions[indexPos+2]>g_pfParticleOrigin[2])	g_pfParticleAttributes[indexAtr+2]-=g_fParticleGravity*deltaTime;
							else g_pfParticleAttributes[indexAtr+2]+=g_fParticleGravity*deltaTime;
							break;
				*/
			}

			// recalculate particle positions
			tX += deltaTime * tVx;
			tY += deltaTime * tVy;
			tZ += deltaTime * tVz;

			// reflect particles on walls
			if(	tX > 2.0f)
			{
				tX = 2.0f;
				tVx *= -g_fImpactSpeedDissipation;
			}
			else if( tX < -2.0f)
			{
				tX = -2.0f;
				tVx *= -g_fImpactSpeedDissipation;
			}
			if(	tY >  1.5f)
			{
				tY = 1.5f;
				tVy *= -g_fImpactSpeedDissipation;
				if(g_iParticleGravityMode==2) tVy *= g_fImpactSpeedDissipation;
			}
			else if( tY < -1.5f)
			{
				tY = -1.5f;
				tVy *= -g_fImpactSpeedDissipation;
				if(g_iParticleGravityMode==1) tVy *= g_fImpactSpeedDissipation;
			}
			if(	tZ > 2.0f)
			{
				tZ = 2.0f;
				tVz *= -g_fImpactSpeedDissipation;
			}
			else if( tZ < -2.0f)
			{
				tZ = -2.0f;
				tVz *= -g_fImpactSpeedDissipation;
			}

			// recalculate the length of the vectors
			tLength = sqrt(tX*tX + tY*tY + tZ*tZ);
			tLengthVelocity = sqrt(tVx*tVx + tVy*tVy + tVz*tVz);

			// reflect particles on the big Sphere
			if( !g_bBunny && tLength < g_fBigSphereRadius && tLengthVelocity > 0)
			{
				// normalize positionVector
				tX /= tLength;
				tY /= tLength;
				tZ /= tLength;

				// normalize velocityVector
				tVx /= tLengthVelocity;
				tVy /= tLengthVelocity;
				tVz /= tLengthVelocity;
				

				// caclulate reflection
				temp1 = -2*( tVx*tX + tVy*tY + tVz*tZ );
				rX = temp1*tX + tVx;
				rY = temp1*tY + tVy;
				rZ = temp1*tZ + tVz;
				
				// Update velocityVector
				temp1 = tLengthVelocity * g_fImpactSpeedDissipation;
				tVx = rX * temp1;
				tVy = rY * temp1;
				tVz = rZ * temp1;
			
				// set new particle position to avoid particles getting stuck in the "atmosphere" 
				temp1 = g_fBigSphereRadius * 1.01;
				tX *= temp1;
				tY *= temp1;
				tZ *= temp1;
			}
		}

		// update positions
		g_pfParticlePositions[indexPos] = tX;
		g_pfParticlePositions[indexPos+1] = tY;
		g_pfParticlePositions[indexPos+2] = tZ;

		// update attributes
		g_pfParticleAttributes[indexAtr] = tVx;
		g_pfParticleAttributes[indexAtr+1] = tVy;
		g_pfParticleAttributes[indexAtr+2] = tVz;
		g_pfParticleAttributes[indexAtr+3] = tLife;
		g_pfParticleAttributes[indexAtr+4] = tLastLife;
	}
	particles.updateVertices(g_pfParticlePositions);
	particles.updateColors(g_pfParticleColors);
}



void updateShadowMap()
{
	// copy projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0f,1.0f,0.2,20.0f);
	glOrtho(-1.7f,1.7f,-1.7f,1.7f,-0.0f,7.0f);
	glGetFloatv(GL_PROJECTION_MATRIX,g_pfLightProjection);
	// copy modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-3.0f);
	glRotatef(90.0f,1,0,0);
	glRotatef(g_pfLightRotationAngle[0],-g_pfLightRotationAngle[1],-g_pfLightRotationAngle[2],-g_pfLightRotationAngle[3]);
	glGetFloatv(GL_MODELVIEW_MATRIX, g_pfLightModelview);


	// FIRST PASS: render from light point of view
	glClear(GL_DEPTH_BUFFER_BIT);
	// setup rendering from the light
	glMatrixMode(GL_PROJECTION);	glLoadMatrixf(g_pfLightProjection);
	//glMatrixMode(GL_MODELVIEW);	glLoadMatrixf(g_pfLightModelview);
	// Manual polygon offset calculations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-3.07f);
	glRotatef(90.0f,1,0,0);
	glRotatef(g_pfLightRotationAngle[0],-g_pfLightRotationAngle[1],-g_pfLightRotationAngle[2],-g_pfLightRotationAngle[3]);
	// setup rendering
	glActiveTexture(GL_TEXTURE2);
	glViewport(0,0,g_iShadowMapSize,g_iShadowMapSize);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glShadeModel(GL_FLAT);
	glColorMask(0,0,0,0);
	// render the Scene
	renderSpheres(true,false);
	renderSmallSpheres();
	// copy DepthBuffer
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,g_iShadowMapTex);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0,0,0,0,0,g_iShadowMapSize, g_iShadowMapSize);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE0);
	// clean up
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1,1,1,1);
}




void centerBunny( float width, float height, float depth) {
	// Belege die Min und Max Werte mit den Werten des 1.Vertex vor
	float minx = g_fpBunnyVertex[0];
	float miny = g_fpBunnyVertex[1];
	float minz = g_fpBunnyVertex[2];
	float maxx = g_fpBunnyVertex[0];
	float maxy = g_fpBunnyVertex[1];
	float maxz = g_fpBunnyVertex[2];
	float dx, dy, dz, factor;

	// Bestimme die maximalen und minimalen Koordinaten
	for( int i=3; i<g_iBunnyVertexSize; i+=3) {
		if( g_fpBunnyVertex[i]<minx) minx = g_fpBunnyVertex[i];
		else if( g_fpBunnyVertex[i]>maxx) maxx = g_fpBunnyVertex[i];

		if( g_fpBunnyVertex[i+1]<miny) miny = g_fpBunnyVertex[i+1];
		else if( g_fpBunnyVertex[i+1]>maxy) maxy = g_fpBunnyVertex[i+1];
		
		if( g_fpBunnyVertex[i+2]<minz) minz = g_fpBunnyVertex[i+2];
		else if( g_fpBunnyVertex[i+2]>maxz) maxz = g_fpBunnyVertex[i+2];
	}
	/* For Debugging
	printf( "Koord\tMAX\tMIN\n");
	printf( "x\t%f\t%f\n", maxx, minx);
	printf( "y\t%f\t%f\n", maxy, miny);
	printf( "z\t%f\t%f\n", maxz, minz);
	*/
	// Errechne die Breite des Models
	dx = maxx - minx;
	dy = maxy - miny;
	dz = maxz - minz;
	
	// Errechne die notwendige Skalierung in x- und y-Richtung und bilde den Mittelwert
	factor = ( width/dx + height/dy + depth/dz) / 3;
	/* For Debugging
	printf( "Bunny's Center: %f\t%f\t%f\n", dx/2+minx, dy/2+miny, dz/2+minz);
	printf( "Bunny's Factor: %f\n", factor);
	*/
	// �dere die Position und die Gr�e des Modells durch Manipulierung des VertexArrays
	for( int i=0; i<g_iBunnyVertexSize; i+=3) {
		g_fpBunnyVertex[i] = (g_fpBunnyVertex[i] - (minx+dx/2)) * factor;
		g_fpBunnyVertex[i+1] = (g_fpBunnyVertex[i+1] - (miny+dy/2)) * factor;
		g_fpBunnyVertex[i+2] = (g_fpBunnyVertex[i+2] - (minz+dz/2)) * factor;
	}
}


void initOldFur()
{
	float *vertices = new float[g_iBunnyVertexSize];
    g_ipFurLayers[0] = loadTexture( "images/fur1.png", GL_RGB);
    g_ipFurLayers[1] = loadTexture( "images/fur2.png", GL_RGB);
    g_ipFurLayers[2] = loadTexture( "images/fur3.png", GL_RGB);
    g_ipFurLayers[3] = loadTexture( "images/fur4.png", GL_RGB);
    g_ipFurLayers[4] = loadTexture( "images/fur5.png", GL_RGB);
    g_ipFurLayers[5] = loadTexture( "images/fur6.png", GL_RGB);

	// generate rendom Tex Coordonates
	g_fpFurTexCoords = new float[(g_iBunnyVertexSize/3)*2];
	for(int i=0;i < (g_iBunnyVertexSize/3);i++)
	{
		float x=g_fpBunnyVertex[i*3];
		float y=g_fpBunnyVertex[i*3+1];
		float z=g_fpBunnyVertex[i*3+2];

		float a = sqrt( pow( x, 2 ) + pow( z, 2 ) );
		float alpha = atan( y/a );
		g_fpFurTexCoords[i*2+0]=0.5 + 0.5*sin(alpha);

		float beta=0;
		if( x >= 0 && z >= 0)	 beta = atan( x / z );
		else if( x >= 0 && z < 0)	beta = M__PI - atan( x / -z );
		else if( x < 0 && z < 0)	beta = M__PI + atan( -x / -z );
		else if( x < 0 && z >= 0)	beta = M__2_PI - atan( -x / z );
		g_fpFurTexCoords[i*2+1]= beta / M__2_PI;
	}

	// copy bunny vertices
	for(int i=0; i < g_iBunnyVertexSize; i++)
	{
		vertices[i] = g_fpBunnyVertex[i];
	}

	fur = new VBObject[g_iFurLayers];
	float offset = g_fFurHeight / (float)g_iFurLayers;
	for(int i=0; i < g_iFurLayers; i++)
	{
		// "inflate" the model
		for(int j=0; j < g_iBunnyVertexSize; j++)
		{
			vertices[j] += g_fpBunnyVertexNormals[j] * offset;
			//vertices[j*3 + 1] += g_fpBunnyVertexNormals[j*3 + 1] * offset;
			//vertices[j*3 + 2] += g_fpBunnyVertexNormals[j*3 + 2] * offset;
		}

		// gen VBO's
		fur[i].initialize(vertices,g_ipBunnyIndex,g_fpBunnyVertexNormals,g_iBunnyVertexSize/3,g_iBunnyIndexSize/3,g_iBunnyVertexSize/3);
		fur[i].initTexCoords(g_fpFurTexCoords,g_iBunnyVertexSize/3,2,GL_STATIC_DRAW_ARB);
		
	}
}

void initExtensions() {
    // check mandatory for extensions
	char missingExtensions[500]="";
	//if (!GLEW_vertex_program && !GLEW_NV_vertex_program2 && !GLEW_NV_vertex_program)   strcat(missingExtensions,"\nGL_vertex_program and GL_NV_vertex_program2");
	//if (!GLEW_fragment_program && !GLEW_NV_fragment_program && !GLEW_NV_texture_shader3) strcat(missingExtensions,"\nGL_fragment_program and GL_NV_fragment_program");
	if (!GLEW_ARB_multitexture)				strcat(missingExtensions,"\nGL_ARB_multitexture");	
	if (!GLEW_ARB_texture_cube_map)			strcat(missingExtensions,"\nGL_ARB_texture_cube_map");	
	if (!GLEW_ARB_vertex_buffer_object)		strcat(missingExtensions,"\nGL_ARB_vertex_buffer_object");	
	if (!GLEW_VERSION_1_3)						strcat(missingExtensions,"\nOpenGL Version 1.3");	
	if (strlen(missingExtensions)>0) {
		printf("ERROR: Some needed extensions are not present:%s\n",missingExtensions);
		char dummy;		scanf( "%c",&dummy);		exit(-1);
	} else {		
		#ifdef _DEBUG
			printf("DEBUG: All mandatory extensions seem to be ok.\n");
		#endif // _DEBUG
	}

}


bool fileExists(char* filename) {
	FILE *stream; stream = fopen(filename, "r");
	if (stream != NULL) {
		fclose( stream );	
		return true;
	} else return false;
}





/*
 * INITIALIZATIONAL stuff should be left here...
 */
void initialize() {

	initExtensions();
   
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0,0,3,0,0,-1,0,1,0);
	
	g_fpRotMatrix[0] = 1; g_fpRotMatrix[5] = 1; g_fpRotMatrix[10] = 1; g_fpRotMatrix[15] = 1;

    	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);     // nice perspective correction
    	glShadeModel(GL_SMOOTH);    // use bilinear interpolation for the shading
    	glDepthFunc(GL_LEQUAL);     // depth buffer is set to display only front-most pixels
    	glEnable(GL_DEPTH_TEST);    // and here it is enabled.

    	glCullFace(GL_BACK);        // back face culling for more speed.
    	glEnable(GL_CULL_FACE);     // be careful with this when using transparency later on.
    
    	glClearColor(0,0,0,1);      // background color is simple black. Fourth component is alpha-channel

	// Initialize the display-lists. This step effectively pre-calculates the expensive sin/cos-terms.
    	bigSphere      = sphere(0.0f, g_fBigSphereRadius, 0.0f, 0.0f, 0.0f, g_iBigSphereLOD );
	
	for (int i = 0;i<4;i++) {
		g_ipSmallSphere[i]=sphere(	g_ipSmallSphereGeo[i][0], g_ipSmallSphereGeo[i][1], 
						g_ipSmallSphereGeo[i][2], g_ipSmallSphereGeo[i][3],
						g_ipSmallSphereGeo[i][4], int(g_ipSmallSphereGeo[i][5]));
	}
	
	printf( "Initializing Model...");
	fflush(stdout);
	// Load Model	
    readMesh( "models/monkeyHigh.obj", &g_fpBunnyVertex, g_iBunnyVertexSize, &g_ipBunnyIndex, g_iBunnyIndexSize );

	// Calculate Model's Normals
	g_fpBunnyVertexNormals	= new float[g_iBunnyVertexSize];
    calculateNormals( g_iBunnyVertexSize/3,
                      g_iBunnyIndexSize/3,
                      g_fpBunnyVertex,
                      g_ipBunnyIndex,
                      g_fpBunnyVertexNormals );
	
	// Center and Scale the Model
	centerBunny( 1.0, 1.0, 1.0);

	//Creating Buffer Objects
	bunny.initialize(g_fpBunnyVertex,g_ipBunnyIndex,g_fpBunnyVertexNormals,g_iBunnyVertexSize/3,g_iBunnyIndexSize/3,g_iBunnyVertexSize/3);
	printf(" done.\n");

	// glass bunny
	printf( "Initializing Glass Bunny...");
	g_fpGlassBunnyVertex = new float[g_iBunnyVertexSize];
	g_ipGlassBunnyIndex = new unsigned int[g_iBunnyIndexSize];
	g_fpGlassBunnyVertexNormals = new float[g_iBunnyVertexSize];
	for(int i=0;i<g_iBunnyVertexSize;i++)
	{
		g_fpGlassBunnyVertex[i] = g_fpBunnyVertex[i];
		g_fpGlassBunnyVertexNormals[i] = g_fpBunnyVertexNormals[i] * g_fGlassBunnyParameter;
	}
	for(int i=0;i<g_iBunnyIndexSize;i++)
	{
		g_ipGlassBunnyIndex[i] = g_ipBunnyIndex[i];
	}
	glassBunny.initialize(g_fpGlassBunnyVertex,g_ipGlassBunnyIndex,g_fpGlassBunnyVertexNormals,g_iBunnyVertexSize/3,g_iBunnyIndexSize/3,g_iBunnyVertexSize/3);
	printf(" done.\n");

	// Create Fur
	printf( "Initializing Fur...");
	fflush(stdout);

	// Calculate Model's Normals
	g_fpBunnyVertexNormals	= new float[g_iBunnyVertexSize];
	calculateNormals(
					g_iBunnyVertexSize/3,
					g_iBunnyIndexSize/3,
					g_fpBunnyVertex,
					g_ipBunnyIndex,
					g_fpBunnyVertexNormals
					);
	
	// Center and Scale the Model
	centerBunny( 1.0, 1.0, 1.0);

	initOldFur();
	printf(" done.\n");

	// setup ligthing
	printf( "Initializing Light...");
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialf( GL_FRONT, GL_SHININESS, 50.0);
	glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfLightSpecular);

	glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
	glLightfv( GL_LIGHT0, GL_AMBIENT, g_pfLigthAmbient);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, g_pfLightDiffuse);
	glLightfv( GL_LIGHT0, GL_SPECULAR, g_pfLightSpecular);
	// Mirror Light
	glLightfv( GL_LIGHT1, GL_POSITION, g_pfLightPos1);
	glLightfv( GL_LIGHT1, GL_SPECULAR, g_pfLightSpecular1);

	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBlendFunc(GL_SRC_COLOR,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	printf( "done\n");
	
	// create / load  textures
    printf( "Loading Textures...");
    g_iBoxBack = loadTexture( "images/backWall.jpg", GL_RGB);
    g_iBoxFront = loadTexture( "images/frontWall.jpg", GL_RGB);
    g_iBoxLeft = genMirrorTexture(16);
    g_iBoxRight = loadTexture( "images/rightWall.jpg", GL_RGB);
    printf( "done\n");

	GLfloat texColor1[4] = {0.0,0.0,0.25,0.0};
	GLfloat texColor2[4] = {0.0,0.0,0.75,0.0};
	g_iBoxTop = genCheckerboard(32,256,texColor1,texColor2);

	GLfloat texColor3[4] = {1.0,0.0,0.0,0.0};
	GLfloat texColor4[4] = {0.25,0.0,0.0,0.0};
	g_iBoxBottom = genCellular(256,256,texColor3,texColor4);
	
    g_iSphereEarth = loadTexture( "images/earth.jpg", GL_RGB);
    g_ipSphereSmall[0] = loadTexture( "images/tex1.jpg", GL_RGB);
    g_ipSphereSmall[1] = loadTexture( "images/tex2.jpg", GL_RGB);
    g_ipSphereSmall[2] = loadTexture( "images/tex3.jpg", GL_RGB);
	g_ipSphereSmall[3] = genColorMap(true, 2.0, 128);
	g_iParticleSprite = genExpSprite(8,1.0f); 
	//g_iParticleSprite = genWhiteCircle(8);
    //g_iParticleSprite = loadTexture( "images/particle1.bmp", GL_LUMINANCE);
	//g_iBoxBottom = g_iFur;
    g_iAtmosphere = loadTexture( "images/atmosphere.jpg", GL_RGB);

	
	// Initialize CubeMap
	printf( "Initializing CubeMap...");
	glDisable( GL_TEXTURE_2D);
	glEnable( GL_TEXTURE_CUBE_MAP);
	glGenTextures( 1, &g_iCubeMap);
	glBindTexture( GL_TEXTURE_CUBE_MAP, g_iCubeMap);
	
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	for (unsigned int i=GL_TEXTURE_CUBE_MAP_POSITIVE_X; i<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; i++) {
		glTexImage2D(i,0,GL_RGB8,g_iCubeMapDetail,g_iCubeMapDetail,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	}

	if(g_bCubeMapOffset)
	{
		g_fCubeMapOffset=g_fBigSphereRadius-(((-g_fTransZ-g_fBigSphereRadius)*g_fBigSphereRadius)/(20.0f-g_fBigSphereRadius));
			
		// convert Azimuth and Elevation to radians
		double azimuth   = (double) g_fAzimuth   * M__PI / (double)180;
		double elevation = (double) g_fElevation * M__PI / (double)180;

		g_fCubeMapOffsetX= g_fCubeMapOffset*sin(azimuth);
		g_fCubeMapOffsetY= g_fCubeMapOffset*sin(elevation);
		g_fCubeMapOffsetZ= g_fCubeMapOffset*cos(azimuth);
	}
	
	glBindTexture( GL_TEXTURE_2D, 0);
	glDisable( GL_TEXTURE_CUBE_MAP);
	glEnable( GL_TEXTURE_2D);

	printf("done\n");

	//Initializing Shadows
	printf( "Initializing Shadows...");
	glActiveTexture(GL_TEXTURE2);
	glGenTextures( 1, &g_iShadowMapTex);
	glBindTexture( GL_TEXTURE_2D, g_iShadowMapTex);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, g_iShadowMapSize, g_iShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture( GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	printf( "done\n");
	
	printf("\n\n");

	setupWireframe();
	setupLighting();
	initParticles();
	updateAtmosphere();
}

void renderCubeMapMirror(int direction) {
	
	//glActiveTexture(GL_TEXTURE0);
	// Mirror the scene
	glLoadIdentity();
	glMultMatrixf(g_fMirrorX);
	

	switch (direction)
	{
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X :
			if(g_bCubeMapOffset) glTranslatef(-g_fCubeMapOffsetZ,-g_fCubeMapOffsetY,-4.0f+g_fCubeMapOffsetX);
			else glTranslatef(0.0f,0.0f,-4.0f);
			glRotatef(90,0,1,0);
			glRotatef(180,1,0,0);
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			glTranslatef(4.0f-g_fCubeMapOffsetX,g_fCubeMapOffsetZ,-g_fCubeMapOffsetY);
			glRotatef(-90,1,0,0);
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			glTranslatef(4.0f-g_fCubeMapOffsetX,-g_fCubeMapOffsetZ,g_fCubeMapOffsetY);
			glRotatef(90,1,0,0);
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			glTranslatef(4.0f-g_fCubeMapOffsetX,-g_fCubeMapOffsetY,-g_fCubeMapOffsetZ);
			glRotatef(180,1,0,0);
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			glTranslatef(-4.0f+g_fCubeMapOffsetX,-g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
			glRotatef(180,0,1,0);
			glRotatef(180,1,0,0);
			break;
	}
	
	
	// Render the mirrored CubeMap
	glFrontFace(GL_CW);
	renderScene(direction==GL_TEXTURE_CUBE_MAP_NEGATIVE_X  && !g_bBunny,true,g_bToggleLighting,false,true,g_bToggleShadows,g_bToggleParticles);
	glFrontFace(GL_CCW);
}

void renderCubeMap() {

	// Setup Viewport
	glViewport(0, 0, g_iCubeMapDetail, g_iCubeMapDetail);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
	gluPerspective(90.0, 1.0, 0.001, 20.0);  
	glMatrixMode(GL_MODELVIEW);
	//glActiveTexture(GL_TEXTURE1);
	glBindTexture( GL_TEXTURE_CUBE_MAP, g_iCubeMap);
	

	// +X RIGHT
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(180,0,0,1);
	glRotatef(90,0,1,0);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);
	

	// -X LEFT
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(g_bMirror)	renderCubeMapMirror(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	glLoadIdentity();
	glRotatef(180,0,0,1);
	glRotatef(-90,0,1,0);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);
	
	// +Y
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(g_bMirror && g_bCubeMapOffset)	renderCubeMapMirror(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	glLoadIdentity();
	glRotatef(-90,1,0,0);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);
	
	// -Y
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(g_bMirror && g_bCubeMapOffset)	renderCubeMapMirror(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    glLoadIdentity();
	glRotatef(90,1,0,0);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);

	// +Z BACK
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(g_bMirror && g_bCubeMapOffset)	renderCubeMapMirror(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    glLoadIdentity();
	glRotatef(180,0,0,1);
	glRotatef(-180,0,1,0);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);
	
	// -Z FRONT
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(g_bMirror && g_bCubeMapOffset)	renderCubeMapMirror(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    glLoadIdentity();
	glRotatef(180,0,0,1);
	//glActiveTexture(GL_TEXTURE0);
	if(g_bCubeMapOffset)	glTranslatef(g_fCubeMapOffsetX,g_fCubeMapOffsetY,g_fCubeMapOffsetZ);
	renderScene(false,true,g_bToggleLighting,true,true,g_bToggleShadows,g_bToggleParticles);
	//glActiveTexture(GL_TEXTURE1);
	glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,0,0,0,0,g_iCubeMapDetail,g_iCubeMapDetail);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	glDisable( GL_TEXTURE_CUBE_MAP);
	//glActiveTexture(GL_TEXTURE0);
}

void switchParticleEmitter()
{
	g_pfParticleOriginSource[0]=g_ipSmallSphereGeo[g_iParticleEmitter][2];
	g_pfParticleOriginSource[1]=g_ipSmallSphereGeo[g_iParticleEmitter][3];
	g_pfParticleOriginSource[2]=g_ipSmallSphereGeo[g_iParticleEmitter][4];
}



void renderParticles()
{
	// render the Points
	glBindTexture(GL_TEXTURE_2D,g_iParticleSprite);
	glEnable(GL_BLEND);
	if(g_bToggleLighting) glDisable(GL_LIGHTING);
	float quadratic[3] = {1.0f,0.1f,0.01f};
	glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB,quadratic);
	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
	
	glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
	glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, g_fParticleSize );
	
	//glPointSize(g_fParticleSize);
	glEnable( GL_POINT_SPRITE_ARB );
	glDepthMask(0);
	/*
	glBegin(GL_POINTS);
	glColor4f(0.0f,1.0f,0.0f,1.0f);
	for(int i=0;i<g_iParticleCount;i++)
	{
		glColor4f(	g_pfParticleColors[i*4+0],
					g_pfParticleColors[i*4+1],
					g_pfParticleColors[i*4+2],
					g_pfParticleColors[i*4+3]	);
		
		glVertex3f( g_pfParticlePositions[i*3+0],
					g_pfParticlePositions[i*3+1],
					g_pfParticlePositions[i*3+2]	);
	}
	glEnd();
	*/
	particles.draw();
	glDepthMask(1);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable( GL_POINT_SPRITE_ARB );
	if(g_bToggleLighting) glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
}


void renderShadows(bool bigSphere, bool smallSpheres,bool lights, bool mirror, bool isCubeMap, bool shadows, bool particles)
{
	// SECOND PASS: render shadowed scene
	glClear(GL_DEPTH_BUFFER_BIT);
	if(mirror)	glCullFace(GL_BACK);
	else if(!isCubeMap)	glCullFace(GL_FRONT);
	if(!isCubeMap)
	{
		// render shadowed scene
		glLightfv( GL_LIGHT0, GL_AMBIENT, g_pfShadowAmbient);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, g_pfShadowDiffuse);
		glLightfv( GL_LIGHT0, GL_SPECULAR, g_pfShadowSpecular);
		glActiveTexture(GL_TEXTURE0);
		renderScene(bigSphere,true,lights,mirror,isCubeMap,false,false);
		glLightfv( GL_LIGHT0, GL_AMBIENT, g_pfLigthAmbient);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, g_pfLightDiffuse);
		glLightfv( GL_LIGHT0, GL_SPECULAR, g_pfLightSpecular);
		glLightfv( GL_LIGHT1, GL_SPECULAR, g_pfLightSpecular1);
	}
	glActiveTexture(GL_TEXTURE2);
	
	
	// THIRD PASS: render scene with lights
	// calculate Texture Matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf(g_pfBiasMatrix);
	glMultMatrixf(g_pfLightProjection);
	glMultMatrixf(g_pfLightModelview);
	glGetFloatv(GL_MODELVIEW_MATRIX,g_pfTextureMatrix);
	glPopMatrix();

	// Transpone texture matrix
	int max=1;	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<max;j++)
                {
                        float temp1=g_pfTextureMatrix[i*4+j];
                        float temp2=g_pfTextureMatrix[j*4+i];
                        g_pfTextureMatrix[i*4+j]=temp2;
                        g_pfTextureMatrix[j*4+i]=temp1;
                }
		max++;
	}
	
	// Set up texture coordinates generation
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

	glTexGenfv(GL_S, GL_EYE_PLANE, &g_pfTextureMatrix[0]);
	glTexGenfv(GL_T, GL_EYE_PLANE, &g_pfTextureMatrix[4]);
	glTexGenfv(GL_R, GL_EYE_PLANE, &g_pfTextureMatrix[8]);
	glTexGenfv(GL_Q, GL_EYE_PLANE, &g_pfTextureMatrix[12]);
	// Bind textures
	glBindTexture(GL_TEXTURE_2D,g_iShadowMapTex);
	glEnable(GL_TEXTURE_2D);
	// Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
	if(!isCubeMap)glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_ALPHA);
	else	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glActiveTexture(GL_TEXTURE0);
	renderScene(bigSphere,true,lights,mirror,isCubeMap,false,false);
	if(mirror)	glCullFace(GL_BACK);
	else if(!isCubeMap)	glCullFace(GL_FRONT);

	// disable stuff
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_TEXTURE_GEN_S);	
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_ALPHA_TEST);
	glActiveTexture(GL_TEXTURE0);
}


void printHelp(void) {
    std::cout << g_sProgramName << std::endl;
    std::cout << "\tq, ESC : Quit" << std::endl;
    std::cout << "\tF1     : This help text" << std::endl;
    std::cout << "\ta      : toggle sphere animation" << std::endl;
    std::cout << "\tA      : toggle light animation" << std::endl;
    std::cout << "\tb      : toggle bunny" << std::endl;
    std::cout << "\tB      : toggle glass bunny" << std::endl;
    std::cout << "\tc      : toggle cubemap" << std::endl;
    std::cout << "\tf      : toggle fulscreen" << std::endl;
    std::cout << "\tF      : toggle bunny fur" << std::endl;
    std::cout << "\tg      : cycle particle gravity fields" << std::endl;
    std::cout << "\tG      : cycle particle gravity fields (reverse)" << std::endl;
    std::cout << "\th      : toggle Wather hose" << std::endl;
    std::cout << "\tl,L    : toggle ligthing" << std::endl;
    std::cout << "\tm      : toggle mirror" << std::endl;
    std::cout << "\tM      : toggle stencil buffer" << std::endl;
    std::cout << "\to      : toggle cubemap offset calculation" << std::endl;
    std::cout << "\tp      : toggle particles" << std::endl;
    std::cout << "\tP      : cycle particle emitter" << std::endl;
    std::cout << "\tr      : reset view" << std::endl;
    std::cout << "\tR      : reset rotations" << std::endl;
    std::cout << "\ts      : toggle shadows" << std::endl;
    std::cout << "\tS      : capture frame" << std::endl;
    std::cout << "\tt      : toggle textures" << std::endl;
    std::cout << "\tw,W    : toggle wireframe" << std::endl;
    std::cout << "\tx      : toggle particle aging" << std::endl;
    std::cout << "\tX      : toggle particle movement" << std::endl;
    std::cout << "\tmouse  : Drag around scene." << std::endl;
    std::cout << g_sAuthor << std::endl;
}

/*
 * MAIN function. Only initiates GLUT.
 */
int main(int argc, char **argv) {
    // ACTUAL MAIN-FUNCTION   
    glutInit(&argc,argv);                     // inits the GLUT
    glutInitWindowSize(g_iWinWidth,g_iWinHeight);   // initial size of viewport (client area of window)
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_STENCIL);    // Requests a double RGB-framebuffer with depth- and stencil-buffer

    glutCreateWindow(g_sProgramName.c_str());	 // Creates the window.

	// init GLEW
	int err = glewInit();
	if (GLEW_OK != err)	{
		// problem: glewInit failed, something is seriously wrong
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		exit(-1);
	}

    // Register the callback-functions. These are then executed by GLUT
    // as soon as the main loop is entered (see below)
    glutDisplayFunc(displayfunc);
    glutReshapeFunc(reshapefunc); 
    glutMouseFunc(mousefunc);
    glutMotionFunc(motionfunc);
    glutKeyboardFunc(keyboardfunc);
    glutSpecialFunc(specialfunc);
    glutIdleFunc(idlefunc);
	
    initialize();   // initialization function. Non-GLUT function.
    printHelp();    // Be nice to the user and help him/her. Non-GLUT function.

    // Start the timer. Non-GLUT functions.
    timerStart();



    glutMainLoop(); // Enter GLUT main loop. Main loop will be terminated only
    // on user request by exit(0). Other means do not currently exist in GLUT.
    return(0); // to avoid errors.
}

