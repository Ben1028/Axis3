
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
	SetWindowText(CMsg(_T("General")));
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
	GetDlgItem(IDC_GR_INFORMATION)->SetWindowText(CMsg(_T("Information")));
	GetDlgItem(IDC_GR_MISC)->SetWindowText(CMsg(_T("Misc")));
	GetDlgItem(IDC_GR_VENDOR)->SetWindowText(CMsg(_T("Vendors")));
	GetDlgItem(IDC_GR_WEATHER)->SetWindowText(CMsg(_T("Weather")));
	GetDlgItem(IDC_GR_SELF)->SetWindowText(CMsg(_T("Self")));
	GetDlgItem(IDC_GR_DISCIPLINE)->SetWindowText(CMsg(_T("Discipline")));
	GetDlgItem(IDC_GR_GMPAGE)->SetWindowText(CMsg(_T("GM Page")));

	GetDlgItem(IDC_ADMIN)->SetWindowText(CMsg(_T("Admin")));
	GetDlgItem(IDC_INFO)->SetWindowText(CMsg(_T("Info")));
	GetDlgItem(IDC_EDIT)->SetWindowText(CMsg(_T("Edit")));
	GetDlgItem(IDC_CLIENTS)->SetWindowText(CMsg(_T("Clients")));
	GetDlgItem(IDC_SERVINFO)->SetWindowText(CMsg(_T("Serv Info")));
	GetDlgItem(IDC_VERSION)->SetWindowText(CMsg(_T("Version")));

	GetDlgItem(IDC_LINK)->SetWindowText(CMsg(_T("Link")));
	GetDlgItem(IDC_FLIP)->SetWindowText(CMsg(_T("Flip")));
	GetDlgItem(IDC_SHRINK)->SetWindowText(CMsg(_T("Shrink")));
	GetDlgItem(IDC_DUPE)->SetWindowText(CMsg(_T("Dupe")));
	GetDlgItem(IDC_REMOVE)->SetWindowText(CMsg(_T("Remove")));
	GetDlgItem(IDC_NUKE)->SetWindowText(CMsg(_T("Nuke")));

	GetDlgItem(IDC_BUY)->SetWindowText(CMsg(_T("Buy")));
	GetDlgItem(IDC_SELL)->SetWindowText(CMsg(_T("Sell")));
	GetDlgItem(IDC_INVENTORY)->SetWindowText(CMsg(_T("Inventory")));
	GetDlgItem(IDC_PURCHASES)->SetWindowText(CMsg(_T("Purchases")));
	GetDlgItem(IDC_SAMPLES)->SetWindowText(CMsg(_T("Samples")));
	GetDlgItem(IDC_RESTOCK)->SetWindowText(CMsg(_T("Restock")));

	GetDlgItem(IDC_SNOW)->SetWindowText(CMsg(_T("Snow")));
	GetDlgItem(IDC_RAIN)->SetWindowText(CMsg(_T("Rain")));
	GetDlgItem(IDC_DRY)->SetWindowText(CMsg(_T("Dry")));
	GetDlgItem(IDC_LIGHTLEVEL)->SetWindowText(CMsg(_T("Light Level")));

	GetDlgItem(IDC_INVUL)->SetWindowText(CMsg(_T("Invul")));
	GetDlgItem(IDC_ALLMOVE)->SetWindowText(CMsg(_T("Allmove")));
	GetDlgItem(IDC_INVISIBLE)->SetWindowText(CMsg(_T("Invisible")));
	GetDlgItem(IDC_FIX)->SetWindowText(CMsg(_T("Fix")));
	GetDlgItem(IDC_TELE)->SetWindowText(CMsg(_T("Tele")));
	GetDlgItem(IDC_HEARALL)->SetWindowText(CMsg(_T("Hear All")));
	GetDlgItem(IDC_GMTOGGLE)->SetWindowText(CMsg(_T("GM Toggle")));
	GetDlgItem(IDC_DETAIL)->SetWindowText(CMsg(_T("Detail")));
	GetDlgItem(IDC_NIGHTSIGHT)->SetWindowText(CMsg(_T("NightSight")));
	GetDlgItem(IDC_DEBUG)->SetWindowText(CMsg(_T("Debug")));

	GetDlgItem(IDC_JAIL)->SetWindowText(CMsg(_T("Jail")));
	GetDlgItem(IDC_FORGIVE)->SetWindowText(CMsg(_T("Forgive")));
	GetDlgItem(IDC_KILL)->SetWindowText(CMsg(_T("Kill")));
	GetDlgItem(IDC_DISCONNECT)->SetWindowText(CMsg(_T("Disconnect")));
	GetDlgItem(IDC_RESURRECT)->SetWindowText(CMsg(_T("Resurrect")));

	GetDlgItem(IDC_PAGEON)->SetWindowText(CMsg(_T("On")));
	GetDlgItem(IDC_PAGELIST)->SetWindowText(CMsg(_T("List")));
	GetDlgItem(IDC_PAGEGOPLAYER)->SetWindowText(CMsg(_T("Go Player")));
	GetDlgItem(IDC_PAGEDISCONNECT)->SetWindowText(CMsg(_T("Disconnect")));
	GetDlgItem(IDC_PAGEBAN)->SetWindowText(CMsg(_T("Ban")));
	GetDlgItem(IDC_PAGEOFF)->SetWindowText(CMsg(_T("Off")));
	GetDlgItem(IDC_PAGEQUEUE)->SetWindowText(CMsg(_T("Queue")));
	GetDlgItem(IDC_PAGEGOORIGIN)->SetWindowText(CMsg(_T("Go Origin")));
	GetDlgItem(IDC_PAGEJAIL)->SetWindowText(CMsg(_T("Jail")));
	GetDlgItem(IDC_PAGEDELETE)->SetWindowText(CMsg(_T("Delete")));

	GetDlgItem(IDC_WORLDSAVE)->SetWindowText(CMsg(_T("Save World")));
	GetDlgItem(IDC_SAVESTATICS)->SetWindowText(CMsg(_T("Save Statics")));
	GetDlgItem(IDC_RESYNC)->SetWindowText(CMsg(_T("Resync")));
	GetDlgItem(IDC_RESTOCKALL)->SetWindowText(CMsg(_T("Restock All NPC")));
	GetDlgItem(IDC_REMCONSOLE)->SetWindowText(CMsg(_T("Remote Console")));
}

void CGeneralTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_cscSlider.m_hWnd)
	{
		m_csSliderValue.SetWindowText(CFrmt(_T("%1!02ld!"),m_cscSlider.GetPos()));
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
		m_cbRemoteConsole.SetWindowText(CMsg(_T("Show Console")));
	}
	else
	{
		// Show it
		m_rcDlg.ShowWindow(SW_SHOW);
		m_cbRemoteConsole.SetWindowText(CMsg(_T("Hide Console")));
	}
}

void CGeneralTab::OnBnClickedAdmin()
{
	SendToUO(CCmd(_T("admin")));
}


void CGeneralTab::OnBnClickedInfo()
{
	SendToUO(CCmd(_T("info")));
}


void CGeneralTab::OnBnClickedEdit()
{
	SendToUO(CCmd(_T("xedit")));
}


void CGeneralTab::OnBnClickedClients()
{
	SendToUO(CCmd(_T("show serv.clients")));
}


void CGeneralTab::OnBnClickedServinfo()
{
	SendToUO(CCmd(_T("information")));
}


void CGeneralTab::OnBnClickedVersion()
{
	SendToUO(CCmd(_T("version")));
}


void CGeneralTab::OnBnClickedLink()
{
	SendToUO(CCmd(_T("link")));
}


void CGeneralTab::OnBnClickedFlip()
{
	SendToUO(CCmd(_T("xflip")));
}


void CGeneralTab::OnBnClickedShrink()
{
	SendToUO(CCmd(_T("shrink")));
}


void CGeneralTab::OnBnClickedDupe()
{
	SendToUO(CCmd(_T("dupe")));
}


void CGeneralTab::OnBnClickedRemove()
{
	SendToUO(CCmd(_T("remove")));
}


void CGeneralTab::OnBnClickedNuke()
{
	SendToUO(CCmd(_T("nuke")));
}


void CGeneralTab::OnBnClickedBuy()
{
	SendToUO(CSpk(_T("buy")));
}


void CGeneralTab::OnBnClickedSell()
{
	SendToUO(CSpk(_T("sell")));
}


void CGeneralTab::OnBnClickedInventory()
{
	SendToUO(CCmd(_T("bank 1a")));
}


void CGeneralTab::OnBnClickedPurchases()
{
	SendToUO(CCmd(_T("bank 1b")));
}


void CGeneralTab::OnBnClickedSamples()
{
	SendToUO(CCmd(_T("bank 1c")));
}


