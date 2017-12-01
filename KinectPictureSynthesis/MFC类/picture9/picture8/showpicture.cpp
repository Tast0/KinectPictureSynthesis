// showpicture.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "picture8.h"
#include "showpicture.h"
#include "afxdialogex.h"


// showpicture �Ի���

IMPLEMENT_DYNAMIC(showpicture, CDialogEx)

showpicture::showpicture(CWnd* pParent /*=NULL*/)
	: CDialogEx(showpicture::IDD, pParent)
{
	
}

showpicture::~showpicture()
{
}

void showpicture::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(showpicture, CDialogEx)
	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// showpicture ��Ϣ�������


void showpicture::showShutPicture(CImage img)
{
	
	CRect rect;//���������   
				GetDlgItem(IDC_SHUTPICTURE)->GetWindowRect(&rect);//�����ھ���ѡ�е�picture�ؼ���   
				ScreenToClient(&rect);//���ͻ���ѡ�е�Picture�ؼ���ʾ�ľ���������   
				  
				GetDlgItem(IDC_SHUTPICTURE)->MoveWindow(rect);//�������ƶ���Picture�ؼ���ʾ�ľ�������   
  
				CWnd *pWnd=GetDlgItem(IDC_SHUTPICTURE);//���pictrue�ؼ����ڵľ��   
  
				pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������   
  
				CDC *pDC=pWnd->GetDC();//���pictrue�ؼ���DC   
				
				CImage imgs;
				//imgs.Load(L"D:\\screenpicture.jpg");
				img.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  

				time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
				tm* t = localtime(&tt);
				CString strTime;
				strTime.Format(_T("%02d%02d%02d"), t->tm_hour, t->tm_min, t->tm_sec);
				CString screenshotPath;
				screenshotPath = _T("../Screenshot/") + strTime + _T(".jpg");

				img.Save(screenshotPath);
				ReleaseDC(pDC);//�ͷ�picture�ؼ���DC

				Sleep(3000);

				//���Ӵ�ӡ��������Ƭ��ӡ
				char szprinter[80];
				char* szDevice, *szDriver, *szOutput;
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
							Escape(hdcprint, STARTDOC, 8, (LPCSTR)(LPCTSTR)screenshotPath, NULL);
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
					MessageBox(_T("û��Ĭ�ϴ�ӡ��,����û�а�װ��ӡ��!"), _T("OK"), MB_OK | MB_SYSTEMMODAL);

					CDialogEx::OnCancel();
					return;
				}

				CDialogEx::OnCancel();
}


BOOL showpicture::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect cr;  
    GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
    ClientToScreen(&cr);//ת��ΪӫĻ����  
 
    //showShutPicture(globalImage);
	//SetTimer(1,3000,NULL);
	int xx = GetSystemMetrics(SM_CXSCREEN);
	int yy = GetSystemMetrics(SM_CYSCREEN);
	
	SetWindowPos(NULL,xx/3,yy/3,0,0,SWP_NOZORDER | SWP_NOSIZE );
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





void showpicture::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

		showShutPicture(globalImage);
	
		
	CDialogEx::OnTimer(nIDEvent);
}
