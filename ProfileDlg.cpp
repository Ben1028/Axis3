#include "stdafx.h"
#include "Axis3.h"
#include "ProfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProfileDlg::CProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProfileDlg::IDD, pParent)
{
	m_iSel = 0;
	m_bLocal = false;
	m_bSelect = false;
}


void CProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCTNAME, m_ccbName);
	DDX_Control(pDX, IDC_ACCT, m_ceAccount);
	DDX_Control(pDX, IDC_IPADDRESS, m_ceAddress);
	DDX_Control(pDX, IDC_PORT, m_cePort);
	DDX_Control(pDX, IDC_PWD, m_cePassword);
	DDX_Control(pDX, IDC_CANCEL, m_cbDelete);
	DDX_Control(pDX, IDC_SELECT, m_cbSelect);
	DDX_Check(pDX, IDC_LOCAL, m_bLocal);
	DDX_Control(pDX, IDC_BROWSE, m_cbBrowse);
	DDX_Control(pDX, IDC_SPHERETABLE, m_csSphereTables);
}


BEGIN_MESSAGE_MAP(CProfileDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ACCTNAME, OnSelchangeAcctname)
	ON_CBN_EDITCHANGE(IDC_ACCTNAME, OnEditchangeAcctname)
	ON_BN_CLICKED(IDC_CANCEL, OnDelete)
	ON_BN_CLICKED(IDC_SELECT, OnSelectProfile)
	ON_BN_CLICKED(IDC_LOCAL, OnLocal)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg message handlers

BOOL CProfileDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_PROFILE_SETTINGS")));

	GetDlgItem(IDC_SNAME)->SetWindowText(CMsg(_T("IDS_NAME")));
	GetDlgItem(IDC_SIP)->SetWindowText(CMsg(_T("IDS_IP_ADDRESS")));
	GetDlgItem(IDC_SACCT)->SetWindowText(CMsg(_T("IDS_ACCOUNT")));
	GetDlgItem(IDC_SPASS)->SetWindowText(CMsg(_T("IDS_PASSWORD")));
	GetDlgItem(IDC_SPORT)->SetWindowText(CMsg(_T("IDS_PORT")));
	GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_LOAD")));
	GetDlgItem(IDC_CANCEL)->SetWindowText(CMsg(_T("IDS_DELETE")));
	GetDlgItem(IDC_SELECT)->SetWindowText(CMsg(_T("IDS_SELECT")));
	Axis->DBLng.CommitTransaction();

	//Fill Combobox
	Table TBRCAccount = Axis->DBSettings.QuerySQL(_T("SELECT * FROM Profiles"));
	TBRCAccount.ResetRow();
	while (TBRCAccount.GoNext())
	{
		m_ccbName.AddString(TBRCAccount.GetValue(_T("Name")));
	}

	//Set to last selected account
	CString csLast = GetDefaultString(_T("Profile"));
	if ( csLast.IsEmpty() )
		m_ccbName.SetCurSel(-1);
	else
		m_ccbName.SelectString(0, csLast);
	OnSelchangeAcctname();

	UpdateData(FALSE);
	return TRUE;

}

void CProfileDlg::OnOK()
{
	// Save Info
	UpdateData(TRUE);
	DWORD dwAddress;
	m_ceAddress.GetAddress(dwAddress);
	csAddress = GetEditString(m_ceAddress);
	csPort = GetEditString(m_cePort);
	csAccount = GetEditString(m_ceAccount);
	csPassword = Encrypt(GetEditString(m_cePassword));
	csAccountName = GetEditString(m_ccbName);
	csPath = GetEditString(m_csSphereTables);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Profiles WHERE Name = '%1'"), csAccountName));
	if(TBRCAccount.GetRowCount() != 0)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("UPDATE Profiles SET IPAddress = '%1!d!', Port = '%2', Account = '%3', Password = '%4', Path = '%5', Type = '%6!d!' WHERE Name = '%7'"),dwAddress,csPort,csAccount,csPassword,csPath,m_bLocal,csAccountName));
	else
		Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO Profiles VALUES('%1','%2!d!','%3','%4','%5','%6','%7!d!')"), csAccountName,dwAddress,csPort,csAccount,csPassword, csPath, m_bLocal));

	SetDefaultString(_T("Profile"), csAccountName);
	Axis->csProfile = csAccountName;
	Axis->DBSettings.ExecuteSQL(CSQL(_T("CREATE TABLE IF NOT EXISTS 'Settings_Profile_%1' (ID TEXT, Value TEXT)"),csAccountName));

	CreateDirectoryTree(csAccountName);

	CDialogEx::OnOK();
}

void CProfileDlg::OnSelectProfile()
{
	csAccountName = GetEditString(m_ccbName);
	SetDefaultString(_T("Profile"), csAccountName);
	Axis->csProfile = csAccountName;
	Axis->DBData.Close();
	Axis->DBData.Open(CMsg(_T("%1/Data.db"),true,csAccountName));
	m_bSelect = true;
	CDialogEx::OnCancel();
}

