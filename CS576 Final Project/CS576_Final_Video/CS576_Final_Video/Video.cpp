#include "Video.h"

MyVideo::MyVideo() 
{
	Data = NULL;
	VideoPath[0] = NULL;
	Width = WIDTH;
	Height = HEIGHT;
	FrameNum = 0;
	VideoSize = 0;
}

MyVideo::~MyVideo()
{
	if ( Data )
		delete Data;
}

// MyVideo::ReadVideo
// Function to read the video given a path
bool MyVideo::readVideo()
{
	FILE * IN_FILE = NULL;

	//Check video parameters
	if (VideoPath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Video or Video properties not defined");
		fprintf(stderr, "Usage is `Video.exe Videofile w h`");
		return false;
	}
	// Create a valid output file pointer
	IN_FILE = fopen(VideoPath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	fseek(IN_FILE, 0L, SEEK_END);  
	
	VideoSize = ftell(IN_FILE);  

	FrameNum = VideoSize /(Width * Height * 3);
	
	rewind(IN_FILE);

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 
	// Allocate Data structure and copy
	Data = new char[VideoSize];

	for(int k = 0; k < FrameNum; ++k){
		
		for (i = 0; i < Width*Height; i ++)
		{
			Rbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Gbuf[i] = fgetc(IN_FILE);
		}
		for (i = 0; i < Width*Height; i ++)
		{
			Bbuf[i] = fgetc(IN_FILE);
		}

		for (i = 0; i < Height*Width; i++)
		{
			Data[3 * (i + k * Height * Width)]		= Bbuf[i];
			Data[3 * (i + k * Height * Width) + 1]	= Gbuf[i];
			Data[3 * (i + k * Height * Width) + 2]	= Rbuf[i];
		}
	
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}

char* MyVideo::getVideoData(int i){
	char *index = Data + i * Width * Height * 3;
	return index;
}

int MyVideo::clamp(int i, const int lb, const int ub){
	if (i > ub)
		return ub;
	else
		return lb;
}
