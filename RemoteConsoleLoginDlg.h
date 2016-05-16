
#pragma once

class CRemoteConsoleLoginDlg : public CDialogEx
{
public:
	CRemoteConsoleLoginDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_REMOTE_CONSOLE_LOGIN_DLG };
	CComboBox	m_ccbName;
	CIPAddressCtrl	m_ceAddress;
	CEdit	m_cePort;
	CEdit	m_ceAccount;
	CEdit	m_cePassword;

	CString csAccountName;
	CString csPassword;
	CString csAccount;
	CString csPort;
	CString csAddress;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	int m_iSel;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCloseDlg();
	afx_msg void OnSelchangeAcctname();
	afx_msg void OnEditchangeAcctname();

	DECLARE_MESSAGE_MAP()
};
