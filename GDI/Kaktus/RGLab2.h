
// RGLab2.h : main header file for the RGLab2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGLab2App:
// See RGLab2.cpp for the implementation of this class
//

class CRGLab2App : public CWinApp
{
public:
	CRGLab2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGLab2App theApp;
