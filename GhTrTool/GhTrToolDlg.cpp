
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
#include "InfoDialog.h"
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
	ON_COMMAND(ID_InfoDialog, &CGhTrToolDlg::OnClickMenuShowIDList)
	ON_COMMAND(ID_BackGroundID, &CGhTrToolDlg::OnClickMenuShowIDList)
	ON_COMMAND(ID_DiffID, &CGhTrToolDlg::OnClickMenuShowIDList)
    ON_BN_CLICKED(IDC_BTN_SUN, &CGhTrToolDlg::OnBnClickedBtnSun)
	ON_BN_CLICKED(IDC_BTN_SP, &CGhTrToolDlg::OnBnClickedBtnModifyEnableFrameDamageNumber)
	ON_BN_CLICKED(IDC_BTN_ModifyEnableFrameDamageData, &CGhTrToolDlg::OnBnClickedBtnModifyEnableFrameDamageData)
	ON_BN_CLICKED(IDC_BTN_DifficultySwitcher, &CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher)
	ON_BN_CLICKED(IDC_BTN_ConvertToWiki, &CGhTrToolDlg::OnBnClickedBtnConvertToWiki)
	ON_BN_CLICKED(IDC_BTN_Plant, &CGhTrToolDlg::OnBnClickedBtnPlant)
    ON_BN_CLICKED(IDC_BTN_SUN_NOP, &CGhTrToolDlg::OnBnClickedBtnPlantWithoutSunReduction)
    ON_BN_CLICKED(IDC_BTN_NO_CD, &CGhTrToolDlg::OnBnClickedBtnDisableCooldowns)
    ON_BN_CLICKED(IDC_BTN_BGId, &CGhTrToolDlg::OnBnClickedBtnBGId)
    ON_BN_CLICKED(IDC_BTN_BUILD, &CGhTrToolDlg::OnBnClickedBtnBuild)
    ON_BN_CLICKED(IDC_BTN_AUTO, &CGhTrToolDlg::OnBnClickedBtnEnableAutoCollectSun)
    ON_BN_CLICKED(IDC_BTN_CARD, &CGhTrToolDlg::OnBnClickedBtnEnableFrameDamage)
    ON_BN_CLICKED(IDC_BTN_Fast, &CGhTrToolDlg::OnBnClickedBtnFast)
    ON_BN_CLICKED(IDC_BTN_TheWorld, &CGhTrToolDlg::OnBnClickedBtnTheWorld)
    ON_BN_CLICKED(IDC_BTN_DisableSkillCooldown, &CGhTrToolDlg::OnBnClickedBtnDisableSkillCooldown)
    ON_BN_CLICKED(IDC_BTN_DisableLawnMowerMovement, &CGhTrToolDlg::OnBnClickedBtnDisableLawnMowerMovement)
	ON_BN_CLICKED(IDC_BTN_EnablePeashooterFrameDamage, &CGhTrToolDlg::OnBnClickedBtnEnablePeashooterFrameDamage)
	ON_BN_CLICKED(IDC_BTN_InstantObstacleDisappearance, &CGhTrToolDlg::OnBnClickedBtnInstantObstacleDisappearance)
	ON_BN_CLICKED(IDC_BTN_NOSUNMAX, &CGhTrToolDlg::OnBnClickedBtnRemoveSunProductionLimit)
    ON_BN_CLICKED(IDC_BTN_ZombiesDropCardsOnDeath, &CGhTrToolDlg::OnBnClickedBtnZombiesDropCardsOnDeath)
    ON_BN_CLICKED(IDC_BTN_PreventPlantCharm, &CGhTrToolDlg::OnBnClickedBtnPreventPlantCharm)
    ON_BN_CLICKED(IDC_BTN_MakePlantsInvincible, &CGhTrToolDlg::OnBnClickedBtnMakePlantsInvincible)
	ON_BN_CLICKED(IDC_BTN_PreventItemDeterioration, &CGhTrToolDlg::OnBnClickedBtnPreventItemDeterioration)
	ON_BN_CLICKED(IDC_BTN_DisableUbSaveDestroy, &CGhTrToolDlg::OnBnClickedBtnDisableUbSaveDestroy)
