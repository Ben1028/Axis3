
#pragma once

class CSearchCritDlg : public CDialogEx
{

public:
	CSearchCritDlg(CWnd* pParent = NULL);
	bool bArea;

	enum { IDD = IDD_SEARCHCRITDLG };
	int		m_crPref;
	CEdit	m_ceValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg void OnSearchId();
	afx_msg void OnSearchDescription();
	afx_msg void OnSearchDefname();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};