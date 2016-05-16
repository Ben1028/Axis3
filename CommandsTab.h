
#pragma once

#include "CommandPage.h"
#include "DialogPage.h"
#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CCommandsTab dialog

class CCommandsTab : public CDialogPage,
	public TabCtrlNotify
{
	DECLARE_DYNCREATE(CCommandsTab)

// Construction
public:
	CCommandsTab(CWnd* pParent = NULL);
	virtual ~CCommandsTab();
	TabCtrlEx<TabCtrlStyle_VS2008_client_blue> m_ComTabCtrl;
	CTypedPtrArray<CPtrArray, CCommandPage*> p_DlgTabs;
	void LoadTabs();
	Database DB;

	enum { IDD = IDD_COMMANDS };
	CEdit ceTabName;
	CEdit ceButtonName;
	CButton cbAddButton;
	CButtonST cbCancelButton;
	CButtonST cbButtonOk;
	CButton cbAddTab;
	CButtonST cbCancelTab;
	CButtonST cbTabOk;
	CRichEditCtrl creButtonCommand;
	CComButton * pEditButton;
	void RemoveTab();
	void EditTab();
	void EditButton();

private:
	void SetTabsPosition();
	virtual void OnTabSelected(TabCtrl *pCtrl, HTAB hTab);

// Implementation
protected:
	CRect crComTabCtrl;
	CRect crButtonCommand;
	CRect crTabName;
	CRect crAddTab;
	CRect crCancelTab;
	CRect crTabOk;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedAddbutton();
	afx_msg void OnBnClickedAddtab();
	afx_msg void OnBnClickedtabOK();
	afx_msg void OnBnClickedtabCancel();
	afx_msg void OnBnClickedbuttonOK();
	afx_msg void OnBnClickedbuttonCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
