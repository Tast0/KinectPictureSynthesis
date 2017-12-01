#include "stdafx.h"
#include "GestureControl.h"
#include "include.h"
#include <Kinect.h>

CGestureControl::CGestureControl()
{
	this->kinect = nullptr;
	this->bady = nullptr;  
	this->pBodyReader = nullptr;  
	this->pBodyFrame = nullptr; 
//	this->pBody[BODY_COUNT] = { 0 }; 
	this->pSingleBody = nullptr;  
	//Joint joints[JointType_Count];
	this->isChangeBg = false;
	this->isScreenshot = false;
	this->isStop = false;
}

CGestureControl::~CGestureControl()
{
}
bool CGestureControl::LeftHandClose()
{
	HandState leftHandState = HandState_Unknown;
	//��ȡ�ֲ�״̬
	pSingleBody->get_HandLeftState(&leftHandState);
	if (HandState_Closed == leftHandState)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//��������Ƿ���ȭ
bool CGestureControl::RightHandClose()
{
	HandState rightHandState = HandState_Unknown;
	//��ȡ�ֲ�״̬
	pSingleBody->get_HandRightState(&rightHandState);
	if (HandState_Closed == rightHandState)
	{
		//cout << "������ȭ\n";
		return true;
	}
	else
	{
		//cout << "�����ſ�\n";
		return false;
	}
}
bool CGestureControl::RightHandLasso()
{
	HandState rightHandState = HandState_Unknown;
	//��ȡ�ֲ�״̬
	pSingleBody->get_HandRightState(&rightHandState);
	if (HandState_Lasso == rightHandState)
	{
		//cout << "��������\n";
		return true;
	}
	else
	{
		return false;
	}
}
//����ֱ��Ƿ�ˮƽչ��
bool CGestureControl::IsArmSpread(int hand, int shoulder)
{
	//�ֱ���չ���ľ���
	const double ArmStretched = 0.45;
	// ��ȡ������Ϣ�ڵ�  
	float handX = joints[hand].Position.X;
	float shoulderX = joints[shoulder].Position.X;

	if (fabs(handX - shoulderX) > ArmStretched)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//����ֲ���ͷ��֮��
bool CGestureControl::IsHandAboveTheHead(int hand, int head)
{
	const double handAboveTheHead = 0.04;//�ֲ���ͷ��֮��
	// ��ȡ������Ϣ�ڵ�  
	float handY = joints[hand].Position.Y;
	float headY = joints[head].Position.Y;
	
	if (handY - headY > handAboveTheHead)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//�򿪹�������ͷ
bool CGestureControl::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	HRESULT hr = S_OK;
	kinect->get_BodyFrameSource(&this->bady);
	hr = bady->OpenReader(&this->pBodyReader); // �򿪻�ȡ������Ϣ��  Reader  
	if (FAILED(hr))
	{
		//std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
		return false;
	}
	else
	{
		return true;
	}
}
//�������
void CGestureControl::GestureRecognition()
{
	this->isChangeBg = false;
	this->isScreenshot = false;
	this->isStop = false;
	HRESULT hr = S_OK;	
	hr = pBodyReader->AcquireLatestFrame(&this->pBodyFrame);
	if (SUCCEEDED(hr))
	{		
		IBody* pBody[BODY_COUNT] = { 0 };
		//���¹�������    
		hr = pBodyFrame->GetAndRefreshBodyData(BODY_COUNT, pBody); // ˢ�¹�����Ϣ��6����  
		if (SUCCEEDED(hr))
		{
			BOOLEAN bTracked = false;

			for (int i = 0; i < BODY_COUNT; i++)
			{
				hr = pBody[i]->get_IsTracked(&bTracked); // ����Ƿ�׷��  
			}
		}
		for (int i = 0; i < BODY_COUNT; i++)
		{
			this->pSingleBody = pBody[i];
			if (this->pSingleBody)
			{
					
				//��ȡ������Ϣ�ڵ�  
				this->pSingleBody->GetJoints(JointType::JointType_Count, this->joints);

				bool isRightHandClose = this->RightHandClose();
				bool isLeftHandClose = this->LeftHandClose();
				bool isRightHandLasso = this->RightHandLasso();
				bool isRightArmSpread = IsArmSpread(JointType_HandRight, JointType_ShoulderRight);
				bool isRightHandAboveTheHead = this->IsHandAboveTheHead(JointType_HandRight, JointType_Head);
				bool isLeftHandAboveTheHead = this->IsHandAboveTheHead(JointType_HandLeft, JointType_Head);
				//����ȭ��չΪ��ת
				if (isRightArmSpread && isRightHandClose)
				{						
					//cout << "�����л�\n";
					this->isChangeBg = true;
				}
				if (isRightHandAboveTheHead && isRightHandLasso)
				{
					//cout << "����\n";
					this->isScreenshot = true;
				}
				if (isLeftHandAboveTheHead && isLeftHandClose)
				{
					//cout << "ֹͣ\n";
					this->isStop = true;
				}
			}

		}//for
		for (int count = 0; count < BODY_COUNT; count++)
		{
			SafeRelease(pBody[count]);
		}
		SafeRelease(this->pBodyFrame);  // �������ͷ� 
	}//if	

	return;
}

