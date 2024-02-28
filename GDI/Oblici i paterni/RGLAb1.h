
// RGLAb1.h : main header file for the RGLAb1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRGLAb1App:
// See RGLAb1.cpp for the implementation of this class
//

class CRGLAb1App : public CWinApp
{
public:
	CRGLAb1App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRGLAb1App theApp;
