#include "includes.h"

using namespace std;

//Global Variables.
int score=0;
int th=0;
int lth=0;
float mouseX=0.0,mouseY=0.0;
int togglePause=0;
int toggleRed=0;
int toggleGreen=0;
int toggleGun=1,enableGun=1;
int toggleLaser=0,isSpider;
float red_bx=-3.5,gre_bx=3.5;
float laser_topx,laser_topy,laser_botmx,laser_botmy,laser_refx,laser_refy;
float tlaser_velx,blaser_velx;
float tlaser_vely,blaser_vely;
float spider_vel=0.01;
float spos=0.0,beam_startx;
float laserLength,len = 2.0;
float shotGap=0.0;
bool double_click=false;
bool toggleRotate=false;
float wsize,hsize,wratio,hratio;
bool toggleGame=false;

GLfloat colorv[3][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};

vector<spider> spiders;

//Sound----------------------//

// Buffers hold sound data.
ALuint Buffers[NUM_BUFFERS];

// Sources are points of emitting sound.
ALuint Sources[NUM_SOURCES];

// Position of the source sounds.
ALfloat SourcesPos[NUM_SOURCES][3];

// Velocity of the source sounds.
ALfloat SourcesVel[NUM_SOURCES][3];


// Position of the listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

// Velocity of the listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

ALboolean LoadALData()
{
	// Variables to load into.

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into buffers.

	alGenBuffers(NUM_BUFFERS, Buffers);

	if (alGetError() != AL_NO_ERROR)
	{
		return AL_FALSE;
	}

	alutLoadWAVFile((ALbyte*)"LASER1.WAV", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[LASER], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile((ALbyte*)"wood.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[CATCH], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alutLoadWAVFile((ALbyte*)"insects.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffers[SPIDER], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind buffers into audio sources.

	alGenSources(NUM_SOURCES, Sources);

	if (alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	alSourcei (Sources[LASER], AL_BUFFER,   Buffers[LASER]  );
	alSourcef (Sources[LASER], AL_PITCH,    1.0              );
	alSourcef (Sources[LASER], AL_GAIN,     1.0              );
	alSourcefv(Sources[LASER], AL_POSITION, SourcesPos[LASER]);
	alSourcefv(Sources[LASER], AL_VELOCITY, SourcesVel[LASER]);
	alSourcei (Sources[LASER], AL_LOOPING,  AL_FALSE          );

	alSourcei (Sources[CATCH], AL_BUFFER,   Buffers[CATCH]  );
	alSourcef (Sources[CATCH], AL_PITCH,    1.0            );
	alSourcef (Sources[CATCH], AL_GAIN,     1.0            );
	alSourcefv(Sources[CATCH], AL_POSITION, SourcesPos[CATCH]);
	alSourcefv(Sources[CATCH], AL_VELOCITY, SourcesVel[CATCH]);
	alSourcei (Sources[CATCH], AL_LOOPING,  AL_FALSE       );

	alSourcei (Sources[SPIDER], AL_BUFFER,   Buffers[SPIDER]  );
	alSourcef (Sources[SPIDER], AL_PITCH,    1.0            );
	alSourcef (Sources[SPIDER], AL_GAIN,     1.0            );
	alSourcefv(Sources[SPIDER], AL_POSITION, SourcesPos[SPIDER]);
	alSourcefv(Sources[SPIDER], AL_VELOCITY, SourcesVel[SPIDER]);
	alSourcei (Sources[SPIDER], AL_LOOPING,  AL_FALSE       );

	// Do another error check and return.

	if( alGetError() != AL_NO_ERROR)
		return AL_FALSE;

	return AL_TRUE;
}

void SetListenerValues()
{
	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

void KillALData()
{
	alDeleteBuffers(NUM_BUFFERS, &Buffers[0]);
	alDeleteSources(NUM_SOURCES, &Sources[0]);
	alutExit();
}


void drawField()
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-3.75,3.75);
	glVertex2f(-3.75,-3.75);
	glVertex2f(3.75,-3.75);
	glVertex2f(3.75,3.75);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_LINES);
	glVertex2f(-3.75,-3.2);
	glVertex2f(3.75,-3.2);
	glEnd();
}

void drawSpray()
{
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.25,-0.25);
	glVertex2f(+0.25,-0.25);
	if(toggleGun&&enableGun)
		glColor3f(1.0,0.0,0.0);
	glVertex2f(0.0,0.25);
	glEnd();
}

