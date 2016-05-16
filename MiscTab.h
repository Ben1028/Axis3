
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CMiscTab dialog

class CMiscTab : public CDialogPage
{
	DECLARE_DYNCREATE(CMiscTab)

// Construction
public:
	CMiscTab(CWnd* pParent = NULL);
	virtual ~CMiscTab();

// Dialog Data
	enum { IDD = IDD_MISC };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
