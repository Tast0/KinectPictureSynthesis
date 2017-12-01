#include "stdafx.h"
#include "PictureSynthesis.h"

#include <string>
#include <iostream>
#include <Kinect.h>
#include <iostream>

using namespace std;

CPictureSynthesis::CPictureSynthesis()
{
	this->kinect = nullptr;

	this->colorHeight = 0;
	this->colorWidth = 0;
	this->myColorSource = nullptr;
	this->myColorReader = nullptr;
	this->myDescription = nullptr;
	this->colorDataSize = 0;
	this->output = nullptr;
	this->myColorFrame = nullptr;

	this->depthHeight = 0;
	this->depthWidth = 0;
	this->myDepthSource = nullptr;
	this->myDepthReader = nullptr;
	this->depthDataSize = 0;
	this->depthData = nullptr;
	this->myDepthframe = nullptr;

	this->bodyHeight = 0;
	this->bodyWidth = 0;
	this->myBodyIndexSource = nullptr;
	this->myBodyIndexReader = nullptr;
	this->bodyDataSize = 0;
	this->bodyData = nullptr;
	this->myBodyIndexFrame = nullptr;

	this->myMaper = nullptr;

	this->photoNumber = 0;
	this->count_B = 1; //���ص�λ�ò�һ��
	this->count_F = 0;

	this->changeBg = false;
	
	this->temp = imread("../Background/test0.jpg");
}

