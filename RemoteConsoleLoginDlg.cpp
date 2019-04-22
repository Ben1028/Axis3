#include "stdafx.h"
#include "Axis3.h"
#include "RemoteConsoleLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRemoteConsoleLoginDlg::CRemoteConsoleLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteConsoleLoginDlg::IDD, pParent)
{
	m_iSel = 0;
}


void CRemoteConsoleLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACCTNAME, m_ccbName);
	DDX_Control(pDX, IDC_ACCT, m_ceAccount);
	DDX_Control(pDX, IDC_IPADDRESS, m_ceAddress);
	DDX_Control(pDX, IDC_PORT, m_cePort);
	DDX_Control(pDX, IDC_PWD, m_cePassword);
}


BEGIN_MESSAGE_MAP(CRemoteConsoleLoginDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ACCTNAME, OnSelchangeAcctname)
	ON_CBN_EDITCHANGE(IDC_ACCTNAME, OnEditchangeAcctname)
	ON_BN_CLICKED(IDC_CANCEL, OnCloseDlg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleLoginDlg message handlers

BOOL CRemoteConsoleLoginDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_REMOTE_CONSOLE_LOGIN")));

	GetDlgItem(IDC_SNAME)->SetWindowText(CMsg(_T("IDS_NAME")));
	GetDlgItem(IDC_SIP)->SetWindowText(CMsg(_T("IDS_IP_ADDRESS")));
	GetDlgItem(IDC_SACCT)->SetWindowText(CMsg(_T("IDS_ACCOUNT")));
	GetDlgItem(IDC_SPASS)->SetWindowText(CMsg(_T("IDS_PASSWORD")));
	GetDlgItem(IDC_SPORT)->SetWindowText(CMsg(_T("IDS_PORT")));
	GetDlgItem(IDOK)->SetWindowText(CMsg(_T("IDS_OK")));
	GetDlgItem(IDC_CANCEL)->SetWindowText(CMsg(_T("IDS_CANCEL")));
	Axis->DBLng.CommitTransaction();

	//Fill Combobox
	Table TBRCAccount = Axis->DBSettings.QuerySQL(_T("SELECT * FROM RemoteConsole"));
	TBRCAccount.ResetRow();
	while(TBRCAccount.GoNext())
		m_ccbName.AddString(TBRCAccount.GetValue(_T("Name")));

	//Set to last selected account
	CString csLast = GetSettingString(_T("LastRemoteLoginAccount"));
	if ( csLast.IsEmpty() )
		m_ccbName.SetCurSel(-1);
	else
		m_ccbName.SelectString(0, csLast);
	OnSelchangeAcctname();

	UpdateData(FALSE);	
	return TRUE;
}

void CRemoteConsoleLoginDlg::OnOK() 
{
	// Save Info
	UpdateData(TRUE);
	DWORD dwAddress;
	m_ceAddress.GetAddress(dwAddress);

	csAddress = GetEditString(m_ceAddress);
	csPort = GetEditString(m_cePort);
	csAccount = GetEditString(m_ceAccount);
	csPassword = GetEditString(m_cePassword);
	csAccountName = GetEditString(m_ccbName);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM RemoteConsole WHERE Name = '%1'"),csAccountName));
	if(TBRCAccount.GetRowCount() != 0)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("UPDATE RemoteConsole SET IPAddress = '%1!d!', Port = '%2', Account = '%3', Password = '%4' WHERE Name = '%5'"),dwAddress,csPort,csAccount,Encrypt(csPassword),csAccountName));
	else
		Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO RemoteConsole VALUES('%1','%2!d!','%3','%4','%5')"),csAccountName,dwAddress,csPort,csAccount,Encrypt(csPassword)));

	SetSettingString(_T("LastRemoteLoginAccount"), csAccountName);

	CDialogEx::OnOK();
}

void CRemoteConsoleLoginDlg::OnCloseDlg() 
{
	CDialogEx::OnCancel();
}

void CRemoteConsoleLoginDlg::OnSelchangeAcctname() 
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
	CString csAccountName;
	m_ccbName.GetLBText(m_iSel, csAccountName);

	Table TBRCAccount = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM RemoteConsole WHERE Name = '%1'"),csAccountName));
	m_ceAddress.SetAddress(_ttoi(TBRCAccount.GetValue(_T("IPAddress"))));
	m_cePort.SetWindowText(TBRCAccount.GetValue(_T("Port")));
	m_ceAccount.SetWindowText(TBRCAccount.GetValue(_T("Account")));
	m_cePassword.SetWindowText(Encrypt(TBRCAccount.GetValue(_T("Password"))));

	SetSettingString(_T("LastRemoteLoginAccount"), csAccountName);
	UpdateData(FALSE);
}

void CRemoteConsoleLoginDlg::OnEditchangeAcctname() 
{
	m_ceAccount.SetWindowText(_T(""));
	m_cePassword.SetWindowText(_T(""));
	m_ceAddress.SetAddress(0,0,0,0);
	m_cePort.SetWindowText(_T(""));
	UpdateData(FALSE);
	m_iSel = -1;
}
