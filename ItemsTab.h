
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
	void FillCategory();
	static int iIDSort;
	static int iNameSort;

// Dialog Data
	enum { IDD = IDD_ITEMS };
	CComboBox	m_ccbCategories;
	CComboBox	m_ccbSubsections;
	CListCtrl	m_clcItems;
	CMulUOP cMulDisplay;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCat();
	afx_msg void OnSelchangeSub();
	afx_msg void OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
