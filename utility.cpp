#include "includes.h"

#define LEN 8192

bool blocked[15]={false};

void printv(va_list args, const char * format)
{
	char buf[LEN];
	char * ch=buf;
	vsnprintf(buf,LEN,format,args);
	while(*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void print(const char * format, ...)
{
	va_list args;
	va_start(args,format);
	printv(args,format);
	va_end(args);
}

void printAt(int x,int y, const char * format, ...)
{
	va_list args;
	glWindowPos2i(x,y);
	va_start(args,format);
	printv(args,format);
	va_end(args);
}

float random_x()
{
	int i=1;
	while(i<16)
	{
		i<<=1;
		i+=(rand()%2);
	}
	float a=-3.5,d=0.5;
	return a + ( ( (float)i - 16)*d);
}

void block(int i)
{
	blocked[i]=1;
}

bool isBlocked(int i)
{
	if(blocked[i])
		return true;
	return false;
}

void unblock()
{
	for(int i=0;i<15;i++)
		blocked[i]=false;
}