CPictureSynthesis::~CPictureSynthesis()
{
	//�ǵø����ͷ�
	delete[] depthData;        
	delete[] bodyData;
	delete[] output;

	this->myMaper->Release();
	this->myColorReader->Release();
	this->myDepthReader->Release();
	this->myBodyIndexReader->Release();
}
//͸��ǰ�����
void CPictureSynthesis::mapToMat(const cv::Mat &srcAlpha, cv::Mat &dest, int x, int y)
{
	int nc = 3;
	int alpha = 0;

	for (int j = 0; j < srcAlpha.rows; j++)
	{
		for (int i = 0; i < srcAlpha.cols * 3; i += 3)
		{
			// Ŀ��ͼƬΪ��ͨ������������ͨ���ı�������ͨ����Դͼ
			// i/3*4��ʾ��i/3�����ص�λ�� 
			// i/3*4 + 3��ʾ�����ص�alphaͨ����ֵ
			alpha = srcAlpha.ptr<uchar>(j)[i / 3 * 4 + 3];
			//alpha = 255-alpha;
			if (alpha != 0) //4ͨ��ͼ���alpha�ж�
			{
				for (int k = 0; k < 3; k++)
				{
					// if (src1.ptr<uchar>(j)[i / nc*nc + k] != 0)
					if ((j + y < dest.rows) && (j + y >= 0) &&
						((i + x * 3) / 3 * 3 + k < dest.cols * 3) && ((i + x * 3) / 3 * 3 + k >= 0) &&
						(i / nc * 4 + k < srcAlpha.cols * 4) && (i / nc * 4 + k >= 0))
					{
						dest.ptr<uchar>(j + y)[(i + x*nc) / nc*nc + k] = srcAlpha.ptr<uchar>(j)[(i) / nc * 4 + k];
					}
				}
			}
		}
	}
}
//��ȡ��ɫ����
bool CPictureSynthesis::OpenColor()
{
	//׼���ò�ɫͼ���Reader����ȡ�ߴ�
	this->kinect->get_ColorFrameSource(&this->myColorSource);
	HRESULT hr = S_OK;
	hr = this->myColorSource->OpenReader(&this->myColorReader);
	if (FAILED(hr))
	{
		return false;
	}
	this->myColorSource->get_FrameDescription(&this->myDescription);
	this->myDescription->get_Height(&this->colorHeight);
	this->myDescription->get_Width(&this->colorWidth);

	this->myDescription->Release();
	this->myColorSource->Release();
	//Ϊͼ��׼��buffer
	this->colorDataSize = this->colorHeight * this->colorWidth;
	this->output = new DepthSpacePoint[this->colorDataSize];
	//׼��buffer
	Mat colorData(colorHeight, colorWidth, CV_8UC4);        
	this->colorData = colorData;

	return true;
}
//��ȡ�������
bool CPictureSynthesis::OpenDepth()
{
	//׼�������ͼ���Reader����ȡ�ߴ�
	this->kinect->get_DepthFrameSource(&this->myDepthSource);
	HRESULT hr = S_OK;
	hr = this->myDepthSource->OpenReader(&this->myDepthReader);
	if (FAILED(hr))
	{
		return false;
	}
	this->myDepthSource->get_FrameDescription(&this->myDescription);
	this->myDescription->get_Height(&this->depthHeight);
	this->myDescription->get_Width(&this->depthWidth);

	this->myDescription->Release();
	this->myDepthSource->Release();
	//Ϊͼ��׼��buffer
	this->depthDataSize = this->depthHeight * this->depthWidth;
	this->depthData = new UINT16[this->depthDataSize];

	return true;
}
//��ȡ����������
bool CPictureSynthesis::OpenBodyIndex()
{
	//׼������������ͼ���Reader����ȡ�ߴ�
	this->kinect->get_BodyIndexFrameSource(&this->myBodyIndexSource);
	HRESULT hr = S_OK;
	hr = this->myBodyIndexSource->OpenReader(&this->myBodyIndexReader);
	if (FAILED(hr))
	{
		return false;
	}
	this->myDepthSource->get_FrameDescription(&this->myDescription);
	this->myDescription->get_Height(&this->bodyHeight);
	this->myDescription->get_Width(&this->bodyWidth);

	this->myDescription->Release();
	this->myBodyIndexSource->Release();
	//Ϊͼ��׼��buffer
	this->bodyDataSize = this->bodyHeight * this->bodyWidth;
	this->bodyData = new BYTE[this->bodyDataSize];

	return true;
}
bool CPictureSynthesis::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	if (!this->OpenColor())
	{
		return false;
	}
	if (!this->OpenDepth())
	{
		return false;
	}
	if (!this->OpenBodyIndex())
	{
		return false;
	}
	//���ҿ���Mappe
	this->kinect->get_CoordinateMapper(&this->myMaper);	

	return true;
}
//�л�����һ������
void CPictureSynthesis::NextScene()
{
	string str1 = "../Background/test";
	char str2[100];
	itoa(count_B, str2, 10);
	string str3 = ".jpg";
	string backgroundPath = str1 + str2 + str3;

	this->temp = imread(backgroundPath);
	count_B = (count_B + 1) % this->photoNumber;
	count_F = (count_F + 1) % this->photoNumber;
	
	changeBg = false;	
}
//���ǰ��
void CPictureSynthesis::AddFrontPhotos()
{
	string str1 = "../Outlook/test";
	char str2[100];
	itoa(count_F, str2, 10);
	string str3 = ".png";
	string frontPhotosPath = str1 + str2 + str3;

	Mat srcAlpha, frontPhoto;
	// imread��һ������Ϊ-1��ʾ��ȡ����alphaͨ����ͼƬ
	srcAlpha = imread(frontPhotosPath, -1);
	//��������ɫͼ��Ĵ�С
	resize(srcAlpha, frontPhoto, Size(colorWidth, colorHeight));
	// ��ʾ��dest�ĵ�(50,100)���ö�srcAlpha�ĵ���

	//mapToMat(frontPhoto, this->copy, 0, 0);
	this->cvAdd4cMat_q(this->copy, frontPhoto, 1.0);
}

