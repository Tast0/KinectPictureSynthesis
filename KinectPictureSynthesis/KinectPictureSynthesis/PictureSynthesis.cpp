#include "PictureSynthesis.h"
#include "include.h"

#include <sstream>
#include <string>
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

	this->photoNumber = 3;
	this->count_B = 1; //���ص�λ�ò�һ��
	this->count_F = 0;

	this->changeBg = false;
	this->screenShot = false;
	
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
	stringstream stream;
	stream << count_B;
	string str2 = stream.str();
	string str3 = ".jpg";
	string backgroundPath = str1 + str2 + str3;

	this->temp = imread(backgroundPath);
	count_B = (count_B + 1) % this->photoNumber;
	count_F = (count_F + 1) % this->photoNumber;
	
	changeBg = false;	
}
void CPictureSynthesis::FullScreen()
{
	this->input = this->copy;
	namedWindow("input", CV_WINDOW_AUTOSIZE);
	moveWindow("input", 0, 0);
	setWindowProperty("input", CV_WINDOW_FULLSCREEN, CV_WINDOW_FULLSCREEN);;
	HWND win_handle = FindWindow(0, L"input");
	if (!win_handle)
	{
		cout << "Failed FindWindow" << endl;
	}

	// Resize  
	unsigned int flags = (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
	flags &= ~SWP_NOSIZE;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = input.cols;
	unsigned int h = input.rows;
	SetWindowPos(win_handle, HWND_NOTOPMOST, x, y, w, h, flags);

	SetWindowLong(win_handle, GWL_STYLE, GetWindowLong(win_handle, GWL_EXSTYLE) | WS_EX_TOPMOST);
	ShowWindow(win_handle, SW_SHOW);
}
//���ǰ��
void CPictureSynthesis::AddFrontPhotos()
{
	string str1 = "../Outlook/test";
	stringstream stream;
	stream << count_F;
	string str2 = stream.str();
	string str3 = ".png";
	string frontPhotosPath = str1 + str2 + str3;

	Mat srcAlpha;
	// imread��һ������Ϊ-1��ʾ��ȡ����alphaͨ����ͼƬ
	srcAlpha = imread(frontPhotosPath, -1);
	// ��ʾ��dest�ĵ�(50,100)���ö�srcAlpha�ĵ���
	mapToMat(srcAlpha, this->copy, 40, 40);		
}
//����
void CPictureSynthesis::ScreenShot()
{
	if (this->screenShot)
	{
		//cout << "��Ŷin����" << endl;
		//�ȴ�2�����
		Sleep(2000);
		IplImage qImg;
		qImg = IplImage(screen); // cv::Mat -> IplImage
		cvSaveImage("../Screenshot/screenshot.jpg", &qImg);
		this->screenShot = false;
	}
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
		//CPictureSynthesis::AddFrontPhotos();
		//ȫ��
	//	CPictureSynthesis::FullScreen();	
		this->input = this->copy;
		screen = input;
		//����
		CPictureSynthesis::ScreenShot();
	}

	return  this->input;
}
Mat CPictureSynthesis::NextPictureSynthesize()
{
	string strB1 = "../Background/test";
	stringstream streamB;
	streamB << (count_B) % this->photoNumber;
	string strB2 = streamB.str();
	string strB3 = ".jpg";
	string nextBackgroundPath = strB1 + strB2 + strB3;
	Mat nextBackground = imread(nextBackgroundPath);

	string strF1 = "../Outlook/test";
	stringstream streamF;
	streamF << (count_F+1) % this->photoNumber;
	string strF2 = streamF.str();
	string strF3 = ".png";
	string nextFrontPhotosPath = strF1 + strF2 + strF3;
	Mat srcAlpha;
	srcAlpha = imread(nextFrontPhotosPath, -1);
	mapToMat(srcAlpha, nextBackground, 0, 0);

	return nextBackground;
}
Mat CPictureSynthesis::LastPictureSynthesize()
{
	string strB1 = "../Background/test";
	stringstream streamB;
	streamB << (count_B + this->photoNumber - 2) % this->photoNumber;
	string strB2 = streamB.str();
	string strB3 = ".jpg";
	string lastBackgroundPath = strB1 + strB2 + strB3;
	Mat lastBackground = imread(lastBackgroundPath);

	string strF1 = "../Outlook/test";
	stringstream streamF;
	streamF << (count_F + this->photoNumber - 1) % this->photoNumber;
	string strF2 = streamF.str();
	string strF3 = ".png";
	string lastFrontPhotosPath = strF1 + strF2 + strF3;
	Mat srcAlpha;
	srcAlpha = imread(lastFrontPhotosPath, -1);
	mapToMat(srcAlpha, lastBackground, 0, 0);

	return lastBackground;
}