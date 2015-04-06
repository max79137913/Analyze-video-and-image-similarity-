#include "CVUtil.h"

void readRGBFile(string fileName, Gzrgb data)
{
	uchar* buffer = new uchar[DATASIZE];

	ifstream in;
	in.open(fileName, ios::binary);
	in.read((char*)buffer, DATASIZE);
	for(int i = 0; i < PICSIZE; i++){
		data[i / WIDTH][i % WIDTH * 3]		= buffer[i + PICSIZE * 2];
		data[i / WIDTH][i % WIDTH * 3 + 1]  = buffer[i + PICSIZE];
		data[i / WIDTH][i % WIDTH * 3 + 2]  = buffer[i];
	}

	delete[] buffer;
	in.close();
}

MatND calcHistHSV(Mat hsv){
	//50 bins->hue & 60 bins->saturation
	int h_bins = 50, s_bins = 60;
	int histSize[] = {h_bins, s_bins};

	//hue的范围[0-179], saturation的范围[0-255]
	float h_ranges[] = {0, 180};
	float s_ranges[] = {0, 256};

	const float* ranges[] = {h_ranges, s_ranges};

	//利用0,1通道
	int channels[] = {0, 1};

	//直方图存储容器
	MatND hist;
	//计算图的直方图
	calcHist(&hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
	normalize (hsv, hsv, 0, 1, NORM_MINMAX, -1, Mat());

	return hist;
}

void playVideoFrom(int i, vector<Mat> videoFrames){
	namedWindow("Play Video", WINDOW_AUTOSIZE);
	for(int j = i; j < videoFrames.size(); ++j){
		Mat src = videoFrames[j];
		imshow("Play Video", src);
		waitKey(33);
	}
	destroyWindow("Play Video");
}

string getVideoFilePath(int i){
	string fpath, i_str, fext = ".rgb";
	int2str(i, i_str);
	if(i>=1 && i <10)
		fpath = ".\\DataSet\\video0" + i_str + fext;
	else if(i >= 10 && i < 100)
		fpath = ".\\DataSet\\video" + i_str + fext;
	else
		fpath = "";
	return fpath;
}
void int2str(const int &int_temp,string &string_temp)
{
	stringstream stream;
	stream<<int_temp;
	string_temp=stream.str();
}
