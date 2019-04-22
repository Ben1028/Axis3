
#include "stdafx.h"
#include "Axis3.h"
#include "ItemsTab.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CItemsTab, CDialogPage)

CItemsTab::CItemsTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CItemsTab::IDD, AfxGetApp()->LoadIcon(IDI_ITEMS), pParent)
{
	m_bLockDown = GetSettingNum(_T("ItemTabLockDown"));
	bSearch = false;
	bIDSort = false;
	bDescSort = false;
}


CItemsTab::~CItemsTab()
{
}

void CItemsTab::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CItemsTab, CDialogPage)
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

BOOL CItemsTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_ITEMS")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	m_clcItems.InsertColumn(0, CMsg(_T("IDS_ITEM_DESCRIPTION")), LVCFMT_LEFT, 150, -1);
	m_clcItems.InsertColumn(1, CMsg(_T("IDS_DEFNAME")), LVCFMT_LEFT, 150, -1);
	m_clcItems.InsertColumn(2, CMsg(_T("IDS_HUE")), LVCFMT_LEFT, 0, -1);
	m_clcItems.InsertColumn(3, CMsg(_T("IDS_ID")), LVCFMT_LEFT, 0, -1);
	m_clcItems.InsertColumn(4, CMsg(_T("IDS_TYPE")), LVCFMT_LEFT, 0, -1);
	FillCategory();
	cMulDisplay.InitDisplay();

	m_ceAmount.SetWindowText(GetSettingString(_T("ItemTabSpawnAmount")));
	m_ceMaxTime.SetWindowText(GetSettingString(_T("ItemTabMaxTime")));
	m_ceMinTime.SetWindowText(GetSettingString(_T("ItemTabMinTime")));
	m_ceMaxDist.SetWindowText(GetSettingString(_T("ItemTabMaxDist")));
	m_ceNudge.SetWindowText(GetSettingString(_T("ItemTabNudge")));
	m_ceZTile.SetWindowText(GetSettingString(_T("ItemTabTileZ")));
	m_ceNukearg.SetWindowText(GetSettingString(_T("ItemTabNukeArg")));

	initialized = TRUE;

	return TRUE;
}

