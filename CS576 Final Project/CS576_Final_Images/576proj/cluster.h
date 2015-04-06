#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

#define WIDTH 352
#define HEIGHT 288
#define PICSIZE (352 * 288)
#define DATASIZE (352 * 288 * 3)
#define START_PIC_NUM 1
#define END_PIC_NUM 300



class cluster
{
public:
	//vector<Mat> allPicData;
	//vector<int> videos;
	vector<bool> isClassified;
	//vector<int> allPic;
	vector<int> facePic;
	vector<int> others;
	vector<vector<int>> siftPic;
	vector<vector<vector<int>>> siftRst;
	

	int startPicNum;
	int endPicNum;
	
	cluster(int start_pic_num, int end_pic_num): startPicNum(start_pic_num), endPicNum(end_pic_num)
	{
		//allPic.reserve(end_pic_num - start_pic_num + 2);
		isClassified.resize(end_pic_num + 1);
		for(int i = 0; i < isClassified.size(); i++)
			isClassified[i] = false;
		//allPicData.resize(end_pic_num - start_pic_num + 2);
	}
	//util
	void readRGBFile(string fileName, uchar data[][WIDTH * 3]);
	string getFileName(int i);
	void int2str(const int &int_temp,string &string_temp);
	string getImageFilePath(int i);
	//void loadAllPicData();
	//sift
	void getSIFTKeyPts(Mat& img, vector<KeyPoint>& keypoint_vec);
	void computeDescriptors(Mat& img, Mat& descriptor, vector<KeyPoint>& keyPoints);
	int getMatchedPointCount(vector<Mat>& descriptors, vector<vector<KeyPoint>> keyPoints, int n1, int n2);
	void siftPics();
	//faceDetect
	//IplImage* convertImageToGreyscale(const IplImage *imageSrc);
	
	
	bool facedetect(IplImage src);
	void getFaces();
	//hist
	MatND calcHistHSV(Mat hsv);
	void mergeSIFT(vector<vector<vector<int>>>& last_rst, vector<vector<int>> &sift_rslt, vector<MatND> &hists);
	void insertToClass(vector<bool> &isClassified, vector<vector<int>> &sift_rslt, vector<MatND> &hists, vector<vector<vector<int>>> &mclass);
	void adjustSiftClasses();
	//show
	void collage(vector<Mat> src,int gridx,int gridy);
	IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight);
	void tile(const vector<Mat> &src, Mat &dst, int grid_x, int grid_y);
	//void onMouse(int event, int x, int y, int flag, void *param);
	void magnifyPic(vector<Mat> src,int index);
	void DivideSquare(const vector<Mat> &src, Mat &dst);
	void Init(const vector<Mat> &src, Mat &dst);
};