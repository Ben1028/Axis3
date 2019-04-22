
#pragma once

class CSettingsGeneral : public CDialogEx
{
public:
	CSettingsGeneral(CWnd* pParent = NULL);

	enum { IDD = IDD_SETTING_GENERAL };
	BOOL	m_bAlwaysOnTop;
	BOOL	m_bSysClose;
	BOOL	m_bSameAsClient;
	BOOL	m_bDevMode;
	CComboBox	m_ccbLang;
	CButton m_cbMulBrowse;
	CEdit	m_ceCommandPrefix;
	CEdit	m_ceUOTitle;
	CStatic	m_csDefaultClient;
	CStatic	m_csDefaultMulPath;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnAlwaysOnTop();
	afx_msg void OnSysClose();
	afx_msg void OnSameAsClient();
	afx_msg void OnResetSettings();
	afx_msg void OnClientbrowse();
	afx_msg void OnMulBrowse();
	afx_msg void OnChangeUOTitle();
	afx_msg void OnChangePrefix();
	afx_msg void OnSelchangeLang();
	afx_msg void OnDevMode();


	DECLARE_MESSAGE_MAP()

};