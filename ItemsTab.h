
#pragma once

#include "DialogPage.h"
#include "CMulUOP.h"

/////////////////////////////////////////////////////////////////////////////
// CItemsTab dialog

class CItemsTab : public CDialogPage
{
	DECLARE_DYNCREATE(CItemsTab)

// Construction
public:
	CItemsTab(CWnd* pParent = NULL);
	virtual ~CItemsTab();
	void FillCategoryTree();

// Dialog Data
	enum { IDD = IDD_ITEMS };
	CTreeCtrl	m_ctcCategories;
	CMulUOP cMulDisplay;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
