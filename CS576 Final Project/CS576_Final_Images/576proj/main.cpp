#include "cluster.h"

vector<Mat> tmpsrc_img;
vector<Mat> characterM;// face vector
vector<vector<Mat>> characterM2;
vector<Mat> characterM22;
vector<Mat> characterM3;
vector<Mat> temp;
float CharacterMsize;
float CharacterMsize2;
float CharacterMsize3;
uchar data1[HEIGHT][WIDTH * 3];

cluster Cluster(START_PIC_NUM, END_PIC_NUM);

int index0, index1;

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




IplImage* resizeImage(const IplImage *origImg, int newWidth, int newHeight)
{
	IplImage *outImg = 0;
	int origWidth;
	int origHeight;
	if (origImg) {
		origWidth = origImg->width;
		origHeight = origImg->height;
	}
	if (newWidth <= 0 || newHeight <= 0 || origImg == 0 || origWidth <= 0 || origHeight <= 0) {
		printf("ERROR in resizeImage: Bad desired image size of %dx%d\n.", newWidth, newHeight);
		exit(1);
	}

	// Scale the image to the new dimensions, even if the aspect ratio will be changed.
	outImg = cvCreateImage(cvSize(newWidth, newHeight), origImg->depth, origImg->nChannels);
	if (newWidth > origImg->width && newHeight > origImg->height) {
		// Make the image larger
		cvResetImageROI((IplImage*)origImg);
		cvResize(origImg, outImg, CV_INTER_LINEAR);	// CV_INTER_CUBIC or CV_INTER_LINEAR is good for enlarging
	}
	else {
		// Make the image smaller
		cvResetImageROI((IplImage*)origImg);
		cvResize(origImg, outImg, CV_INTER_AREA);	// CV_INTER_AREA is good for shrinking / decimation, but bad at enlarging.
	}

	return outImg;
}

void magnifyPic(vector<Mat> src,int index)
{
	IplImage SrcImage = src[index]; 
	IplImage *pSrcImage = &SrcImage; 

	pSrcImage=resizeImage(pSrcImage,WIDTH*2,HEIGHT*2);

	const char *PicWin ="Picture window";  
    cvNamedWindow(PicWin, CV_WINDOW_AUTOSIZE);  
    cvShowImage(PicWin, pSrcImage);  
	
	
                                         // Wait for a keystroke in the window
}



void DivideSquare(const vector<Mat> &src, Mat &dst)
{
    // patch size
    int width  = WIDTH/2;
    int height = HEIGHT/2;
    // iterate through grid
    int k = 0;
	
    for(int i = 0; i < 2; i++)
	{
        for(int j = 0; j < 2; j++) {
		
			if(i*2+j<src.size())
			{
            Mat s = src[k++];
            resize(s,s,Size(width,height));
            s.copyTo(dst(Rect(j*width,i*height,width,height)));
			}
			
										}
    }

	
    
}
void Init(const vector<Mat> &src, Mat &dst)
{
	 // patch size
    int width  = WIDTH;
    int height = HEIGHT;
    // iterate through grid
    int k = 0;
   
	for(int j = 0; j < src.size(); j++) {
		
            Mat s = src[k++];
            resize(s,s,Size(width,height));
            s.copyTo(dst(Rect(width*j+80*j,height,width,height)));
			
									}
    


}


void collage(vector<Mat> src,int gridx,int gridy, int level,int position);
void preCollage(vector<vector<Mat>> src,int gridx,int gridy);


void onMouse(int event, int x, int y, int flag, void *ptr)
{
	switch(event){
    case CV_EVENT_LBUTTONDOWN:
		if(flag) {printf("%d  %d\n",x,y);}
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
		int Level=gridsize[9];
		int Position=gridsize[10];
		

		for(int i = 0; i < gridy; i++) {
        for(int j = 0; j < gridx; j++) {
			if(srcSize>j+i*gridx)
			{
		Point p(x+scrolWidth, y+scrolHight);
		
	
		Rect rect(initLocationX+j*intervalX+j*WIDTH,+initLocationY+i*intervalY+i*HEIGHT,WIDTH,HEIGHT);
		if(rect.contains(p))
		{
			printf("The %d index",j+i*gridx);
			//magnifyPic(tmpsrc_img,j+i*gridx);
			switch(Level)
			{
			case 0:
				index0 = j + i * gridx;
				if(j+i*gridx==0)collage(characterM,ceil(CharacterMsize),floor(CharacterMsize), 1,index0);
				if(j+i*gridx==1)preCollage(characterM2,ceil(CharacterMsize2),floor(CharacterMsize2));
				if(j+i*gridx==2)collage(characterM3,ceil(CharacterMsize),floor(CharacterMsize3), 1,index0);
				break;
			case 1:
				index1 = j + i * gridx;
				
				if(Position==0)magnifyPic(characterM,j+i*gridx);
				if(Position==1)magnifyPic(temp,j+i*gridx);
				if(Position==2)magnifyPic(characterM3,j+i*gridx);
				break;

			case 2:
				int clickedIndex = j + i * gridx;
				if(Cluster.siftRst[clickedIndex].size() == 1){
					
					temp.clear();
					for(int p = 0; p < Cluster.siftRst[clickedIndex][0].size(); p++){
						Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.siftRst[clickedIndex][0][p]), data1);
						Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);
						temp.push_back(img.clone());
					}
					float tempSize = sqrt((float)temp.size());
					collage(temp,ceil(tempSize),floor(tempSize), 1,1);
				}
				break;
				//else{
					//preCollage(characterM2,ceil(CharacterMsize2),floor(CharacterMsize2),1);
				//}
				
			//case 3:

			}
		}
			}
										}
        }
		
        break;
	}
}

