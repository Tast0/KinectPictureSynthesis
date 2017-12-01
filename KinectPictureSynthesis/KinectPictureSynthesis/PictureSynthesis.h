#pragma once

#include "include.h"

class CPictureSynthesis
{
public:
	CPictureSynthesis();
	~CPictureSynthesis();
private:
	//kinect�豸
	IKinectSensor* kinect;
	//��ɫ����
	int colorHeight, colorWidth;
	IColorFrameSource* myColorSource;
	IColorFrameReader* myColorReader;
	IFrameDescription* myDescription;
	UINT    colorDataSize;
	DepthSpacePoint* output;
	IColorFrame* myColorFrame;
	//�������
	int depthHeight, depthWidth;
	IDepthFrameSource* myDepthSource;
	IDepthFrameReader* myDepthReader;
	UINT    depthDataSize;
	UINT16* depthData;
	IDepthFrame* myDepthframe;
	//���������������
	int bodyHeight, bodyWidth;
	IBodyIndexFrameSource* myBodyIndexSource;
	IBodyIndexFrameReader* myBodyIndexReader;
	UINT    bodyDataSize = 0;
	BYTE* bodyData;
	IBodyIndexFrame* myBodyIndexFrame;
	//����mapper
	ICoordinateMapper* myMaper;  
	//����ͼƬ����
	int count_B;  
	//ǰ��ͼƬ����
	int count_F;  

	Mat colorData;
	//���뱳��ͼ��
	Mat temp;
	//�޸Ĵ�С��ı���ͼ��
	Mat background;
	//����һ�ݱ�������ͼ����
	Mat copy;
	//����Ҫ�����ͼ��
	Mat input;
	//����ͼ��
	Mat screen;

public:
	//��Ƭ����
	int photoNumber;
	//�ж��Ƿ��л�����һ������
	bool changeBg;
	//�ж��Ƿ����
	bool screenShot;

private:
	//͸��ǰ�����
	void mapToMat(const cv::Mat &srcAlpha, cv::Mat &dest, int x, int y);
	//������ɫ֡
	bool OpenColor();
    //�������֡
	bool OpenDepth();
	//���������������֡
	bool OpenBodyIndex();
	//�л�����һ������
	void NextScene();
	//ȫ����
	void FullScreen();
	//���ǰ��
	void AddFrontPhotos();
	//��������
	void ScreenShot();

public:
	//�����־�ͷ���������
	bool Open(IKinectSensor* kinect);
	Mat PictureSynthesis();
	Mat NextPictureSynthesize();
	Mat LastPictureSynthesize();
};

