#include "GestureControl.h"
#include "PictureSynthesis.h"
#include "MouseControl.h"
#include <iostream>
#include "include.h"

using namespace std;

void main(void)
{
	IKinectSensor *kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  �õ�Ĭ�ϵ��豸  

	if (FAILED(hr) || kinect == NULL)
	{
		cout << "���� sensor ʧ��\n";
		return;
	}
	if (kinect->Open() != S_OK) // �Ƿ�򿪳ɹ�  
	{
		cout << "Kinect sensor û׼����\n";
		return;
	}

	CGestureControl gC;
	CPictureSynthesis pS;
	CMouseControl mC;

	gC.Open(kinect);
	mC.Open(kinect);
	pS.Open(kinect);

	while (true)
	{
		//mC.KinectMouseControl();
		gC.GestureRecognition();
		pS.changeBg = gC.isChangeBg;
		pS.screenShot = gC.isScreenshot;

		Mat input = pS.PictureSynthesis();
		Mat show;
		int  cx = GetSystemMetrics(SM_CXSCREEN);
		int  cy = GetSystemMetrics(SM_CYSCREEN);
		resize(input, show, Size(cx, cy));
		imshow("input", show);
		waitKey(33);
	}
	

	kinect->Close();  // ���ŵĹر�kinect
	cout << "kinect�ر�\n";
}