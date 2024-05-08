#pragma once
#include "afxdialogex.h"


// InfoDialog 对话框

class InfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDialog)

public:
	InfoDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~InfoDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLANTID };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CListCtrl m_List2;
	CListCtrl m_List3;
};