Mat CPictureSynthesis::PictureSynthesis()
{
	if (this->changeBg)
	{
		CPictureSynthesis::NextScene();
	}

	resize(this->temp, this->background, Size(colorWidth, colorHeight));   //��������ɫͼ��Ĵ�С
	
	while (this->myColorReader->AcquireLatestFrame(&this->myColorFrame) != S_OK);   //��ȡcolorͼ
	this->myColorFrame->CopyConvertedFrameDataToArray(this->colorDataSize * 4, this->colorData.data, ColorImageFormat_Bgra);
	this->myColorFrame->Release();
		
	while (this->myDepthReader->AcquireLatestFrame(&this->myDepthframe) != S_OK);   //��ȡdepthͼ
	this->myDepthframe->CopyFrameDataToArray(this->depthDataSize, this->depthData);
	this->myDepthframe->Release();
		
	while (this->myBodyIndexReader->AcquireLatestFrame(&this->myBodyIndexFrame) != S_OK);
	this->myBodyIndexFrame->CopyFrameDataToArray(this->bodyDataSize, this->bodyData);
	this->myBodyIndexFrame->Release();

	this->copy = this->background.clone();                  //����һ�ݱ���ͼ��������
	if (this->myMaper->MapColorFrameToDepthSpace(this->depthDataSize, this->depthData, this->colorDataSize, this->output) == S_OK)
	{
		for (int i = 0; i < this->colorHeight; ++i)
		for (int j = 0; j < this->colorWidth; ++j)
		{
			DepthSpacePoint tPoint = this->output[i * colorWidth + j];    //ȡ�ò�ɫͼ���ϵ�һ�㣬�˵����������Ӧ�����ͼ�ϵ�����
			if (tPoint.X >= 0 && tPoint.X < this->depthWidth && tPoint.Y >= 0 && tPoint.Y < this->depthHeight)  //�ж��Ƿ�Ϸ�
			{
				int index = (int)tPoint.Y * this->depthWidth + (int)tPoint.X; //ȡ�ò�ɫͼ���ǵ��Ӧ��BodyIndex���ֵ(ע��Ҫǿת)
				if (this->bodyData[index] <= 5)                   //����жϳ���ɫͼ��ĳ�������壬���������滻����ͼ�϶�Ӧ�ĵ�
				{
					Vec4b   color = colorData.at<Vec4b>(i, j);
					this->copy.at<Vec3b>(i, j) = Vec3b(color[0], color[1], color[2]);
				}
			}
		}
		//ǰ�����
		if (this->count_F % 3 == 2)
		{
			CPictureSynthesis::AddFrontPhotos();
		}		
		//ȫ��
		//CPictureSynthesis::FullScreen();	
		this->input = this->copy;

	}

	return  this->input;
}
Mat CPictureSynthesis::NextPictureSynthesize()
{
	string strB1 = "../Background/test";
	char strB2[100];
	itoa((count_B) % this->photoNumber, strB2, 10);
	string strB3 = ".jpg";
	string nextBackgroundPath = strB1 + strB2 + strB3;

	Mat next = imread(nextBackgroundPath);
	Mat nextBackground;
	resize(next, nextBackground, Size(colorWidth, colorHeight));   //��������ɫͼ��Ĵ�С

	return nextBackground;
}
Mat CPictureSynthesis::LastPictureSynthesize()
{
	string strB1 = "../Background/test";
	char strB2[100];
	itoa((count_B + this->photoNumber - 2) % this->photoNumber, strB2, 10);
	string strB3 = ".jpg";
	string lastBackgroundPath = strB1 + strB2 + strB3;

	Mat last = imread(lastBackgroundPath);
	Mat lastBackground;
	resize(last, lastBackground, Size(colorWidth, colorHeight));

	return lastBackground;
}
Mat CPictureSynthesis::NowPictureSynthesize()
{
	string strB1 = "../Background/test";
	char strB2[100];
	itoa((count_B + this->photoNumber-1) % this->photoNumber, strB2, 10);
	string strB3 = ".jpg";
	string nowBackgroundPath = strB1 + strB2 + strB3;

	Mat now = imread(nowBackgroundPath);
	Mat nowBackground;
	resize(now, nowBackground, Size(colorWidth, colorHeight));

	return nowBackground;
}
//��Ƭ�ϳ��㷨�Ż�
int CPictureSynthesis::cvAdd4cMat_q(Mat &dst, Mat &scr, double scale)
{
	if (dst.channels() != 3 || scr.channels() != 4)
	{
		return true;
	}
	if (scale < 0.01)
		return false;
	std::vector<cv::Mat>scr_channels;
	std::vector<cv::Mat>dstt_channels;
	split(scr, scr_channels);
	split(dst, dstt_channels);
	CV_Assert(scr_channels.size() == 4 && dstt_channels.size() == 3);

	if (scale < 1)
	{
		scr_channels[3] *= scale;
		scale = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		dstt_channels[i] = dstt_channels[i].mul(255.0 / scale - scr_channels[3], scale / 255.0);
		dstt_channels[i] += scr_channels[i].mul(scr_channels[3], scale / 255.0);
	}
	merge(dstt_channels, dst);
	return true;
}
