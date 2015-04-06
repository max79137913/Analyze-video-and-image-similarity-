#include"cluster.h"

MatND cluster::calcHistHSV(Mat hsv){
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

void cluster::mergeSIFT(vector<vector<vector<int>>>& result, vector<vector<int>> &sift_rslt, vector<MatND> &hists){
	//vector<vector<vector<int>>> result;

	vector<bool> is_classified;
	//Initialize bool
	for(int i = 0; i < sift_rslt.size(); ++i)
		is_classified.push_back(false);

	//Merge Class
	float r_val;
	for(int i = 0; i < sift_rslt.size(); ++i){
		//cout<<"Image"<<sift_rslt[i][0]<<":"<<endl;
		//Already Classified
		vector<vector<int>> temp;
		
		if(is_classified[i] == true)
			continue;
		temp.push_back(sift_rslt[i]);
		/*for(int j = i + 1; j < sift_rslt.size(); ++j){
			r_val = compareHist(hists[sift_rslt[i][0]], hists[sift_rslt[j][0]], CV_COMP_BHATTACHARYYA);
			if(r_val < 0.45 && is_classified[j] == false){
				//cout<<sift_rslt[j][0]<<" ";
				is_classified[j] = true;
				temp.push_back(sift_rslt[j]);
				
			}
		}*/
		result.push_back(temp);
		//cout<<endl<<"-------------------------"<<endl;
	}

	//return result;
}

void cluster::insertToClass(vector<bool> &isClassified, vector<vector<int>> &sift_rslt, vector<MatND> &hists, vector<vector<vector<int>>> &mclass){
	double r_val;
	for(int k = startPicNum; k <= endPicNum; ++k){
		if(isClassified[k] == true)
			continue;
		double min_val = 1.0, class_id = 0;
		for(int j = 0; j < sift_rslt.size(); ++j){
			r_val = compareHist(hists[k], hists[sift_rslt[j][0]], CV_COMP_BHATTACHARYYA);
			if(r_val < min_val){
				min_val = r_val;
				class_id = sift_rslt[j][0];
			}
		}
		if(min_val > 0.5)
			continue;
		for(int m = 0; m < mclass.size(); ++m){
			for(int n = 0; n < mclass[m].size(); ++n){
				if(mclass[m][n][0] == class_id){
					mclass[m][n].push_back(k);
					isClassified[k] = true;
					//cout<<"Class id:"<<class_id<<" "<<"Image id:"<<k+1<<endl;
				}
			}
		}

		//cout<<endl<<"-------------------------"<<endl;
	}
}

void cluster::adjustSiftClasses()
{
	//HSV图容器
	//vector<Mat> hsv_img;
	//直方图存储容器
	vector<MatND> hists;
	
	
	hists.resize(startPicNum);//////////////////////////////////////////////////
	
	
	//vector<bool> isClassified;
	Mat src;
	uchar data[HEIGHT][WIDTH * 3];
	Scalar tempVal;
	float tempMean; 
	/*
	for(int i = 0; i < 300; ++i){
		if(i < 60)
			isClassified.push_back(false);
		else
			isClassified.push_back(true);
	}*/

	//读取图片
	for(int i = startPicNum; i <= endPicNum; ++i){
		readRGBFile(getImageFilePath(i), data);
		src = Mat(HEIGHT, WIDTH, CV_8UC3, data);
		//转换到HSV空间
		cvtColor(src, src, COLOR_BGR2HSV);
		//hsv_img.push_back(src.clone());
		//计算每张图的直方图
		hists.push_back(calcHistHSV(src).clone());
	}
	/*isClassified[64] = isClassified[73] = isClassified[78] = isClassified[79] = false;
	isClassified[88] = isClassified[100] = isClassified[104] = isClassified[105] = false;
	isClassified[113] = isClassified[117] = isClassified[126] = isClassified[127] = false;
	isClassified[131] = isClassified[133] = isClassified[136] = isClassified[147] = false;
	isClassified[148] = isClassified[154] = isClassified[218] = isClassified[259] = false;
	isClassified[278] = isClassified[285];

	int sift_rslt[300] = {69, 70, 73, 76, 78, 83, 88, 93, 104, 108, 113, 122, 124, 126,
						  130, 135, 136, 137, 138, 146, 163, 177, 212, 224, 231, 251, 
						  264, 266, 275, 281, 292, 298, 300};
	
	vector<vector<int>> sift;
	for(int i = 0; i < 33; ++i){
		vector<int> temp;
		temp.push_back(sift_rslt[i]);
		sift.push_back(temp);
	}*/
	mergeSIFT(siftRst, siftPic, hists);
	insertToClass(isClassified, siftPic, hists, siftRst);
	ofstream out("result.txt");
	for(int i = 0; i < siftRst.size(); ++i){
		cout<<"Class "<<i<<endl;
		out<<"Class "<<i<<endl;
		for(int j = 0; j < siftRst[i].size(); ++j){
			cout<<"-->Sub Class "<<j<<": ";
			out<<"-->Sub Class "<<j<<": ";
			for(int k = 0; k < siftRst[i][j].size(); ++k){
				cout<<siftRst[i][j][k]<<" ";
				out<<siftRst[i][j][k]<<" ";
			}
			cout<<endl;
			out<<endl;
		}
		cout<<endl;
		out<<endl;
	}

	for(int i = 1; i <= endPicNum; i++){
		if(isClassified[i] == false)
			others.push_back(i);
	}
	/*for(int i = 0; i < 300; ++i){
		if(isClassified[i] == false)
			cout<<i+1<<" ";
	}*/
	cout<<endl;
}