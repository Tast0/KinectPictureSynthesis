// Dlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "picture8.h"
#include "Dlg2.h"
#include "afxdialogex.h"
#include "picture8Dlg.h"

#include <Kinect.h>
#include "GestureControl.h"
#include "PictureSynthesis.h"
#include "MouseControl.h"
#include "include.h"

using namespace cv;


// Dlg2 �Ի���

IMPLEMENT_DYNAMIC(Dlg2, CDialogEx)

Dlg2::Dlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dlg2::IDD, pParent)
{

}
Dlg2::Dlg2(BOOL firstIsPrint)
{
	wetherPrint = firstIsPrint;
}


Dlg2::~Dlg2()
{
}

void Dlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dlg2, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &Dlg2::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL Dlg2::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ��kinect
	kinect = NULL;
	HRESULT hr = S_OK;
	hr = GetDefaultKinectSensor(&kinect);  //  �õ�Ĭ�ϵ��豸  

	if (FAILED(hr) || kinect == NULL)
	{
		//cout << "���� sensor ʧ��\n";
		//return;
	}
	if (kinect->Open() != S_OK) // �Ƿ�򿪳ɹ�  
	{
		//cout << "Kinect sensor û׼����\n";
		//return;
	}

	this->gC.Open(kinect);
	this->pS.Open(kinect);
	this->pS.photoNumber = this->numberScenarios;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��'
	strFilter = L"D:\\screenpicture.jpg";  //��ʼ��strFilter
	GetClientRect(&m_rect);
	
	/*�ڴ�ӡģʽ�£����������д�ӡ�Ͳ���ӡ��ť���������ں��û����������������ť��Ontimer�ľͱ��ٴε����ˣ���ɲ��ϵص����������ڵĺ��
	��wetherChoose�������ڱ���û��Ƿ�����ť��wetherChoose����Ϊfalse,��������������ʱ��wetherChoose��Ϊtrue����������������ϵİ�ť��wetherChoose��λfalse.*/
	wetherChoose = false;  
	doShutScreen = true;  //���ڱ���Ƿ����
	//wetherPrint = false;    //���ڱ���Ƿ��ӡ
	
	count = 0;

	 //������������Ϊȫ��
	allScreen();

	
	SetTimer(1,33,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void Dlg2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void Dlg2::drawPicture(void)
{
	
	
	gC.GestureRecognition();
	pS.changeBg = gC.isChangeBg;
	//��������ʾͼƬ
	Mat test = this->pS.PictureSynthesis();   //********************Mat�ӿ�***********************************
	//�ĸ�����ͼƬ�Ľ��   fuyuanbin(0)
	Mat navigate1 = this->pS.LastPictureSynthesize(); //��ǰ��ʾ��ͼƬǰ���ǰ���ͼƬ
	Mat naivgate2 = this->pS.NowPictureSynthesize(); //��ǰ��ʾ��ͼƬǰ���ͼƬ
	Mat navigate3 = this->pS.NextPictureSynthesize();//��ǰ��ʾ��ͼƬ�����ͼƬ
	
	
	//�ڶ���������ʾͼƬ��������ǰͼƬ�͵������������ͼƬ
	ShowInSecondDialog(test,navigate1,naivgate2,navigate3);

	doShutScreen = this->gC.isScreenshot;
	if(doShutScreen)  //doShutScreen�ǲ������������ڱ���Ƿ����
	{
		SetTimer(2, 800, nullptr);
	}

	if (this->gC.isStop)
	{
		this->getOut();
	}

	doShutScreen = false;
	
	
	
}

//��mIDΪmyId��ͼƬ�ؼ�����ʾIplImage*���͵�TheImage
void Dlg2::ShowImage(IplImage* TheImage, UINT myId)
{
	 CDC* pDC=GetDlgItem(myId)->GetDC();
	 HDC hDC=pDC->GetSafeHdc();
	 CRect rect;

	 GetDlgItem(myId)->GetClientRect(&rect);

	 SetRect(rect,rect.left,rect.top,rect.right,rect.bottom);

	 CvvImage cimg;
	 cimg.CopyOf(TheImage);
	 cimg.DrawToHDC(hDC,&rect);
	 ReleaseDC(pDC);
}

//void Dlg2::ResizeImage(IplImage* img)
//{
//	//cvSetImageROI( TheImage, cvRect( 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT));// ��ͼƬ img �������ţ������뵽 TheImage �У��൱������������
//	//cvResize( img, TheImage );// ���� TheImage �� ROI ׼��������һ��ͼƬ
//	//cvResetImageROI( TheImage );
//}


Mat Dlg2::fuyuanbin(int order)
{
	char number[6];  //ȷ��·����
	sprintf(number,"%d",count);
	strcat(number,".jpg");
	
	Mat temp = imread(number,1);
	count = (count + 1) % 7;

	
	return temp;
}


void Dlg2::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		if (!wetherChoose)  //��������������е���ˡ���ӡ���򡰲���ӡ���İ�ť����wetherChoose������Ϊfalse
		{
			this->EnableWindow(true); //�������ڽ���������Ϣ����ֻ�������������е����ť�������������ں󣬶������ڲ��ܽ���������Ϣ
			this->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  //��������������Ϊ����ǰ����ʾ
			drawPicture();
		}
		break;
	case 2:
		if (wetherPrint)
		{
			showChoose();  //showChoose������ʾ������ͬʱ�����û�ѡ���ӡ���ǲ���ӡ
		}
		else
		{
			//ֻ����Ƭ��3��ر�
			ShutScreen();  //ShutSceen()�������ڽ���
		}
		KillTimer(2);        //�رն�ʱ��1
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void Dlg2::OnBnClickedCancel()
{
	
	
	
}


void Dlg2::ShutScreen(void)
{
	

	CRect rcPlot;  
			GetDlgItem(IDC_PHOTO)->GetWindowRect(&rcPlot); //��ȡIDC_PHOTO��������Ļ�е�����λ��,�Ȼ�ȡICD_PHOTO��ָ�룬�ٸ���ָ���ȡIDC_PHOTO�ؼ��Ĵ�С������rcPlot  
			ScreenToClient(&rcPlot);//��ȡIDC_PHOTO�ڸ������е�λ�� ,ScreenToClient()�ǽ���Ļ����ת��Ϊ�ͻ������� 
      
			HDC hDC= ::GetWindowDC(GetDlgItem(IDC_PHOTO)->GetSafeHwnd());  
			HDC hDCMem= ::CreateCompatibleDC(hDC);// ��������Ļ���ݵ�bitmap   
      
			HBITMAP hBitmap= ::CreateCompatibleBitmap(hDC,rcPlot.Width(),rcPlot.Height());  //rcPlot�ǿͻ���������
			HBITMAP hOldBitmap= (HBITMAP)::SelectObject(hDCMem,hBitmap);////��memBitmapѡ���ڴ�DC   
      
			::BitBlt(hDCMem,0,0,rcPlot.Width(),rcPlot.Height(),hDC,0,0,SRCCOPY);// ����߶ȿ��   
			::SelectObject(hDCMem,hOldBitmap);  
			CImage myImage;  
			myImage.Attach(hBitmap); //��λͼ����ͼƬ����  
			
			/*******�����������Ի���****************/
			this->EnableWindow(false);  //��Ϊ��������ʱģ̬�Ի������Ե����������ڵ�ǰ��������������Ϊ������Ӧ��Ϣ
			showpicture *showing = new showpicture();
			showing->Create(IDD_SHUTSCREEN); 
			showing->ShowWindow(SW_SHOW); 
			showing->showShutPicture(myImage);  //�����������ڵ���ʾ��ͼ����
			
			

			
}


void Dlg2::showChoose(void)
{
	CRect rcPlot;  
			GetDlgItem(IDC_PHOTO)->GetWindowRect(&rcPlot); //��ȡIDC_PHOTO��������Ļ�е�����λ��,�Ȼ�ȡICD_PHOTO��ָ�룬�ٸ���ָ���ȡIDC_PHOTO�ؼ��Ĵ�С������rcPlot  
			ScreenToClient(&rcPlot);//��ȡIDC_PHOTO�ڸ������е�λ�� ,ScreenToClient()�ǽ���Ļ����ת��Ϊ�ͻ������� 
      
			HDC hDC= ::GetWindowDC(GetDlgItem(IDC_PHOTO)->GetSafeHwnd());  
			HDC hDCMem= ::CreateCompatibleDC(hDC);// ��������Ļ���ݵ�bitmap   
      
			HBITMAP hBitmap= ::CreateCompatibleBitmap(hDC,rcPlot.Width(),rcPlot.Height());  //rcPlot�ǿͻ���������
			HBITMAP hOldBitmap= (HBITMAP)::SelectObject(hDCMem,hBitmap);////��memBitmapѡ���ڴ�DC   
      
			::BitBlt(hDCMem,0,0,rcPlot.Width(),rcPlot.Height(),hDC,0,0,SRCCOPY);// ����߶ȿ��   
			::SelectObject(hDCMem,hOldBitmap);  
			CImage myImage;  
			myImage.Attach(hBitmap); //��λͼ����ͼƬ����  
			
			/*******�����������Ի���****************/
			this->EnableWindow(false);  //��Ϊ��������ʱģ̬�Ի������Ե����������ڵ�ǰ��������������Ϊ������Ӧ��Ϣ
			Choose *choosing = new Choose();
			choosing->Create(IDD_CHOOSEPRINT); 
			choosing->ShowWindow(SW_SHOW); 
			choosing->showShutScreen(myImage,&wetherChoose);  //�����������ڵ���ʾ��������ʾ��ͼ�������벼������wetherChoose�ĵ�ַ���������������е����ťʱ���ı�wetherChoose��ֵ��wetherChoose�ڼ�ʱ��OnTimer9)�������ж϶��������Ƿ�ʼ������Ӧ��Ϣ
			
			
}


void Dlg2::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(nType!=SIZE_MINIMIZED )  //�жϴ����ǲ�����С���ˣ���Ϊ������С��֮�� �����ڵĳ��Ϳ����0����ǰһ�α仯��ʱ�ͻ���ֳ���0�Ĵ������
          {
                  ChangeSize(IDC_PHOTO,cx,cy);  //��ÿһ���ؼ��Ĵ�С����������
				  ChangeSize(IDC_NAVIGATE1,cx,cy);
				  ChangeSize(IDC_NAVIGATE2,cx,cy);
				  ChangeSize(IDC_NAVIGATE3,cx,cy);
				 // ChangeSize(IDCANCEL,cx,cy);

					CRect sizeOfPicture;
					CWnd *pWnd = GetDlgItem(IDC_PHOTO);//IDC_pictureΪpicture�ؼ�ID
					if(pWnd!=NULL)
					{
						pWnd->GetWindowRect(&sizeOfPicture);//sizeOfPictureΪ����ʾ����ͼƬ�ؼ��Ĵ�С��
						int x=sizeOfPicture.right;
						int y=sizeOfPicture.bottom;

						GetDlgItem(IDC_NAVIGATE1)->SetWindowPos(NULL,0,y,0,0,SWP_NOZORDER | SWP_NOSIZE );  //�ı䵼���������ͼƬ��λ�ã�����Ӧȫ����Ķ�������

						CRect sizeOfnavigate;    //sizeOfnavigateΪ������ÿ��ͼƬ�ؼ��Ĵ�С
						GetDlgItem(IDC_NAVIGATE1)->GetClientRect(&sizeOfnavigate);
						int widthOfNavigate = sizeOfnavigate.right;

						GetDlgItem(IDC_NAVIGATE2)->SetWindowPos(NULL,(x-widthOfNavigate)/2,y,0,0,SWP_NOZORDER | SWP_NOSIZE );  //�ı䵼�����м�ͼƬ��λ�ã�����Ӧȫ����Ķ�������
						GetDlgItem(IDC_NAVIGATE3)->SetWindowPos(NULL,x-widthOfNavigate,y,0,0,SWP_NOZORDER | SWP_NOSIZE );   //�ı䵼�������ұ�ͼƬ��λ�ã�����Ӧȫ����Ķ�������
						//GetDlgItem(IDCANCEL)->SetWindowPos(NULL,3*widthOfNavigate,y,0,0,SWP_NOZORDER | SWP_NOSIZE );   //�ı䵼������ȡ����ť��λ�ã�����Ӧȫ����Ķ�������
						
					}
                  GetClientRect(&m_rect);   //���Ҫ���¶Ի���Ĵ�С��������һ�α仯�ľ����ꣻ
         }
}


