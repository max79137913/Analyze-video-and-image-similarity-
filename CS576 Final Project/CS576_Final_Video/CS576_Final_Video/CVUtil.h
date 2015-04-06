#ifndef CV_UTIL_H
#define CV_UTIL_H

//CPP Libs
#include <iostream>
#include <fstream>
#include <Windows.h> 
//OpenCV libs
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//Constance
#define WIDTH 352
#define HEIGHT 288
#define PICSIZE (352 * 288)
#define DATASIZE (352 * 288 * 3)

//Type Definition
typedef uchar Gzrgb[HEIGHT][WIDTH * 3]; 

//Function
void	readRGBFile(string fileName, Gzrgb data);
MatND	calcHistHSV(Mat hsv);
string  getVideoFilePath(int i);
void	int2str(const int &int_temp,string &string_temp);
void	playVideoFrom(int i, vector<Mat> videoFrames);

#endif