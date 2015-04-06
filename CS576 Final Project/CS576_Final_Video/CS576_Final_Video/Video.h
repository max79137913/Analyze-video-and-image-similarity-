#ifndef VIDEO_DISPLAY
#define VIDEO_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include "CVUtil.h"

#define  round(r) ((r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5))

using std::vector;
using namespace std;

// Class structure of Image 
// Use to encapsulate an RGB image
class MyVideo
{

private:
	int		Width;					// Width of video
	int		Height;					// Height of video
	char	VideoPath[_MAX_PATH];	// Video location
	char*	Data;					// RGB data of the video
	int     FrameNum;               // Number of frames
	int     VideoSize;              // Size of the video


public:
	// Constructor
	MyVideo();
	// Destructor
	~MyVideo();

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; };
	void	setVideoPath(const char* path){strcpy(VideoPath, path); };

	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getVideoData(int frame_no);
	int		getFrameNumer(){ return FrameNum; };

	// Input Output operations
	bool	readVideo();
	
	//Value Boundary Clampping 
	int		clamp(int i, const int lb, const int ub);

};

#endif //IMAGE_DISPLAY