#ifdef _DEBUG
	ON_BN_CLICKED(IDC_BTN_UnpackGrpFile, &CGhTrToolDlg::OnBnClickedBtnUnpackGrpFile)
	ON_BN_CLICKED(IDC_BTN_PackGrpFile, &CGhTrToolDlg::OnBnClickedBtnPackGrpFile)
#else
	ON_BN_CLICKED(IDC_BTN_UnpackGrpFile, &CGhTrToolDlg::DebugOnlyMessageBox)
	ON_BN_CLICKED(IDC_BTN_PackGrpFile, &CGhTrToolDlg::DebugOnlyMessageBox)
#endif
	ON_BN_CLICKED(IDC_BTN_EnableOpticaltropFrameDamage, &CGhTrToolDlg::OnBnClickedBtnEnableOpticaltropFrameDamage)
	ON_BN_CLICKED(IDC_BTN_PlantWithoutSunCost, &CGhTrToolDlg::OnBnClickedBtnPlantWithoutSunCost)
	ON_BN_CLICKED(IDC_BTN_CompleteLevelWithTrophy, &CGhTrToolDlg::OnBnClickedBtnCompleteLevelWithTrophy)
	ON_BN_CLICKED(IDC_BTN_EnableCriticalHitsForLorus, &CGhTrToolDlg::OnBnClickedBtnEnableCriticalHitsForLorus)
	ON_BN_CLICKED(IDC_BTN_EnableInstantCascutaProjectile, &CGhTrToolDlg::OnBnClickedBtnEnableInstantCascutaProjectile)
	ON_BN_CLICKED(IDC_BTN_POINT, &CGhTrToolDlg::OnBnClickedBtnRefusePrideEffect)
	ON_BN_CLICKED(IDC_BTN_ModifyPrideOnLevelEntry, &CGhTrToolDlg::OnBnClickedBtnRefusePrideEffect2)
	ON_BN_CLICKED(IDC_BTN_DX, &CGhTrToolDlg::OnBnClickedBtnDX)
	ON_BN_CLICKED(IDC_BTN_InstantCherryBombExplosion, &CGhTrToolDlg::OnBnClickedBtnInstantCherryBombExplosion)
	ON_BN_CLICKED(IDC_BTN_PreventCherryBombExplosion, &CGhTrToolDlg::OnBnClickedBtnPreventCherryBombExplosion)
	ON_BN_CLICKED(IDC_BTN_ApplayerDisableCooldowns, &CGhTrToolDlg::OnBnClickedBtnApplayerDisableCooldowns)
	ON_BN_CLICKED(IDC_BTN_RemoveApplayerDelay, &CGhTrToolDlg::OnBnClickedBtnRemoveApplayerDelay)
	ON_BN_CLICKED(IDC_BTN_PlantageDisableCooldowns, &CGhTrToolDlg::OnBnClickedBtnPlantageDisableCooldowns)
	ON_BN_CLICKED(IDC_BTN_SunFlowerDisableCooldowns, &CGhTrToolDlg::OnBnClickedBtnSunFlowerDisableCooldowns)
	ON_BN_CLICKED(IDC_BTN_PeaDisableCooldowns, &CGhTrToolDlg::OnBnClickedBtnPeaDisableCooldowns)
	ON_BN_CLICKED(IDC_BTN_EnhanceLightningReedAbility, &CGhTrToolDlg::OnBnClickedBtnEnhanceLightningReedAbility)
	ON_BN_CLICKED(IDC_BTN_PowerFlowerDisableCooldowns, &CGhTrToolDlg::OnBnClickedBtnPowerFlowerDisableCooldowns)
	ON_BN_CLICKED(IDC_BTN_MaintainMaximumPowerPlantSize, &CGhTrToolDlg::OnBnClickedBtnMaintainMaximumPowerPlantSize)
	ON_BN_CLICKED(IDC_BTN_InstantSunGeneration, &CGhTrToolDlg::OnBnClickedBtnInstantSunGeneration)
	ON_BN_CLICKED(IDC_BTN_DeployFormationInstantly, &CGhTrToolDlg::OnBnClickedBtnDeployFormationInstantly)
	ON_BN_CLICKED(IDC_BTN_RemoveAllPlants, &CGhTrToolDlg::OnBnClickedBtnRemoveAllPlants)
	ON_BN_CLICKED(IDC_BTN_CorrectCrashIssue, &CGhTrToolDlg::OnBnClickedBtnCorrectCrashIssue)
	ON_BN_CLICKED(IDC_BTN_SwitchToHomingThistleMode, &CGhTrToolDlg::OnBnClickedBtnSwitchToHomingThistleMode)
	ON_BN_CLICKED(IDC_BTN_EnableCreationOfUbSaves, &CGhTrToolDlg::OnBnClickedBtnEnableCreationOfUbSaves)
	ON_BN_CLICKED(IDC_BTN_SwitchToRedStingerMode, &CGhTrToolDlg::OnBnClickedBtnSwitchToRedStingerMode)
	ON_BN_CLICKED(IDC_BTN_RemoveAllZombies, &CGhTrToolDlg::OnBnClickedBtnRemoveAllZombies)
	ON_BN_CLICKED(IDC_BTN_RemoveAllProjectiles, &CGhTrToolDlg::OnBnClickedBtnRemoveAllProjectiles)
	ON_BN_CLICKED(IDC_BTN_Plant2, &CGhTrToolDlg::OnBnClickedBtnInfoDialogList)
	ON_BN_CLICKED(IDC_BTN_Plant3, &CGhTrToolDlg::OnBnClickedBtnInfoDialogList)
	ON_BN_CLICKED(IDC_BTN_Plant4, &CGhTrToolDlg::OnBnClickedBtnInfoDialogList)
	ON_BN_CLICKED(IDC_BTN_Plant5, &CGhTrToolDlg::OnBnClickedBtnInfoDialogList)
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
	InfoDialog dlg;
	dlg.DoModal(); // 显示对话框
}
void CGhTrToolDlg::OnClickMenuOpenSaveDir()
{
	ShellExecute(NULL, L"open", L"C:\\ProgramData\\PerfectVoyage", NULL, NULL, SW_SHOWDEFAULT);
}
void CGhTrToolDlg::OnClickMenuShowSaveFunction()
{
	GhTrManager GhTr = GhTrManager();
	GhTr.WriteConfig();
	SwitchEnableSava = !SwitchEnableSava;
	if (SwitchEnableSava)
	{
		GetDlgItem(IDC_BTN_DifficultySwitcher)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_Diff)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SwitchToHomingThistleMode)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_EnableCreationOfUbSaves)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_SwitchToRedStingerMode)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_DisableUbSaveDestroy)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DiffID)->ShowWindow(SW_SHOW);
		AfxMessageBox(_T("已显示该类功能，请谨慎使用，你的存档以及游戏已被标记！"), MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		      
		GetDlgItem(IDC_BTN_DifficultySwitcher)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_Diff)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SwitchToHomingThistleMode)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_EnableCreationOfUbSaves)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SwitchToRedStingerMode)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_DisableUbSaveDestroy)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DiffID)->ShowWindow(SW_HIDE);
		AfxMessageBox(_T("已禁用该类功能！"), MB_ICONINFORMATION | MB_OK);
	}
}
BOOL CGhTrToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateText();
#ifdef _DEBUG
	GetDlgItem(IDC_BTN_UnpackGrpFile)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_PackGrpFile)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_PACK)->ShowWindow(SW_SHOW);
