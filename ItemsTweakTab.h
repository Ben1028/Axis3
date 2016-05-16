
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CItemTweakTab dialog

class CItemTweakTab : public CDialogPage
{
	DECLARE_DYNCREATE(CItemTweakTab)

// Construction
public:
	CItemTweakTab(CWnd* pParent = NULL);
	virtual ~CItemTweakTab();

// Dialog Data
	enum { IDD = IDD_ITEMTWEAK };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
