
// PvzTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPvzToolApp: 
// �йش����ʵ�֣������ PvzTool.cpp
//

class CPvzToolApp : public CWinApp
{
public:
	CPvzToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPvzToolApp theApp;