void collage(vector<Mat> src,int gridx,int gridy, int level,int position)
{
/*	
	Mat img;

    namedWindow("image", WINDOW_NORMAL);
    int gridx=5, gridy=5;
	int index=1;
    vector<Mat> vec;
    VideoCapture cap(0);
    for(;;)
    {
		readRGBFile(getImageFilePath(index), data);
		img = Mat(HEIGHT, WIDTH, CV_8UC3, data);
		vec.push_back(img.clone());
        imshow("image", img); 
       
        if ( vec.size() == gridx*gridy ) break;
		index++;
    }
*/

	if(gridx*gridy<src.size()){gridy+=1;}
	tmpsrc_img=src;
	
	int gridsize[11]={gridx,gridy,src.size(),0,0,15,15,0,0,level,position};



	//int gridsize[5];
	int  nWidth=GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    h
	int  nHeight=GetSystemMetrics(SM_CYSCREEN); //屏幕高度
/////////////set up control win
	if(nHeight>=gridy*HEIGHT)nHeight=gridy*HEIGHT-1;
	if(nWidth>=gridx*WIDTH)nWidth=gridx*WIDTH-1;
	int scrolHight=0;
	int scrolWidth=0;  

	if(level == 1){
		namedWindow("Picture Level 1", WINDOW_AUTOSIZE);
		namedWindow("Control Picture Level 1", WINDOW_AUTOSIZE);
		cvCreateTrackbar("Hscroll", "Control Picture Level 1", &scrolHight, (gridy*HEIGHT-nHeight));
		cvCreateTrackbar("Wscroll", "Control Picture Level 1", &scrolWidth, (gridx*WIDTH-nWidth));
		Mat res = Mat(gridy*HEIGHT+15*gridy,gridx*WIDTH+15*gridx,CV_8UC3);
		tile(src,res,gridx,gridy);
	///////
	
		setMouseCallback("Picture Level 1", onMouse,&gridsize);
		while(waitKey(10)!='q'){
		Mat winImage= res( Rect(scrolWidth,scrolHight,nWidth,nHeight) );
		imshow("Picture Level 1",winImage);

		gridsize[3]=scrolWidth;
		gridsize[4]=scrolHight;
	
		}//while
	destroyWindow("Picture Level 1");
	destroyWindow("Control Picture Level 1");

	
	}
	if(level == 2){
		namedWindow("Picture Level 2", WINDOW_AUTOSIZE);
		namedWindow("Control Picture Level 2", WINDOW_AUTOSIZE);
		cvCreateTrackbar("Hscroll", "Control Picture Level 2", &scrolHight, (gridy*HEIGHT-nHeight));
		cvCreateTrackbar("Wscroll", "Control Picture Level 2", &scrolWidth, (gridx*WIDTH-nWidth));
		Mat res = Mat(gridy*HEIGHT+15*gridy,gridx*WIDTH+15*gridx,CV_8UC3);
		tile(src,res,gridx,gridy);
	///////
	
		setMouseCallback("Picture Level 2", onMouse,&gridsize);
		while(waitKey(10)!='q'){
		Mat winImage= res( Rect(scrolWidth,scrolHight,nWidth,nHeight) );
		imshow("Picture Level 2",winImage);

		gridsize[3]=scrolWidth;
		gridsize[4]=scrolHight;
	
		}//while
		destroyWindow("Picture Level 2");
	destroyWindow("Control Picture Level 2");

	}
	

	

//////////////
   // Mat res = Mat(nHeight,nWidth,CV_8UC3);
	
	
///////


   // imshow("image",res);
	

}

