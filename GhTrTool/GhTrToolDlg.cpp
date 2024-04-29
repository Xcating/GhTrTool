
// GhTrToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GhTrTool.h"
#include "GhTrToolDlg.h"
#include "afxdialogex.h"
#include<string>
#include <sstream>
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
	DDX_Control(pDX, IDC_EDIT_JsonText, m_edit);
}

BEGIN_MESSAGE_MAP(CGhTrToolDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, &CGhTrToolDlg::OnClickMenuAbout)
	ON_COMMAND(ID_SaveDir, &CGhTrToolDlg::OnClickMenuOpenSaveDir)
	ON_COMMAND(ID_UpdateLog, &CGhTrToolDlg::OnClickMenuAbout)
	ON_COMMAND(ID_ChangeSave, &CGhTrToolDlg::OnClickMenuShowSaveFunction)
	ON_COMMAND(ID_PlantID, &CGhTrToolDlg::OnClickMenuShowIDList)
	ON_COMMAND(ID_BackGroundID, &CGhTrToolDlg::OnClickMenuShowIDList)
	ON_COMMAND(ID_DiffID, &CGhTrToolDlg::OnClickMenuShowIDList)
    ON_BN_CLICKED(IDC_BTN_SUN, &CGhTrToolDlg::OnBnClickedBtnSun)
	ON_BN_CLICKED(IDC_BTN_SP, &CGhTrToolDlg::OnBnClickedBtnModifyCardNumber)
	ON_BN_CLICKED(IDC_BTN_ModifyCardData, &CGhTrToolDlg::OnBnClickedBtnModifyCardData)
	ON_BN_CLICKED(IDC_BTN_DifficultySwitcher, &CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher)
	ON_BN_CLICKED(IDC_BTN_ConvertToWiki, &CGhTrToolDlg::OnBnClickedBtnConvertToWiki)
	ON_BN_CLICKED(IDC_BTN_Plant, &CGhTrToolDlg::OnBnClickedBtnPlant)
    ON_BN_CLICKED(IDC_BTN_SUN_NOP, &CGhTrToolDlg::OnBnClickedBtnPlantNoSubSun)
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
	ON_BN_CLICKED(IDC_BTN_NoUbBroken, &CGhTrToolDlg::OnBnClickedBtnNoUbBroken)
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
	ON_BN_CLICKED(IDC_BTN_ArrUb, &CGhTrToolDlg::OnBnClickedBtnArrUb)
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
void CGhTrToolDlg::OnClickMenuAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
void CGhTrToolDlg::OnClickMenuShowIDList()
{
	PlantID dlg;
	dlg.DoModal(); // 显示对话框
}
void CGhTrToolDlg::OnClickMenuOpenSaveDir()
{
	ShellExecute(NULL, L"open", L"C:\\ProgramData\\PerfectVoyage", NULL, NULL, SW_SHOWDEFAULT);
}
void CGhTrToolDlg::OnClickMenuShowSaveFunction()
{
	GhTrManager pvz = GhTrManager();
	pvz.WriteConfig();
	SwitchEnableSava = !SwitchEnableSava;
	if (SwitchEnableSava)
	{
		GetDlgItem(IDC_BTN_DifficultySwitcher)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_Diff)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ToDaoXiangJi)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ArrUb)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ToHongZhen)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_NoUbBroken)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DiffID)->ShowWindow(SW_SHOW);
		AfxMessageBox(_T("已显示该类功能，请谨慎使用，你的存档以及游戏已被标记！"), MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		      
		GetDlgItem(IDC_BTN_DifficultySwitcher)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_Diff)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ToDaoXiangJi)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ArrUb)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ToHongZhen)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_NoUbBroken)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DiffID)->ShowWindow(SW_HIDE);
		AfxMessageBox(_T("已禁用该类功能！"), MB_ICONINFORMATION | MB_OK);
	}
}
BOOL CGhTrToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateText();
	GetDlgItem(IDC_BTN_NoUbBroken)->ShowWindow(SW_HIDE);
	CMenu* pMenu = new CMenu();
	pMenu->LoadMenu(IDR_MENU1);  // 使用实际的菜单资源ID代替
	SetMenu(pMenu);
	m_edit.SetLimitText(100000);
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
		bool bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标  当应用程序主窗口不是对话框时，框架将自动
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
//  来绘制该图标  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成
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
void CGhTrToolDlg::ToggleFeature(UINT nID, void (GhTrManager::* featureFunc)(bool))
{
	GhTrManager pvz;
	CButton* pCheck = (CButton*)GetDlgItem(nID);
	if (!pvz.CheckGamePid(pvz.GetGamePid(),false)) {
		pCheck->SetCheck(BST_UNCHECKED);
		return;
	}
	pvz.WriteConfig();
	bool isFeatureEnabled = pCheck->GetCheck() == BST_CHECKED;
	(pvz.*featureFunc)(isFeatureEnabled);
}
void CGhTrToolDlg::PlantAtPositions(GhTrManager& pvz, DWORD dwXP, DWORD dwYP, DWORD dwID) {
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
	GhTrManager pvz = GhTrManager();
	int pid = pvz.GetGamePid();
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
    GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
    pvz.ModifySunValue(dwSun);
}

void CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher()
{
	DWORD dwDiff = GetDlgItemInt(IDC_EDIT_Diff);

	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.DifficultySwitcher(dwDiff);
}
void CGhTrToolDlg::OnBnClickedBtnConvertToWiki()
{
	CString strText;
	GetDlgItemText(IDC_EDIT_JsonText, strText);
	GhTrManager pvz = GhTrManager();
	pvz.ConvertToWiki(strText);
}

void CGhTrToolDlg::OnBnClickedBtnModifyCardData()
{
	DWORD dwID = GetDlgItemInt(IDC_EDIT_SPID);
	DWORD dwNum = GetDlgItemInt(IDC_EDIT_SPNUM);
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ModifyCardData(dwID , dwNum);
}

void CGhTrToolDlg::OnBnClickedBtnModifyCardNumber()
{
	DWORD dwSP = GetDlgItemInt(IDC_EDIT_SP2);

	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ModifyCardNumber(dwSP);
}

void CGhTrToolDlg::OnBnClickedBtnPlant() {
	GhTrManager pvz = GhTrManager();
	DWORD dwPid = pvz.GetGamePid();
	DWORD dwXP = GetDlgItemInt(IDC_EDIT_XP);
	DWORD dwYP = GetDlgItemInt(IDC_EDIT_YP);
	DWORD dwID = GetDlgItemInt(IDC_EDIT_ID);
	DWORD dwPid2 = pvz.GetGamePid();
	if (!pvz.CheckLawn(dwPid))
	{
		MessageBoxA(NULL, "未进入战场", "提示", MB_OK);
		return;
	}
	if (!pvz.CheckGamePid(dwPid2, true)) return;
	pvz.WriteConfig();
	PlantAtPositions(pvz, dwXP, dwYP, dwID);
}

void CGhTrToolDlg::OnBnClickedBtnPlantNoSubSun()
{
	ToggleFeature(IDC_BTN_SUN_NOP, &GhTrManager::PlantNoSubSun);
}


void CGhTrToolDlg::OnBnClickedBtnNoCd()
{
	ToggleFeature(IDC_BTN_NO_CD, &GhTrManager::NoCd);
}


void CGhTrToolDlg::OnBnClickedBtnBGId()
{
    DWORD dwBGId = GetDlgItemInt(IDC_EDIT_BGId);
    GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
    pvz.ModifyBGIdValue(dwBGId);
}


void CGhTrToolDlg::OnBnClickedBtnBuild()
{
	ToggleFeature(IDC_BTN_BUILD, &GhTrManager::Build);
}


void CGhTrToolDlg::OnBnClickedBtnAuto()
{
	ToggleFeature(IDC_BTN_AUTO, &GhTrManager::Auto);
}


void CGhTrToolDlg::OnBnClickedBtnCard()
{
	ToggleFeature(IDC_BTN_CARD, &GhTrManager::Card);
}

void CGhTrToolDlg::OnBnClickedBtnPoint()
{
	ToggleFeature(IDC_BTN_POINT, &GhTrManager::Point);
}
void CGhTrToolDlg::OnBnClickedBtnPoint2()
{
	ToggleFeature(IDC_BTN_POINT2, &GhTrManager::Point2);
}
void CGhTrToolDlg::OnBnClickedBtnDX()
{
	ToggleFeature(IDC_BTN_DX, &GhTrManager::DX);
}

void CGhTrToolDlg::OnBnClickedBtnFast()
{
	ToggleFeature(IDC_BTN_Fast, &GhTrManager::Fast);
}


void CGhTrToolDlg::OnBnClickedBtnTheWorld()
{
	ToggleFeature(IDC_BTN_TheWorld, &GhTrManager::TheWorld);
}


void CGhTrToolDlg::OnBnClickedBtnNoModelCD()
{
	ToggleFeature(IDC_BTN_NoModelCD, &GhTrManager::NoModelCD);
}


void CGhTrToolDlg::OnBnClickedBtnMowers()
{
	ToggleFeature(IDC_BTN_Mowers, &GhTrManager::Mowers);
}

void CGhTrToolDlg::OnBnClickedBtnPeaSDamage()
{
	ToggleFeature(IDC_BTN_PeaSDamage, &GhTrManager::PeaSDamage);
}

void CGhTrToolDlg::OnBnClickedBtnNoBuildTime()
{
	ToggleFeature(IDC_BTN_NoBuildTime, &GhTrManager::NoBuildTime);
}

