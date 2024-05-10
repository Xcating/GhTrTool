
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
    BOOL SwitchEnableSava=0;
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    void OnClickMenuAbout();
    void OnClickMenuShowIDList();
    void OnClickMenuOpenSaveDir();
    void OnClickMenuShowSaveFunction();
    virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    void ToggleFeature(UINT nID, void(GhTrManager::* featureFunc)(bool));
    void PlantAtPositions(GhTrManager& GhTr, DWORD dwXP, DWORD dwYP, DWORD dwID);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSun();
    afx_msg void OnBnClickedBtnModifyEnableFrameDamageNumber();
    afx_msg void OnBnClickedBtnPlant();
    afx_msg void OnBnClickedBtnPlantWithoutSunReduction();
    afx_msg void OnBnClickedBtnDisableCooldowns();
    afx_msg void OnBnClickedBtnBGId();
    afx_msg void OnBnClickedBtnBuild();
    afx_msg void OnBnClickedBtnEnableAutoCollectSun();
    afx_msg void OnBnClickedBtnEnableFrameDamage();
    afx_msg void OnBnClickedBtnRefusePrideEffect();
    afx_msg void OnBnClickedBtnRefusePrideEffect2();
    afx_msg void OnBnClickedBtnDX();
    afx_msg void OnBnClickedBtnFast();
    afx_msg void OnBnClickedBtnTheWorld();
    afx_msg void OnBnClickedBtnDisableSkillCooldown();
    afx_msg void OnBnClickedBtnDisableLawnMowerMovement();
    afx_msg void OnBnClickedBtnEnablePeashooterFrameDamage();
    afx_msg void OnBnClickedBtnInstantObstacleDisappearance();
    afx_msg void OnBnClickedBtnRemoveSunProductionLimit();
    afx_msg void OnBnClickedBtnZombiesDropCardsOnDeath();
    afx_msg void OnBnClickedBtnPreventPlantCharm();
    afx_msg void OnBnClickedBtnMakePlantsInvincible();
    afx_msg void OnBnClickedBtnPlantWithoutSunCost();
    afx_msg void OnBnClickedBtnEnableCriticalHitsForLorus();
    afx_msg void OnBnClickedBtnInstantCherryBombExplosion();
    afx_msg void OnBnClickedBtnEnableInstantCascutaProjectile();
    afx_msg void OnBnClickedBtnPreventCherryBombExplosion();
    afx_msg void OnBnClickedBtnCompleteLevelWithTrophy();
    afx_msg void OnBnClickedBtnEnableOpticaltropFrameDamage();
    afx_msg void OnBnClickedBtnApplayerDisableCooldowns();
    afx_msg void OnBnClickedBtnRemoveApplayerDelay();
    afx_msg void OnBnClickedBtnPlantageDisableCooldowns();
    afx_msg void OnBnClickedBtnSunFlowerDisableCooldowns();
    afx_msg void OnBnClickedBtnPeaDisableCooldowns();
    afx_msg void OnBnClickedBtnEnhanceLightningReedAbility();
    afx_msg void OnBnClickedBtnPowerFlowerDisableCooldowns();
    afx_msg void OnBnClickedBtnMaintainMaximumPowerPlantSize();
    afx_msg void OnBnClickedBtnPreventItemDeterioration();
    afx_msg void OnBnClickedBtnDisableUbSaveDestroy();
    afx_msg void OnBnClickedBtnUnpackGrpFile();
    afx_msg void OnBnClickedBtnPackGrpFile();
    afx_msg void OnBnClickedBtnInstantSunGeneration();
    afx_msg void OnBnClickedBtnDeployFormationInstantly();
    afx_msg void OnBnClickedBtnModifyEnableFrameDamageData();
    afx_msg void OnBnClickedBtnDifficultySwitcher();
    afx_msg void OnBnClickedBtnConvertToWiki();
    afx_msg void OnBnClickedBtnRemoveAllPlants();
    afx_msg void OnBnClickedBtnRemoveAllProjectiles();
    afx_msg void OnBnClickedBtnRemoveAllZombies();
    afx_msg void OnBnClickedBtnCorrectCrashIssue();
    afx_msg void OnBnClickedBtnSwitchToRedStingerMode();
    afx_msg void OnBnClickedBtnSwitchToHomingThistleMode();
    afx_msg void OnBnClickedBtnEnableCreationOfUbSaves();
    afx_msg void UpdateText();
    afx_msg void OnBnClickedBtnInfoDialogList();
    void OnTimer(UINT_PTR nIDEvent);
    CEdit m_edit;
};