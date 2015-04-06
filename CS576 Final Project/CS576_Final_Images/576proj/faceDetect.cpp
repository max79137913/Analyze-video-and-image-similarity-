#include"cluster.h"

bool cluster::facedetect(IplImage src)
{
	bool success=0;

	// 加载Haar特征检测分类器  
	// haarcascade_frontalface_alt.xml系OpenCV自带的分类器 下面是我机器上的文件路径  
	const char *pstrCascadeFileName = "haarcascade_frontalface_alt2.xml"; 

	CvHaarClassifierCascade *pHaarCascade = NULL;  
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);  

	// 载入图像  
	IplImage SrcImage = src; 
	IplImage *pSrcImage = &SrcImage; 

	IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);  

	cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);  

	// 人脸识别与标记  
	if (pHaarCascade != NULL)  
	{         
		CvScalar FaceCirclecolors[] =   
		{  
			{{0, 0, 255}},  
			{{0, 128, 255}},  
			{{0, 255, 255}},  
			{{0, 255, 0}},  
			{{255, 128, 0}},  
			{{255, 255, 0}},  
			{{255, 0, 0}},  
			{{255, 0, 255}}  
		};  

		CvMemStorage *pcvMStorage = cvCreateMemStorage(0);  
		cvClearMemStorage(pcvMStorage);  
		// 识别  
		DWORD dwTimeBegin, dwTimeEnd;  
		dwTimeBegin = GetTickCount();  

		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);  //important

		dwTimeEnd = GetTickCount();  

		printf("face numbers: %d   take times: %d ms\n", pcvSeqFaces->total, dwTimeEnd - dwTimeBegin);  

		if(pcvSeqFaces->total>0)
		{
			success=1;
		}
		// 标记  
		for(int i = 0; i <pcvSeqFaces->total; i++)  
		{  
			CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i); // (original)
			
			CvPoint center;  
			int radius;  
			center.x = cvRound((r->x + r->width * 0.5));  
			center.y = cvRound((r->y + r->height * 0.5));  
			radius = cvRound((r->width + r->height) * 0.25);  
			cvCircle(pSrcImage, center, radius, FaceCirclecolors[i % 8], 2);  
		}  
		cvReleaseMemStorage(&pcvMStorage);  
	}  

	const char *pstrWindowsTitle = "face search";  

	cvDestroyWindow(pstrWindowsTitle);     
	cvReleaseImage(&pGrayImage);  

	return success;
}

void cluster::getFaces()
{
	vector<Mat> src_img;
	uchar data[HEIGHT][WIDTH * 3];
	//vector<Mat> characterM;
	//vector<int> character;
	//	Gzrgb data;
	//Mat temp;
	int i;
	bool success;
	//vector<int> photo;

	/*for(i=0;i<300;i++){
		photo.push_back(i);
	}*/
	
	for(int i = startPicNum - 1; i <= endPicNum - 1; ++i)
	{
		readRGBFile(getImageFilePath(i), data);
		Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data);
		//namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		//imshow("calcHist Demo", img);
		//waitKey(500);
		src_img.push_back(img.clone());
	}

	for(i=startPicNum-1;i<=endPicNum-1;i++)
	{
		cout<<"detecting img "<<i + 1<<endl;
		success=facedetect(src_img[i]);
		if(success)
		{	
			/*vector<int>::iterator it;

			it = find (photo.begin(), photo.end(), i);
			if (it != photo.end()){

				photo.erase(it);
				character.push_back(i);
			}*/
			facePic.push_back(i);		
		}
	}
	for(int i = 0; i < facePic.size(); i++){
		isClassified[facePic[i]] = true;
	}
}