void collage2(vector<Mat> src,int gridx,int gridy)
{

	if(gridx*gridy<src.size()){gridy+=1;}

	tmpsrc_img=src;
	namedWindow("image",WINDOW_AUTOSIZE);  //new
	namedWindow("controlWin",WINDOW_AUTOSIZE); //new
	int gridsize[11]={gridx,gridy,src.size(),0,0,15,15,0,0,2,1};



	//int gridsize[5];
	int  nWidth=GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    h
	int  nHeight=GetSystemMetrics(SM_CYSCREEN); //屏幕高度
/////////////set up control win
	if(nHeight>=gridy*HEIGHT)nHeight=gridy*HEIGHT-1;
	if(nWidth>=gridx*WIDTH)nWidth=gridx*WIDTH-1;
	int scrolHight=0;
	int scrolWidth=0;  
	cvCreateTrackbar("Hscroll", "controlWin", &scrolHight, (gridy*HEIGHT-nHeight));
	cvCreateTrackbar("Wscroll", "controlWin", &scrolWidth, (gridx*WIDTH-nWidth));

	

//////////////
   // Mat res = Mat(nHeight,nWidth,CV_8UC3);
	Mat res = Mat(gridy*HEIGHT+15*gridy,gridx*WIDTH+15*gridx,CV_8UC3);
    tile(src,res,gridx,gridy);
///////
	
	setMouseCallback("image", onMouse,&gridsize);
	while(waitKey(10)!='q'){
	Mat winImage= res( Rect(scrolWidth,scrolHight,nWidth,nHeight) );
	imshow("image",winImage);

	gridsize[3]=scrolWidth;
	gridsize[4]=scrolHight;
	
			}//while
	
	
///////


   // imshow("image",res);
	
    waitKey(0);


}

void preCollage(vector<vector<Mat>> src,int gridx,int gridy)
{

	vector<Mat> bigclass;
	for(int i=0;i<src.size();i++)
	{
	Mat DivcharacterM = Mat(HEIGHT,WIDTH,CV_8UC3);
	DivideSquare(src[i],DivcharacterM);
	bigclass.push_back(DivcharacterM);
	}
	float bigclasssize=sqrt((float)bigclass.size());

collage2(bigclass,ceil(bigclasssize),floor(bigclasssize));


}
int main()
{
	
	//Cluster.loadAllPicData();
	
	
	
	cout<<"Getting Faces..."<<endl;
	qCluster.getFaces();
	
	cout<<"SIFTing..."<<endl;
	Cluster.siftPics();






	cout<<"Adjusting classes..."<<endl;
	Cluster.adjustSiftClasses();
/*	*/
	//for(int i = 0; i < ClusterisClassified.siz

	//int i = 1;
	

	vector <Mat> src_img;

	vector<Mat> catagoryList;
	uchar data1[HEIGHT][WIDTH * 3];
	//allPicData.resize(1);
	for(int i = 1; i <= 300; ++i)
	{
		Cluster.readRGBFile(Cluster.getImageFilePath(i), data1);
		Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);
		src_img.push_back(img.clone());
	}
	//////////////

	Cluster.facePic.push_back(22);
	Cluster.facePic.push_back(23);
	Cluster.facePic.push_back(24);
	Cluster.facePic.push_back(25);


	for(int i = 0; i < Cluster.facePic.size(); ++i)
	{
		Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.facePic[i]), data1);
		Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);		
		characterM.push_back(img.clone());
	}

	for(int i = 0; i < Cluster.siftRst.size(); i++){
		Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.siftRst[i][0][0]), data1);
		Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);	
		characterM22.push_back(img.clone());
	}

	for(int i = 0; i < Cluster.siftRst.size(); i++){
		vector<Mat> temp;
		//Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.siftRst[i][0][0]), data1);
		Mat img;	
		//temp.push_back(img.clone());
		//Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.siftRst[i][0][1]), data1);
		int count = 0;
		for(int j = 0; j < Cluster.siftRst[i].size(); j++){
			for(int k = 0; k < Cluster.siftRst[i][j].size(); k++){
				if(count < 4){
					Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.siftRst[i][j][k]), data1);
					img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);	
					temp.push_back(img.clone());
					count++;
				}
			}
		}
		characterM2.push_back(temp);
	}

	for(int i = 0; i < Cluster.others.size(); i++){
		Cluster.readRGBFile(Cluster.getImageFilePath(Cluster.others[i]), data1);
		Mat img = Mat(HEIGHT, WIDTH, CV_8UC3, data1);		
		characterM3.push_back(img.clone());
	}
	CharacterMsize=sqrt((float)Cluster.facePic.size());
	CharacterMsize2=sqrt((float)Cluster.siftRst.size());
	CharacterMsize3=sqrt((float)Cluster.others.size());

	Mat DivcharacterM = Mat(HEIGHT,WIDTH,CV_8UC3);
	DivideSquare(characterM,DivcharacterM);

	Mat DivcharacterM2 = Mat(HEIGHT,WIDTH,CV_8UC3);
	DivideSquare(characterM22,DivcharacterM2);

	Mat DivcharacterM3 = Mat(HEIGHT,WIDTH,CV_8UC3);
	DivideSquare(characterM3,DivcharacterM3);

	catagoryList.push_back(DivcharacterM.clone());
	catagoryList.push_back(DivcharacterM2.clone());
	catagoryList.push_back(DivcharacterM3.clone());

	Mat category = Mat(GetSystemMetrics(SM_CYSCREEN),GetSystemMetrics(SM_CXSCREEN),CV_8UC3);
	Init(catagoryList,category);

	int gridsize[11]={3,1,3,0,0,80,0,0,HEIGHT,0,0};

	
	
	namedWindow("Picture Level 0", WINDOW_AUTOSIZE);
	imshow("Picture Level 0",category);
	setMouseCallback("Picture Level 0", onMouse,&gridsize);

	waitKey(0);


	//system("pause");
	return 0;
}