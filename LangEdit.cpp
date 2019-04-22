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
	csTable = _T("ENU");
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
	ON_EN_CHANGE(IDC_LID, OnEditchangeID)
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
	SetWindowText(CMsg(_T("IDS_LANG_EDITOR")));

	GetDlgItem(IDC_STATICLANG)->SetWindowText(CMsg(_T("IDS_LANG")));
	GetDlgItem(IDC_STATICNAME)->SetWindowText(CMsg(_T("IDS_NAME")));
	GetDlgItem(IDC_STATICCODE)->SetWindowText(CMsg(_T("IDS_LANGCODE")));
	GetDlgItem(IDC_LADDEDIT)->SetWindowText(CMsg(_T("IDS_ADDEDIT")));
	GetDlgItem(IDC_ADDLANG)->SetWindowText(CMsg(_T("IDS_ADDLANG")));
	GetDlgItem(IDC_DELLANG)->SetWindowText(CMsg(_T("IDS_DELETE")));
	GetDlgItem(IDC_DELDATA)->SetWindowText(CMsg(_T("IDS_DELETE")));
	Axis->DBLng.CommitTransaction();

	m_ceCode.SetLimitText(3);

	CString csPath = CMsg(_T("%1Lang"), true, Axis->m_csRootDirectory);
	dDatabase.Open(csPath);

	m_clcData.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 200, -1);
	m_clcData.InsertColumn(1, _T("String"), LVCFMT_LEFT, 400, -1);

	Table TBData = dDatabase.QuerySQL(_T("SELECT * FROM LangList ORDER BY Name"));
	TBData.ResetRow();
	while (TBData.GoNext())
	{
		m_ccbLang.AddString(TBData.GetValue(0));
	}
	m_ccbLang.AddString(_T("Commands"));
	m_ccbLang.SetCurSel(0);

	OnSelchangeLang();
	GetWindowRect(&MinSize);
	ScreenToClient(&MinSize);

	return TRUE;

}

void CLangEditDlg::FillData()
{
	m_clcData.SetHotItem(-1);
	m_clcData.DeleteAllItems();

	Table TBData = dDatabase.QuerySQL(CSQL(_T("SELECT * FROM %1 ORDER BY ID"), csTable));
	TBData.ResetRow();
	int iCount = 0;
	while (TBData.GoNext())
	{
		m_clcData.InsertItem(iCount, TBData.GetValue(0), iCount);
		m_clcData.SetItemText(iCount, 1, TBData.GetValue(1));
		iCount++;
	}
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
	else if (csLang == _T("English"))
	{
		csTable = _T("ENU");
		m_cbDelLang.EnableWindow(false);
	}
	else
	{
		Table TBCode = dDatabase.QuerySQL(CSQL(_T("SELECT * FROM LangList WHERE Name = '%1'"), csLang));
		csTable = TBCode.GetValue(1);
		m_cbDelLang.EnableWindow();
	}
	FillData();

	UpdateData(FALSE);
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
			m_cbDelData.EnableWindow();
			return;
		}
	}
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	iSelIndex = -1;

	*pResult = 0;
}

void CLangEditDlg::OnBnClickedAddEdit()
{
	// Save Info
	CString csID, csValue;
	csID = GetEditString(m_ceID);
	csValue = SQLStrip(GetEditString(m_ceValue));
	if (csID != "")
	{
		Table TBData = dDatabase.QuerySQL(CSQL(_T("SELECT * FROM %1 WHERE ID = '%2'"), csTable, csID));
		if (TBData.GetRowCount() != 0)
		{
			dDatabase.ExecuteSQL(CSQL(_T("UPDATE %1 SET String = '%2' WHERE ID = '%3'"), csTable, csValue, csID));
		}
		else
		{
			dDatabase.ExecuteSQL(CSQL(_T("INSERT INTO %1 VALUES('%2','%3')"), csTable, csID, csValue));
		}

		FillData();
		m_clcData.SetHotItem(iSelIndex);
		m_clcData.SetItemState(iSelIndex, LVNI_SELECTED, LVIS_SELECTED);
		m_ceID.SetWindowText(_T(""));
		m_ceValue.SetWindowText(_T(""));
		m_cbDelData.EnableWindow(false);
	}
}

void CLangEditDlg::OnBnClickedDelData()
{
	dDatabase.ExecuteSQL(CSQL(_T("DELETE FROM %1 WHERE ID = '%2'"), csTable, GetEditString(m_ceID)));
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	FillData();
}

void CLangEditDlg::OnBnClickedDelLang()
{
	CString csLang;
	m_ccbLang.GetLBText(m_ccbLang.GetCurSel(), csLang);
	dDatabase.ExecuteSQL(CSQL(_T("DROP TABLE %1"), csTable));
	dDatabase.ExecuteSQL(CSQL(_T("DELETE FROM LangList WHERE Name = '%1'"), csLang));
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
		dDatabase.ExecuteSQL(CSQL(_T("INSERT INTO LangList VALUES('%1','%2')"), csLang, csCode));
		dDatabase.ExecuteSQL(CSQL(_T("CREATE TABLE IF NOT EXISTS '%1' (ID TEXT, String TEXT)"), csCode));

		Table TBData = dDatabase.QuerySQL(_T("SELECT ID FROM ENU ORDER BY ID"));
		TBData.ResetRow();
		while (TBData.GoNext())
		{
			dDatabase.ExecuteSQL(CSQL(_T("INSERT INTO %1 VALUES('%2','')"), csCode, TBData.GetValue(0)));
		}
		dDatabase.CommitTransaction();
		m_ccbLang.SetCurSel(m_ccbLang.AddString(csLang));
		OnSelchangeLang();
	}
}

void CLangEditDlg::OnEditchangeID()
{
	m_cbDelData.EnableWindow(false);
	UpdateData(FALSE);
}

void CLangEditDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = MinSize.Width();
	lpMMI->ptMinTrackSize.y = MinSize.Height();

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}