#include <iostream>
#include <Windows.h>
#include <string>
#include <opencv2\opencv.hpp>
#include "CVUtil.h"
#include <math.h>
#include "Video.h"

using namespace cv;
using namespace std;

void tile(const vector<Mat> &src, Mat &dst, int grid_x, int grid_y);
void collage(vector<Mat> src,int gridx,int gridy, int level);
void onMouse(int event, int x, int y, int flag, void *param);

vector<Mat> tmpsrc_img;
vector<vector<int>> ikeyFrames;
vector<vector<Mat>> videoFrames;
vector<Mat> keyFrames;
int index0, index1;

int main(){
	MyVideo myvideo;
	uchar* buffer = new uchar[DATASIZE];
	//Read videos
	for(int i = 0; i < 10; ++i){
		string fpath = getVideoFilePath(i+1);
		cout<<"Reading video file->"<<fpath<<endl;
		myvideo.setVideoPath(fpath.c_str());
		myvideo.readVideo();
		//store frames of each video to vectors
		vector<Mat> temp_f;
		for(int j = 0; j < myvideo.getFrameNumer(); ++j){
			buffer =  (uchar*)myvideo.getVideoData(j);
			Mat src_f = Mat(HEIGHT, WIDTH, CV_8UC3, buffer);
			temp_f.push_back(src_f.clone());
		}
		videoFrames.push_back(temp_f);
		cout<<"Reading Finish, Processing now..."<<endl;
		
		//store the first frame of each video
		vector<int> itemp;
		itemp.push_back(0);
		
		//Calculate Histogram
		vector<MatND> hists;
		for(int j = 0; j < myvideo.getFrameNumer(); ++j){
			//Convert to HSV
			Mat src = temp_f[j].clone();
			cvtColor(src, src, COLOR_BGR2HSV);
			hists.push_back(calcHistHSV(src).clone());
		}
		
		//Set the first frame as base case for comparison
		MatND base = hists[0];
		for(int k = 1; k < hists.size(); ++k){
			float r_val = compareHist(base, hists[k], CV_COMP_BHATTACHARYYA);
			if(r_val > 0.25){
				itemp.push_back(k);
				base = hists[k];
			}
		}
		ikeyFrames.push_back(itemp);
		cout<<"Processing Finish!"<<endl;
	}
	
	for(int i = 0; i < 10; ++i){
		keyFrames.push_back(videoFrames[i][0]);
	}

	collage(keyFrames, 3, 4, 0);

	return 0;
}

void tile(const vector<Mat> &src, Mat &dst, int grid_x, int grid_y)
{
	// patch size
	int width  = WIDTH;
	int height = HEIGHT;
	// iterate through grid
	int k = 0;
	for(int i = 0; i < grid_y; i++) {
		for(int j = 0; j < grid_x; j++) {
			if(src.size()>j+i*grid_x)
			{
				Mat s = src[k++];
				resize(s,s,Size(width,height));
				s.copyTo(dst(Rect(j*15+j*width,i*15+i*height,width,height)));
			}
		}
	}

}