void CItemsTab::SetLanguage()
{
	GetDlgItem(IDC_REMOVE)->SetWindowText(CMsg(_T("IDS_REMOVE")));
	GetDlgItem(IDC_FLIP)->SetWindowText(CMsg(_T("IDS_FLIP")));
	GetDlgItem(IDC_TILE)->SetWindowText(CMsg(_T("IDS_TILE")));
	GetDlgItem(IDC_CREATE)->SetWindowText(CMsg(_T("IDS_CREATE")));
	GetDlgItem(IDC_NUKE)->SetWindowText(CMsg(_T("IDS_NUKE")));
	GetDlgItem(IDC_INIT)->SetWindowText(CMsg(_T("IDS_INIT")));
	GetDlgItem(IDC_PLACE)->SetWindowText(CMsg(_T("IDS_PLACE")));
	GetDlgItem(IDC_STATICCAT)->SetWindowText(CMsg(_T("IDS_CAT")));
	GetDlgItem(IDC_STATICSUB)->SetWindowText(CMsg(_T("IDS_SUB")));
	GetDlgItem(IDC_STATICMOVE)->SetWindowText(CMsg(_T("IDS_MOVE")));
	GetDlgItem(IDC_STATICNUDGE)->SetWindowText(CMsg(_T("IDS_NUDGE")));
	GetDlgItem(IDC_STATICMAXAMOUNT)->SetWindowText(CMsg(_T("IDS_MAX_AMOUNT")));
	GetDlgItem(IDC_STATICMAXDIST)->SetWindowText(CMsg(_T("IDS_MAX_DIST")));
	GetDlgItem(IDC_STATICMINTIME)->SetWindowText(CMsg(_T("IDS_MIN_TIME")));
	GetDlgItem(IDC_STATICMAXTIME)->SetWindowText(CMsg(_T("IDS_MAX_TIME")));
	GetDlgItem(IDC_STATICSPAWN)->SetWindowText(CMsg(_T("IDS_ITEM_SPAWN")));
	GetDlgItem(IDC_STATICZ)->SetWindowText(CMsg(_T("IDS_Z")));
	GetDlgItem(IDC_LOCKITEM)->SetWindowText(CMsg(_T("IDS_LOCK")));

	m_cbNudgeDown.InitButton(IDI_ARROWDOWN, CMsg(_T("IDS_NUDGEDOWN")));
	m_cbNudgeUp.InitButton(IDI_ARROWUP, CMsg(_T("IDS_NUDGEUP")));
	m_cbMove1.InitButton(IDI_ARROWUP, CMsg(_T("IDS_UP")));
	m_cbMove3.InitButton(IDI_ARROWRIGHT, CMsg(_T("IDS_RIGHT")));
	m_cbMove5.InitButton(IDI_ARROWDOWN, CMsg(_T("IDS_DOWN")));
	m_cbMove7.InitButton(IDI_ARROWLEFT, CMsg(_T("IDS_LEFT")));
	m_cbMove2.InitButton(IDI_ARROWUPRIGHT, CMsg(_T("IDS_RIGHTUP")));
	m_cbMove4.InitButton(IDI_ARROWDOWNRIGHT, CMsg(_T("IDS_DOWNRIGHT")));
	m_cbMove6.InitButton(IDI_ARROWDOWNLEFT, CMsg(_T("IDS_LEFTDOWN")));
	m_cbMove8.InitButton(IDI_ARROWUPLEFT, CMsg(_T("IDS_UPLEFT")));
	m_cbfinditem.InitButton(IDI_FIND, CMsg(_T("IDS_FINDITEM")));
}

void CItemsTab::FillCategory()
{
	//Fill Combobox

	Table TBCats = Axis->DBData.QuerySQL(_T("SELECT Cat FROM Item GROUP BY Cat ORDER BY Cat"));
	TBCats.ResetRow();
	while (TBCats.GoNext())
	{
		m_ccbCategories.AddString(TBCats.GetValue(_T("Cat")));
	}

	m_ccbCategories.SetCurSel(0);
	OnSelchangeCat();
}

void CItemsTab::OnSelchangeCat()
{
	UpdateData(TRUE);
	m_ccbSubsections.ResetContent();

	int iSel = m_ccbCategories.GetCurSel();
	if (iSel == -1)
	{
		UpdateData(FALSE);
		return;
	}

	CString csCat;
	m_ccbCategories.GetLBText(iSel, csCat);
	Table TBSubs = Axis->DBData.QuerySQL(CSQL(_T("SELECT Cat,Sub FROM Item WHERE Cat = '%1' GROUP BY Sub ORDER BY Sub"), SQLStrip(csCat)));
	TBSubs.ResetRow();
	while (TBSubs.GoNext())
	{
		m_ccbSubsections.AddString(TBSubs.GetValue(_T("Sub")));
	}

	m_ccbSubsections.SetCurSel(0);
	OnSelchangeSub();

	UpdateData(FALSE);
}