void drawLaser()
{
	glColor3f(1.0,1.0,0.0);
	if(laser_topy>laser_refy && laser_botmy<laser_refy)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2f(laser_topx,laser_topy);
		glVertex2f(laser_refx,laser_refy);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(laser_botmx,laser_botmy);
		glEnd();
	}
	else{
		glBegin(GL_LINES);
		glVertex2f(laser_topx,laser_topy);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(laser_botmx,laser_botmy);
		glEnd();
	}
}

void drawSpider(int color,int line)
{
	glColor3f(colorv[color][0],colorv[color][1],colorv[color][2]);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<360;i++){
		glVertex2f(0.15*cosine(i) , 0.15*sine(i));
	}
	glEnd();
	if(line){
		glBegin(GL_LINES);	
		glVertex2f(0.0,8.0);
		glVertex2f(0.0,0.0);
		glEnd();
	}
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.15*sine(60),0.15*cosine(60));
	glVertex2f(0.15*sine(60)+0.1,0.15*cosine(60)+0.1);
	glVertex2f(0.25*sine(60),0.25*cosine(60));
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.15*sine(300),0.15*cosine(300));
	glVertex2f(0.15*sine(300)-0.1,0.15*cosine(300)+0.1);
	glVertex2f(0.25*sine(300),0.25*cosine(300));
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.15*sine(240),0.15*cosine(240));
	glVertex2f(0.15*sine(240)-0.1,0.15*cosine(240)+0.1);
	glVertex2f(0.25*sine(240),0.25*cosine(240));
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.15*sine(120),0.15*cosine(120));
	glVertex2f(0.15*sine(120)+0.1,0.15*cosine(120)+0.1);
	glVertex2f(0.25*sine(120),0.25*cosine(120));
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.15*sine(120),0.15*cosine(120));
	glVertex2f(0.15*sine(120)+0.1,0.15*cosine(120)+0.1);
	glVertex2f(0.25*sine(120),0.25*cosine(120));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.15*sine(160),0.15*cosine(160));
	glVertex2f(0.25*sine(160),0.25*cosine(160));
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0.15*sine(200),0.15*cosine(200));
	glVertex2f(0.25*sine(200),0.25*cosine(200));
	glEnd();
}


void drawGBox()
{
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
	glVertex2f(gre_bx-0.25,-3.2);
	glVertex2f(gre_bx-0.25,-3.7);
	if(toggleGreen)
		glColor3f(0.6,1.0,0.6);
	glVertex2f(gre_bx+0.25,-3.7);
	glVertex2f(gre_bx+0.25,-3.2);
	glEnd();
	glPushMatrix();
	glTranslatef(gre_bx,-3.2,0.0);
	glRotatef(40,1.0,0.0,0.0);
	glColor3f(1.0,1.0,0.5);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<=360;i++){
		glVertex2f(0.25*cosine(i),0.25*sine(i));
	}
	glEnd();
	glPopMatrix();
}

void drawRBox()
{
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
	glVertex2f(red_bx-0.25,-3.2);
	glVertex2f(red_bx-0.25,-3.7);
	if(toggleRed)
		glColor3f(1.0,0.6,0.6);
	glVertex2f(red_bx+0.25,-3.7);
	glVertex2f(red_bx+0.25,-3.2);
	glEnd();
	glPushMatrix();
	glTranslatef(red_bx,-3.2,0.0);
	glRotatef(40,1.0,0.0,0.0);
	glColor3f(1.0,1.0,0.5);
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0;i<=360;i++){
		glVertex2f(0.25*cosine(i),0.25*sine(i));
	}
	glEnd();
	glPopMatrix();
}

