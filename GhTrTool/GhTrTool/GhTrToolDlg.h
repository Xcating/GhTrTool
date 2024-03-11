
// GhTrToolDlg.h : 头文件
//

#pragma once
#include <afxwin.h>
#include <Psapi.h>
// CGhTrToolDlg 对话框
class CGhTrToolDlg : public CDialogEx
{
// 构造
public:
	CGhTrToolDlg(CWnd* pParent = NULL);	// 标准构造函数
    CStatic m_StaticText;
    DWORD GetGamePid();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GhTrTool_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
    CBrush m_brush;
    BOOL m_bIsRed=1;
    CFont m_font;
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    // 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSun();
    afx_msg void OnBnClickedBtnSeedPacket();
    afx_msg void OnBnClickedBtnPlant();
    afx_msg void OnBnClickedBtnSunNop();
    afx_msg void OnBnClickedBtnNoCd();
    afx_msg void OnBnClickedBtnBGId();
    afx_msg void OnBnClickedBtnBuild();
    afx_msg void OnBnClickedBtnAuto();
    afx_msg void OnBnClickedBtnCard();
    afx_msg void OnBnClickedBtnPoint();
    afx_msg void OnBnClickedBtnPoint2();
    afx_msg void OnBnClickedBtnDX();
    afx_msg void OnBnClickedBtnFast();
    afx_msg void OnBnClickedBtnTheWorld();
    afx_msg void OnBnClickedBtnNoModelCD();
    afx_msg void OnBnClickedBtnMowers();
    afx_msg void OnBnClickedBtnPeaSDamage();
    afx_msg void OnBnClickedBtnNoBuildTime();
    afx_msg void OnBnClickedBtnNoSunMax();
    afx_msg void OnBnClickedBtnZombieDC();
    afx_msg void OnBnClickedBtnNotSubvert();
    afx_msg void OnBnClickedBtnGodMode();
    afx_msg void OnBnClickedBtnIgnoreSun();
    afx_msg void OnBnClickedBtnLoursMC();
    afx_msg void OnBnClickedBtnCherryFast();
    afx_msg void OnBnClickedBtnMeowFast();
    afx_msg void OnBnClickedBtnCherryNo();
    afx_msg void OnBnClickedBtnSummonCup();
    afx_msg void OnBnClickedBtnLingSDamage();
    afx_msg void OnBnClickedBtnApplayerNoCD();
    afx_msg void OnBnClickedBtnApplayerNoLag();
    afx_msg void OnBnClickedBtnPlantageNoCD();
    afx_msg void OnBnClickedBtnSunFlowerNoCD();
    afx_msg void OnBnClickedBtnPeaNoCD();
    afx_msg void OnBnClickedBtnSuperReed();
    afx_msg void OnBnClickedBtnPowerFlowerNoCD();
    afx_msg void OnBnClickedBtnAwayMax();
    afx_msg void OnBnClickedBtnItemNoDie();
    afx_msg void OnBnClickedBtnSunNoDelay();
    afx_msg void OnBnClickedBtnBuildTheArray();
    afx_msg void OnBnClickedBtnModifySeedPacket();
    afx_msg void OnBnClickedBtnDifficultySwitcher();
    afx_msg void OnBnClickedBtnClearPlant();
    afx_msg void OnBnClickedBtnClearBullet();
    afx_msg void OnBnClickedBtnClearZombie();
    afx_msg void OnBnClickedBtnFixCrashBug();
    afx_msg void OnBnClickedBtnToHongZhen();
    afx_msg void OnBnClickedBtnToDaoXiangJi();
    afx_msg void UpdateText();
    afx_msg void OnBnClickedBtnPlantIDList();
    void OnTimer(UINT_PTR nIDEvent);
};