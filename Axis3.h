
// Axis3.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAxis3App:
// See Axis3.cpp for the implementation of this class
//

class CAxis3App : public CWinApp
{
public:
	CAxis3App();
	virtual ~CAxis3App();
	CString m_csRootDirectory;	//Root Directory
	CString csLangCode;			//Language Setting
	CString csProfile;
	Database DBLng;				//Language DB
	Database DBSettings;		//Settings DB
	Database DBData;			//Data DB

	CString GetVersionTitle();
	CString GetBuildTimestamp();

	//Mul/UOP
	DWORD * m_dwColorMap;
	CDefArray m_daBodydef;
	CDefArray m_daBodyConv;
	CPtrArray m_aUopAddress;
	CPtrArray m_aHueGroups;
	bool IsArtUOP;
	bool IsMapUOP;
	bool IsMapML;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
};

extern CAxis3App * Axis;
extern CAxis3App theApp;