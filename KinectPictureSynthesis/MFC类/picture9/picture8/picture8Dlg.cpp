
// picture8Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "picture8.h"
#include "picture8Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cpicture8Dlg �Ի���




Cpicture8Dlg::Cpicture8Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cpicture8Dlg::IDD, pParent)
	, m_nNumberScenarios(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cpicture8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nNumberScenarios);
}

BEGIN_MESSAGE_MAP(Cpicture8Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &Cpicture8Dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &Cpicture8Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Cpicture8Dlg ��Ϣ�������

BOOL Cpicture8Dlg::OnInitDialog()
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

	this->mC.Open(kinect);
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	isPrint = false;  //Ĭ��Ϊ�������ڲ��Ǵ�ӡģʽ
	//myDlg = new Dlg2();   //����һ����ģ̬�Ի���Ϊ��������
	//myDlg->Create(IDD_SECOND_DEGREE);  
	
	SetTimer(1, 33, nullptr);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cpicture8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cpicture8Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cpicture8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//һ������ѡ�����Ӧ��������ѡ���й�ѡ������ӡģʽ����isPrint��Ϊtrue
void Cpicture8Dlg::OnBnClickedCheck1()  
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	isPrint = !isPrint;
	//myDlg.wetherPrint = isPrint;  //���ö������ڶ����еĳ�Ա����wetherPrint��wetherPrint���ڶ������ڱ���Լ��Ƿ�Ϊ��ӡģʽ
}

//һ�������С�start����ť����Ӧ����
void Cpicture8Dlg::OnBnClickedButton1()  
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
		//myDlg.ShowWindow(SW_SHOW); //��ʾ��ģ̬�Ի��� 
		Dlg2 myDlg;
		myDlg.wetherPrint = isPrint;
		UpdateData();
		myDlg.numberScenarios = this->m_nNumberScenarios;
		UpdateData(false);
		myDlg.DoModal();
		//myDlg.wetherPrint = isPrint;
		//myDlg.SetTimer(1,33,NULL);
	
}


//void Cpicture8Dlg::doDrawPicture(void) 
//{
//	myDlg.drawPicture();
//}


void Cpicture8Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->mC.KinectMouseControl();

	CDialogEx::OnTimer(nIDEvent);
}
