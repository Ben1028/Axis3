#include "stdafx.h"
#include "Axis3.h"
#include "LangEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLangEditDlg::CLangEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLangEditDlg::IDD, pParent)
{
	iSelIndex = -1;
	csTable = _T("CMD");
}


void CLangEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LLIST, m_clcData);
	DDX_Control(pDX, IDC_LVALUE, m_ceValue);
	DDX_Control(pDX, IDC_LID, m_ceID);
	DDX_Control(pDX, IDC_LADDEDIT, m_cbAddEdit);
	DDX_Control(pDX, IDC_LANGSELECT, m_ccbLang);
	DDX_Control(pDX, IDC_LANGNAME, m_ceName);
	DDX_Control(pDX, IDC_LANGCODE, m_ceCode);
	DDX_Control(pDX, IDC_ADDLANG, m_cbAddLang);
	DDX_Control(pDX, IDC_DELLANG, m_cbDelLang);
	DDX_Control(pDX, IDC_DELDATA, m_cbDelData);
}


BEGIN_MESSAGE_MAP(CLangEditDlg, CDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LLIST, OnItemchangedItems)
	ON_CBN_SELCHANGE(IDC_LANGSELECT, OnSelchangeLang)
	ON_BN_CLICKED(IDC_LADDEDIT, OnBnClickedAddEdit)
	ON_BN_CLICKED(IDC_ADDLANG, OnBnClickedAddLang)
	ON_BN_CLICKED(IDC_DELDATA, OnBnClickedDelData)
	ON_BN_CLICKED(IDC_DELLANG, OnBnClickedDelLang)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangEditDlg message handlers

BOOL CLangEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Language Editor")));

	GetDlgItem(IDC_STATICLANG)->SetWindowText(CMsg(_T("Language")));
	GetDlgItem(IDC_STATICNAME)->SetWindowText(CMsg(_T("Name")));
	GetDlgItem(IDC_STATICCODE)->SetWindowText(CMsg(_T("Code")));
	GetDlgItem(IDC_LADDEDIT)->SetWindowText(CMsg(_T("Edit Value")));
	GetDlgItem(IDC_ADDLANG)->SetWindowText(CMsg(_T("Add Lang")));
	GetDlgItem(IDC_DELLANG)->SetWindowText(CMsg(_T("Delete")));
	GetDlgItem(IDC_DELDATA)->SetWindowText(CMsg(_T("Delete")));
	Axis->DBLng.CommitTransaction();

	m_ceCode.SetLimitText(3);

	CString csPath = CFrmt(_T("%1Lang"), Axis->m_csRootDirectory);
	dDatabase.Open(csPath);

	m_clcData.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 200, -1);
	m_clcData.InsertColumn(1, _T("String"), LVCFMT_LEFT, 400, -1);

	m_ccbLang.AddString(_T("Commands"));
	Table TBData = dDatabase.QuerySQL(_T("SELECT * FROM LangList ORDER BY Name"));
	TBData.ResetRow();
	while (TBData.GoNext())
	{
		m_ccbLang.AddString(TBData.GetValue(0));
	}
	m_ccbLang.SetCurSel(0);

	OnSelchangeLang();
	GetWindowRect(&MinSize);
	ScreenToClient(&MinSize);

	return TRUE;

}

void CLangEditDlg::OnSelchangeLang()
{
	UpdateData(TRUE);

	int iSel = m_ccbLang.GetCurSel();
	if (iSel == -1)
	{
		UpdateData(FALSE);
		return;
	}

	CString csLang;
	m_ccbLang.GetLBText(iSel, csLang);
	if (csLang == _T("Commands"))
	{
		csTable = _T("CMD");
		m_cbDelLang.EnableWindow(false);
	}
	else
	{
		Table TBCode = dDatabase.QuerySQL(CFrmt(_T("SELECT * FROM LangList WHERE Name = '%1'"), csLang));
		csTable = TBCode.GetValue(1);
		m_cbDelLang.EnableWindow();
	}
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	m_cbAddEdit.EnableWindow(false);
	FillData();

	UpdateData(FALSE);
}

