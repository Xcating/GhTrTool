
// GhTrToolDlg.h : ͷ�ļ�
//

#pragma once


// CGhTrToolDlg �Ի���
class CGhTrToolDlg : public CDialogEx
{
// ����
public:
	CGhTrToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GhTrTool_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
    afx_msg void OnBnClickedBtnNoBuildTIme();
    afx_msg void OnBnClickedBtnNoSunMax();
    afx_msg void OnBnClickedBtnZombieDC();
    afx_msg void OnBnClickedBtnNotSubvert();
    afx_msg void OnBnClickedBtnGodMode();
    afx_msg void OnBnClickedBtnLoursMC();
    afx_msg void OnBnClickedBtnCherryFast();
    afx_msg void OnBnClickedBtnMeowFast();
    afx_msg void OnBnClickedBtnCherryNo();
    afx_msg void OnBnClickedBtnSummonCup();
};