void CGhTrToolDlg::OnBnClickedBtnNoSunMax()
{
	ToggleFeature(IDC_BTN_NOSUNMAX, &GhTrManager::NoSunMax);
}

void CGhTrToolDlg::OnBnClickedBtnZombieDC()
{
	ToggleFeature(IDC_BTN_ZombieDC, &GhTrManager::ZombieDC);
}


void CGhTrToolDlg::OnBnClickedBtnNotSubvert()
{
	ToggleFeature(IDC_BTN_NotSubvert, &GhTrManager::NotSubvert);
}


void CGhTrToolDlg::OnBnClickedBtnGodMode()
{
	ToggleFeature(IDC_BTN_GodMode, &GhTrManager::GodMode);
}

void CGhTrToolDlg::OnBnClickedBtnItemNoDie()
{
	ToggleFeature(IDC_BTN_ItemNoDie, &GhTrManager::ItemNoDie);
}

void CGhTrToolDlg::OnBnClickedBtnNoUbBroken()
{
	ToggleFeature(IDC_BTN_NoUbBroken, &GhTrManager::NoUbBroken);
}

void CGhTrToolDlg::OnBnClickedBtnSunNoDelay()
{
	ToggleFeature(IDC_BTN_SunNoDelay, &GhTrManager::SunNoDelay);
}

void CGhTrToolDlg::OnBnClickedBtnBuildTheArray()
{
		GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
		pvz.BuildTheArray();
}

void CGhTrToolDlg::OnBnClickedBtnClearZombie()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ClearZombie();
}

void CGhTrToolDlg::OnBnClickedBtnFixCrashBug()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.FixCrashBug();
}

void CGhTrToolDlg::OnBnClickedBtnToDaoXiangJi()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ToDaoXiangJi();
}

void CGhTrToolDlg::OnBnClickedBtnArrUb()
{
	GhTrManager pvz = GhTrManager();
	pvz.ArrUb();
}

void CGhTrToolDlg::OnBnClickedBtnToHongZhen()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ToHongZhen();
}

void CGhTrToolDlg::OnBnClickedBtnClearPlant()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ClearPlant();
}

void CGhTrToolDlg::OnBnClickedBtnClearBullet()
{
	GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
	pvz.ClearBullet();
}

void CGhTrToolDlg::OnBnClickedBtnSuperReed()
{
	ToggleFeature(IDC_BTN_SuperReed, &GhTrManager::SuperReed);
}

void CGhTrToolDlg::OnBnClickedBtnPowerFlowerNoCD()
{
	ToggleFeature(IDC_BTN_PowerFlowerNoCD, &GhTrManager::PowerFlowerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnAwayMax()
{
	ToggleFeature(IDC_BTN_AwayMax, &GhTrManager::AwayMax);
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoCD()
{
	ToggleFeature(IDC_BTN_ApplayerNoCD, &GhTrManager::ApplayerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnApplayerNoLag()
{
	ToggleFeature(IDC_BTN_ApplayerNoLag, &GhTrManager::ApplayerNoLag);
}

void CGhTrToolDlg::OnBnClickedBtnPlantageNoCD()
{
	ToggleFeature(IDC_BTN_PlantageNoCD, &GhTrManager::PlantageNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnPeaNoCD()
{
	ToggleFeature(IDC_BTN_PeaNoCD, &GhTrManager::PeaNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnSunFlowerNoCD()
{
	ToggleFeature(IDC_BTN_SunFlowerNoCD, &GhTrManager::SunFlowerNoCD);
}

void CGhTrToolDlg::OnBnClickedBtnLingSDamage()
{
	ToggleFeature(IDC_BTN_LingSDamage, &GhTrManager::LingSDamage);
}

void CGhTrToolDlg::OnBnClickedBtnIgnoreSun()
{
	ToggleFeature(IDC_BTN_IgnoreSun, &GhTrManager::IgnoreSun);
}

void CGhTrToolDlg::OnBnClickedBtnSummonCup()
{
		GhTrManager pvz = GhTrManager(); pvz.WriteConfig();
		pvz.SummonCup();
}

void CGhTrToolDlg::OnBnClickedBtnLoursMC()
{
	ToggleFeature(IDC_BTN_LoursMC, &GhTrManager::LoursMC);
}

void CGhTrToolDlg::OnBnClickedBtnMeowFast()
{
	ToggleFeature(IDC_BTN_MeowFast, &GhTrManager::Fast);
}

void CGhTrToolDlg::OnBnClickedBtnCherryNo()
{
	ToggleFeature(IDC_BTN_CherryNo, &GhTrManager::CherryNo);
}


void CGhTrToolDlg::OnBnClickedBtnCherryFast()
{
	ToggleFeature(IDC_BTN_CherryFast, &GhTrManager::CherryFast);
}