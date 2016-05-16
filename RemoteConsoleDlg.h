
#pragma once

class CRemoteConsoleDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CRemoteConsoleDlg)

public:
	CStringArray m_saMessages;//

	virtual ~CRemoteConsoleDlg();
	CRemoteConsoleDlg(CWnd* pParent = NULL);
	CString m_csAddress;
	CString m_csPort;
	CString m_csAccount;
	CString m_csPassword;
	CString m_csCommand;
	bool m_bCommandPending;
	bool m_bIsConnected;
	bool m_bContinue;
	int m_iReceiveTimeout;
	int m_nIDTimer;
	HICON m_hIcon;

	void OnConMessage(CString csText);

	// Dialog Data
	enum { IDD = IDD_REMOTECONSOLEDLG };
	CRichEditCtrl	m_creConsole;
	BOOL	m_bBroadcast;
	CEdit	m_ceConsoleSend;
	CButton	m_cbReconnect;
	CButton m_cbBroadcast;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CRect m_crBroadcast;
	CRect m_crCommandText;
	CRect m_crReConnect;
	CRect m_crConsole;
	CRect m_crOriginal;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnReconnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSendcommand();
	afx_msg void OnBroadcast();
	
	DECLARE_MESSAGE_MAP()
};

UINT ClientThread(LPVOID lpParam);