#else
	GetDlgItem(IDC_BTN_UnpackGrpFile)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_PackGrpFile)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PACK)->ShowWindow(SW_HIDE);
#endif
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
#ifdef _DEBUG
#else
void CGhTrToolDlg::DebugOnlyMessageBox()
{
	MessageBoxA(NULL, "该功能仅供DEBUG使用，RELEASE编译模式无法使用。", "提示", MB_OK | MB_ICONERROR);
}
#endif
void CGhTrToolDlg::ToggleFeature(UINT nID, void (GhTrManager::* featureFunc)(bool))
{
	GhTrManager GhTr;
	CButton* pCheck = (CButton*)GetDlgItem(nID);
	if (!GhTr.CheckGamePid(GhTr.GetGamePid(),false)) {
		pCheck->SetCheck(BST_UNCHECKED);
		return;
	}
	GhTr.WriteConfig();
	bool isFeatureEnabled = pCheck->GetCheck() == BST_CHECKED;
	(GhTr.*featureFunc)(isFeatureEnabled);
}
void CGhTrToolDlg::PlantAtPositions(GhTrManager& GhTr, DWORD dwXP, DWORD dwYP, DWORD dwID) {
	if (dwXP == 0 && dwYP == 0) {
		for (int X = 1; X <= 9; ++X) {
			for (int Y = 1; Y <= 5; ++Y) {
				GhTr.Plant(X, Y, dwID);
			}
		}
	}
	else if (dwXP == 0) {
		for (int X = 1; X <= 9; ++X) {
			GhTr.Plant(X, dwYP, dwID);
			Sleep(80);
		}
	}
	else if (dwYP == 0) {
		for (int Y = 1; Y <= 5; ++Y) {
			GhTr.Plant(dwXP, Y, dwID);
			Sleep(80);
		}
	}
	else {
		GhTr.Plant(dwXP, dwYP, dwID);
	}
}
void CGhTrToolDlg::UpdateText()
{
	GhTrManager GhTr = GhTrManager();
	int pid = GhTr.GetGamePid();
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

void CGhTrToolDlg::OnBnClickedBtnInfoDialogList()
{
	InfoDialog dlg;
	dlg.DoModal(); // 显示对话框
}

void CGhTrToolDlg::OnBnClickedBtnSun()
{
    DWORD dwSun = GetDlgItemInt(IDC_EDIT_SUN);
    GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
    GhTr.ModifySunValue(dwSun);
}

void CGhTrToolDlg::OnBnClickedBtnDifficultySwitcher()
{
	DWORD dwDiff = GetDlgItemInt(IDC_EDIT_Diff);

	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.DifficultySwitcher(dwDiff);
}
void CGhTrToolDlg::OnBnClickedBtnConvertToWiki()
{
	CString strText;
	GetDlgItemText(IDC_EDIT_JsonText, strText);
	GhTrManager GhTr = GhTrManager();
	GhTr.ConvertToWiki(strText);
}

void CGhTrToolDlg::OnBnClickedBtnModifyEnableFrameDamageData()
{
	DWORD dwID = GetDlgItemInt(IDC_EDIT_SPID);
	DWORD dwNum = GetDlgItemInt(IDC_EDIT_SPNUM);
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.ModifyEnableFrameDamageData(dwID , dwNum);
}

void CGhTrToolDlg::OnBnClickedBtnModifyEnableFrameDamageNumber()
{
	DWORD dwSP = GetDlgItemInt(IDC_EDIT_SP2);

	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.ModifyEnableFrameDamageNumber(dwSP);
}

void CGhTrToolDlg::OnBnClickedBtnPlant() {
	GhTrManager GhTr = GhTrManager();
	DWORD dwPid = GhTr.GetGamePid();
	DWORD dwXP = GetDlgItemInt(IDC_EDIT_XP);
	DWORD dwYP = GetDlgItemInt(IDC_EDIT_YP);
	DWORD dwID = GetDlgItemInt(IDC_EDIT_ID);
	DWORD dwPid2 = GhTr.GetGamePid();
	if (!GhTr.CheckLawn(dwPid))
	{
		MessageBoxA(NULL, "未进入战场", "提示", MB_OK);
		return;
	}
	if (!GhTr.CheckGamePid(dwPid2, true)) return;
	GhTr.WriteConfig();
	PlantAtPositions(GhTr, dwXP, dwYP, dwID);
}

void CGhTrToolDlg::OnBnClickedBtnPlantWithoutSunReduction()
{
	ToggleFeature(IDC_BTN_SUN_NOP, &GhTrManager::PlantWithoutSunReduction);
}


void CGhTrToolDlg::OnBnClickedBtnDisableCooldowns()
{
	ToggleFeature(IDC_BTN_NO_CD, &GhTrManager::DisableCooldowns);
}


void CGhTrToolDlg::OnBnClickedBtnBGId()
{
    DWORD dwBGId = GetDlgItemInt(IDC_EDIT_BGId);
    GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
    GhTr.ChangeBackgroundID(dwBGId);
}


void CGhTrToolDlg::OnBnClickedBtnBuild()
{
	ToggleFeature(IDC_BTN_BUILD, &GhTrManager::Build);
}


void CGhTrToolDlg::OnBnClickedBtnEnableAutoCollectSun()
{
	ToggleFeature(IDC_BTN_AUTO, &GhTrManager::EnableAutoCollectSun);
}


void CGhTrToolDlg::OnBnClickedBtnEnableFrameDamage()
{
	ToggleFeature(IDC_BTN_CARD, &GhTrManager::EnableFrameDamage);
}

void CGhTrToolDlg::OnBnClickedBtnRefusePrideEffect()
{
	ToggleFeature(IDC_BTN_POINT, &GhTrManager::RefusePrideEffect);
}
void CGhTrToolDlg::OnBnClickedBtnRefusePrideEffect2()
{
	ToggleFeature(IDC_BTN_ModifyPrideOnLevelEntry, &GhTrManager::RefusePrideEffect2);
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


void CGhTrToolDlg::OnBnClickedBtnDisableSkillCooldown()
{
	ToggleFeature(IDC_BTN_DisableSkillCooldown, &GhTrManager::DisableSkillCooldown);
}


void CGhTrToolDlg::OnBnClickedBtnDisableLawnMowerMovement()
{
	ToggleFeature(IDC_BTN_DisableLawnMowerMovement, &GhTrManager::DisableLawnMowerMovement);
}

void CGhTrToolDlg::OnBnClickedBtnEnablePeashooterFrameDamage()
{
	ToggleFeature(IDC_BTN_EnablePeashooterFrameDamage, &GhTrManager::EnablePeashooterFrameDamage);
}

void CGhTrToolDlg::OnBnClickedBtnInstantObstacleDisappearance()
{
	ToggleFeature(IDC_BTN_InstantObstacleDisappearance, &GhTrManager::InstantObstacleDisappearance);
}

void CGhTrToolDlg::OnBnClickedBtnRemoveSunProductionLimit()
{
	ToggleFeature(IDC_BTN_NOSUNMAX, &GhTrManager::RemoveSunProductionLimit);
}

void CGhTrToolDlg::OnBnClickedBtnZombiesDropCardsOnDeath()
{
	ToggleFeature(IDC_BTN_ZombiesDropCardsOnDeath, &GhTrManager::ZombiesDropCardsOnDeath);
}


void CGhTrToolDlg::OnBnClickedBtnPreventPlantCharm()
{
	ToggleFeature(IDC_BTN_PreventPlantCharm, &GhTrManager::PreventPlantCharm);
}


void CGhTrToolDlg::OnBnClickedBtnMakePlantsInvincible()
{
	ToggleFeature(IDC_BTN_MakePlantsInvincible, &GhTrManager::MakePlantsInvincible);
}

void CGhTrToolDlg::OnBnClickedBtnPreventItemDeterioration()
{
	ToggleFeature(IDC_BTN_PreventItemDeterioration, &GhTrManager::PreventItemDeterioration);
}

void CGhTrToolDlg::OnBnClickedBtnDisableUbSaveDestroy()
{
	ToggleFeature(IDC_BTN_DisableUbSaveDestroy, &GhTrManager::DisableUbSaveDestroy);
}
#ifdef _DEBUG

void CGhTrToolDlg::OnBnClickedBtnUnpackGrpFile()
{
	GhTrManager GhTr = GhTrManager();
	GhTr.UnpackGrpFile();
}

void CGhTrToolDlg::OnBnClickedBtnPackGrpFile()
{
	GhTrManager GhTr = GhTrManager();
	GhTr.PackGrpFile();
}
#endif
void CGhTrToolDlg::OnBnClickedBtnInstantSunGeneration()
{
	ToggleFeature(IDC_BTN_InstantSunGeneration, &GhTrManager::InstantSunGeneration);
}

void CGhTrToolDlg::OnBnClickedBtnDeployFormationInstantly()
{
		GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
		GhTr.DeployFormationInstantly();
}

void CGhTrToolDlg::OnBnClickedBtnRemoveAllPlants()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.RemoveAllPlants();
}

void CGhTrToolDlg::OnBnClickedBtnCorrectCrashIssue()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.CorrectCrashIssue();
}

