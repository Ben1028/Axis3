
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CSpawnTab dialog

class CSpawnTab : public CDialogPage
{
	DECLARE_DYNCREATE(CSpawnTab)

// Construction
public:
	CSpawnTab(CWnd* pParent = NULL);
	virtual ~CSpawnTab();

// Dialog Data
	enum { IDD = IDD_SPAWN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
