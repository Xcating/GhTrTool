
// GhTrToolDlg.h : 头文件
//

#pragma once


// CGhTrToolDlg 对话框
class CGhTrToolDlg : public CDialogEx
{
// 构造
public:
	CGhTrToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GhTrTool_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSun();
    afx_msg void OnBnClickedBtnSunNop();
    afx_msg void OnBnClickedBtnNoCd();
    afx_msg void OnBnClickedBtnCoin();
    afx_msg void OnBnClickedBtnBuild();
    afx_msg void OnBnClickedBtnAuto();
    afx_msg void OnBnClickedBtnCard();
    afx_msg void OnBnClickedBtnPoint();
    afx_msg void OnBnClickedBtnPoint2();
    afx_msg void OnBnClickedBtnDX();
    afx_msg void OnBnClickedBtnMore();
    afx_msg void OnBnClickedBtnAllScreen();
    afx_msg void OnBnClickedBtnUnAllScreen();
    afx_msg void OnBnClickedBtnCool();
    afx_msg void OnBnClickedBtnStop();
    afx_msg void OnBnClickedBtnDead();
    afx_msg void OnBnClickedBtnY();
};
