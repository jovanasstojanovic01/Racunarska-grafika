
// RGLab3.h : main header file for the RGLab3 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGLab3App:
// See RGLab3.cpp for the implementation of this class
//

class CRGLab3App : public CWinApp
{
public:
	CRGLab3App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGLab3App theApp;
