
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGeneralTab dialog

class CCharTweakTab : public CDialogPage
{
	DECLARE_DYNCREATE(CCharTweakTab)

// Construction
public:
	CCharTweakTab(CWnd* pParent = NULL);
	virtual ~CCharTweakTab();

// Dialog Data
	enum { IDD = IDD_CHARTWEAK };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
