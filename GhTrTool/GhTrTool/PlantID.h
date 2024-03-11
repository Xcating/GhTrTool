#pragma once
#include "afxdialogex.h"


// PlantID 对话框

class PlantID : public CDialogEx
{
	DECLARE_DYNAMIC(PlantID)

public:
	PlantID(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PlantID();

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
