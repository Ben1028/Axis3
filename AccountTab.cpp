
#include "stdafx.h"
#include "Axis3.h"
#include "AccountTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CAccountTab, CDialogPage)

CAccountTab::CAccountTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CAccountTab::IDD, AfxGetApp()->LoadIcon(IDI_ACCOUNT), pParent)
{
	iPrivLevel = 0;
	iResdisp = 0;
	dwPrivs = 0;
}


CAccountTab::~CAccountTab()
{
}

void CAccountTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRIVVALUE, m_csPrivValue);
}


BEGIN_MESSAGE_MAP(CAccountTab, CDialogPage)
	ON_BN_CLICKED(IDC_PLV_0, OnGuestprivs)
	ON_BN_CLICKED(IDC_PLV_1, OnPlayerprivs)
	ON_BN_CLICKED(IDC_PLV_2, OnCounselorprivs)
	ON_BN_CLICKED(IDC_PLV_3, OnSeerprivs)
	ON_BN_CLICKED(IDC_PLV_4, OnGmprivs)
	ON_BN_CLICKED(IDC_PLV_5, OnDeveloperprivs)
	ON_BN_CLICKED(IDC_PLV_6, OnAdministratorprivs)
	ON_BN_CLICKED(IDC_PLV_7, OnOwnerprivs)
	ON_BN_CLICKED(IDC_SET_PLV, OnSetPLV)

	ON_BN_CLICKED(IDC_RDSP_0, OnResdisp0)
	ON_BN_CLICKED(IDC_RDSP_1, OnResdisp1)
	ON_BN_CLICKED(IDC_RDSP_2, OnResdisp2)
	ON_BN_CLICKED(IDC_RDSP_3, OnResdisp3)
	ON_BN_CLICKED(IDC_RDSP_4, OnResdisp4)
	ON_BN_CLICKED(IDC_RDSP_5, OnResdisp5)
	ON_BN_CLICKED(IDC_RDSP_6, OnResdisp6)
	ON_BN_CLICKED(IDC_RDSP_7, OnResdisp7)
	ON_BN_CLICKED(IDC_RDSP_8, OnResdisp8)
	ON_BN_CLICKED(IDC_RDSP_9, OnResdisp9)
	ON_BN_CLICKED(IDC_SET_RESDISP, OnSetresdisp)

	ON_BN_CLICKED(IDC_PRIV02, OnPriv02)
	ON_BN_CLICKED(IDC_PRIV08, OnPriv08)
	ON_BN_CLICKED(IDC_PRIV010, OnPriv010)
	ON_BN_CLICKED(IDC_PRIV020, OnPriv020)
	ON_BN_CLICKED(IDC_PRIV040, OnPriv040)
	ON_BN_CLICKED(IDC_PRIV080, OnPriv080)
	ON_BN_CLICKED(IDC_PRIV0200, OnPriv0200)
	ON_BN_CLICKED(IDC_PRIV0400, OnPriv0400)
	ON_BN_CLICKED(IDC_PRIV0800, OnPriv0800)
	ON_BN_CLICKED(IDC_PRIV02000, OnPriv02000)
	ON_BN_CLICKED(IDC_PRIV04000, OnPriv04000)
	ON_BN_CLICKED(IDC_SET_PRIVS, OnSetprivs)
	ON_BN_CLICKED(IDC_RESET_PRIVS, OnResetPrivs)

END_MESSAGE_MAP()

BOOL CAccountTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_ACCOUNT")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	initialized = TRUE;

	return TRUE;
}

