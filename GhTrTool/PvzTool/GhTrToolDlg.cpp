
// GhTrToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GhTrTool.h"
#include "GhTrToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGhTrToolDlg 对话框



CGhTrToolDlg::CGhTrToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GhTrTool_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGhTrToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGhTrToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_SUN, &CGhTrToolDlg::OnBnClickedBtnSun)
    ON_BN_CLICKED(IDC_BTN_SUN_NOP, &CGhTrToolDlg::OnBnClickedBtnSunNop)
    ON_BN_CLICKED(IDC_BTN_NO_CD, &CGhTrToolDlg::OnBnClickedBtnNoCd)
    ON_BN_CLICKED(IDC_BTN_COIN, &CGhTrToolDlg::OnBnClickedBtnCoin)
    ON_BN_CLICKED(IDC_BTN_BUILD, &CGhTrToolDlg::OnBnClickedBtnBuild)
    ON_BN_CLICKED(IDC_BTN_AUTO, &CGhTrToolDlg::OnBnClickedBtnAuto)
    ON_BN_CLICKED(IDC_BTN_CARD, &CGhTrToolDlg::OnBnClickedBtnCard)
    ON_BN_CLICKED(IDC_BTN_MORE, &CGhTrToolDlg::OnBnClickedBtnMore)
    ON_BN_CLICKED(IDC_BTN_ALL_SCREEN, &CGhTrToolDlg::OnBnClickedBtnAllScreen)
    ON_BN_CLICKED(IDC_BTN_UN_ALL_SCREEN, &CGhTrToolDlg::OnBnClickedBtnUnAllScreen)
    ON_BN_CLICKED(IDC_BTN_COOL, &CGhTrToolDlg::OnBnClickedBtnCool)
    ON_BN_CLICKED(IDC_BTN_STOP, &CGhTrToolDlg::OnBnClickedBtnStop)
    ON_BN_CLICKED(IDC_BTN_DEAD, &CGhTrToolDlg::OnBnClickedBtnDead)
    ON_BN_CLICKED(IDC_BTN_Y, &CGhTrToolDlg::OnBnClickedBtnY)
END_MESSAGE_MAP()


// CGhTrToolDlg 消息处理程序

BOOL CGhTrToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGhTrToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGhTrToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGhTrToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGhTrToolDlg::OnBnClickedBtnSun()
{
    DWORD dwSun = GetDlgItemInt(IDC_EDIT_SUN);

    CPvz pvz = CPvz();
    pvz.ModifySunValue(dwSun);
}


void CGhTrToolDlg::OnBnClickedBtnSunNop()
{
    CPvz pvz = CPvz();
    pvz.SunNop();
}


void CGhTrToolDlg::OnBnClickedBtnNoCd()
{
    CPvz pvz = CPvz();
    pvz.NoCd();
}


void CGhTrToolDlg::OnBnClickedBtnCoin()
{
    DWORD dwCoin = GetDlgItemInt(IDC_EDIT_COIN);

    CPvz pvz = CPvz();
    pvz.ModifyCoinValue(dwCoin);
}


void CGhTrToolDlg::OnBnClickedBtnBuild()
{
    CPvz pvz = CPvz();
    pvz.Build();
}


void CGhTrToolDlg::OnBnClickedBtnAuto()
{
    CPvz pvz = CPvz();
    pvz.Auto();
}


void CGhTrToolDlg::OnBnClickedBtnCard()
{
    CPvz pvz = CPvz();
    pvz.Card();
}


void CGhTrToolDlg::OnBnClickedBtnMore()
{
    CPvz pvz = CPvz();
    pvz.More();
}


void CGhTrToolDlg::OnBnClickedBtnAllScreen()
{
    CPvz pvz = CPvz();
    pvz.AllScreen();
}


void CGhTrToolDlg::OnBnClickedBtnUnAllScreen()
{
    CPvz pvz = CPvz();
    pvz.UnAllScreen();
}


void CGhTrToolDlg::OnBnClickedBtnCool()
{
    CPvz pvz = CPvz();
    pvz.Cool();
}


void CGhTrToolDlg::OnBnClickedBtnStop()
{
    CPvz pvz = CPvz();
    pvz.Stop();
}


void CGhTrToolDlg::OnBnClickedBtnDead()
{
    CPvz pvz = CPvz();
    pvz.Dead();
}


void CGhTrToolDlg::OnBnClickedBtnY()
{
    CPvz pvz = CPvz();
    pvz.Attract();
}
