#pragma once


// showpicture �Ի���

class showpicture : public CDialogEx
{
	DECLARE_DYNAMIC(showpicture)

public:
	showpicture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~showpicture();

// �Ի�������
	enum { IDD = IDD_SHUTSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void showShutPicture(CImage img);
	virtual BOOL OnInitDialog();
	CImage globalImage;
	
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
