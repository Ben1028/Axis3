
#include "stdafx.h"
#include "Axis3.h"
#include "GeneralTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGeneralTab, CDialogPage)

CGeneralTab::CGeneralTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CGeneralTab::IDD, AfxGetApp()->LoadIcon(IDI_GENERAL), pParent)
{
}


CGeneralTab::~CGeneralTab()
{
}

void CGeneralTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHT_SLIDE, m_cscSlider);
	DDX_Control(pDX, IDC_SLIDERVALUE, m_csSliderValue);
	DDX_Control(pDX, IDC_REMCONSOLE, m_cbRemoteConsole);
}


BEGIN_MESSAGE_MAP(CGeneralTab, CDialogPage)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_REMCONSOLE, OnRconsole)
	ON_BN_CLICKED(IDC_ADMIN, OnBnClickedAdmin)
	ON_BN_CLICKED(IDC_INFO, OnBnClickedInfo)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_CLIENTS, OnBnClickedClients)
	ON_BN_CLICKED(IDC_SERVINFO, OnBnClickedServinfo)
	ON_BN_CLICKED(IDC_VERSION, OnBnClickedVersion)
	ON_BN_CLICKED(IDC_LINK, OnBnClickedLink)
	ON_BN_CLICKED(IDC_FLIP, OnBnClickedFlip)
	ON_BN_CLICKED(IDC_SHRINK, OnBnClickedShrink)
	ON_BN_CLICKED(IDC_DUPE, OnBnClickedDupe)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDC_NUKE, OnBnClickedNuke)
	ON_BN_CLICKED(IDC_BUY, OnBnClickedBuy)
	ON_BN_CLICKED(IDC_SELL, OnBnClickedSell)
	ON_BN_CLICKED(IDC_INVENTORY, OnBnClickedInventory)
	ON_BN_CLICKED(IDC_PURCHASES, OnBnClickedPurchases)
	ON_BN_CLICKED(IDC_SAMPLES, OnBnClickedSamples)
	ON_BN_CLICKED(IDC_RESTOCK, OnBnClickedRestock)
	ON_BN_CLICKED(IDC_SNOW, OnBnClickedSnow)
	ON_BN_CLICKED(IDC_RAIN, OnBnClickedRain)
	ON_BN_CLICKED(IDC_DRY, OnBnClickedDry)
	ON_BN_CLICKED(IDC_LIGHTLEVEL, OnBnClickedLightlevel)
	ON_BN_CLICKED(IDC_INVUL, OnBnClickedInvul)
	ON_BN_CLICKED(IDC_ALLMOVE, OnBnClickedAllmove)
	ON_BN_CLICKED(IDC_INVISIBLE, OnBnClickedInvisible)
	ON_BN_CLICKED(IDC_FIX, OnBnClickedFix)
	ON_BN_CLICKED(IDC_TELE, OnBnClickedTele)
	ON_BN_CLICKED(IDC_HEARALL, OnBnClickedHearall)
	ON_BN_CLICKED(IDC_GMTOGGLE, OnBnClickedGmtoggle)
	ON_BN_CLICKED(IDC_DETAIL, OnBnClickedDetail)
	ON_BN_CLICKED(IDC_NIGHTSIGHT, OnBnClickedNightsight)
	ON_BN_CLICKED(IDC_DEBUG, OnBnClickedDebug)
	ON_BN_CLICKED(IDC_JAIL, OnBnClickedJail)
	ON_BN_CLICKED(IDC_FORGIVE, OnBnClickedForgive)
	ON_BN_CLICKED(IDC_KILL, OnBnClickedKill)
	ON_BN_CLICKED(IDC_DISCONNECT, OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_RESURRECT, OnBnClickedResurrect)
	ON_BN_CLICKED(IDC_PAGEON, OnBnClickedPageon)
	ON_BN_CLICKED(IDC_PAGELIST, OnBnClickedPagelist)
	ON_BN_CLICKED(IDC_PAGEGOPLAYER, OnBnClickedPagegoplayer)
	ON_BN_CLICKED(IDC_PAGEDISCONNECT, OnBnClickedPagedisconnect)
	ON_BN_CLICKED(IDC_PAGEBAN, OnBnClickedPageban)
	ON_BN_CLICKED(IDC_PAGEOFF, OnBnClickedPageoff)
	ON_BN_CLICKED(IDC_PAGEQUEUE, OnBnClickedPagequeue)
	ON_BN_CLICKED(IDC_PAGEGOORIGIN, OnBnClickedPagegoorigin)
	ON_BN_CLICKED(IDC_PAGEJAIL, OnBnClickedPagejail)
	ON_BN_CLICKED(IDC_PAGEDELETE, OnBnClickedPagedelete)
	ON_BN_CLICKED(IDC_WORLDSAVE, OnBnClickedWorldsave)
	ON_BN_CLICKED(IDC_SAVESTATICS, OnBnClickedSavestatics)
	ON_BN_CLICKED(IDC_RESYNC, OnBnClickedResync)
	ON_BN_CLICKED(IDC_RESTOCKALL, OnBnClickedRestockall)
