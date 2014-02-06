#include "includes.h"

#define PI 3.141592653589
#define sine(th) sin( th*PI/180)
#define cosine(th) cos(th*PI/180)

using namespace std;

int th=0;
int mth=0;
int hth=0;
float len = 2.0;


void drawHand()
{
	glBegin(GL_LINES);
	glColor3f(0.0,1.0,0.0);
	glVertex2f(0.0,0.0);
	glVertex2f(0.0,len);
	glEnd();
}	

void drawMnHand()
{
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(0.0,0.0);
	glVertex2f(0.0,len+0.5);
	glEnd();
}


void drawHrHand()
{
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	glVertex2f(0.0,0.0);
	glVertex2f(0.0,len-0.5);
	glEnd();
}

void drawClock()
{

	glPushMatrix();
	glRotatef(-th,0.0,0.0,1.0);
	drawHand();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-mth,0.0,0.0,1.0);
	drawMnHand();
	glPopMatrix();

	
	glPushMatrix();
	glRotatef(-hth,0.0,0.0,1.0);
	drawHrHand();
	glPopMatrix();
	
	glColor3f(1.0,1.0,1.0);	
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<=360;i++)
	{
		glVertex3f(3.0*cosine(i),3.0*sine(i),0.0);
	}
	glEnd();

	glBegin(GL_POINTS);
	for(int i=0;i<360 ;i+=6)
		glVertex3f(2.8*cosine(i),2.8*sine(i),0.0);
	glEnd();

	for(int i=0;i<360;i+=30)
	{
		glRasterPos3f(2.7*sine(i),2.7*cosine(i),0.0);
		if(i==0)
			print("12");
		else
		{
			stringstream ss;
			ss<<i/30;
			string str=ss.str();
			char * fi = (char *)str.c_str();
			print("%s",fi);
		}
	}
}

void drawBoard()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	glTranslatef(0.0,0.0,-8.0);	
	glColor3f(1.0,1.0,1.0);

	drawClock();

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(float)w/(float)h,0.1f,200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update(int value)
{
	th+=6;
	if(th%60==0)
		mth++;
	if(mth%60==0&&th==0)
		hth++;
	th%=360;
	mth%=360;
	hth%=360;
	glutTimerFunc(1000,update,0);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w=glutGet(GLUT_SCREEN_WIDTH);
	int h=glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize(w/2,h/2);
	glutCreateWindow("My Game");
	initRendering();

	glutDisplayFunc(drawBoard);
	glutIdleFunc(drawBoard);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000,update,0);

	glutMainLoop();
	return 0;
}
