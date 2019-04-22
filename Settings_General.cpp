#include "stdafx.h"
#include "Axis3.h"
#include "Settings_General.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSettingsGeneral::CSettingsGeneral(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingsGeneral::IDD, pParent)
{
	m_bAlwaysOnTop = GetSettingNum(_T("AlwaysOnTop"));
	m_bSysClose = GetSettingNum(_T("SysClose"));
	m_bSameAsClient = GetSettingNum(_T("MulSameAsClient"));
	m_bDevMode = GetSettingNum(_T("DevMode"));
}

void CSettingsGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_ALWAYS_ONTOP, m_bAlwaysOnTop);
	DDX_Check(pDX, IDC_SYSCLOSE, m_bSysClose);
	DDX_Check(pDX, IDC_AS_USESAMEPATHASCLIENT, m_bSameAsClient);
	DDX_Check(pDX, IDC_DEVMODE, m_bDevMode);
	DDX_Control(pDX, IDC_LANGSELECT, m_ccbLang);
	DDX_Control(pDX, IDC_PREFIX, m_ceCommandPrefix);
	DDX_Control(pDX, IDC_UOTITLE, m_ceUOTitle);
	DDX_Control(pDX, IDC_DEFAULTCLIENT, m_csDefaultClient);
	DDX_Control(pDX, IDC_DEFUALTMULPATH, m_csDefaultMulPath);
	DDX_Control(pDX, IDC_MULBROWSE, m_cbMulBrowse);

}


BEGIN_MESSAGE_MAP(CSettingsGeneral, CDialogEx)
	ON_BN_CLICKED(IDC_ALWAYS_ONTOP, OnAlwaysOnTop)
	ON_BN_CLICKED(IDC_SYSCLOSE, OnSysClose)
	ON_BN_CLICKED(IDC_AS_USESAMEPATHASCLIENT, OnSameAsClient)
	ON_BN_CLICKED(IDC_DEVMODE, OnDevMode)
	ON_BN_CLICKED(IDC_RESETSTING, OnResetSettings)
	ON_BN_CLICKED(IDC_CLIENTBROWSE, OnClientbrowse)
	ON_BN_CLICKED(IDC_MULBROWSE, OnMulBrowse)
	ON_EN_CHANGE(IDC_PREFIX, OnChangePrefix)
	ON_EN_CHANGE(IDC_UOTITLE, OnChangeUOTitle)
	ON_CBN_SELCHANGE(IDC_LANGSELECT, OnSelchangeLang)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneral message handlers

BOOL CSettingsGeneral::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_SETTING_GENERAL")));

	GetDlgItem(IDC_SYSCLOSE)->SetWindowText(CMsg(_T("IDS_SYSCLOSE")));
	GetDlgItem(IDC_ALWAYS_ONTOP)->SetWindowText(CMsg(_T("IDS_ONTOP")));
	GetDlgItem(IDC_STATIC_PREFIX)->SetWindowText(CMsg(_T("IDS_CMD_PREFIX")));
	GetDlgItem(IDC_STATIC_LANG)->SetWindowText(CMsg(_T("IDS_INTERFACE_LANG")));
	GetDlgItem(IDC_STATIC_DEFCLIENT)->SetWindowText(CMsg(_T("IDS_DEF_CLIENT")));
	GetDlgItem(IDC_STATIC_CLIENTTITLE)->SetWindowText(CMsg(_T("IDS_CUSTOM_CLIENT")));
	GetDlgItem(IDC_STATIC_MULPATH)->SetWindowText(CMsg(_T("IDS_MULPATH")));
	GetDlgItem(IDC_AS_USESAMEPATHASCLIENT)->SetWindowText(CMsg(_T("IDS_MUL_SAME_CLIENT")));
	GetDlgItem(IDC_RESETSTING)->SetWindowText(CMsg(_T("IDS_RESET_ALL_SETTINGS")));
	GetDlgItem(IDC_CLIENTBROWSE)->SetWindowText(CMsg(_T("IDS_BROWSE")));
	GetDlgItem(IDC_MULBROWSE)->SetWindowText(CMsg(_T("IDS_BROWSE")));
	GetDlgItem(IDC_DEVMODE)->SetWindowText(CMsg(_T("IDS_DEVMODE")));
	Axis->DBLng.CommitTransaction();
	
	Table TBData = Axis->DBLng.QuerySQL(_T("SELECT * FROM LangList ORDER BY Name"));
	TBData.ResetRow();
	CString csCurLang;
	while (TBData.GoNext())
	{
		m_ccbLang.AddString(TBData.GetValue(_T("Name")));
		if (TBData.GetValue(_T("Code")) == GetSettingString(_T("LangCode")))
		{
			csCurLang = TBData.GetValue(_T("Name"));
		}
	}
	m_ccbLang.SetWindowText(csCurLang);

	m_ceCommandPrefix.SetWindowText(GetSettingString(_T("CommandPrefix")));

	m_ceUOTitle.SetWindowText(GetSettingString(_T("UOTitle")));
	m_csDefaultClient.SetWindowText(GetSettingString(_T("UOClient")));
	m_csDefaultMulPath.SetWindowText(GetSettingString(_T("MulPath")));
	m_cbMulBrowse.EnableWindow(m_bSameAsClient ? false : true);
	m_csDefaultMulPath.EnableWindow(m_bSameAsClient ? false : true);

	UpdateData(false);

	return TRUE;
}

