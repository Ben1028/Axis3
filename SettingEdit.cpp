#include "stdafx.h"
#include "Axis3.h"
#include "SettingEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSettingEditDlg::CSettingEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingEditDlg::IDD, pParent)
{
	iSelIndex = -1;
	csTable = _T("Settings");
}


void CSettingEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_clcData);
	DDX_Control(pDX, IDC_VALUE, m_ceValue);
	DDX_Control(pDX, IDC_ID, m_ceID);
	DDX_Control(pDX, IDC_ADDEDIT, m_cbAddEdit);
	DDX_Control(pDX, IDC_DELSETDATA, m_cbDelData);
	DDX_Control(pDX, IDC_SETTINGSELECT, m_ccbSettings);
}


BEGIN_MESSAGE_MAP(CSettingEditDlg, CDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnItemchangedItems)
	ON_CBN_SELCHANGE(IDC_SETTINGSELECT, OnSelchangeSettings)
	ON_EN_CHANGE(IDC_ID, OnEditchangeID)
	ON_BN_CLICKED(IDC_ADDEDIT, OnBnClickedAddEdit)
	ON_BN_CLICKED(IDC_DELSETDATA, OnBnClickedDelData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingEditDlg message handlers

BOOL CSettingEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Settings Editor")));

	GetDlgItem(IDC_STATICSETTYPE)->SetWindowText(CMsg(_T("Setting Type")));
	GetDlgItem(IDC_ADDEDIT)->SetWindowText(CMsg(_T("Add / Edit Value")));
	GetDlgItem(IDC_DELSETDATA)->SetWindowText(CMsg(_T("Delete")));
	Axis->DBLng.CommitTransaction();

	CString csPath = CFrmt(_T("%1Settings"), Axis->m_csRootDirectory);
	dDatabase.Open(csPath);

	m_clcData.InsertColumn(0, _T("Setting"), LVCFMT_LEFT, 150, -1);
	m_clcData.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 400, -1);

	Table TBData = dDatabase.QuerySQL(_T("SELECT name FROM sqlite_master WHERE type = 'table' AND name LIKE 'Settings%'"));
	TBData.ResetRow();
	while (TBData.GoNext())
	{
		m_ccbSettings.AddString(TBData.GetValue(0));
	}
	m_ccbSettings.AddString(_T("MulPath"));
	m_ccbSettings.SetCurSel(0);

	OnSelchangeSettings();
	GetWindowRect(&MinSize);
	ScreenToClient(&MinSize);

	return TRUE;

}

void CSettingEditDlg::FillData()
{
	m_clcData.SetHotItem(-1);
	m_clcData.DeleteAllItems();

	Table TBData = dDatabase.QuerySQL(CFrmt(_T("SELECT * FROM %1 ORDER BY 1"), csTable));
	TBData.ResetRow();
	int iCount = 0;
	while (TBData.GoNext())
	{
		m_clcData.InsertItem(iCount, TBData.GetValue(0), iCount);
		m_clcData.SetItemText(iCount, 1, TBData.GetValue(1));
		iCount++;
	}
}

void CSettingEditDlg::OnSelchangeSettings()
{
	UpdateData(TRUE);

	int iSel = m_ccbSettings.GetCurSel();
	if (iSel == -1)
	{
		UpdateData(FALSE);
		return;
	}

	m_ccbSettings.GetLBText(iSel, csTable);
	FillData();

	UpdateData(FALSE);
}

void CSettingEditDlg::OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult)
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

void CSettingEditDlg::OnBnClickedAddEdit()
{
	// Save Info
	CString csID, csValue;
	csID = GetEditString(m_ceID);
	csValue = SQLStrip(GetEditString(m_ceValue));

	CString csCol = _T("ID");
	if (csTable == _T("MulPath"))
	{
		csCol = _T("Name");
	}

	if (csID != "")
	{
		Table TBData = dDatabase.QuerySQL(CFrmt(_T("SELECT * FROM %1 WHERE %3 = '%2'"), csTable, csID, csCol));
		if (TBData.GetRowCount() != 0)
		{
			dDatabase.ExecuteSQL(CFrmt(_T("UPDATE %1 SET Value = '%2' WHERE %4 = '%3'"), csTable, csValue, csID, csCol));
		}
		else
		{
			dDatabase.ExecuteSQL(CFrmt(_T("INSERT INTO %1 VALUES('%2','%3')"), csTable, csID, csValue));
		}

		FillData();
		m_clcData.SetHotItem(iSelIndex);
		m_clcData.SetItemState(iSelIndex, LVNI_SELECTED, LVIS_SELECTED);
		m_ceID.SetWindowText(_T(""));
		m_ceValue.SetWindowText(_T(""));
		m_cbDelData.EnableWindow(false);
	}
}

void CSettingEditDlg::OnBnClickedDelData()
{
	CString csCol = _T("ID");
	if (csTable == _T("MulPath"))
	{
		csCol = _T("Name");
	}
	dDatabase.ExecuteSQL(CFrmt(_T("DELETE FROM %1 WHERE %3 = '%2'"), csTable, GetEditString(m_ceID), csCol));
	m_ceID.SetWindowText(_T(""));
	m_ceValue.SetWindowText(_T(""));
	m_cbDelData.EnableWindow(false);
	FillData();
}

void CSettingEditDlg::OnEditchangeID()
{
	m_cbDelData.EnableWindow(false);
	UpdateData(FALSE);
}

void CSettingEditDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = MinSize.Width();
	lpMMI->ptMinTrackSize.y = MinSize.Height();

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}