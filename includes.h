#ifndef INCLUDES
#define INCLUDES

#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
#include<sstream>
#include<ctime>
#include<cstdarg>
#include<cstdio>
#include<vector>

#define GL_GLEXT_PROTOTYPES

#include<GL/glut.h>
#include<AL/al.h>
#include<AL/alc.h>
#include<AL/alut.h>

#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define LASER     0
#define CATCH     1
#define SPIDER    2
#define PI 3.141592653589
#define sine(th) sin( th*PI/180)
#define cosine(th) cos(th*PI/180)
#define dis(a,b,c,d) sqrt( (a-c)*(a-c) + (b-d)*(b-d) )

void printv(va_list args, const char * format);
void print(const char * format, ...);
void printAt(int x,int y, const char * format, ...);
void update(int value);
void handleKeys(unsigned char key,int x,int y);
bool isBlocked(int i);
void block(int i);
float random_x();
void unblock();

typedef struct spider{
	float x,y;
	int color;
	bool toggle,reached;

}spider;


#endif