void CLangEditDlg::FillData()
{
	m_clcData.SetHotItem(-1);
	m_clcData.DeleteAllItems();

	if (csTable == _T("CMD"))
	{
		Table TBData = dDatabase.QuerySQL(_T("SELECT * FROM CMD ORDER BY ID"));
		TBData.ResetRow();
		int iCount = 0;
		while (TBData.GoNext())
		{
			m_clcData.InsertItem(iCount, TBData.GetValue(0), iCount);
			m_clcData.SetItemText(iCount, 1, TBData.GetValue(1));
			iCount++;
		}
	}
	else
	{
		Table TBData = dDatabase.QuerySQL(_T("SELECT * FROM IDList ORDER BY ID"));
		TBData.ResetRow();
		int iCount = 0;
		while (TBData.GoNext())
		{
			m_clcData.InsertItem(iCount, TBData.GetValue(0), iCount);
			Table TBValue = dDatabase.QuerySQL(CFrmt(_T("SELECT * FROM %1 WHERE ID = '%2'"), csTable, TBData.GetValue(0)));
			if (TBValue.GetValue(1))
				m_clcData.SetItemText(iCount, 1, TBValue.GetValue(1));
			iCount++;
		}
	}
}

void CLangEditDlg::OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		iSelIndex = m_clcData.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex != -1)
		{
			m_clcData.SetHotItem(iSelIndex);
			m_ceID.SetWindowText(m_clcData.GetItemText(iSelIndex, 0));
			m_ceValue.SetWindowText(m_clcData.GetItemText(iSelIndex, 1));
			if (csTable != _T("CMD"))
				m_cbDelData.EnableWindow();
			m_cbAddEdit.EnableWindow();
			return;
		}
	}
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	m_cbAddEdit.EnableWindow(false);
	iSelIndex = -1;

	*pResult = 0;
}

void CLangEditDlg::OnBnClickedAddEdit()
{
	CString csID, csValue;
	csID = GetEditString(m_ceID);
	csValue = SQLStrip(GetEditString(m_ceValue));
	if (csValue != "")
	{
		dDatabase.ExecuteSQL(CFrmt(_T("INSERT OR REPLACE INTO %1 VALUES('%2','%3')"), csTable, csID, csValue));

		FillData();
		m_clcData.SetHotItem(iSelIndex);
		m_clcData.SetItemState(iSelIndex, LVNI_SELECTED, LVIS_SELECTED);
		m_ceID.SetWindowText(_T(""));
		m_ceValue.SetWindowText(_T(""));
		m_cbDelData.EnableWindow(false);
		m_cbAddEdit.EnableWindow(false);
	}
}

void CLangEditDlg::OnBnClickedDelData()
{
	dDatabase.ExecuteSQL(CFrmt(_T("DELETE FROM %1 WHERE ID = '%2'"), csTable, GetEditString(m_ceID)));
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	m_cbAddEdit.EnableWindow(false);
	FillData();
}

void CLangEditDlg::OnBnClickedDelLang()
{
	CString csLang;
	m_ccbLang.GetLBText(m_ccbLang.GetCurSel(), csLang);
	dDatabase.ExecuteSQL(CFrmt(_T("DROP TABLE %1"), csTable));
	dDatabase.ExecuteSQL(CFrmt(_T("DELETE FROM LangList WHERE Name = '%1'"), csLang));
	m_ccbLang.DeleteString(m_ccbLang.GetCurSel());
	m_ccbLang.SetCurSel(0);
	OnSelchangeLang();
	m_cbDelLang.EnableWindow(false);
}

void CLangEditDlg::OnBnClickedAddLang()
{
	CString csLang = GetEditString(m_ceName);
	CString csCode = GetEditString(m_ceCode);

	if ((csLang != "") && (csCode != ""))
	{
		dDatabase.BeginTransaction();
		dDatabase.ExecuteSQL(CFrmt(_T("INSERT INTO LangList VALUES('%1','%2')"), csLang, csCode));
		dDatabase.ExecuteSQL(CFrmt(_T("CREATE TABLE IF NOT EXISTS '%1' (ID TEXT PRIMARY KEY NOT NULL, String TEXT)"), csCode));
		dDatabase.CommitTransaction();
		m_ccbLang.SetCurSel(m_ccbLang.AddString(csLang));
		OnSelchangeLang();
	}
}

void CLangEditDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = MinSize.Width();
	lpMMI->ptMinTrackSize.y = MinSize.Height();

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}