void CGhTrToolDlg::OnBnClickedBtnSwitchToHomingThistleMode()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.SwitchToHomingThistleMode();
}

void CGhTrToolDlg::OnBnClickedBtnEnableCreationOfUbSaves()
{
	GhTrManager GhTr = GhTrManager();
	GhTr.EnableCreationOfUbSaves();
}

void CGhTrToolDlg::OnBnClickedBtnSwitchToRedStingerMode()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.SwitchToRedStingerMode();
}

void CGhTrToolDlg::OnBnClickedBtnRemoveAllZombies()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.RemoveAllZombies();
}

void CGhTrToolDlg::OnBnClickedBtnRemoveAllProjectiles()
{
	GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
	GhTr.RemoveAllProjectiles();
}

void CGhTrToolDlg::OnBnClickedBtnEnhanceLightningReedAbility()
{
	ToggleFeature(IDC_BTN_EnhanceLightningReedAbility, &GhTrManager::EnhanceLightningReedAbility);
}

void CGhTrToolDlg::OnBnClickedBtnPowerFlowerDisableCooldowns()
{
	ToggleFeature(IDC_BTN_PowerFlowerDisableCooldowns, &GhTrManager::PowerFlowerDisableCooldowns);
}

void CGhTrToolDlg::OnBnClickedBtnMaintainMaximumPowerPlantSize()
{
	ToggleFeature(IDC_BTN_MaintainMaximumPowerPlantSize, &GhTrManager::MaintainMaximumPowerPlantSize);
}

