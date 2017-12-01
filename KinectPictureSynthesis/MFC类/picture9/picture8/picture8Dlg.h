
// picture8Dlg.h : ͷ�ļ�
//

#pragma once
#include "Dlg2.h" //���������ڵ�ͷ�ļ���������

#include <Kinect.h>
#include "MouseControl.h"
#include "include.h"
// Cpicture8Dlg �Ի���
class Cpicture8Dlg : public CDialogEx
{
// ����
public:
	Cpicture8Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	int isPrint;   //����Ƿ�����ӡģʽ
	//Dlg2 *myDlg;   //�����������ڵĶ���
	//Dlg2 myDlg; 
	 

// �Ի�������
	enum { IDD = IDD_PICTURE8_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
//	void doDrawPicture(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	IKinectSensor *kinect = NULL;
	CMouseControl mC;
	int m_nNumberScenarios;
};
