// Choose.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "picture8.h"
#include "picture8Dlg.h"
#include "Choose.h"
#include "afxdialogex.h"




// Choose �Ի���

IMPLEMENT_DYNAMIC(Choose, CDialogEx)

Choose::Choose(CWnd* pParent /*=NULL*/)
	: CDialogEx(Choose::IDD, pParent)
{

}

Choose::~Choose()
{
}

void Choose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Choose, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Choose::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &Choose::OnBnClickedCancel)
END_MESSAGE_MAP()


// Choose ��Ϣ�������


void Choose::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	printOrNot = true;
	this->ShowWindow(SW_HIDE);
	if(changed!=NULL)
		*changed = false;
	//changed = NULL;

	//���Ӵ�ӡ��������Ƭ��ӡ
	char szprinter[80];
	char* szDevice, * szDriver, * szOutput;
	HDC hdcprint; // ����һ���豸�������
	//����һ����ӡ��ҵ
	static DOCINFO di = { sizeof(DOCINFO), _T("printer"), NULL };
	// �õ��豸�ַ�����������szprinter��
	GetProfileString((LPTSTR)(LPCTSTR)"windows", (LPTSTR)(LPCTSTR)"device", (LPTSTR)(LPCTSTR)",,,", (LPTSTR)(LPCTSTR)szprinter, 80);
	// ���豸�ַ����ֽ�
	if (NULL != (szDevice = strtok(szprinter, ",")) && NULL != (szDriver = strtok(NULL, ",")) &&
		NULL != (szOutput = strtok(NULL, ",")))
	{
		// ����һ����ӡ���豸���	
		if ((hdcprint = CreateDC((LPWSTR)szDriver, (LPWSTR)szDevice, (LPWSTR)szOutput, NULL)) != 0)
		{
			if (StartDoc(hdcprint, &di) > 0) //��ʼִ��һ����ӡ��ҵ
			{
				StartPage(hdcprint); //��ӡ����ֽ,��ʼ��ӡ
				SaveDC(hdcprint); //�����ӡ���豸���
				// ���һ������
				Escape(hdcprint, STARTDOC, 8, (LPCSTR)(LPCTSTR)this->printPath, NULL);
				RestoreDC(hdcprint, -1); //�ָ���ӡ���豸���
				EndPage(hdcprint); //��ӡ��ֽͣ,ֹͣ��ӡ
				EndDoc(hdcprint); //����һ����ӡ��ҵ
				MessageBox(_T("��ӡ���!"), _T("��ʾ"), MB_OK | MB_SYSTEMMODAL);
			}
			// ��API����DeleteDC����һ����ӡ���豸��� 
			DeleteDC(hdcprint);

			CDialogEx::OnCancel();
		}
	}
	else
	{
		MessageBox( _T("û��Ĭ�ϴ�ӡ��,����û�а�װ��ӡ��!"), _T("OK"), MB_OK | MB_SYSTEMMODAL);

		CDialogEx::OnCancel();
		return;
	}

	CDialogEx::OnCancel();
}


BOOL Choose::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect cr;  
    GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
    ClientToScreen(&cr);//ת��ΪӫĻ���� 
	
	printOrNot = false;
	changed = NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void Choose::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	printOrNot = true;
	this->ShowWindow(SW_HIDE);
	if(changed!=NULL)
		*changed = false;
	//changed =NULL;
	CDialogEx::OnCancel();
}


//void Choose::showShutScreen2(CImage img)
//{
//	CRect rect;//���������   
//				
//				  
//				
//
//
//				GetDlgItem(IDC_CHOOSESCREEN)->GetWindowRect(&rect);//�����ھ���ѡ�е�picture�ؼ���   
//				ScreenToClient(&rect);//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������    
//
//
//				GetDlgItem(IDC_CHOOSESCREEN)->MoveWindow(rect);//�������ƶ���Picture�ؼ���ʾ�ľ�������   
//  
//				CWnd *pWnd=GetDlgItem(IDC_CHOOSESCREEN);//���pictrue�ؼ����ڵľ��   
//  
//				pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������   
//  
//				CDC *pDC=pWnd->GetDC();//���pictrue�ؼ���DC   
//				
//				img.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
//				img.Save(L"D:\\screenpicture.jpg");
//				ReleaseDC(pDC);//�ͷ�picture�ؼ���DC
//
//				
//}


void Choose::showShutScreen(CImage img,BOOL *weChoose)
{
	changed = weChoose;
	*changed = true;
	CRect rect;//���������   
				GetDlgItem(IDC_CHOOSESCREEN)->GetWindowRect(&rect);//�����ھ���ѡ�е�picture�ؼ���   
				ScreenToClient(&rect);//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������    


				GetDlgItem(IDC_CHOOSESCREEN)->MoveWindow(rect);//�������ƶ���Picture�ؼ���ʾ�ľ�������   
  
				CWnd *pWnd=GetDlgItem(IDC_CHOOSESCREEN);//���pictrue�ؼ����ڵľ��   
  
				pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������   
  
				CDC *pDC=pWnd->GetDC();//���pictrue�ؼ���DC   
				
				img.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  

				time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
				tm* t = localtime(&tt);
				CString strTime;
				strTime.Format(_T("%02d%02d%02d"), t->tm_hour, t->tm_min, t->tm_sec);
				CString screenshotPath;
				screenshotPath = _T("../Screenshot/") + strTime + _T(".jpg");

				this->printPath = screenshotPath;

				img.Save(screenshotPath);
				ReleaseDC(pDC);//�ͷ�picture�ؼ���DC
		
}
