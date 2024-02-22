
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
	ON_BN_CLICKED(IDC_BTN_SP, &CGhTrToolDlg::OnBnClickedBtnSeedPacket)
	ON_BN_CLICKED(IDC_BTN_Plant, &CGhTrToolDlg::OnBnClickedBtnPlant)
    ON_BN_CLICKED(IDC_BTN_SUN_NOP, &CGhTrToolDlg::OnBnClickedBtnSunNop)
    ON_BN_CLICKED(IDC_BTN_NO_CD, &CGhTrToolDlg::OnBnClickedBtnNoCd)
    ON_BN_CLICKED(IDC_BTN_BGId, &CGhTrToolDlg::OnBnClickedBtnBGId)
    ON_BN_CLICKED(IDC_BTN_BUILD, &CGhTrToolDlg::OnBnClickedBtnBuild)
    ON_BN_CLICKED(IDC_BTN_AUTO, &CGhTrToolDlg::OnBnClickedBtnAuto)
    ON_BN_CLICKED(IDC_BTN_CARD, &CGhTrToolDlg::OnBnClickedBtnCard)
    ON_BN_CLICKED(IDC_BTN_Fast, &CGhTrToolDlg::OnBnClickedBtnFast)
    ON_BN_CLICKED(IDC_BTN_TheWorld, &CGhTrToolDlg::OnBnClickedBtnTheWorld)
    ON_BN_CLICKED(IDC_BTN_NoModelCD, &CGhTrToolDlg::OnBnClickedBtnNoModelCD)
    ON_BN_CLICKED(IDC_BTN_Mowers, &CGhTrToolDlg::OnBnClickedBtnMowers)
	ON_BN_CLICKED(IDC_BTN_PeaSDamage, &CGhTrToolDlg::OnBnClickedBtnPeaSDamage)
	ON_BN_CLICKED(IDC_BTN_NoBuildTime, &CGhTrToolDlg::OnBnClickedBtnNoBuildTime)
	ON_BN_CLICKED(IDC_BTN_NOSUNMAX, &CGhTrToolDlg::OnBnClickedBtnNoSunMax)
    ON_BN_CLICKED(IDC_BTN_ZombieDC, &CGhTrToolDlg::OnBnClickedBtnZombieDC)
    ON_BN_CLICKED(IDC_BTN_NotSubvert, &CGhTrToolDlg::OnBnClickedBtnNotSubvert)
    ON_BN_CLICKED(IDC_BTN_GodMode, &CGhTrToolDlg::OnBnClickedBtnGodMode)
	ON_BN_CLICKED(IDC_BTN_LingSDamage, &CGhTrToolDlg::OnBnClickedBtnLingSDamage)
	ON_BN_CLICKED(IDC_BTN_IgnoreSun, &CGhTrToolDlg::OnBnClickedBtnIgnoreSun)
	ON_BN_CLICKED(IDC_BTN_SummonCup, &CGhTrToolDlg::OnBnClickedBtnSummonCup)
	ON_BN_CLICKED(IDC_BTN_LoursMC, &CGhTrToolDlg::OnBnClickedBtnLoursMC)
	ON_BN_CLICKED(IDC_BTN_MeowFast, &CGhTrToolDlg::OnBnClickedBtnMeowFast)
	ON_BN_CLICKED(IDC_BTN_POINT, &CGhTrToolDlg::OnBnClickedBtnPoint)
	ON_BN_CLICKED(IDC_BTN_POINT2, &CGhTrToolDlg::OnBnClickedBtnPoint2)
	ON_BN_CLICKED(IDC_BTN_DX, &CGhTrToolDlg::OnBnClickedBtnDX)
	ON_BN_CLICKED(IDC_BTN_CherryFast, &CGhTrToolDlg::OnBnClickedBtnCherryFast)
	ON_BN_CLICKED(IDC_BTN_CherryNo, &CGhTrToolDlg::OnBnClickedBtnCherryNo)
	ON_BN_CLICKED(IDC_BTN_ApplayerNoCD, &CGhTrToolDlg::OnBnClickedBtnApplayerNoCD)
	ON_BN_CLICKED(IDC_BTN_ApplayerNoLag, &CGhTrToolDlg::OnBnClickedBtnApplayerNoLag)
	ON_BN_CLICKED(IDC_BTN_PlantageNoCD, &CGhTrToolDlg::OnBnClickedBtnPlantageNoCD)
	ON_BN_CLICKED(IDC_BTN_SunFlowerNoCD, &CGhTrToolDlg::OnBnClickedBtnSunFlowerNoCD)
	ON_BN_CLICKED(IDC_BTN_PeaNoCD, &CGhTrToolDlg::OnBnClickedBtnPeaNoCD)
	ON_BN_CLICKED(IDC_BTN_SuperReed, &CGhTrToolDlg::OnBnClickedBtnSuperReed)
	ON_BN_CLICKED(IDC_BTN_PowerFlowerNoCD, &CGhTrToolDlg::OnBnClickedBtnPowerFlowerNoCD)
	ON_BN_CLICKED(IDC_BTN_AwayMax, &CGhTrToolDlg::OnBnClickedBtnAwayMax)
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

