#pragma once

#include "include.h"

class CGestureControl
{
public:
	CGestureControl();
	~CGestureControl();

private:
	//kinect�豸
	IKinectSensor* kinect;
	// ��ȡԴ  
	IBodyFrameSource* bady;  
	//�����ܶ�ȡ
	IBodyFrameReader* pBodyReader;   
	//������
	IBodyFrame* pBodyFrame; 
	// Ĭ�ϵ��� 6 ������ ����ʼ�����еĹ�����Ϣ 
	IBody* pBody[BODY_COUNT];
	//��������
	IBody* pSingleBody;  
	Joint joints[JointType_Count];

private:
	//��������Ƿ���ȭ
	bool RightHandClose();
	//����ֱ��Ƿ�ˮƽչ��
	bool IsArmSpread(int hand, int shoulder);
	//����ֲ���ͷ��֮��
	bool IsHandAboveTheHead(int hand, int head);

public:
	//�ж��Ƿ��л�����һ������
	bool isChangeBg;
	//�ж��Ƿ����
	bool isScreenshot;
	//�ж��Ƿ�ֹͣ��������
	bool isStop;

public:
	//�򿪹���֡
	bool Open(IKinectSensor* kinect);
	//����ʶ��
	void GestureRecognition();
};