END_MESSAGE_MAP()

BOOL CGeneralTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_GENERAL")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	m_cscSlider.SetRange(0, 30);
	m_rcDlg.Create(IDD_REMOTECONSOLEDLG, this);

	initialized = TRUE;

	return TRUE;
}

void CGeneralTab::SetLanguage()
{
	GetDlgItem(IDC_GR_INFORMATION)->SetWindowText(CMsg(_T("IDS_INFORMATION")));
	GetDlgItem(IDC_GR_MISC)->SetWindowText(CMsg(_T("IDS_MISC")));
	GetDlgItem(IDC_GR_VENDOR)->SetWindowText(CMsg(_T("IDS_VENDORS")));
	GetDlgItem(IDC_GR_WEATHER)->SetWindowText(CMsg(_T("IDS_WEATHER")));
	GetDlgItem(IDC_GR_SELF)->SetWindowText(CMsg(_T("IDS_SELF")));
	GetDlgItem(IDC_GR_DISCIPLINE)->SetWindowText(CMsg(_T("IDS_DISCIPLINE")));
	GetDlgItem(IDC_GR_GMPAGE)->SetWindowText(CMsg(_T("IDS_GMPAGE")));

	GetDlgItem(IDC_ADMIN)->SetWindowText(CMsg(_T("IDS_ADMIN")));
	GetDlgItem(IDC_INFO)->SetWindowText(CMsg(_T("IDS_INFO")));
	GetDlgItem(IDC_EDIT)->SetWindowText(CMsg(_T("IDS_EDIT")));
	GetDlgItem(IDC_CLIENTS)->SetWindowText(CMsg(_T("IDS_CLIENT")));
	GetDlgItem(IDC_SERVINFO)->SetWindowText(CMsg(_T("IDS_SERV_INFO")));
	GetDlgItem(IDC_VERSION)->SetWindowText(CMsg(_T("IDS_VERSION")));

	GetDlgItem(IDC_LINK)->SetWindowText(CMsg(_T("IDS_LINK")));
	GetDlgItem(IDC_FLIP)->SetWindowText(CMsg(_T("IDS_FLIP")));
	GetDlgItem(IDC_SHRINK)->SetWindowText(CMsg(_T("IDS_SHRINK")));
	GetDlgItem(IDC_DUPE)->SetWindowText(CMsg(_T("IDS_DUPE")));
	GetDlgItem(IDC_REMOVE)->SetWindowText(CMsg(_T("IDS_REMOVE")));
	GetDlgItem(IDC_NUKE)->SetWindowText(CMsg(_T("IDS_NUKE")));

	GetDlgItem(IDC_BUY)->SetWindowText(CMsg(_T("IDS_BUY")));
	GetDlgItem(IDC_SELL)->SetWindowText(CMsg(_T("IDS_SELL")));
	GetDlgItem(IDC_INVENTORY)->SetWindowText(CMsg(_T("IDS_INVENTORY")));
	GetDlgItem(IDC_PURCHASES)->SetWindowText(CMsg(_T("IDS_PURCHASES")));
	GetDlgItem(IDC_SAMPLES)->SetWindowText(CMsg(_T("IDS_SAMPLES")));
	GetDlgItem(IDC_RESTOCK)->SetWindowText(CMsg(_T("IDS_RESTOCK")));

	GetDlgItem(IDC_SNOW)->SetWindowText(CMsg(_T("IDS_SNOW")));
	GetDlgItem(IDC_RAIN)->SetWindowText(CMsg(_T("IDS_RAIN")));
	GetDlgItem(IDC_DRY)->SetWindowText(CMsg(_T("IDS_DRY")));
	GetDlgItem(IDC_LIGHTLEVEL)->SetWindowText(CMsg(_T("IDS_LIGHT_LVL")));

	GetDlgItem(IDC_INVUL)->SetWindowText(CMsg(_T("IDS_INVUL")));
	GetDlgItem(IDC_ALLMOVE)->SetWindowText(CMsg(_T("IDS_ALLMOVE")));
	GetDlgItem(IDC_INVISIBLE)->SetWindowText(CMsg(_T("IDS_INVIS")));
	GetDlgItem(IDC_FIX)->SetWindowText(CMsg(_T("IDS_FIX")));
	GetDlgItem(IDC_TELE)->SetWindowText(CMsg(_T("IDS_TELE")));
	GetDlgItem(IDC_HEARALL)->SetWindowText(CMsg(_T("IDS_HEARALL")));
	GetDlgItem(IDC_GMTOGGLE)->SetWindowText(CMsg(_T("IDS_GM")));
	GetDlgItem(IDC_DETAIL)->SetWindowText(CMsg(_T("IDS_DETAIL")));
	GetDlgItem(IDC_NIGHTSIGHT)->SetWindowText(CMsg(_T("IDS_NIGHTSIGHT")));
	GetDlgItem(IDC_DEBUG)->SetWindowText(CMsg(_T("IDS_DEBUG")));

	GetDlgItem(IDC_JAIL)->SetWindowText(CMsg(_T("IDS_JAIL")));
	GetDlgItem(IDC_FORGIVE)->SetWindowText(CMsg(_T("IDS_FORGIVE")));
	GetDlgItem(IDC_KILL)->SetWindowText(CMsg(_T("IDS_KILL")));
	GetDlgItem(IDC_DISCONNECT)->SetWindowText(CMsg(_T("IDS_DISCONNECT")));
	GetDlgItem(IDC_RESURRECT)->SetWindowText(CMsg(_T("IDS_RESURRECT")));

	GetDlgItem(IDC_PAGEON)->SetWindowText(CMsg(_T("IDS_ON")));
	GetDlgItem(IDC_PAGELIST)->SetWindowText(CMsg(_T("IDS_LIST")));
	GetDlgItem(IDC_PAGEGOPLAYER)->SetWindowText(CMsg(_T("IDS_GO_PLAYER")));
	GetDlgItem(IDC_PAGEDISCONNECT)->SetWindowText(CMsg(_T("IDS_DISCONNECT")));
	GetDlgItem(IDC_PAGEBAN)->SetWindowText(CMsg(_T("IDS_BAN")));
	GetDlgItem(IDC_PAGEOFF)->SetWindowText(CMsg(_T("IDS_OFF")));
	GetDlgItem(IDC_PAGEQUEUE)->SetWindowText(CMsg(_T("IDS_QUEUE")));
	GetDlgItem(IDC_PAGEGOORIGIN)->SetWindowText(CMsg(_T("IDS_GO_ORIGIN")));
	GetDlgItem(IDC_PAGEJAIL)->SetWindowText(CMsg(_T("IDS_JAIL")));
	GetDlgItem(IDC_PAGEDELETE)->SetWindowText(CMsg(_T("IDS_DELETE")));

	GetDlgItem(IDC_WORLDSAVE)->SetWindowText(CMsg(_T("IDS_WORLDSAVE")));
	GetDlgItem(IDC_SAVESTATICS)->SetWindowText(CMsg(_T("IDS_SAVESTATICS")));
	GetDlgItem(IDC_RESYNC)->SetWindowText(CMsg(_T("IDS_RESYNC")));
	GetDlgItem(IDC_RESTOCKALL)->SetWindowText(CMsg(_T("IDS_RESTOCKALL")));
	GetDlgItem(IDC_REMCONSOLE)->SetWindowText(CMsg(_T("IDS_REMCONSOLE")));
}

void CGeneralTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_cscSlider.m_hWnd)
	{
		m_csSliderValue.SetWindowText(CMsg(_T("%1!02ld!"),true,m_cscSlider.GetPos()));
		return;
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGeneralTab::OnRconsole() 
{
	if (m_rcDlg.IsWindowVisible())
	{
		// Hide it
		m_rcDlg.ShowWindow(SW_HIDE);
		m_cbRemoteConsole.SetWindowText(CMsg(_T("IDS_SHOW_CONSOLE")));
	}
	else
	{
		// Show it
		m_rcDlg.ShowWindow(SW_SHOW);
		m_cbRemoteConsole.SetWindowText(CMsg(_T("IDS_HIDE_CONSOLE")));
	}
}

void CGeneralTab::OnBnClickedAdmin()
{
	CommandToUO(CCmd(_T("admin")));
}


void CGeneralTab::OnBnClickedInfo()
{
	CommandToUO(CCmd(_T("info")));
}


void CGeneralTab::OnBnClickedEdit()
{
	CommandToUO(CCmd(_T("xedit")));
}


void CGeneralTab::OnBnClickedClients()
{
	CommandToUO(CCmd(_T("show serv.clients")));
}


void CGeneralTab::OnBnClickedServinfo()
{
	CommandToUO(CCmd(_T("information")));
}


void CGeneralTab::OnBnClickedVersion()
{
	CommandToUO(CCmd(_T("version")));
}


void CGeneralTab::OnBnClickedLink()
{
	CommandToUO(CCmd(_T("link")));
}


void CGeneralTab::OnBnClickedFlip()
{
	CommandToUO(CCmd(_T("xflip")));
}


void CGeneralTab::OnBnClickedShrink()
{
	CommandToUO(CCmd(_T("shrink")));
}


void CGeneralTab::OnBnClickedDupe()
{
	CommandToUO(CCmd(_T("dupe")));
}


void CGeneralTab::OnBnClickedRemove()
{
	CommandToUO(CCmd(_T("remove")));
}


void CGeneralTab::OnBnClickedNuke()
{
	CommandToUO(CCmd(_T("nuke")));
}


void CGeneralTab::OnBnClickedBuy()
{
	SendToUO(CCmd(_T("buy")));
}


void CGeneralTab::OnBnClickedSell()
{
	SendToUO(CCmd(_T("sell")));
}


void CGeneralTab::OnBnClickedInventory()
{
	CommandToUO(CCmd(_T("bank 1a")));
}


void CGeneralTab::OnBnClickedPurchases()
{
	CommandToUO(CCmd(_T("bank 1b")));
}


void CGeneralTab::OnBnClickedSamples()
{
	CommandToUO(CCmd(_T("bank 1c")));
}


void CGeneralTab::OnBnClickedRestock()
{
	CommandToUO(CCmd(_T("xrestock")));
}


void CGeneralTab::OnBnClickedSnow()
{
	CommandToUO(CCmd(_T("sector.snow")));
}


void CGeneralTab::OnBnClickedRain()
{
	CommandToUO(CCmd(_T("sector.rain")));
}


void CGeneralTab::OnBnClickedDry()
{
	CommandToUO(CCmd(_T("sector.dry")));
}


void CGeneralTab::OnBnClickedLightlevel()
{
	CString csVal = GetEditString(m_csSliderValue);
	CommandToUO(CCmd(_T("sector.light %1"),true, csVal));
}


void CGeneralTab::OnBnClickedInvul()
{
	CommandToUO(CCmd(_T("invulnerable")));
}


void CGeneralTab::OnBnClickedAllmove()
{
	CommandToUO(CCmd(_T("allmove")));
}


void CGeneralTab::OnBnClickedInvisible()
{
	CommandToUO(CCmd(_T("invisible")));
}


void CGeneralTab::OnBnClickedFix()
{
	CommandToUO(CCmd(_T("fix")));
}


void CGeneralTab::OnBnClickedTele()
{
	CommandToUO(CCmd(_T("tele")));
}


void CGeneralTab::OnBnClickedHearall()
{
	CommandToUO(CCmd(_T("hearall")));
}


void CGeneralTab::OnBnClickedGmtoggle()
{
	CommandToUO(CCmd(_T("gm")));
}


void CGeneralTab::OnBnClickedDetail()
{
	CommandToUO(CCmd(_T("detail")));
}


void CGeneralTab::OnBnClickedNightsight()
{
	CommandToUO(CCmd(_T("nightsight")));
}


void CGeneralTab::OnBnClickedDebug()
{
	CommandToUO(CCmd(_T("debug")));
}


void CGeneralTab::OnBnClickedJail()
{
	CommandToUO(CCmd(_T("jail")));
}


void CGeneralTab::OnBnClickedForgive()
{
	CommandToUO(CCmd(_T("forgive")));
}


void CGeneralTab::OnBnClickedKill()
{
	CommandToUO(CCmd(_T("kill")));
}


void CGeneralTab::OnBnClickedDisconnect()
{
	CommandToUO(CCmd(_T("xdisconnect")));
}


void CGeneralTab::OnBnClickedResurrect()
{
	CommandToUO(CCmd(_T("xresurrect")));
}


void CGeneralTab::OnBnClickedPageon()
{
	CommandToUO(CCmd(_T("page on")));
}


void CGeneralTab::OnBnClickedPagelist()
{
	CommandToUO(CCmd(_T("page list")));
}


void CGeneralTab::OnBnClickedPagegoplayer()
{
	CommandToUO(CCmd(_T("page player")));
}


void CGeneralTab::OnBnClickedPagedisconnect()
{
	if(AfxMessageBox(CMsg(_T("IDS_WARNING_DISCONNECT")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		CommandToUO(CCmd(_T("page disconnect")));
}


void CGeneralTab::OnBnClickedPageban()
{
	if(AfxMessageBox(CMsg(_T("IDS_WARNING_BAN")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		CommandToUO(CCmd(_T("page ban")));
}


void CGeneralTab::OnBnClickedPageoff()
{
	CommandToUO(CCmd(_T("page off")));
}


void CGeneralTab::OnBnClickedPagequeue()
{
	CommandToUO(CCmd(_T("page queue")));
}


void CGeneralTab::OnBnClickedPagegoorigin()
{
	CommandToUO(CCmd(_T("page origin")));
}


void CGeneralTab::OnBnClickedPagejail()
{
	if(AfxMessageBox(CMsg(_T("IDS_WARNING_JAIL")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		CommandToUO(CCmd(_T("page jail")));
}


void CGeneralTab::OnBnClickedPagedelete()
{
	CommandToUO(CCmd(_T("page delete")));
}


void CGeneralTab::OnBnClickedWorldsave()
{
	CommandToUO(CCmd(_T("serv.save")));
}


void CGeneralTab::OnBnClickedSavestatics()
{
	CommandToUO(CCmd(_T("serv.savestatics")));
}


void CGeneralTab::OnBnClickedResync()
{
	CommandToUO(CCmd(_T("serv.resync")));
}


void CGeneralTab::OnBnClickedRestockall()
{
	CommandToUO(CCmd(_T("serv.restock")));
}
