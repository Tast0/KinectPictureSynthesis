#pragma once
#include "CvvImage.h"
#include "showpicture.h" //��ӡģʽ���������ڵ�ͷ�ļ�
//#include "picture8Dlg.h"
#include "Choose.h"  //�Ǵ�ӡģʽ�µ��������ڵ�ͷ�ļ�

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Kinect.h>
#include "GestureControl.h"
#include "PictureSynthesis.h"
#include "MouseControl.h"
#include "include.h"
using namespace cv;


// Dlg2 �Ի���

class Dlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg2)

public:
	Dlg2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dlg2();

// �Ի�������
	enum { IDD = IDD_SECOND_DEGREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
//	virtual void PreInitDialog();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CString strFilter; //��ӡ·��
	
	BOOL wetherChoose;
	BOOL doShutScreen;//�ж��Ƿ���Ҫ����
	
	BOOL wetherPrint;   //�����������ڱ���Լ��Ƿ�Ϊ��ӡģʽ����ֵ��һ������ָ��
	int count;   //��fuyuanbin()��������ڶ���jpgͼƬ�Բ���Mat�ӿڣ�����fuyuanbin()��������kinect�ĺ���ȡ��
	void drawPicture(void);  
	void ShowImage(IplImage* TheImage, UINT myId);   //����������ʾͼƬ�ĺ���
//	void ResizeImage(IplImage* img);
	Mat fuyuanbin(int order);
	int dialogWidth;
	int dialogHeight;
	CRect m_rect;



	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	Dlg2(BOOL firstIsPrint);
	void ShutScreen(void);
	void showChoose(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(UINT nID, int xlabel, int ylabel);

	void ShowInSecondDialog(Mat total, Mat navi1, Mat navi2, Mat navi3);
	void getOut(void);
	void allScreen(void);

public:
	IKinectSensor *kinect = NULL;
	CGestureControl gC;
	CPictureSynthesis pS;
	int numberScenarios;
};
