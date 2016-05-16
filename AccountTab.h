
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountTab dialog

class CAccountTab : public CDialogPage
{
	DECLARE_DYNCREATE(CAccountTab)

// Construction
public:
	CAccountTab(CWnd* pParent = NULL);
	virtual ~CAccountTab();
	void SetLanguage();

// Dialog Data
	enum { IDD = IDD_ACCOUNT };
	CStatic	m_csPrivValue;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	int iPrivLevel;
	int iResdisp;
	DWORD dwPrivs;

	virtual BOOL OnInitDialog();

	afx_msg void OnOwnerprivs();
	afx_msg void OnAdministratorprivs();
	afx_msg void OnDeveloperprivs();
	afx_msg void OnGmprivs();
	afx_msg void OnSeerprivs();
	afx_msg void OnCounselorprivs();
	afx_msg void OnPlayerprivs();
	afx_msg void OnGuestprivs();
	afx_msg void OnSetPLV();

	afx_msg void OnResdisp0();
	afx_msg void OnResdisp1();
	afx_msg void OnResdisp2();
	afx_msg void OnResdisp3();
	afx_msg void OnResdisp4();
	afx_msg void OnResdisp5();
	afx_msg void OnResdisp6();
	afx_msg void OnResdisp7();
	afx_msg void OnSetresdisp();

	afx_msg void OnPriv02();
	afx_msg void OnPriv08();
	afx_msg void OnPriv010();
	afx_msg void OnPriv020();
	afx_msg void OnPriv040();
	afx_msg void OnPriv080();
	afx_msg void OnPriv0200();
	afx_msg void OnPriv0400();
	afx_msg void OnPriv0800();
	afx_msg void OnPriv02000();
	afx_msg void OnPriv04000();
	afx_msg void OnSetprivs();
	afx_msg void OnResetPrivs();

	DECLARE_MESSAGE_MAP()
};