void CGeneralTab::OnBnClickedRestock()
{
	SendToUO(CCmd(_T("xrestock")));
}


void CGeneralTab::OnBnClickedSnow()
{
	SendToUO(CCmd(_T("sector.snow")));
}


void CGeneralTab::OnBnClickedRain()
{
	SendToUO(CCmd(_T("sector.rain")));
}


void CGeneralTab::OnBnClickedDry()
{
	SendToUO(CCmd(_T("sector.dry")));
}


void CGeneralTab::OnBnClickedLightlevel()
{
	CString csVal = GetEditString(m_csSliderValue);
	SendToUO(CCmd(_T("sector.light %1"),true, csVal));
}


void CGeneralTab::OnBnClickedInvul()
{
	SendToUO(CCmd(_T("invulnerable")));
}


void CGeneralTab::OnBnClickedAllmove()
{
	SendToUO(CCmd(_T("allmove")));
}


void CGeneralTab::OnBnClickedInvisible()
{
	SendToUO(CCmd(_T("invisible")));
}


void CGeneralTab::OnBnClickedFix()
{
	SendToUO(CCmd(_T("fix")));
}


void CGeneralTab::OnBnClickedTele()
{
	SendToUO(CCmd(_T("tele")));
}


void CGeneralTab::OnBnClickedHearall()
{
	SendToUO(CCmd(_T("hearall")));
}


void CGeneralTab::OnBnClickedGmtoggle()
{
	SendToUO(CCmd(_T("gm")));
}


void CGeneralTab::OnBnClickedDetail()
{
	SendToUO(CCmd(_T("detail")));
}


void CGeneralTab::OnBnClickedNightsight()
{
	SendToUO(CCmd(_T("nightsight")));
}


void CGeneralTab::OnBnClickedDebug()
{
	SendToUO(CCmd(_T("debug")));
}


void CGeneralTab::OnBnClickedJail()
{
	SendToUO(CCmd(_T("jail")));
}


void CGeneralTab::OnBnClickedForgive()
{
	SendToUO(CCmd(_T("forgive")));
}


void CGeneralTab::OnBnClickedKill()
{
	SendToUO(CCmd(_T("kill")));
}


void CGeneralTab::OnBnClickedDisconnect()
{
	SendToUO(CCmd(_T("xdisconnect")));
}


void CGeneralTab::OnBnClickedResurrect()
{
	SendToUO(CCmd(_T("xresurrect")));
}


void CGeneralTab::OnBnClickedPageon()
{
	SendToUO(CCmd(_T("page on")));
}


void CGeneralTab::OnBnClickedPagelist()
{
	SendToUO(CCmd(_T("page list")));
}


void CGeneralTab::OnBnClickedPagegoplayer()
{
	SendToUO(CCmd(_T("page player")));
}


void CGeneralTab::OnBnClickedPagedisconnect()
{
	if(AfxMessageBox(CMsg(_T("Are you sure you wish to disconnect the client that sent this page?")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		SendToUO(CCmd(_T("page disconnect")));
}


void CGeneralTab::OnBnClickedPageban()
{
	if(AfxMessageBox(CMsg(_T("Are you sure you wish to BAN the client that sent this page?")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		SendToUO(CCmd(_T("page ban")));
}


void CGeneralTab::OnBnClickedPageoff()
{
	SendToUO(CCmd(_T("page off")));
}


void CGeneralTab::OnBnClickedPagequeue()
{
	SendToUO(CCmd(_T("page queue")));
}


void CGeneralTab::OnBnClickedPagegoorigin()
{
	SendToUO(CCmd(_T("page origin")));
}


void CGeneralTab::OnBnClickedPagejail()
{
	if(AfxMessageBox(CMsg(_T("Are you sure you wish to jail the client that sent this page?")), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		SendToUO(CCmd(_T("page jail")));
}


void CGeneralTab::OnBnClickedPagedelete()
{
	SendToUO(CCmd(_T("page delete")));
}


void CGeneralTab::OnBnClickedWorldsave()
{
	SendToUO(CCmd(_T("serv.save")));
}


void CGeneralTab::OnBnClickedSavestatics()
{
	SendToUO(CCmd(_T("serv.savestatics")));
}


void CGeneralTab::OnBnClickedResync()
{
	SendToUO(CCmd(_T("serv.resync")));
}


void CGeneralTab::OnBnClickedRestockall()
{
	SendToUO(CCmd(_T("serv.restock")));
}
