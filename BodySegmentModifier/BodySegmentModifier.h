
// BodySegmentModifier.h : main header file for the BodySegmentModifier application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBodySegmentModifierApp:
// See BodySegmentModifier.cpp for the implementation of this class
//

class CBodySegmentModifierApp : public CWinAppEx
{
public:
	CBodySegmentModifierApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CBodySegmentModifierApp theApp;