void drawBoard()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(0.0,0.0,-9.0);
	glColor3f(1.0,1.0,1.0);
	drawField();
	if(toggleGame){
		glPushMatrix();
		glTranslatef(spos,-3.45,0.0);
		glRotatef(th,0.0,0.0,1.0);
		drawSpray();
		glTranslatef(-spos,0.0,0.0);
		if(toggleLaser)
		{
			glTranslatef(beam_startx,0.0,0.0);
			glRotatef(-th,0.0,0.0,1.0);
			drawLaser();
		}
		glPopMatrix();

		for(int i=0;i<spiders.size();i++)
		{
			if(spiders[i].toggle)
			{
				glPushMatrix();
				glTranslatef(spiders[i].x,spiders[i].y,0.0);
				if(spiders[i].y<=-3.2)
					drawSpider(spiders[i].color,0);
				else
					drawSpider(spiders[i].color,1);
				glPopMatrix();
			}
		}
		printAt((int)wsize*5/6,(int)hsize/2,"Score : %d",score);
		drawGBox();
		drawRBox();
	}
	else{
		printAt((int)wsize*5/6,(int)hsize/2,"Score : %d",score);
		printAt((int)wsize/3,(int)hsize/2,"Press S to Start the Game");
		printAt((int)wsize/3,(int)hsize/2.2,"Press Q to Exit");
	}
	glPopMatrix();
	glutSwapBuffers();
}

void initRendering()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
}

