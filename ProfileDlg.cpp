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
}


void CProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCTNAME, m_ccbName);
	DDX_Control(pDX, IDC_ACCT, m_ceAccount);
	DDX_Control(pDX, IDC_IPADDRESS, m_ceAddress);
	DDX_Control(pDX, IDC_PORT, m_cePort);
	DDX_Control(pDX, IDC_PWD, m_cePassword);
}


BEGIN_MESSAGE_MAP(CProfileDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_ACCTNAME, OnSelchangeAcctname)
	ON_CBN_EDITCHANGE(IDC_ACCTNAME, OnEditchangeAcctname)
	ON_BN_CLICKED(IDC_CANCEL, OnDelete)
	ON_BN_CLICKED(IDC_SELECT, OnSelectProfile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProfileDlg message handlers

BOOL CProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

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
	while(TBRCAccount.GoNext())
		m_ccbName.AddString(TBRCAccount.GetValue(_T("Name")));

	//Set to last selected account
	CString csLast = GetSettingString(_T("Profile"));
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
	m_ceAddress.GetWindowText(csAddress);
	m_cePort.GetWindowText(csPort);
	m_ceAccount.GetWindowText(csAccount);
	m_cePassword.GetWindowText(csPassword);
	m_ccbName.GetWindowText(csAccountName);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Profiles WHERE Name = '%1'"),csAccountName));
	if(TBRCAccount.GetRowCount() != 0)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("UPDATE Profiles SET IPAddress = '%1!d!', Port = '%2', Account = '%3', Password = '%4' WHERE Name = '%5'"),dwAddress,csPort,csAccount,Encrypt(csPassword),csAccountName));
	else
		Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO Profiles VALUES('%1','%2!d!','%3','%4','%5')"),csAccountName,dwAddress,csPort,csAccount,Encrypt(csPassword)));

	SetDefaultValue(_T("Profile"), csAccountName);
	Axis->csProfile = csAccountName;
	Axis->DBSettings.ExecuteSQL(CSQL(_T("CREATE TABLE IF NOT EXISTS 'Settings_%1' (ID TEXT, Value TEXT)"),csAccountName));

	CDialogEx::OnOK();
}

void CProfileDlg::OnSelectProfile()
{
	m_ccbName.GetWindowText(csAccountName);
	SetDefaultValue(_T("Profile"), csAccountName);
	Axis->csProfile = csAccountName;
	Axis->DBData.Close();
	Axis->DBData.Open(CMsg(_T("Data_%1.db"),true,csAccountName));
	CDialogEx::OnCancel();
}

void CProfileDlg::OnDelete()
{
	if (csAccountName == "Default")
	{
		AfxMessageBox(CMsg(_T("IDS_NO_DEL_DEF")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (csAccountName == Axis->csProfile)
	{
		AfxMessageBox(CMsg(_T("IDS_NO_DEL_LOADED")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	Axis->DBSettings.ExecuteSQL(CSQL(_T("DELETE FROM Profiles WHERE Name = '%1'"),csAccountName));
	Axis->DBSettings.ExecuteSQL(CSQL(_T("DROP TABLE Settings_%1"),csAccountName));
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
		UpdateData(FALSE);
		return;
	}
	m_ccbName.GetLBText(m_iSel, csAccountName);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Profiles WHERE Name = '%1'"),csAccountName));
	m_ceAddress.SetAddress(_ttoi(TBRCAccount.GetValue(_T("IPAddress"))));
	m_cePort.SetWindowText(TBRCAccount.GetValue(_T("Port")));
	m_ceAccount.SetWindowText(TBRCAccount.GetValue(_T("Account")));
	m_cePassword.SetWindowText(Encrypt(TBRCAccount.GetValue(_T("Password"))));

	CFile fData;
	if ( fData.Open(CMsg(_T("Data_%1.db"),true,csAccountName), CFile::modeRead | CFile::shareDenyNone) )
	{
		fData.Close();
		GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_RELOAD")));
		GetDlgItem(IDC_SELECT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_LOAD")));
		GetDlgItem(IDC_SELECT)->EnableWindow(false);
	}

	UpdateData(FALSE);
}

void CProfileDlg::OnEditchangeAcctname() 
{
	m_ceAccount.SetWindowText(_T(""));
	m_cePassword.SetWindowText(_T(""));
	m_ceAddress.SetAddress(0,0,0,0);
	m_cePort.SetWindowText(_T(""));
	UpdateData(FALSE);
	m_iSel = -1;
	csAccountName = " ";
}
