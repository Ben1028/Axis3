
#pragma once

#include "DialogPage.h"
#include "CMulUOP.h"

/////////////////////////////////////////////////////////////////////////////
// CTravelTab dialog

class CTravelTab : public CDialogPage
{
	DECLARE_DYNCREATE(CTravelTab)

// Construction
public:
	CTravelTab(CWnd* pParent = NULL);
	virtual ~CTravelTab();
	void SetLanguage();
	void FillCategory();
	bool bPointSort;
	bool bDescSort;
	bool bSearch;
	CString csSortQuerry;
	long xscroll;
	long yscroll;
	void Find();

// Dialog Data
	enum { IDD = IDD_TRAVEL };
	CMulUOP cMapDisplay;
	CComboBox	m_ccbCategories;
	CComboBox	m_ccbSubsections;
	CListCtrl	m_clcLocations;
	CButtonST	cb_zoomin;
	CButtonST	cb_zoomout;
	CButtonST	cb_findarea;
	CScrollBar	m_csbXScroll;
	CScrollBar	m_csbYScroll;
	CEdit		m_ceX;
	CEdit		m_ceY;
	CEdit		m_ceZ;
	CEdit		m_ceM;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnSelchangeCat();
	afx_msg void OnSelchangeSub();
	afx_msg void OnItemchangedLocations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLocation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickLocations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGo();
	afx_msg void OnSend();
	afx_msg void OnWhere();
	afx_msg void OnZoomout();
	afx_msg void OnZoomin();
	afx_msg void OnFindarea();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeX();
	afx_msg void OnChangeY();
	afx_msg void OnChangeZ();
	afx_msg void OnChangeM();

	DECLARE_MESSAGE_MAP()
};