void CProfileDlg::OnDelete()
{
	Axis->DBSettings.ExecuteSQL(CSQL(_T("DELETE FROM Profiles WHERE Name = '%1'"),csAccountName));
	Axis->DBSettings.ExecuteSQL(CSQL(_T("DROP TABLE Settings_Profile_%1"),csAccountName));
	DeleteDirectoryTree(csAccountName);
	CDialogEx::OnCancel();
}

void CProfileDlg::OnSelchangeAcctname() 
{
	UpdateData(TRUE);
	m_iSel = m_ccbName.GetCurSel();
	if ( m_iSel == -1 )
	{
		m_ceAccount.SetWindowText(_T(""));
		m_cePassword.SetWindowText(_T(""));
		m_ceAddress.SetAddress(0,0,0,0);
		m_cePort.SetWindowText(_T(""));
		m_csSphereTables.SetWindowText(_T(""));
		m_bLocal = false;
		UpdateData(FALSE);
		return;
	}
	m_ccbName.GetLBText(m_iSel, csAccountName);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Profiles WHERE Name = '%1'"), csAccountName));
	m_ceAddress.SetAddress(_ttoi(TBRCAccount.GetValue(_T("IPAddress"))));
	m_cePort.SetWindowText(TBRCAccount.GetValue(_T("Port")));
	m_ceAccount.SetWindowText(TBRCAccount.GetValue(_T("Account")));
	m_cePassword.SetWindowText(Encrypt(TBRCAccount.GetValue(_T("Password"))));
	m_csSphereTables.SetWindowText(TBRCAccount.GetValue(_T("Path")));
	m_bLocal = _ttoi(TBRCAccount.GetValue(_T("Type")));
	if ((csAccountName == "Default") || (csAccountName == Axis->csProfile))
	{
		m_cbDelete.EnableWindow(false);
	}
	else 
	{
		m_cbDelete.EnableWindow(true);
	}

	CFile fData;
	if ( fData.Open(CMsg(_T("%1/Data.db"),true,csAccountName), CFile::modeRead | CFile::shareDenyNone) )
	{
		fData.Close();
		GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_RELOAD")));
		m_cbSelect.EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_LOAD")));
		m_cbSelect.EnableWindow(false);
	}
	if (m_bLocal)
	{
		m_csSphereTables.EnableWindow(true);
		m_cbBrowse.EnableWindow(true);
		m_cePort.EnableWindow(false);
		m_ceAccount.EnableWindow(false);
		m_cePassword.EnableWindow(false);
		m_ceAddress.EnableWindow(false);
	}
	else
	{
		m_csSphereTables.EnableWindow(false);
		m_cbBrowse.EnableWindow(false);
		m_cePort.EnableWindow(true);
		m_ceAccount.EnableWindow(true);
		m_cePassword.EnableWindow(true);
		m_ceAddress.EnableWindow(true);
	}
	UpdateData(FALSE);
}

void CProfileDlg::OnEditchangeAcctname() 
{
	m_ceAccount.SetWindowText(_T(""));
	m_cePassword.SetWindowText(_T(""));
	m_ceAddress.SetAddress(0,0,0,0);
	m_cePort.SetWindowText(_T(""));
	m_csSphereTables.SetWindowText(_T(""));
	m_bLocal = false;
	m_csSphereTables.EnableWindow(false);
	m_cbBrowse.EnableWindow(false);
	m_cePort.EnableWindow(true);
	m_ceAccount.EnableWindow(true);
	m_cePassword.EnableWindow(true);
	m_ceAddress.EnableWindow(true);
	UpdateData(FALSE);
	m_iSel = -1;
	csAccountName = " ";
	m_cbSelect.EnableWindow(false);
	GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_LOAD")));
	m_cbDelete.EnableWindow(false);
}

void CProfileDlg::OnLocal()
{
	UpdateData();
	if (m_bLocal)
	{
		m_csSphereTables.EnableWindow(true);
		m_cbBrowse.EnableWindow(true);
		m_cePort.EnableWindow(false);
		m_ceAccount.EnableWindow(false);
		m_cePassword.EnableWindow(false);
		m_ceAddress.EnableWindow(false);
	}
	else
	{
		m_csSphereTables.EnableWindow(false);
		m_cbBrowse.EnableWindow(false);
		m_cePort.EnableWindow(true);
		m_ceAccount.EnableWindow(true);
		m_cePassword.EnableWindow(true);
		m_ceAddress.EnableWindow(true);
	}
}

void CProfileDlg::OnBrowse()
{
	CFileDialog dlg(true, _T("scp"), _T(""), 0, _T("SphereTables |spheretables.scp||"), nullptr);
	if (dlg.DoModal() == IDOK)
	{
		CString csTablePath = dlg.GetPathName();
		m_csSphereTables.SetWindowText(csTablePath);
	}
}