void CGhTrToolDlg::OnBnClickedBtnApplayerDisableCooldowns()
{
	ToggleFeature(IDC_BTN_ApplayerDisableCooldowns, &GhTrManager::ApplayerDisableCooldowns);
}

void CGhTrToolDlg::OnBnClickedBtnRemoveApplayerDelay()
{
	ToggleFeature(IDC_BTN_RemoveApplayerDelay, &GhTrManager::RemoveApplayerDelay);
}

void CGhTrToolDlg::OnBnClickedBtnPlantageDisableCooldowns()
{
	ToggleFeature(IDC_BTN_PlantageDisableCooldowns, &GhTrManager::PlantageDisableCooldowns);
}

void CGhTrToolDlg::OnBnClickedBtnPeaDisableCooldowns()
{
	ToggleFeature(IDC_BTN_PeaDisableCooldowns, &GhTrManager::PeaDisableCooldowns);
}

void CGhTrToolDlg::OnBnClickedBtnSunFlowerDisableCooldowns()
{
	ToggleFeature(IDC_BTN_SunFlowerDisableCooldowns, &GhTrManager::SunFlowerDisableCooldowns);
}

void CGhTrToolDlg::OnBnClickedBtnEnableOpticaltropFrameDamage()
{
	ToggleFeature(IDC_BTN_EnableOpticaltropFrameDamage, &GhTrManager::EnableOpticaltropFrameDamage);
}

