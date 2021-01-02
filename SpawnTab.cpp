
#include "stdafx.h"
#include "Axis3.h"
#include "SpawnTab.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSpawnTab, CDialogPage)

CSpawnTab::CSpawnTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CSpawnTab::IDD, AfxGetApp()->LoadIcon(IDI_CHAR), pParent)
{
	bSearch = false;
	bIDSort = false;
	bDescSort = false;
}


CSpawnTab::~CSpawnTab()
{
}

void CSpawnTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEMS, m_clcItems);
	DDX_Control(pDX, IDC_CATDROP, m_ccbCategories);
	DDX_Control(pDX, IDC_SUBDROP, m_ccbSubsections);
	DDX_Control(pDX, IDC_ART, cMulDisplay);
	DDX_Control(pDX, IDC_ITEMID, m_csItemID);
	DDX_Control(pDX, IDC_ZTILE, m_ceZTile);
	DDX_Control(pDX, IDC_NUKEARG, m_ceNukearg);
	DDX_Control(pDX, IDC_MINTIME, m_ceMinTime);
	DDX_Control(pDX, IDC_MAXTIME, m_ceMaxTime);
	DDX_Control(pDX, IDC_MAXDIST, m_ceMaxDist);
	DDX_Control(pDX, IDC_AMOUNT, m_ceAmount);
	DDX_Control(pDX, IDC_NUDGEAMOUNT, m_ceNudge);
	DDX_Control(pDX, IDC_NUDGEUP, m_cbNudgeUp);
	DDX_Control(pDX, IDC_NUDGEDOWN, m_cbNudgeDown);
	DDX_Control(pDX, IDC_MOVE1, m_cbMove1);
	DDX_Control(pDX, IDC_MOVE2, m_cbMove2);
	DDX_Control(pDX, IDC_MOVE3, m_cbMove3);
	DDX_Control(pDX, IDC_MOVE4, m_cbMove4);
	DDX_Control(pDX, IDC_MOVE5, m_cbMove5);
	DDX_Control(pDX, IDC_MOVE6, m_cbMove6);
	DDX_Control(pDX, IDC_MOVE7, m_cbMove7);
	DDX_Control(pDX, IDC_MOVE8, m_cbMove8);
	DDX_Control(pDX, IDC_FINDITEM, m_cbfinditem);
	DDX_Check(pDX, IDC_LOCKITEM, m_bLockDown);
	DDX_Control(pDX, IDC_LOCKITEM, m_cbLockDown);
}


BEGIN_MESSAGE_MAP(CSpawnTab, CDialogPage)
	ON_CBN_SELCHANGE(IDC_CATDROP, OnSelchangeCat)
	ON_CBN_SELCHANGE(IDC_SUBDROP, OnSelchangeSub)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEMS, OnItemchangedItems)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_LOCKITEM, OnLockitem)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_TILE, OnTile)
	ON_BN_CLICKED(IDC_FLIP, OnFlip)
	ON_BN_CLICKED(IDC_PLACE, OnPlacespawn)
	ON_BN_CLICKED(IDC_INIT, OnInitspawn)
	ON_BN_CLICKED(IDC_NUKE, OnNuke)
	ON_EN_CHANGE(IDC_MAXTIME, OnChangeMaxTime)
	ON_EN_CHANGE(IDC_MINTIME, OnChangeMinTime)
	ON_EN_CHANGE(IDC_AMOUNT, OnChangeAmount)
	ON_EN_CHANGE(IDC_MAXDIST, OnChangeMaxDist)
	ON_EN_CHANGE(IDC_NUDGEAMOUNT, OnChangeNudge)
	ON_EN_CHANGE(IDC_ZTILE, OnChangeTile)
	ON_EN_CHANGE(IDC_NUKEARG, OnChangeNuke)
	ON_BN_CLICKED(IDC_NUDGEUP, OnNudgeup)
	ON_BN_CLICKED(IDC_NUDGEDOWN, OnNudgedown)
	ON_BN_CLICKED(IDC_MOVE1, OnMove1)
	ON_BN_CLICKED(IDC_MOVE2, OnMove2)
	ON_BN_CLICKED(IDC_MOVE3, OnMove3)
	ON_BN_CLICKED(IDC_MOVE4, OnMove4)
	ON_BN_CLICKED(IDC_MOVE5, OnMove5)
	ON_BN_CLICKED(IDC_MOVE6, OnMove6)
	ON_BN_CLICKED(IDC_MOVE7, OnMove7)
	ON_BN_CLICKED(IDC_MOVE8, OnMove8)
	ON_BN_CLICKED(IDC_FINDITEM, OnFinditem)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEMS, OnDblclkItems)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ITEMS, OnColumnclickItems)
END_MESSAGE_MAP()

BOOL CSpawnTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Spawns")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	initialized = TRUE;

	return TRUE;
}