void CItemsTab::OnSelchangeSub()
{
	UpdateData(TRUE);

	m_clcItems.SetHotItem(-1);
	m_clcItems.DeleteAllItems();

	int iSel = m_ccbCategories.GetCurSel();
	CString csCat, csSub;
	m_ccbCategories.GetLBText(iSel, csCat);
	iSel = m_ccbSubsections.GetCurSel();
	m_ccbSubsections.GetLBText(iSel, csSub);

	Table TBDecs = Axis->DBData.QuerySQL(CSQL(_T("SELECT * FROM Item WHERE Cat = '%1' AND Sub = '%2' ORDER BY Desc"), SQLStrip(csCat), SQLStrip(csSub)));
	csSortQuerry = CSQL(_T("SELECT * FROM Item WHERE Cat = '%1' AND Sub = '%2' ORDER BY"), SQLStrip(csCat), SQLStrip(csSub));
	TBDecs.ResetRow();
	int iCount = 0;
	while (TBDecs.GoNext())
	{
		m_clcItems.InsertItem(iCount, TBDecs.GetValue(_T("Desc")), iCount);
		m_clcItems.SetItemText(iCount, 1, TBDecs.GetValue(_T("Def")));
		m_clcItems.SetItemText(iCount, 2, TBDecs.GetValue(_T("Color")));
		m_clcItems.SetItemText(iCount, 3, TBDecs.GetValue(_T("ID")));
		m_clcItems.SetItemText(iCount, 4, TBDecs.GetValue(_T("Type")));
		iCount++;
	}
	bSearch = false;
}

void CItemsTab::OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		int iSelIndex = m_clcItems.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex != -1)
		{
			m_clcItems.SetHotItem(iSelIndex);
			m_csItemID.SetWindowText(m_clcItems.GetItemText(iSelIndex, 1));

			if (m_clcItems.GetItemText(iSelIndex, 4) == "2")
			{
				cMulDisplay.DrawMulti(ahextoi(m_clcItems.GetItemText(iSelIndex, 3)));
				cMulDisplay.SetAutoScale((bool)GetSettingNum(_T("ViewerScaleMultis")));
				cMulDisplay.DisplayObj();
			}
			else
			{
				cMulDisplay.DrawArt(ahextoi(m_clcItems.GetItemText(iSelIndex, 3)), (WORD)ahextoi(m_clcItems.GetItemText(iSelIndex, 2)));
				cMulDisplay.SetAutoScale((bool)GetSettingNum(_T("ViewerScaleItems")));
				cMulDisplay.DisplayObj();
			}
			if (bSearch)
			{
				Table TBInfo = Axis->DBData.QuerySQL(CSQL(_T("SELECT Cat,Sub FROM Item WHERE Def = '%1'"), m_clcItems.GetItemText(iSelIndex, 1)));
				TBInfo.ResetRow();
				if (TBInfo.GoNext())
				{
					CString csCat = TBInfo.GetValue(_T("Cat"));
					m_ccbCategories.SelectString(0, csCat);
					Table TBSubs = Axis->DBData.QuerySQL(CSQL(_T("SELECT Cat,Sub FROM Item WHERE Cat = '%1' GROUP BY Sub ORDER BY Sub"), SQLStrip(csCat)));
					TBSubs.ResetRow();
					m_ccbSubsections.ResetContent();
					while (TBSubs.GoNext())
					{
						m_ccbSubsections.AddString(TBSubs.GetValue(_T("Sub")));
					}
					m_ccbSubsections.SelectString(0, TBInfo.GetValue(_T("Sub")));
				}
			}
			return;
		}
	}
	m_csItemID.SetWindowText(_T(""));
	cMulDisplay.DrawArt(-1, 0);
	*pResult = 0;
}