void CGhTrToolDlg::OnBnClickedBtnSeedPacket()
{
	DWORD dwSP = GetDlgItemInt(IDC_EDIT_SP2);

	CPvz pvz = CPvz();
	pvz.SeedPacket(dwSP);
}

void CGhTrToolDlg::OnBnClickedBtnPlant()
{
	DWORD dwXP = GetDlgItemInt(IDC_EDIT_XP);
	DWORD dwYP = GetDlgItemInt(IDC_EDIT_YP);
	DWORD dwID = GetDlgItemInt(IDC_EDIT_ID);
	CPvz pvz = CPvz();
	pvz.Plant(dwXP,dwYP,dwID);

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


void CGhTrToolDlg::OnBnClickedBtnBGId()
{
    DWORD dwBGId = GetDlgItemInt(IDC_EDIT_BGId);

    CPvz pvz = CPvz();
    pvz.ModifyBGIdValue(dwBGId);
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

void CGhTrToolDlg::OnBnClickedBtnPoint()
{
	CPvz pvz = CPvz();
	pvz.Point();
}
void CGhTrToolDlg::OnBnClickedBtnPoint2()
{
	CPvz pvz = CPvz();
	pvz.Point2();
}
void CGhTrToolDlg::OnBnClickedBtnDX()
{
	CPvz pvz = CPvz();
	pvz.DX();
}

void CGhTrToolDlg::OnBnClickedBtnFast()
{
    CPvz pvz = CPvz();
    pvz.Fast();
}


void CGhTrToolDlg::OnBnClickedBtnTheWorld()
{
    CPvz pvz = CPvz();
    pvz.TheWorld();
}


void CGhTrToolDlg::OnBnClickedBtnNoModelCD()
{
    CPvz pvz = CPvz();
    pvz.NoModelCD();
}


void CGhTrToolDlg::OnBnClickedBtnMowers()
{
    CPvz pvz = CPvz();
    pvz.Mowers();
}

void CGhTrToolDlg::OnBnClickedBtnPeaSDamage()
{
	CPvz pvz = CPvz();
	pvz.PeaSDamage();
}

void CGhTrToolDlg::OnBnClickedBtnNoBuildTime()
{
	CPvz pvz = CPvz();
	pvz.NoBuildTime();
}

void CGhTrToolDlg::OnBnClickedBtnNoSunMax()
{
	CPvz pvz = CPvz();
	pvz.NoSunMax();
}

void CGhTrToolDlg::OnBnClickedBtnZombieDC()
{
    CPvz pvz = CPvz();
    pvz.ZombieDC();
}


void CGhTrToolDlg::OnBnClickedBtnNotSubvert()
{
    CPvz pvz = CPvz();
    pvz.NotSubvert();
}


void CGhTrToolDlg::OnBnClickedBtnGodMode()
{
    CPvz pvz = CPvz();
    pvz.GodMode();
}

void CGhTrToolDlg::OnBnClickedBtnSuperReed()
{
	CPvz pvz = CPvz();
	pvz.SuperReed();
}

void CGhTrToolDlg::OnBnClickedBtnPowerFlowerNoCD()
{
	CPvz pvz = CPvz();
	pvz.PowerFlowerNoCD();
}

void CGhTrToolDlg::OnBnClickedBtnAwayMax()
{
	CPvz pvz = CPvz();
	pvz.AwayMax();
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoCD()
{
	CPvz pvz = CPvz();
	pvz.ApplayerNoCD();
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoLag()
{
	CPvz pvz = CPvz();
	pvz.ApplayerNoLag();
}

void CGhTrToolDlg::OnBnClickedBtnPlantageNoCD()
{
	CPvz pvz = CPvz();
	pvz.PlantageNoCD();
}

void CGhTrToolDlg::OnBnClickedBtnPeaNoCD()
{
	CPvz pvz = CPvz();
	pvz.PeaNoCD();
}

void CGhTrToolDlg::OnBnClickedBtnSunFlowerNoCD()
{
	CPvz pvz = CPvz();
	pvz.SunFlowerNoCD();
}

void CGhTrToolDlg::OnBnClickedBtnLingSDamage()
{
	CPvz pvz = CPvz();
	pvz.LingSDamage();
}

void CGhTrToolDlg::OnBnClickedBtnIgnoreSun()
{
	CPvz pvz = CPvz();
	pvz.IgnoreSun();
}

void CGhTrToolDlg::OnBnClickedBtnSummonCup()
{
	CPvz pvz = CPvz();
	pvz.SummonCup();
}

void CGhTrToolDlg::OnBnClickedBtnLoursMC()
{
	CPvz pvz = CPvz();
	pvz.LoursMC();
}

void CGhTrToolDlg::OnBnClickedBtnMeowFast()
{
	CPvz pvz = CPvz();
	pvz.MeowFast();
}

void CGhTrToolDlg::OnBnClickedBtnCherryNo()
{
	CPvz pvz = CPvz();
	pvz.CherryNo();
}


void CGhTrToolDlg::OnBnClickedBtnCherryFast()
{
	CPvz pvz = CPvz();
	pvz.CherryFast();
}