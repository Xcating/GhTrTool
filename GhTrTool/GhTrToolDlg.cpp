
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
#include "PlantID.h"
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
	ON_BN_CLICKED(IDC_BTN_ModifySeedPacket, &CGhTrToolDlg::OnBnClickedBtnModifySeedPacket)
	ON_BN_CLICKED(IDC_BTN_DifficultySwitcher, &CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher)
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
	ON_BN_CLICKED(IDC_BTN_BuildTheArray, &CGhTrToolDlg::OnBnClickedBtnBuildTheArray)
	ON_BN_CLICKED(IDC_BTN_ClearZombie, &CGhTrToolDlg::OnBnClickedBtnClearZombie)
	ON_BN_CLICKED(IDC_BTN_FixCrashBug, &CGhTrToolDlg::OnBnClickedBtnFixCrashBug)
	ON_BN_CLICKED(IDC_BTN_ToDaoXiangJi, &CGhTrToolDlg::OnBnClickedBtnToDaoXiangJi)
	ON_BN_CLICKED(IDC_BTN_ToHongZhen, &CGhTrToolDlg::OnBnClickedBtnToHongZhen)
	ON_BN_CLICKED(IDC_BTN_ClearPlant, &CGhTrToolDlg::OnBnClickedBtnClearPlant)
	ON_BN_CLICKED(IDC_BTN_ClearBullet, &CGhTrToolDlg::OnBnClickedBtnClearBullet)
	ON_BN_CLICKED(IDC_BTN_Plant2, &CGhTrToolDlg::OnBnClickedBtnPlantIDList)
	ON_BN_CLICKED(IDC_BTN_Plant3, &CGhTrToolDlg::OnBnClickedBtnPlantIDList)
	ON_BN_CLICKED(IDC_BTN_Plant4, &CGhTrToolDlg::OnBnClickedBtnPlantIDList)
	ON_BN_CLICKED(IDC_BTN_Plant5, &CGhTrToolDlg::OnBnClickedBtnPlantIDList)
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
	SetDlgItemText(IDC_EDIT_YP, _T("0"));
	SetDlgItemText(IDC_EDIT_XP, _T("0"));
	SetDlgItemText(IDC_EDIT_ID, _T("1"));
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
HCURSOR CGhTrToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CGhTrToolDlg::ToggleFeature(UINT nID, void (CPvz::* featureFunc)(bool))
{
	CPvz pvz;
	CButton* pCheck = (CButton*)GetDlgItem(nID);
	if (!pvz.check_dwPid(GetGamePid(),false)) {
		pCheck->SetCheck(BST_UNCHECKED);
		return;
	}
	pvz.WriteConfig();
	bool isFeatureEnabled = pCheck->GetCheck() == BST_CHECKED;
	(pvz.*featureFunc)(isFeatureEnabled);
}
void CGhTrToolDlg::PlantAtPositions(CPvz& pvz, DWORD dwXP, DWORD dwYP, DWORD dwID) {
	if (dwXP == 0 && dwYP == 0) {
		for (int X = 1; X <= 9; ++X) {
			for (int Y = 1; Y <= 5; ++Y) {
				pvz.Plant(X, Y, dwID);
			}
		}
	}
	else if (dwXP == 0) {
		for (int X = 1; X <= 9; ++X) {
			pvz.Plant(X, dwYP, dwID);
			Sleep(80);
		}
	}
	else if (dwYP == 0) {
		for (int Y = 1; Y <= 5; ++Y) {
			pvz.Plant(dwXP, Y, dwID);
			Sleep(80);
		}
	}
	else {
		pvz.Plant(dwXP, dwYP, dwID);
	}
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

void CGhTrToolDlg::OnBnClickedBtnPlantIDList()
{
	PlantID dlg;
	dlg.DoModal(); // 显示对话框
}

void CGhTrToolDlg::OnBnClickedBtnSun()
{
    DWORD dwSun = GetDlgItemInt(IDC_EDIT_SUN);
    CPvz pvz = CPvz(); pvz.WriteConfig();
    pvz.ModifySunValue(dwSun);
}

void CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher()
{
	DWORD dwDiff = GetDlgItemInt(IDC_EDIT_Diff);

	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.DifficultySwitcher(dwDiff);
}

void CGhTrToolDlg::OnBnClickedBtnModifySeedPacket()
{
	DWORD dwID = GetDlgItemInt(IDC_EDIT_SPID);
	DWORD dwNum = GetDlgItemInt(IDC_EDIT_SPNUM);
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ModifySeedPacket(dwID , dwNum);
}

void CGhTrToolDlg::OnBnClickedBtnSeedPacket()
{
	DWORD dwSP = GetDlgItemInt(IDC_EDIT_SP2);

	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.SeedPacket(dwSP);
}

void CGhTrToolDlg::OnBnClickedBtnPlant() {
	DWORD dwPid = GetGamePid();
	DWORD dwXP = GetDlgItemInt(IDC_EDIT_XP);
	DWORD dwYP = GetDlgItemInt(IDC_EDIT_YP);
	DWORD dwID = GetDlgItemInt(IDC_EDIT_ID);
	CPvz pvz = CPvz();
	DWORD dwPid2 = GetGamePid();
	if (!pvz.check_battlefield(dwPid))
	{
		MessageBoxA(NULL, "未进入战场", "提示", MB_OK);
		return;
	}
	if (!pvz.check_dwPid(dwPid2, true)) return;
	pvz.WriteConfig();
	PlantAtPositions(pvz, dwXP, dwYP, dwID);
}

void CGhTrToolDlg::OnBnClickedBtnSunNop()
{
	ToggleFeature(IDC_BTN_SUN_NOP, &CPvz::SunNop);
}


void CGhTrToolDlg::OnBnClickedBtnNoCd()
{
	ToggleFeature(IDC_BTN_NO_CD, &CPvz::NoCd);
}


void CGhTrToolDlg::OnBnClickedBtnBGId()
{
    DWORD dwBGId = GetDlgItemInt(IDC_EDIT_BGId);
    CPvz pvz = CPvz(); pvz.WriteConfig();
    pvz.ModifyBGIdValue(dwBGId);
}


void CGhTrToolDlg::OnBnClickedBtnBuild()
{
	ToggleFeature(IDC_BTN_BUILD, &CPvz::Build);
}


void CGhTrToolDlg::OnBnClickedBtnAuto()
{
	ToggleFeature(IDC_BTN_AUTO, &CPvz::Auto);
}


void CGhTrToolDlg::OnBnClickedBtnCard()
{
	ToggleFeature(IDC_BTN_CARD, &CPvz::Card);
}

void CGhTrToolDlg::OnBnClickedBtnPoint()
{
	ToggleFeature(IDC_BTN_POINT, &CPvz::Point);
}
void CGhTrToolDlg::OnBnClickedBtnPoint2()
{
	ToggleFeature(IDC_BTN_POINT2, &CPvz::Point2);
}
void CGhTrToolDlg::OnBnClickedBtnDX()
{
	ToggleFeature(IDC_BTN_DX, &CPvz::DX);
}

void CGhTrToolDlg::OnBnClickedBtnFast()
{
	ToggleFeature(IDC_BTN_Fast, &CPvz::Fast);
}


void CGhTrToolDlg::OnBnClickedBtnTheWorld()
{
	ToggleFeature(IDC_BTN_TheWorld, &CPvz::TheWorld);
}


void CGhTrToolDlg::OnBnClickedBtnNoModelCD()
{
	ToggleFeature(IDC_BTN_NoModelCD, &CPvz::NoModelCD);
}


void CGhTrToolDlg::OnBnClickedBtnMowers()
{
	ToggleFeature(IDC_BTN_Mowers, &CPvz::Mowers);
}

void CGhTrToolDlg::OnBnClickedBtnPeaSDamage()
{
	ToggleFeature(IDC_BTN_PeaSDamage, &CPvz::PeaSDamage);
}

void CGhTrToolDlg::OnBnClickedBtnNoBuildTime()
{
	ToggleFeature(IDC_BTN_NoBuildTime, &CPvz::NoBuildTime);
}

void CGhTrToolDlg::OnBnClickedBtnNoSunMax()
{
	ToggleFeature(IDC_BTN_NOSUNMAX, &CPvz::NoSunMax);
}

void CGhTrToolDlg::OnBnClickedBtnZombieDC()
{
	ToggleFeature(IDC_BTN_ZombieDC, &CPvz::ZombieDC);
}


void CGhTrToolDlg::OnBnClickedBtnNotSubvert()
{
	ToggleFeature(IDC_BTN_NotSubvert, &CPvz::NotSubvert);
}


void CGhTrToolDlg::OnBnClickedBtnGodMode()
{
	ToggleFeature(IDC_BTN_GodMode, &CPvz::GodMode);
}

void CGhTrToolDlg::OnBnClickedBtnItemNoDie()
{
	ToggleFeature(IDC_BTN_ItemNoDie, &CPvz::ItemNoDie);
}

void CGhTrToolDlg::OnBnClickedBtnSunNoDelay()
{
	ToggleFeature(IDC_BTN_SunNoDelay, &CPvz::SunNoDelay);
}

void CGhTrToolDlg::OnBnClickedBtnBuildTheArray()
{
		CPvz pvz = CPvz(); pvz.WriteConfig();
		pvz.BuildTheArray();
}

void CGhTrToolDlg::OnBnClickedBtnClearZombie()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ClearZombie();
}