void onMouse(int event, int x, int y, int flag, void *ptr)
{
	switch(event){
	case CV_EVENT_LBUTTONDOWN:
		if(flag) {
			//printf("%d  %d\n",x,y);
		}
		int* gridsize=(int*)ptr;
		int gridx=gridsize[0]; // 多少水平的塊
		int gridy=gridsize[1]; // 多少垂直的塊
		int srcSize=gridsize[2]; // 總共多少塊
		int ImgSizeY=gridy*HEIGHT+15*gridy;
		int ImgSizeX=gridx*WIDTH+15*gridx;
		int scrolWidth=gridsize[3]; // scroll寬
		int scrolHight=gridsize[4]; // scroll高
		int intervalX=gridsize[5];  // x距離區間
		int intervalY=gridsize[6];  // y距離區間
		int initLocationX=gridsize[7];       // 開始位置
		int initLocationY=gridsize[8];       // 開始位置


		for(int i = 0; i < gridy; i++) {
			for(int j = 0; j < gridx; j++) {
				if(srcSize>j+i*gridx)
				{
					Point p(x+scrolWidth, y+scrolHight);


					Rect rect(initLocationX+j*intervalX+j*WIDTH,+initLocationY+i*intervalY+i*HEIGHT,WIDTH,HEIGHT);
					if(rect.contains(p))
					{	
						printf("The %d index\n",j+i*gridx);
						if(gridsize[9] == 0){
							index0 = j + i * gridx;
							vector<Mat> subKeyFrames;
							for(int x = 0; x < ikeyFrames[index0].size(); ++x){
								subKeyFrames.push_back(videoFrames[index0][ikeyFrames[index0][x]].clone());
							}
							collage(subKeyFrames, 3, 4, 1);
						}
						else if(gridsize[9] == 1){
							index1 = j + i * gridx;
							int frame_no = ikeyFrames[index0][index1];
							cout<<"KeyFrame:"<<frame_no;
							playVideoFrom(frame_no, videoFrames[index0]);
						}
					}
				}
			}
		}

		break;
	}
}

void collage(vector<Mat> src,int gridx,int gridy, int level)
{
	tmpsrc_img=src;

	int gridsize[10]={gridx,gridy,src.size(),0,0,15,15,0,0,level};
	//int gridsize[5];
	int  nWidth  = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度
	int  nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
	/////////////set up control win
	if(nHeight>=gridy*HEIGHT)nHeight=gridy*HEIGHT-1;
	if(nWidth>=gridx*WIDTH)nWidth=gridx*WIDTH-1;
	int scrolHight=0;
	int scrolWidth=0;  

	if(level == 0){
		namedWindow("Video Level 1",WINDOW_AUTOSIZE);  //new
		namedWindow("Control Video Level 1",WINDOW_AUTOSIZE); //new
		
		cvCreateTrackbar("Hscroll", "Control Video Level 1", &scrolHight, (gridy*HEIGHT-nHeight));
		cvCreateTrackbar("Wscroll", "Control Video Level 1", &scrolWidth, (gridx*WIDTH-nWidth));

		Mat res = Mat(gridy*HEIGHT+15*gridy,gridx*WIDTH+15*gridx,CV_8UC3);
		tile(src,res,gridx,gridy);

		setMouseCallback("Video Level 1", onMouse, &gridsize);
		while(waitKey(10)!='q'){
			Mat winImage= res( Rect(scrolWidth,scrolHight,nWidth,nHeight) );
			imshow("Video Level 1",winImage);
			gridsize[3]=scrolWidth;
			gridsize[4]=scrolHight;

		}
		destroyWindow("Video Level 1");
		destroyWindow("Control Video Level 1");

	}

	if(level == 1){
		namedWindow("Video Level 2",WINDOW_AUTOSIZE);  //new
		namedWindow("Control Video Level 2",WINDOW_AUTOSIZE); //new
		
		cvCreateTrackbar("Hscroll", "Control Video Level 2", &scrolHight, (gridy*HEIGHT-nHeight));
		cvCreateTrackbar("Wscroll", "Control Video Level 2", &scrolWidth, (gridx*WIDTH-nWidth));

		Mat res = Mat(gridy*HEIGHT+15*gridy,gridx*WIDTH+15*gridx,CV_8UC3);
		tile(src,res,gridx,gridy);

		setMouseCallback("Video Level 2", onMouse, &gridsize);
		while(waitKey(10)!='q'){
			Mat winImage= res( Rect(scrolWidth,scrolHight,nWidth,nHeight) );
			imshow("Video Level 2",winImage);
			gridsize[3]=scrolWidth;
			gridsize[4]=scrolHight;

		}
		destroyWindow("Video Level 2");
		destroyWindow("Control Video Level 2");
		collage(keyFrames, 3, 4, 0);
	}

}