void CSettingsGeneral::OnAlwaysOnTop()
{
	UpdateData();
	SetSettingNum(_T("AlwaysOnTop"), m_bAlwaysOnTop);
	if (Axis->m_pMainWnd)
	{
		Axis->m_pMainWnd->SetWindowPos(m_bAlwaysOnTop ? &wndTopMost : &wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CSettingsGeneral::OnSysClose()
{
	UpdateData();
	SetSettingNum(_T("SysClose"), m_bSysClose);
}

void CSettingsGeneral::OnDevMode()
{
	UpdateData();
	SetSettingNum(_T("DevMode"), m_bDevMode);
}

void CSettingsGeneral::OnSameAsClient()
{
	UpdateData();
	SetSettingNum(_T("MulSameAsClient"), m_bSameAsClient);

	CString csUOPath;
	m_csDefaultClient.GetWindowText(csUOPath);
	CString csMulPath = csUOPath.Left(csUOPath.ReverseFind('\\') + 1);
	SetSettingString(_T("MulPath"), csMulPath);
	m_csDefaultMulPath.SetWindowText(csMulPath);

	m_cbMulBrowse.EnableWindow(m_bSameAsClient ? false : true);
	m_csDefaultMulPath.EnableWindow(m_bSameAsClient ? false : true);
	
	//Reload
	UnLoadUOPArtData();
	UnLoadHues();
	UnLoadBodyDef();
	UnLoadBodyConvert();
	Axis->m_staticdata.RemoveAll();

	AfxBeginThread(LoadUOPArtData, nullptr);
	AfxBeginThread(LoadHues, nullptr);
	AfxBeginThread(LoadBodyDef, nullptr);
	AfxBeginThread(LoadBodyConvert, nullptr);
	AfxBeginThread(LoadRadarcol, nullptr);
	AfxBeginThread(LoadTiledata, nullptr);
	DetectMapFormat();
}

void CSettingsGeneral::OnSelchangeLang() 
{
	UpdateData();
	CString csLang;
	m_ccbLang.GetWindowText(csLang);
	Table TBData = Axis->DBLng.QuerySQL(CSQL(_T("SELECT * FROM LangList WHERE Name = %1"),true, csLang));
	TBData.ResetRow();
	if (TBData.GoNext())
	{
		SetSettingString(_T("LangCode"), TBData.GetValue(_T("Code")));
	}
	AfxMessageBox(CMsg(_T("IDS_EFFECT_RESTART")));
}

void CSettingsGeneral::OnChangePrefix()
{
	CString csPrefix;
	m_ceCommandPrefix.GetWindowText(csPrefix);
	SetSettingString(_T("CommandPrefix"), csPrefix);
}

void CSettingsGeneral::OnChangeUOTitle()
{
	CString csTitle;
	m_ceUOTitle.GetWindowText(csTitle);
	SetSettingString(_T("UOTitle"), csTitle);
}

void CSettingsGeneral::OnResetSettings()
{
	if (AfxMessageBox(CMsg(_T("IDS_CONFIRM_RESET_ALL")), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return;

	Axis->DBSettings.ExecuteSQL(CSQL(_T("DELETE FROM Settings_Profile_%1 WHERE ID != 'UOClient' AND ID != 'MulPath' AND ID != 'MulSameAsClient'"), true, Axis->csProfile));
	AfxMessageBox(CMsg(_T("IDS_EFFECT_RESTART")));
}

void CSettingsGeneral::OnClientbrowse()
{
	CFileDialog dlg(true, _T("exe"), _T(""), 0, _T("UO Client |client.exe|Application files (*.exe)|*.exe|All files (*.*)|*.*||"), nullptr);
	if (dlg.DoModal() == IDOK)
	{
		CString csUOPath = dlg.GetPathName();
		SetSettingString(_T("UOClient"), csUOPath);
		m_csDefaultClient.SetWindowText(csUOPath);
		if (m_bSameAsClient)
		{
			CString csMulPath = csUOPath.Left(csUOPath.ReverseFind('\\') + 1);
			SetSettingString(_T("MulPath"), csMulPath);
			m_csDefaultMulPath.SetWindowText(csMulPath);

			//Reload
			UnLoadUOPArtData();
			UnLoadHues();
			UnLoadBodyDef();
			UnLoadBodyConvert();
			Axis->m_staticdata.RemoveAll();

			AfxBeginThread(LoadUOPArtData, nullptr);
			AfxBeginThread(LoadHues, nullptr);
			AfxBeginThread(LoadBodyDef, nullptr);
			AfxBeginThread(LoadBodyConvert, nullptr);
			AfxBeginThread(LoadRadarcol, nullptr);
			AfxBeginThread(LoadTiledata, nullptr);
			DetectMapFormat();
		}
	}
}

void CSettingsGeneral::OnMulBrowse()
{
	CFolderPickerDialog dlg;
	dlg.m_ofn.lpstrTitle = _T("Path To .mul/.uop UO files");
	dlg.m_ofn.lpstrInitialDir = _T("C:\\");
	if (dlg.DoModal() == IDOK)
	{
		CString csMulPath = dlg.GetPathName();
		csMulPath += _T("\\");
		SetSettingString(_T("MulPath"), csMulPath);
		m_csDefaultMulPath.SetWindowText(csMulPath);

		//Reload
		UnLoadUOPArtData();
		UnLoadHues();
		UnLoadBodyDef();
		UnLoadBodyConvert();
		Axis->m_staticdata.RemoveAll();

		AfxBeginThread(LoadUOPArtData, nullptr);
		AfxBeginThread(LoadHues, nullptr);
		AfxBeginThread(LoadBodyDef, nullptr);
		AfxBeginThread(LoadBodyConvert, nullptr);
		AfxBeginThread(LoadRadarcol, nullptr);
		AfxBeginThread(LoadTiledata, nullptr);
		DetectMapFormat();
	}
}