void Dlg2::ChangeSize(UINT nID, int xlabel, int ylabel)
{
	CWnd *pWnd;
    pWnd=GetDlgItem(nID); 
    if(pWnd!=NULL)     //�ж��Ƿ�Ϊ�գ���Ϊ�ڴ��ڴ�����ʱ��Ҳ�����OnSize���������Ǵ�ʱ�����ؼ���û�д�����PwndΪ��
    {
            CRect rec;
            pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
            ScreenToClient(&rec);   //���ؼ���Сװ��λ�ڶԻ����е���������
            rec.left=rec.left*xlabel/m_rect.Width();  //���ձ��������ռ����λ��
            rec.top=rec.top*ylabel/m_rect.Height();
            rec.bottom=rec.bottom*ylabel/m_rect.Height();
            rec.right=rec.right*xlabel/m_rect.Width();
            pWnd->MoveWindow(rec);   //�����ؼ�
      }
}







void Dlg2::ShowInSecondDialog(Mat total, Mat navi1, Mat navi2, Mat navi3)
{
	//��Mat����ת��ΪIplImage����
	IplImage myipl(total); 
	IplImage myipl1(navi1);
	IplImage myipl2(navi2);
	IplImage myipl3(navi3);
	//IplImage myipl4(navi4);

	IplImage *ipl = &myipl;
	IplImage *ipl1 = &myipl1;
	IplImage *ipl2 = &myipl2;
	IplImage *ipl3 = &myipl3;
	//IplImage *ipl4 = &myipl4;
	

	
	if(!ipl||!ipl1||!ipl2||!ipl3)return;
	//����������ʾͼƬ
	ShowImage( ipl, IDC_PHOTO );// ���ÿؼ���ʾͼƬ,����iplImage*��UNIT���͵Ĳ�������UNIT�ؼ�����ʾͼƬ��iplIPmage���;�����ʾʲôͼƬ����������Ҫ������С��UNIT�ؼ��Ĵ�Сһ��
	ShowImage( ipl1, IDC_NAVIGATE1 );
	ShowImage( ipl2, IDC_NAVIGATE2 );
	ShowImage( ipl3, IDC_NAVIGATE3 );
	//ShowImage( ipl4, IDC_NAVIGATE4 );
	//�ͷ��ڴ�
	ipl = NULL;
	ipl1 = NULL;
	ipl2 = NULL;
	ipl3 = NULL;
	//ipl4 = NULL;
}

//ȡ����ť����Ӧ��������õĺ������������ٶ�������
void Dlg2::getOut(void)
{
	
	CDialogEx::OnCancel();
}


void Dlg2::allScreen(void)
{
	dialogWidth = GetSystemMetrics(SM_CXSCREEN);
	dialogHeight = GetSystemMetrics(SM_CYSCREEN);
	CRect rcTemp; 
	rcTemp.BottomRight() = CPoint(dialogWidth, dialogHeight); 
	rcTemp.TopLeft() = CPoint(0, 0); 
	MoveWindow(&rcTemp);
}
