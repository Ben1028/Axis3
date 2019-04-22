
#pragma once

class CSettingEditDlg : public CDialogEx
{
public:
	CSettingEditDlg(CWnd* pParent = NULL);

	int			iSelIndex;
	CString		csTable;
	Database	dDatabase;
	CRect		MinSize;

	enum { IDD = IDD_SETTINGEDIT};
	CListCtrl	m_clcData;
	CEdit		m_ceID;
	CEdit		m_ceValue;
	CButton		m_cbAddEdit;
	CButton		m_cbDelData;
	CComboBox	m_ccbSettings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnBnClickedAddEdit();
	afx_msg void OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeSettings();
	afx_msg void OnBnClickedDelData();
	afx_msg void OnEditchangeID();

	void FillData();

	DECLARE_MESSAGE_MAP()
};