void reshape(int w,int h)
{
	glViewport(0,0,w,h);
	wsize=(float)w;hsize=(float)h;
	wratio=(float)w/13.2;
	hratio=(float)h/7.5;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(float)w/(float)h,0.1f,200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

float move_left(float pos)
{
	pos-=0.5;
	if(pos==-4.0||isBlocked((int)(2*(pos+3.5))))
		pos+=0.5;
	return pos;
}

float move_right(float pos)
{
	pos+=0.5;
	if(pos==4.0||isBlocked((int)(2*(pos+3.5))))
		pos-=0.5;
	return pos;
}

void handleArrows(int key,int x,int y)
{
	if(!togglePause && toggleGame){
		if(key==GLUT_KEY_LEFT)
		{
			if(toggleGun)
			{
				spos=move_left(spos);
			}
			else if(toggleGreen)
			{
				gre_bx=move_left(gre_bx);
			}
			else if(toggleRed)
			{
				red_bx=move_left(red_bx);
			}
		}
		else if(key==GLUT_KEY_RIGHT)
		{
			if(toggleGun)
			{
				spos=move_right(spos);
			}
			else if(toggleGreen)
			{
				gre_bx=move_right(gre_bx);
			}
			else if(toggleRed)
			{
				red_bx=move_right(red_bx);
			}
		}
		else if(key==GLUT_KEY_UP&&toggleGun)
			th+=13;
		else if(key==GLUT_KEY_DOWN&&toggleGun)
			th-=13;
	}
}


void handleKeys(unsigned char key,int x,int y)
{
	if(key==27||key=='q'||key=='Q')exit(0);
	else if(enableGun && key==32 && toggleGun && !togglePause &&toggleGame){
		enableGun=0;
		toggleLaser=1;
		lth=-th;
		laser_topx=laser_botmx=0.25*sine(lth);
		laser_topy=laser_botmy=0.25*cosine(lth);
		blaser_velx=tlaser_velx=0.5*sine(lth);
		blaser_vely=tlaser_vely=0.5*cosine(lth);
		laser_refx=(lth > 0) ? 3.75:(-3.75);
		beam_startx=spos;
		laser_refx-=beam_startx;
		if(lth>0)
			laser_refy= (3.75-laser_topx-spos)*cosine(lth)/sine(lth);
		else
			laser_refy= (-3.75-laser_topx-spos)*cosine(lth)/sine(lth);
		laser_refy= (laser_refy < 0) ? (-1*laser_refy) : laser_refy;

		alSourcefv(Sources[LASER], AL_POSITION, SourcesPos[LASER]);
		alSourcePlay(Sources[LASER]);

	}
	else if((key=='b'||key=='B') && !togglePause && toggleGame)
	{
		toggleGun=1;toggleGreen=toggleRed=0;
	}
	else if((key=='r'||key=='R') && !togglePause && toggleGame)
	{
		toggleRed=1;toggleGreen=toggleGun=0;
	}
	else if((key=='g'||key=='G') && !togglePause && toggleGame)
	{
		toggleGreen=1;toggleGun=toggleRed=0;
	}
	else if(key=='p'||key=='P'&&toggleGame)
		togglePause = (1-togglePause);
	else if(key=='s'||key=='S'&& !toggleGame)
	{
		alSourcef (Sources[SPIDER], AL_PITCH,    1.0            );
		toggleGame=true;
		score=0;
	}
}

void click(int value)
{
	double_click=false;
}


void handleMouse(int btn, int state,int x,int y)
{
	mouseX=((float)x-(wsize/2))/wratio;
	mouseY=(-(float)y+(hsize/2))/hratio;
	if(btn==GLUT_LEFT_BUTTON && !togglePause&&toggleGame)
	{
		float min,gun,red,green;
		if(double_click)
		{
			gun=dis(mouseX,mouseY,spos,-3.45);
			red=dis(mouseX,mouseY,red_bx,-3.45);
			green=dis(mouseX,mouseY,gre_bx,-3.45);
			if(gun<0.15)
			{
				toggleGun=1;toggleGreen=0;toggleRed=0;
			}
			if(red<0.15&&red<gun)
			{
				toggleGun=0;toggleGreen=0;toggleRed=1;
			}
			if(green<0.15&&green<red&&green<gun)
			{
				toggleGun=0;toggleGreen=1;toggleRed=0;
			}
		}
		else
		{
			double_click=true;
			glutTimerFunc(1000,click,0);
		}
	}
	if(btn == GLUT_RIGHT_BUTTON && state == 0)
		toggleRotate=true;
	if(btn == GLUT_RIGHT_BUTTON && state==1)
		toggleRotate=false;
	glutPostRedisplay();
}

float drag(float pos)
{
	float temp, moves;
	if(dis(mouseX,mouseY,pos,-3.45)<0.7)
	{
		temp=mouseX;
		moves=(floor)(2*(temp-pos));
		if(moves>0)
		{	
			if(pos!=3.5&&  !isBlocked((int)(2*(pos+4.0))))
				pos+=(moves*0.5);
		}
		else if(moves<0)
		{	
			if(pos!=-3.5 && !isBlocked((int)(2*(pos+3.0))))
				pos+=(moves*0.5);
		}
	}
	return pos;
}


void dragRotateWithMouse(int x,int y)
{
	mouseX=((float)x-(wsize/2))/wratio;
	mouseY=(-(float)y+(hsize/2))/hratio;
	if(!togglePause&&toggleGame)
	{
		if(toggleRed)
		{
			red_bx=drag(red_bx);
		}
		else if(toggleGreen)
		{
			gre_bx=drag(gre_bx);
		}
		else if(toggleGun)
		{
			if(toggleRotate)
			{
				float dis;
				if(dis(mouseX,mouseY,spos,-3.2)>0.5)
				{
					dis=mouseX-spos;
					int deg = (int)2*dis;
					th=-1*(deg*13);
				}
			}
			else
				spos=drag(spos);
		}
	}
}

void pause(int value)
{
	//Pause Function
	if(togglePause)glutTimerFunc(10,pause,0);
	else glutTimerFunc(10,update,0);

}

void start(int value)
{
	if(toggleGame)glutTimerFunc(10,update,0);
	else glutTimerFunc(10,start,0);
}

void update(int value)
{
	if(toggleLaser)
	{
		if(laser_topy>laser_refy && tlaser_velx*blaser_velx >0)
			tlaser_velx*=(-1);
		if(laser_botmy>laser_refy)
		{
			blaser_velx*=(-1);
			if(lth>0)
			{
				laser_refy+= (7.5*cosine(lth)/sine(lth));
			}
			else
			{
				laser_refy-= (7.5*cosine(lth)/sine(lth));
			}
			if(laser_refx>0)
				laser_refx-=7.5;
			else
				laser_refx+=7.5;
		}
		laser_topx+=tlaser_velx;
		laser_topy+=tlaser_vely;
		if(dis(laser_topx,laser_topy,laser_botmx,laser_botmy)>laserLength || tlaser_velx*blaser_velx <0)
		{
			laser_botmx+=blaser_velx;
			laser_botmy+=blaser_vely;
		}
		if(laser_botmy>8.0)
			toggleLaser=0;
	}
	if(enableGun==0)
	{
		shotGap+=0.0198;
		if(shotGap>=1)
		{
			enableGun=1;
			shotGap=0;
		}
	}

	isSpider=rand()%70;
	if(isSpider==7)
	{
		spider newSpider;
		newSpider.x=random_x();
		if(newSpider.x>3.5)
			newSpider.x-=2.5;
		newSpider.y=4.5;
		newSpider.toggle=true;
		newSpider.reached=false;
		newSpider.color=rand()%3;
		spiders.push_back(newSpider);
		alSourcefv(Sources[SPIDER], AL_POSITION, SourcesPos[SPIDER]);
		alSourcePlay(Sources[SPIDER]);
	}

	for(int i=0;i<spiders.size();i++)
	{
		if(toggleLaser && spiders[i].toggle && (dis((laser_topx+beam_startx),(laser_topy-3.45),spiders[i].x,spiders[i].y ))<0.25)
		{
			spiders[i].toggle=false;
			toggleLaser=0;
			if(spiders[i].color==2)
				score+=1;
		}
		else if(spiders[i].toggle && (spiders[i].y<(-3.2) && spiders[i].y >(-3.35)))
		{
			if(spiders[i].color==0 && spiders[i].x==red_bx){
				spiders[i].toggle=false;
				score+=1;
				alSourcefv(Sources[CATCH], AL_POSITION, SourcesPos[CATCH]);
				alSourcePlay(Sources[CATCH]);
			}
			else if(spiders[i].color==1 && spiders[i].x==gre_bx){
				spiders[i].toggle=false;
				score+=1;
				alSourcefv(Sources[CATCH], AL_POSITION, SourcesPos[CATCH]);
				alSourcePlay(Sources[CATCH]);
			}
			else if(spiders[i].x==gre_bx||spiders[i].x==red_bx){
				spiders[i].toggle=false;
				score-=1;
				alSourcefv(Sources[CATCH], AL_POSITION, SourcesPos[CATCH]);
				alSourcePlay(Sources[CATCH]);
			}
			else
				spiders[i].y-=spider_vel;
		}
		else if( spiders[i].toggle && spiders[i].y > (-3.45) )
		{
			spiders[i].y-=spider_vel;
		}
		else if(spiders[i].toggle && spiders[i].y<=(-3.45)){
			if(spiders[i].x==spos)
			{
				spos=0.0;
				red_bx=-3.5;
				gre_bx=3.5;
				spiders.clear();
				unblock();
				alSourcef (Sources[SPIDER], AL_PITCH,    0.0            );
				toggleGame=false;
			}
			else if(!spiders[i].reached){
				spiders[i].reached=true;
				score-=5;
				spiders[i].y=-3.45;
				block((int)(2*(spiders[i].x+3.5)));
			}	
		}
	}

	spider_vel+=0.0000015678;

	if(togglePause && toggleGame)glutTimerFunc(10,pause,0);
	else if(!toggleGame)glutTimerFunc(10,start,0);
	else glutTimerFunc(10,update,0);
}

int main(int argc, char **argv)
{
	//ALUT Initialisation
	alutInit(NULL, 0);
	alGetError();
	if (LoadALData() == AL_FALSE)
	{
		return 0;
	}
	SetListenerValues();
	atexit(KillALData);

	//GLUT Initialisation
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w=glutGet(GLUT_SCREEN_WIDTH);
	int h=glutGet(GLUT_SCREEN_HEIGHT);
	laserLength=0.001*(float)w;

	glutInitWindowSize(w*2/3,h*2/3);
	glutCreateWindow("My Game");
	initRendering();

	glutDisplayFunc(drawBoard);
	glutIdleFunc(drawBoard);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(handleKeys);
	glutSpecialFunc(handleArrows);
	glutMouseFunc(handleMouse);
	glutMotionFunc(dragRotateWithMouse);
	if(togglePause)glutTimerFunc(10,pause,0);
	else glutTimerFunc(10,update,0);

	glutMainLoop();
	return 0;
}