void CGhTrToolDlg::OnBnClickedBtnPlantWithoutSunCost()
{
	ToggleFeature(IDC_BTN_PlantWithoutSunCost, &GhTrManager::PlantWithoutSunCost);
}

void CGhTrToolDlg::OnBnClickedBtnCompleteLevelWithTrophy()
{
		GhTrManager GhTr = GhTrManager(); GhTr.WriteConfig();
		GhTr.CompleteLevelWithTrophy();
}

void CGhTrToolDlg::OnBnClickedBtnEnableCriticalHitsForLorus()
{
	ToggleFeature(IDC_BTN_EnableCriticalHitsForLorus, &GhTrManager::EnableCriticalHitsForLorus);
}

void CGhTrToolDlg::OnBnClickedBtnEnableInstantCascutaProjectile()
{
	ToggleFeature(IDC_BTN_EnableInstantCascutaProjectile, &GhTrManager::Fast);
}

void CGhTrToolDlg::OnBnClickedBtnPreventCherryBombExplosion()
{
	ToggleFeature(IDC_BTN_PreventCherryBombExplosion, &GhTrManager::PreventCherryBombExplosion);
}


void CGhTrToolDlg::OnBnClickedBtnInstantCherryBombExplosion()
{
	ToggleFeature(IDC_BTN_InstantCherryBombExplosion, &GhTrManager::InstantCherryBombExplosion);
}