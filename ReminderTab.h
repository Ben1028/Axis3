
#pragma once

#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CReminderTab dialog

class CReminderTab : public CDialogPage
{
	DECLARE_DYNCREATE(CReminderTab)

// Construction
public:
	CReminderTab(CWnd* pParent = NULL);
	virtual ~CReminderTab();
	void SetLanguage();

// Dialog Data
	enum { IDD = IDD_REMINDER };
	CListCtrl m_clcList;
	CEdit m_ceTitle;
	CRichEditCtrl m_ceText;
	CComboBox m_ccbWeekday;
	CDateTimeCtrl m_cdtpDatePicker;
	CDateTimeCtrl m_cdtpTimePicker;
	CButton m_crMonthly;
	CButton m_crWeekly;
	CButton m_crDaily;
	CButton m_crOnce;
	CButton m_crNever;
	CButton m_cbStartup;
	CButton m_cbAdd;
	CButton m_cbEdit;
	CButton m_cbDelete;
	CButton m_cbSave;
	CButton m_cbCancel;
	BOOL m_bStartup;

// Implementation
protected:
	int iReminder;
	bool bEditor;
	CTime tNow;
	void LoadReminders();

	CRect crText;
	CRect crTitle;
	CRect crList;
	CRect crDatePicker;
	//CRect crGrRemind;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnListChange(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnMonthly();
	afx_msg void OnWeekly();
	afx_msg void OnDaily();
	afx_msg void OnOnce();
	afx_msg void OnNever();
	afx_msg void OnStartup();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnSave();
	afx_msg void OnCancelReminder();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