void CItemsTab::OnCreate()
{
	CString csValue = GetEditString(m_csItemID);

	if (csValue == "")
	{
		AfxMessageBox(CMsg(_T("IDS_NO_ITEM_SELECTED")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	CString csCmd;
	if (m_bLockDown)
	{
		CommandToUO(CCmd(_T("static %1"),true, csValue));
	}
	else
	{
		CommandToUO(CCmd(_T("add %1"), true, csValue));
	}
}

void CItemsTab::OnLockitem()
{
	UpdateData();
	SetSettingNum(_T("ItemTabLockDown"), m_bLockDown);
}

void CItemsTab::OnRemove()
{
	CommandToUO(CCmd(_T("remove")));
}

void CItemsTab::OnTile()
{
	CString csID = GetEditString(m_csItemID);
	if (csID == "")
	{
		AfxMessageBox(CMsg(_T("IDS_NO_ITEM_SELECTED")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	CString csValue = GetEditString(m_ceZTile);
	if (csValue.SpanIncluding(_T("0123456789-")) != csValue)
	{
		AfxMessageBox(CMsg(_T("IDS_INVALID_Z")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (csValue == "")
		csValue = "0";
	CommandToUO(CCmd(_T("tile %1 %2"),true, csValue, csID));
}

void CItemsTab::OnFlip()
{
	CommandToUO(CCmd(_T("xflip")));
}

void CItemsTab::OnPlacespawn()
{
	CommandToUO(CCmd(_T("add 01ea7")));
}

void CItemsTab::OnNuke()
{
	CommandToUO(CCmd(_T("nuke %1"),true, GetEditString(m_ceNukearg)));
}

void CItemsTab::OnInitspawn()
{
	CString csID = GetEditString(m_csItemID);

	if (csID == "")
	{
		AfxMessageBox(CMsg(_T("IDS_NO_ITEM_SELECTED")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int iAmount = GetEditNum(m_ceAmount);
	if (iAmount == 0)
		iAmount = 1;
	int iMaxDist = GetEditNum(m_ceMaxDist);
	int iMinTime = GetEditNum(m_ceMinTime);
	int iMaxTime = GetEditNum(m_ceMaxTime);
	if (iMaxTime <= iMinTime)
		iMaxTime = iMinTime + 1;

	CommandToUO(CCmd(_T("act.type t_spawn_item")));
	CommandToUO(CCmd(_T("act.amount %1!d!"),true, iAmount));
	Sleep(700);
	CommandToUO(CCmd(_T("act.more %1"), true, csID));
	CommandToUO(CCmd(_T("act.morep %1!d! %2!d! %3!d!"), true, iMinTime, iMaxTime, iMaxDist));
	CommandToUO(CCmd(_T("act.attr 00b0")));
	CommandToUO(CCmd(_T("act.timer 1")));
}

void CItemsTab::OnChangeAmount()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabSpawnAmount"), GetEditNum(m_ceAmount));
}

void CItemsTab::OnChangeMaxTime()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabMaxTime"), GetEditNum(m_ceMaxTime));
}
void CItemsTab::OnChangeMinTime()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabMinTime"), GetEditNum(m_ceMinTime));
}
void CItemsTab::OnChangeMaxDist()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabMaxDist"), GetEditNum(m_ceMaxDist));
}
void CItemsTab::OnChangeNudge()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabNudge"), GetEditNum(m_ceNudge));
}
void CItemsTab::OnChangeTile()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingNum(_T("ItemTabTileZ"), GetEditNum(m_ceZTile));
}
void CItemsTab::OnChangeNuke()
{
	UpdateData();
	if (!IsWindowVisible())
		return;
	SetSettingString(_T("ItemTabNukeArg"), GetEditString(m_ceNukearg));
}

void CItemsTab::OnNudgeup()
{
	CommandToUO(CCmd(_T("nudgeup %1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnNudgedown()
{
	CommandToUO(CCmd(_T("nudgedown %1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove1()
{
	CommandToUO(CCmd(_T("xmove 0 -%1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove2()
{
	CommandToUO(CCmd(_T("xmove %1!d! -%1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove3()
{
	CommandToUO(CCmd(_T("xmove %1!d! 0"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove4()
{
	CommandToUO(CCmd(_T("xmove %1!d! %1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove5()
{
	CommandToUO(CCmd(_T("xmove 0 %1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove6()
{
	CommandToUO(CCmd(_T("xmove -%1!d! %1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove7()
{
	CommandToUO(CCmd(_T("xmove -%1!d! 0"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnMove8()
{
	CommandToUO(CCmd(_T("xmove -%1!d! -%1!d!"), true, GetEditNum(m_ceNudge)));
}

void CItemsTab::OnFinditem()
{
	CSearchCritDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
	if (GetSettingString(_T("SearchObjectPattern")) != "")
	{
		Find();
	}
	return;
}

void CItemsTab::Find()
{
	CString csPattern = SQLStrip(GetSettingString(_T("SearchObjectPattern")));
	int iCriteria = GetSettingNum(_T("SearchCriteria"));
	Table TBSearch;

	switch (iCriteria)
	{
		case 0:
		{
			TBSearch = Axis->DBData.QuerySQL(CSQL(_T("SELECT * FROM Item WHERE ID LIKE '%1' ORDER BY Desc"), csPattern));
			csSortQuerry = CSQL(_T("SELECT * FROM Item WHERE ID LIKE '%1' ORDER BY"), csPattern);
			break;
		}
		case 1:
		{
			TBSearch = Axis->DBData.QuerySQL(CSQL(_T("SELECT * FROM Item WHERE Def LIKE '%1' ORDER BY Desc"), csPattern));
			csSortQuerry = CSQL(_T("SELECT * FROM Item WHERE Def LIKE '%1' ORDER BY"), csPattern);
			break;
		}
		case 2:
		{
			TBSearch = Axis->DBData.QuerySQL(CSQL(_T("SELECT * FROM Item WHERE Desc LIKE '%1' ORDER BY Desc"), csPattern));
			csSortQuerry = CSQL(_T("SELECT * FROM Item WHERE Desc LIKE '%1' ORDER BY"), csPattern);
			break;
		}
	}
	TBSearch.ResetRow();

	if (TBSearch.GetRowCount())
	{
		m_clcItems.SetHotItem(-1);
		m_clcItems.DeleteAllItems();

		int iCount = 0;
		while (TBSearch.GoNext())
		{
			m_clcItems.InsertItem(iCount, TBSearch.GetValue(_T("Desc")), iCount);
			m_clcItems.SetItemText(iCount, 1, TBSearch.GetValue(_T("Def")));
			m_clcItems.SetItemText(iCount, 2, TBSearch.GetValue(_T("Color")));
			m_clcItems.SetItemText(iCount, 3, TBSearch.GetValue(_T("ID")));
			m_clcItems.SetItemText(iCount, 4, TBSearch.GetValue(_T("Type")));
			iCount++;
		}
		bSearch = true;
	}
	else 
	{
		AfxMessageBox(CMsg(_T("IDS_EMPTY_SEARCH")));
	}
}

void CItemsTab::OnDblclkItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnCreate();
	*pResult = 0;
}

void CItemsTab::OnColumnclickItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString csType, csOrder;
	if (pNMListView->iSubItem == 0)
	{
		csType = _T("Desc");
		bDescSort = !bDescSort;
		csOrder = (bDescSort ? _T("DESC") : _T("ASC"));
	}
	if (pNMListView->iSubItem == 1)
	{
		csType = _T("Def");
		bIDSort = !bIDSort;
		csOrder = (bIDSort ? _T("DESC") : _T("ASC"));
	}
	Table TBSort;
	TBSort = Axis->DBData.QuerySQL(CSQL(_T("%1 %2 %3"), csSortQuerry, csType, csOrder));
	TBSort.ResetRow();

	if (TBSort.GetRowCount())
	{
		m_clcItems.SetHotItem(-1);
		m_clcItems.DeleteAllItems();

		int iCount = 0;
		while (TBSort.GoNext())
		{
			m_clcItems.InsertItem(iCount, TBSort.GetValue(_T("Desc")), iCount);
			m_clcItems.SetItemText(iCount, 1, TBSort.GetValue(_T("Def")));
			m_clcItems.SetItemText(iCount, 2, TBSort.GetValue(_T("Color")));
			m_clcItems.SetItemText(iCount, 3, TBSort.GetValue(_T("ID")));
			m_clcItems.SetItemText(iCount, 4, TBSort.GetValue(_T("Type")));
			iCount++;
		}
	}
	*pResult = 0;
}