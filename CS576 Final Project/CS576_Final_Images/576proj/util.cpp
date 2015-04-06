#include"cluster.h"

void cluster::readRGBFile(string fileName, uchar data[][WIDTH * 3])
{
	uchar* buffer = new uchar[DATASIZE];
	ifstream in;
	in.open(fileName, ios::binary);
	in.read((char*)buffer, DATASIZE);
	for(int i = 0; i < PICSIZE; i++){
		data[i / WIDTH][i % WIDTH * 3] = buffer[i + PICSIZE * 2];
		data[i / WIDTH][i % WIDTH * 3 + 1] = buffer[i + PICSIZE];
		data[i / WIDTH][i % WIDTH * 3 + 2] = buffer[i];
	}
	delete[] buffer;
	in.close();
}

string cluster::getFileName(int i)
{
	stringstream fileName;
	if(i < 10)
		fileName<<"image00"<<i<<".rgb";
	else if(i < 100)
		fileName<<"image0"<<i<<".rgb";
	else
		fileName<<"image"<<i<<".rgb";
	return fileName.str();
}

void cluster::int2str(const int &int_temp,string &string_temp)
{
	stringstream stream;
	stream<<int_temp;
	string_temp=stream.str();   //此处也可以用 stream>>string_temp
}

string cluster::getImageFilePath(int i)
{
	string fpath, i_str, fext = ".rgb";
	int2str(i, i_str);
	if(i>=1 && i <10)
		fpath = ".\\DataSet\\image00" + i_str + fext;
	else if(i >= 10 && i < 100)
		fpath = ".\\DataSet\\image0" + i_str + fext;
	else if(i >= 100 && i < 1000)
		fpath = ".\\DataSet\\image" + i_str + fext;
	else
		fpath = "";
	return fpath;
}
