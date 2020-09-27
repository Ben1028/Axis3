
#pragma once

class CProfileDlg : public CDialogEx
{
public:
	CProfileDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_PROFILE_LOGIN_DLG };
	CComboBox	m_ccbName;
	CIPAddressCtrl	m_ceAddress;
	CEdit	m_cePort;
	CEdit	m_ceAccount;
	CEdit	m_cePassword;
	CButton m_cbDelete;
	CButton m_cbSelect;

	CString csAccountName;
	CString csPassword;
	CString csAccount;
	CString csPort;
	CString csAddress;
	CString csPath;

	BOOL	m_bLocal;
	BOOL	m_bSelect;
	CStatic	m_csSphereTables;
	CButton m_cbBrowse;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	int m_iSel;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnSelchangeAcctname();
	afx_msg void OnEditchangeAcctname();
	afx_msg void OnSelectProfile();
	afx_msg void OnLocal();
	afx_msg void OnBrowse();

	DECLARE_MESSAGE_MAP()
};