void CAccountTab::SetLanguage()
{
	GetDlgItem(IDC_GR_PLEVEL)->SetWindowText(CMsg(_T("IDS_PLEVEL")));
	GetDlgItem(IDC_PLV_0)->SetWindowText(CMsg(_T("IDS_GUEST")));
	GetDlgItem(IDC_PLV_1)->SetWindowText(CMsg(_T("IDS_PLAYERNPC")));
	GetDlgItem(IDC_PLV_2)->SetWindowText(CMsg(_T("IDS_SEER")));
	GetDlgItem(IDC_PLV_3)->SetWindowText(CMsg(_T("IDS_COUNSELOR")));
	GetDlgItem(IDC_PLV_4)->SetWindowText(CMsg(_T("IDS_GAMEMASTER")));
	GetDlgItem(IDC_PLV_5)->SetWindowText(CMsg(_T("IDS_DEVELOPER")));
	GetDlgItem(IDC_PLV_6)->SetWindowText(CMsg(_T("IDS_ADMINISTRATOR")));
	GetDlgItem(IDC_PLV_7)->SetWindowText(CMsg(_T("IDS_OWNER")));
	GetDlgItem(IDC_SET_PLV)->SetWindowText(CMsg(_T("IDS_SET_PLVL")));

	GetDlgItem(IDC_GR_RESDISP)->SetWindowText(CMsg(_T("IDS_RESDISP")));
	GetDlgItem(IDC_RDSP_0)->SetWindowText(CMsg(_T("IDS_PRET2A")));
	GetDlgItem(IDC_RDSP_1)->SetWindowText(CMsg(_T("IDS_T2A")));
	GetDlgItem(IDC_RDSP_2)->SetWindowText(CMsg(_T("IDS_LBR")));
	GetDlgItem(IDC_RDSP_3)->SetWindowText(CMsg(_T("IDS_AOS")));
	GetDlgItem(IDC_RDSP_4)->SetWindowText(CMsg(_T("IDS_SE")));
	GetDlgItem(IDC_RDSP_5)->SetWindowText(CMsg(_T("IDS_ML")));
	GetDlgItem(IDC_RDSP_6)->SetWindowText(CMsg(_T("IDS_KR")));
	GetDlgItem(IDC_RDSP_7)->SetWindowText(CMsg(_T("IDS_SA")));
	GetDlgItem(IDC_RDSP_8)->SetWindowText(CMsg(_T("IDS_HS")));
	GetDlgItem(IDC_RDSP_9)->SetWindowText(CMsg(_T("IDS_TOL")));
	GetDlgItem(IDC_SET_RESDISP)->SetWindowText(CMsg(_T("IDS_SET_RESDISP")));

	GetDlgItem(IDC_GR_PRIVS)->SetWindowText(CMsg(_T("IDS_PRIVFLAGS")));
	GetDlgItem(IDC_PRIV02)->SetWindowText(CMsg(_T("IDS_PRIV02")));
	GetDlgItem(IDC_PRIV08)->SetWindowText(CMsg(_T("IDS_PRIV08")));
	GetDlgItem(IDC_PRIV010)->SetWindowText(CMsg(_T("IDS_PRIV010")));
	GetDlgItem(IDC_PRIV020)->SetWindowText(CMsg(_T("IDS_PRIV020")));
	GetDlgItem(IDC_PRIV040)->SetWindowText(CMsg(_T("IDS_PRIV040")));
	GetDlgItem(IDC_PRIV080)->SetWindowText(CMsg(_T("IDS_PRIV080")));
	GetDlgItem(IDC_PRIV0200)->SetWindowText(CMsg(_T("IDS_PRIV0200")));
	GetDlgItem(IDC_PRIV0400)->SetWindowText(CMsg(_T("IDS_PRIV0400")));
	GetDlgItem(IDC_PRIV0800)->SetWindowText(CMsg(_T("IDS_PRIV0800")));
	GetDlgItem(IDC_PRIV02000)->SetWindowText(CMsg(_T("IDS_PRIV02000")));
	GetDlgItem(IDC_PRIV04000)->SetWindowText(CMsg(_T("IDS_PRIV04000")));
	GetDlgItem(IDC_RESET_PRIVS)->SetWindowText(CMsg(_T("IDS_RESET_PRIVS")));
	GetDlgItem(IDC_SET_PRIVS)->SetWindowText(CMsg(_T("IDS_SET_PRIVS")));
}

void CAccountTab::OnSetPLV() 
{
	SendToUO(CCmd(_T("privset %1!d!"),true, iPrivLevel));
}

void CAccountTab::OnOwnerprivs() 
{
	iPrivLevel = 7;	
}

void CAccountTab::OnAdministratorprivs() 
{
	iPrivLevel = 6;	
}

void CAccountTab::OnDeveloperprivs() 
{
	iPrivLevel = 5;	
}

