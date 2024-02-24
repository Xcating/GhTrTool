
// GhTrToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GhTrTool.h"
#include "GhTrToolDlg.h"
#include "afxdialogex.h"
#include<string>

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
	m_bIsRed = true;
}
void CGhTrToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_StaticText);
}

BEGIN_MESSAGE_MAP(CGhTrToolDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
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
	ON_BN_CLICKED(IDC_BTN_ItemNoDie, &CGhTrToolDlg::OnBnClickedBtnItemNoDie)
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
	ON_BN_CLICKED(IDC_BTN_SunNoDelay, &CGhTrToolDlg::OnBnClickedBtnSunNoDelay)
END_MESSAGE_MAP()
// CGhTrToolDlg 消息处理程序
HBRUSH CGhTrToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// 如果当前控件是你要改变颜色的静态文本
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT)
	{
		if(m_bIsRed)
			pDC->SetTextColor(RGB(255, 100, 103));
		else
			pDC->SetTextColor(RGB(55, 55, 255));
		pDC->SelectObject(&m_font);  // 设置字体
		// 返回画刷句柄
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


BOOL CGhTrToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。
	UpdateText();
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	m_font.CreatePointFont(200, _T("微软雅黑"));
	SetTimer(1, 1000, NULL); 
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
DWORD CGhTrToolDlg::GetGamePid()
{
	HWND hWnd = ::FindWindow(NULL, GAME_NAME);

	if (hWnd == NULL)
	{
		return -1;
	}

	DWORD dwPid = 0;
	::GetWindowThreadProcessId(hWnd, &dwPid);

	return dwPid;
}
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

void CGhTrToolDlg::UpdateText()
{
	int pid = GetGamePid();
	if (pid == -1)
	{
		m_bIsRed = 1;
		m_StaticText.SetWindowText(_T("游戏未找到"));
	}
	else
	{
		m_bIsRed = 0;
		m_StaticText.SetWindowText(_T("游戏已找到"));
	}
}
void CGhTrToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		UpdateText();
	}

	CDialogEx::OnTimer(nIDEvent);
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
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_SUN_NOP);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.SunNop(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.SunNop(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnNoCd()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_NO_CD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.NoCd(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.NoCd(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnBGId()
{
    DWORD dwBGId = GetDlgItemInt(IDC_EDIT_BGId);
    CPvz pvz = CPvz();
    pvz.ModifyBGIdValue(dwBGId);
}


void CGhTrToolDlg::OnBnClickedBtnBuild()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_BUILD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Build(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Build(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnAuto()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_AUTO);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Auto(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Auto(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnCard()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_CARD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Card(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Card(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnPoint()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_POINT);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Point(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Point(0);
	}
}
void CGhTrToolDlg::OnBnClickedBtnPoint2()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_POINT2);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Point2(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Point2(0);
	}
}
void CGhTrToolDlg::OnBnClickedBtnDX()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_DX);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.DX(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.DX(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnFast()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_Fast);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Fast(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Fast(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnTheWorld()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_TheWorld);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.TheWorld(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.TheWorld(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnNoModelCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_NoModelCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.NoModelCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.NoModelCD(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnMowers()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_Mowers);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.Mowers(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.Mowers(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnPeaSDamage()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_PeaSDamage);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.PeaSDamage(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.PeaSDamage(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnNoBuildTime()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_NoBuildTime);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.NoBuildTime(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.NoBuildTime(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnNoSunMax()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_NOSUNMAX);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.NoSunMax(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.NoSunMax(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnZombieDC()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_ZombieDC);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.ZombieDC(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.ZombieDC(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnNotSubvert()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_NotSubvert);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.NotSubvert(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.NotSubvert(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnGodMode()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_GodMode);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.GodMode(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.GodMode(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnItemNoDie()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_ItemNoDie);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.ItemNoDie(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.ItemNoDie(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnSunNoDelay()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_SunNoDelay);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.SunNoDelay(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.SunNoDelay(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnSuperReed()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_SuperReed);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.SuperReed(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.SuperReed(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnPowerFlowerNoCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_PowerFlowerNoCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.PowerFlowerNoCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.PowerFlowerNoCD(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnAwayMax()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_AwayMax);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.AwayMax(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.AwayMax(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_ApplayerNoCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.ApplayerNoCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.ApplayerNoCD(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoLag()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_ApplayerNoLag);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.ApplayerNoLag(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.ApplayerNoLag(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnPlantageNoCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_PlantageNoCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.PlantageNoCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.PlantageNoCD(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnPeaNoCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_PeaNoCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.PeaNoCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.PeaNoCD(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnSunFlowerNoCD()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_SunFlowerNoCD);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.SunFlowerNoCD(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.SunFlowerNoCD(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnLingSDamage()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_LingSDamage);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.LingSDamage(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.LingSDamage(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnIgnoreSun()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_IgnoreSun);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.IgnoreSun(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.IgnoreSun(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnSummonCup()
{
		CPvz pvz = CPvz();
		pvz.SummonCup();
}

void CGhTrToolDlg::OnBnClickedBtnLoursMC()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_LoursMC);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.LoursMC(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.LoursMC(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnMeowFast()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_MeowFast);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.MeowFast(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.MeowFast(0);
	}
}

void CGhTrToolDlg::OnBnClickedBtnCherryNo()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_CherryNo);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.CherryNo(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.CherryNo(0);
	}
}


void CGhTrToolDlg::OnBnClickedBtnCherryFast()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_BTN_CherryFast);
	int checkState = pCheck->GetCheck();
	if (checkState == BST_CHECKED)
	{
		CPvz pvz = CPvz();
		pvz.CherryFast(1);
	}
	else
	{
		CPvz pvz = CPvz();
		pvz.CherryFast(0);
	}
}