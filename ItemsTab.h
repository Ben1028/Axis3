
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
	void SetLanguage();
	void FillCategory();
	void Find();
	bool bIDSort;
	bool bDescSort;
	bool bSearch;
	CString csSortQuerry;

// Dialog Data
	enum { IDD = IDD_ITEMS };
	CComboBox	m_ccbCategories;
	CComboBox	m_ccbSubsections;
	CListCtrl	m_clcItems;
	CMulUOP		cMulDisplay;
	BOOL		m_bLockDown;
	CButton		m_cbLockDown;
	CStatic		m_csItemID;
	CEdit		m_ceZTile;
	CEdit		m_ceNukearg;
	CEdit		m_ceMinTime;
	CEdit		m_ceMaxTime;
	CEdit		m_ceMaxDist;
	CEdit		m_ceAmount;
	CEdit		m_ceNudge;
	CButtonST	m_cbNudgeUp;
	CButtonST	m_cbNudgeDown;
	CButtonST	m_cbMove1;
	CButtonST	m_cbMove2;
	CButtonST	m_cbMove3;
	CButtonST	m_cbMove4;
	CButtonST	m_cbMove5;
	CButtonST	m_cbMove6;
	CButtonST	m_cbMove7;
	CButtonST	m_cbMove8;
	CButtonST	m_cbfinditem;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCat();
	afx_msg void OnSelchangeSub();
	afx_msg void OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCreate();
	afx_msg void OnLockitem();
	afx_msg void OnRemove();
	afx_msg void OnTile();
	afx_msg void OnFlip();
	afx_msg void OnPlacespawn();
	afx_msg void OnInitspawn();
	afx_msg void OnNuke();
	afx_msg void OnChangeAmount();
	afx_msg void OnChangeMaxTime();
	afx_msg void OnChangeMinTime();
	afx_msg void OnChangeMaxDist();
	afx_msg void OnChangeNudge();
	afx_msg void OnChangeTile();
	afx_msg void OnChangeNuke();
	afx_msg void OnNudgeup();
	afx_msg void OnNudgedown();
	afx_msg void OnMove1();
	afx_msg void OnMove2();
	afx_msg void OnMove3();
	afx_msg void OnMove4();
	afx_msg void OnMove5();
	afx_msg void OnMove6();
	afx_msg void OnMove7();
	afx_msg void OnMove8();
	afx_msg void OnFinditem();
	afx_msg void OnDblclkItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickItems(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