void CAccountTab::OnGmprivs() 
{
	iPrivLevel = 4;	
}

void CAccountTab::OnSeerprivs() 
{
	iPrivLevel = 3;	
}

void CAccountTab::OnCounselorprivs() 
{
	iPrivLevel = 2;	
}

void CAccountTab::OnPlayerprivs() 
{
	iPrivLevel = 1;	
}

void CAccountTab::OnGuestprivs() 
{
	iPrivLevel = 0;	
}

void CAccountTab::OnResdisp0()
{
	iResdisp = 0;
}

void CAccountTab::OnResdisp1()
{
	iResdisp = 1;
}

void CAccountTab::OnResdisp2()
{
	iResdisp = 2;
}

void CAccountTab::OnResdisp3()
{
	iResdisp = 3;
}

void CAccountTab::OnResdisp4()
{
	iResdisp = 4;
}

void CAccountTab::OnResdisp5()
{
	iResdisp = 5;
}

void CAccountTab::OnResdisp6()
{
	iResdisp = 6;
}

void CAccountTab::OnResdisp7()
{
	iResdisp = 7;
}

void CAccountTab::OnResdisp8()
{
	iResdisp = 8;
}

void CAccountTab::OnResdisp9()
{
	iResdisp = 9;
}

void CAccountTab::OnSetresdisp() 
{
	SendToUO(CCmd(_T("set account.resdisp %1!d!"),true, iResdisp));
}

void CAccountTab::OnPriv02()
{
	if (dwPrivs & 0x02)
		dwPrivs ^= 0x02;
	else
		dwPrivs |= 0x02;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv08()
{
	if (dwPrivs & 0x08)
		dwPrivs ^= 0x08;
	else
		dwPrivs |= 0x08;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv010()
{
	if (dwPrivs & 0x010)
		dwPrivs ^= 0x010;
	else
		dwPrivs |= 0x010;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv020()
{
	if (dwPrivs & 0x020)
		dwPrivs ^= 0x020;
	else
		dwPrivs |= 0x020;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv040()
{
	if (dwPrivs & 0x040)
		dwPrivs ^= 0x040;
	else
		dwPrivs |= 0x040;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv080()
{
	if (dwPrivs & 0x080)
		dwPrivs ^= 0x080;
	else
		dwPrivs |= 0x080;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv0200()
{
	if (dwPrivs & 0x0200)
		dwPrivs ^= 0x0200;
	else
		dwPrivs |= 0x0200;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv0400()
{
	if (dwPrivs & 0x0400)
		dwPrivs ^= 0x0400;
	else
		dwPrivs |= 0x0400;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv0800()
{
	if (dwPrivs & 0x0800)
		dwPrivs ^= 0x0800;
	else
		dwPrivs |= 0x0800;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv02000()
{
	if (dwPrivs & 0x02000)
		dwPrivs ^= 0x02000;
	else
		dwPrivs |= 0x02000;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnPriv04000()
{
	if (dwPrivs & 0x04000)
		dwPrivs ^= 0x04000;
	else
		dwPrivs |= 0x04000;
	m_csPrivValue.SetWindowText(CMsg(_T("%1!05x!"),true, dwPrivs));
}

void CAccountTab::OnSetprivs()
{
	SendToUO(CCmd(_T("set account.priv %1!05x!"),true, dwPrivs));
}

void CAccountTab::OnResetPrivs()
{
	CheckDlgButton(IDC_PRIV02, 0);
	CheckDlgButton(IDC_PRIV08, 0);
	CheckDlgButton(IDC_PRIV010, 0);
	CheckDlgButton(IDC_PRIV020, 0);
	CheckDlgButton(IDC_PRIV040, 0);
	CheckDlgButton(IDC_PRIV080, 0);
	CheckDlgButton(IDC_PRIV0200, 0);
	CheckDlgButton(IDC_PRIV0400, 0);
	CheckDlgButton(IDC_PRIV080, 0);
	CheckDlgButton(IDC_PRIV02000, 0);
	CheckDlgButton(IDC_PRIV04000, 0);
	dwPrivs = 0;
	m_csPrivValue.SetWindowText(_T("00000"));
}