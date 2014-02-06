#include "includes.h"

int main(int argc, char **argv)
{
	alutInit(&argc,argv);
	ALuint buffer =alutCreateBufferFromFile("mario.wav");
	ALuint source;
	alGenSources(1,&source);
	alSourcei(source,AL_BUFFER,buffer);
	alSourcePlay(source);
	int error =alGetError();
	if(error)
		printf("%s Error\n",alutGetErrorString(error));
	return 0;
}
