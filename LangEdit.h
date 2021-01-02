
#pragma once

class CLangEditDlg : public CDialogEx
{
public:
	CLangEditDlg(CWnd* pParent = NULL);

	int iSelIndex;
	CString csTable;
	Database	dDatabase;
	CRect MinSize;

	enum { IDD = IDD_LANGEDIT };
	CListCtrl	m_clcData;
	CEdit		m_ceID;
	CEdit		m_ceValue;
	CEdit		m_ceName;
	CEdit		m_ceCode;
	CButton		m_cbAddEdit;
	CButton		m_cbAddLang;
	CButton		m_cbDelLang;
	CButton		m_cbDelData;
	CComboBox	m_ccbLang;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnBnClickedAddEdit();
	afx_msg void OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeLang();
	afx_msg void OnBnClickedDelData();
	afx_msg void OnBnClickedDelLang();
	afx_msg void OnBnClickedAddLang();

	void FillData();

	DECLARE_MESSAGE_MAP()
};
