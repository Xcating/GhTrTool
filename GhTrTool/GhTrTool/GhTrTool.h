
// 
// .h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGhTrToolApp: 
// �йش����ʵ�֣������ GhTrTool.cpp
//

class CGhTrToolApp : public CWinApp
{
public:
	CGhTrToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGhTrToolApp theApp;