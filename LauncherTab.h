
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CLauncherTab dialog

class CLauncherTab : public CDialogPage
{
	DECLARE_DYNCREATE(CLauncherTab)

// Construction
public:
	CLauncherTab(CWnd* pParent = NULL);
	virtual ~CLauncherTab();

// Dialog Data
	enum { IDD = IDD_LAUNCHER };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
