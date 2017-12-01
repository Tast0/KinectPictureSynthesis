#pragma once

#include "include.h"

class CMouseControl
{
public:
	CMouseControl();
	~CMouseControl();
private:
	//kinect�豸
	IKinectSensor* kinect;
	// ��ȡԴ 
	IBodyFrameSource* bady;   
	IBodyFrameReader* pBodyReader;
	IBodyFrame* pBodyFrame;
	// Ĭ�ϵ��� 6 ������ ����ʼ�����еĹ�����Ϣ
	IBody* pBody[BODY_COUNT];  
	IBody* pSingleBody;

private:
	//�ж������Ƿ���ȭ
	bool RightHandClose();
	//�ж��ұ��Ƿ���ǰ�ƽ�
	bool IsRightArmSpread();
	
public:
	//�򿪹�������ͷ
	bool Open(IKinectSensor* kinect);
	//����������
	void KinectMouseControl();
};