void CGhTrToolDlg::OnBnClickedBtnFixCrashBug()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.FixCrashBug();
}

void CGhTrToolDlg::OnBnClickedBtnToDaoXiangJi()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ToDaoXiangJi();
}

void CGhTrToolDlg::OnBnClickedBtnToHongZhen()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ToHongZhen();
}

void CGhTrToolDlg::OnBnClickedBtnClearPlant()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ClearPlant();
}

void CGhTrToolDlg::OnBnClickedBtnClearBullet()
{
	CPvz pvz = CPvz(); pvz.WriteConfig();
	pvz.ClearBullet();
}

void CGhTrToolDlg::OnBnClickedBtnSuperReed()
{
	ToggleFeature(IDC_BTN_SuperReed, &CPvz::SuperReed);
}

void CGhTrToolDlg::OnBnClickedBtnPowerFlowerNoCD()
{
	ToggleFeature(IDC_BTN_PowerFlowerNoCD, &CPvz::PowerFlowerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnAwayMax()
{
	ToggleFeature(IDC_BTN_AwayMax, &CPvz::AwayMax);
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoCD()
{
	ToggleFeature(IDC_BTN_ApplayerNoCD, &CPvz::ApplayerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoLag()
{
	ToggleFeature(IDC_BTN_ApplayerNoLag, &CPvz::ApplayerNoLag);
}

void CGhTrToolDlg::OnBnClickedBtnPlantageNoCD()
{
	ToggleFeature(IDC_BTN_PlantageNoCD, &CPvz::PlantageNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnPeaNoCD()
{
	ToggleFeature(IDC_BTN_PeaNoCD, &CPvz::PeaNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnSunFlowerNoCD()
{
	ToggleFeature(IDC_BTN_SunFlowerNoCD, &CPvz::SunFlowerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnLingSDamage()
{
	ToggleFeature(IDC_BTN_LingSDamage, &CPvz::LingSDamage);
}

void CGhTrToolDlg::OnBnClickedBtnIgnoreSun()
{
	ToggleFeature(IDC_BTN_IgnoreSun, &CPvz::IgnoreSun);
}

void CGhTrToolDlg::OnBnClickedBtnSummonCup()
{
		CPvz pvz = CPvz(); pvz.WriteConfig();
		pvz.SummonCup();
}

void CGhTrToolDlg::OnBnClickedBtnLoursMC()
{
	ToggleFeature(IDC_BTN_LoursMC, &CPvz::LoursMC);
}

void CGhTrToolDlg::OnBnClickedBtnMeowFast()
{
	ToggleFeature(IDC_BTN_MeowFast, &CPvz::Fast);
}

void CGhTrToolDlg::OnBnClickedBtnCherryNo()
{
	ToggleFeature(IDC_BTN_CherryNo, &CPvz::CherryNo);
}


void CGhTrToolDlg::OnBnClickedBtnCherryFast()
{
	ToggleFeature(IDC_BTN_CherryFast, &CPvz::CherryFast);
}