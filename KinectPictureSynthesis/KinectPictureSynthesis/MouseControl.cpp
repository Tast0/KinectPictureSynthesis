#include "MouseControl.h"
#include "include.h"

CMouseControl::CMouseControl()
{
	this->kinect = nullptr;
	this->bady = nullptr;
	this->pBodyReader = nullptr;
	this->pBodyFrame = nullptr;
	this->pBody[BODY_COUNT] = { 0 };
	this->pSingleBody = nullptr;
}

CMouseControl::~CMouseControl()
{
}
//��������Ƿ���ȭ
bool CMouseControl::RightHandClose()
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
//������ֱ��Ƿ���ǰ�ƽ�
bool CMouseControl::IsRightArmSpread()
{
	const double rightArmSpread = 0.30;//�ֱ���ǰ�ƽ��ľ���
	Joint joints[JointType_Count];
	// ��ȡ������Ϣ�ڵ�  
	pSingleBody->GetJoints(JointType::JointType_Count, joints);
	float rightHandZ = joints[JointType_HandRight].Position.Z;
	float spineBaseZ = joints[JointType::JointType_SpineBase].Position.Z;

	if (fabs(rightHandZ - spineBaseZ) > rightArmSpread)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool CMouseControl::Open(IKinectSensor* kinect)
{
	this->kinect = kinect;
	HRESULT hr = S_OK;
	kinect->get_BodyFrameSource(&bady);
	hr = bady->OpenReader(&pBodyReader); // �򿪻�ȡ������Ϣ��  Reader  
	if (FAILED(hr))
	{
		//std::cerr << "Error : IBodyFrameSource::OpenReader()" << std::endl;
		return false;
	}
	return true;
}
void CMouseControl::KinectMouseControl()
{
	
	HRESULT hr = S_OK;	
	//cout << "��⿪ʼ" << endl;
	if(true)
	{
		
		hr = pBodyReader->AcquireLatestFrame(&pBodyFrame);

		if (SUCCEEDED(hr))
		{
			
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
				pSingleBody = pBody[i];
				if (pSingleBody)
				{
					Joint joints[JointType_Count];
					//��ȡ������Ϣ�ڵ�  
					pSingleBody->GetJoints(JointType::JointType_Count, joints);

					// �õ����ֹ���λ��
					CameraSpacePoint handLeft = joints[JointType_HandLeft].Position;
					CameraSpacePoint handRight = joints[JointType_HandRight].Position;
					CameraSpacePoint spineBase = joints[JointType_SpineBase].Position;
					// ���������ǰ�ƽ�
					if (IsRightArmSpread())
					{
						//������������x�����ǲ�ʹ���Ҽ���Ϊ�ο���
						//��Ϊ�����Ҽ�ͨ�����������ĺ��棬�����λ�����ƶϺͲ��ȶ��ġ�
						//��Ϊ��׵�ײ������ֵ���ߣ����Ǽ���0.05f��ʹ�����ӽ��ұߡ�
						float x = handRight.X - spineBase.X + 0.05f;
						//����������㡣���ļ����ײ����ҷ�Ҫ�ͣ����Ǽ���0.51f��������Ϊhiger��
						//ֵ0.51��ͨ����β���������ģ�����԰�������Ϊ��ϲ������һ����
						float y = spineBase.Y - handRight.Y + 0.51f;
						//��ȡ��Ļ��С
						int  screenWidth = GetSystemMetrics(SM_CXSCREEN);
						int  screenHeight = GetSystemMetrics(SM_CYSCREEN);
						// �õ���ǰ���λ��
						POINT p;
						GetCursorPos(&p);//��ȡ�������  
						//ʹ�õ�ƽ����Ӧ����0-0.95f�����ǵ��㷨��:oldPos + (newPos - oldPos) * smoothValue
						float smoothing = 1 - 0.8f;
						// ���ù��λ��
						SetCursorPos((int)(p.x + (x  * 1.8f * screenWidth - p.x) * smoothing), (int)(p.y + ((y + 0.25f) * 1.8f * screenHeight - p.y) * smoothing));

					}
					if (IsRightArmSpread() && RightHandClose())
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
						Sleep(10);//Ҫ����ĳЩӦ�õķ�Ӧʱ��   
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}

				}

			}//for
			for (int count = 0; count < BODY_COUNT; count++)
			{
				SafeRelease(pBody[count]);
			}
			SafeRelease(pBodyFrame);  // �������ͷ� 
		}//if	

	}

	return;
}