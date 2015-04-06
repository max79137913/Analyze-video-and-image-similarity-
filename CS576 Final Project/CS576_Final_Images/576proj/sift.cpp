#include "cluster.h"

void cluster::getSIFTKeyPts(Mat& img, vector<KeyPoint>& keypoint_vec)
{
	SiftFeatureDetector detector; 
	detector.detect(img, keypoint_vec);
}

void cluster::computeDescriptors(Mat& img, Mat& descriptor, vector<KeyPoint>& keyPoints)
{
	SiftDescriptorExtractor extractor; 
	extractor.compute(img, keyPoints, descriptor);//计算特征向量
}

int cluster::getMatchedPointCount(vector<Mat>& descriptors, vector<vector<KeyPoint>> keyPoints, int n1, int n2)
{
	BFMatcher matcher(NORM_L2, true);
	vector<DMatch> matches;
	matcher.match(descriptors[n1], descriptors[n2], matches);
	if(matches.size() == 0)
	{
	cout<<"No matches!!!"<<endl;
		return 0;
	}
	else{
		double min_dist = 50.0;  

		for(unsigned int i = 0; i < matches.size(); i++)  
		{  
			double dist = matches[i].distance;  
			if(dist < min_dist) min_dist = dist;  
		}  

		//筛选出较好的匹配点  
		int matchedPts = 0;
		for(unsigned int i = 0; i<matches.size(); i++)  {  
			if(matches[i].distance < 3 * min_dist){  
				matchedPts++;
			}  
		}
		return matchedPts;
	}
}

void cluster::siftPics()
{
	uchar data1[HEIGHT][WIDTH * 3];
	
	//ofstream out;
	//out.open("result.txt");

	SiftFeatureDetector detector;
	SiftDescriptorExtractor extractor;//定义描述子对象
	vector<vector<KeyPoint>> keyPoints;
	vector<Mat> descriptors;
	keyPoints.resize(endPicNum + 1);
	descriptors.resize(endPicNum + 1);
	Mat img;
	for(int i = startPicNum; i <= endPicNum; i++){
		cout<<"Doing pic "<<i<<"..."<<endl;
		readRGBFile(getImageFilePath(i), data1);
		img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);
		//imwrite(getFileName(i)+".jpg", img);
		getSIFTKeyPts(img, keyPoints[i]);
		computeDescriptors(img, descriptors[i], keyPoints[i]);
	}
	
	BFMatcher matcher(NORM_L1, true);//bf matcher?
	//vector<bool> flag; //vector<int[2]> pair;
	//flag.resize(endPicNum + 1);
	//for(int i = 0; i < endPicNum + 1; i++)
		//isSiftClassified[i] = false;
	
	for(int i = endPicNum; i >= startPicNum; i--){
		if(!isClassified[i]){
			cout<<i<<endl;
			vector<int> similar;
			for(int j = i - 1; j >= startPicNum; j--){
				if(!isClassified[j]){
					int result = getMatchedPointCount(descriptors, keyPoints, i, j);
					if(result >= 19){
						isClassified[j] = true;
						similar.push_back(j);
					}
				}				
			}
			if(similar.size() > 0){
				/*stringstream outLine;
				outLine<<i<<": ";
				for(int i = 0; i < similar.size(); i++)
					outLine<<similar[i]<<" ";
				outLine<<endl;
				out<<outLine.str();*/
				isClassified[i] = true;
				vector<int> group;
				group.push_back(i);
				for(int i = 0; i < similar.size(); i++)
					group.push_back(similar[i]);
				siftPic.push_back(group);
			}	
		}
	}
	//out.close();
}