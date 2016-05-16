
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

// Dialog Data
	enum { IDD = IDD_TRAVEL };
	CMulUOP cMapDisplay;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);

	DECLARE_MESSAGE_MAP()
};
