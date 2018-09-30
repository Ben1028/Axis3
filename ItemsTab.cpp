
#include "stdafx.h"
#include "Axis3.h"
#include "ItemsTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CItemsTab, CDialogPage)

CItemsTab::CItemsTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CItemsTab::IDD, AfxGetApp()->LoadIcon(IDI_ITEMS), pParent)
{

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
}


BEGIN_MESSAGE_MAP(CItemsTab, CDialogPage)
	ON_CBN_SELCHANGE(IDC_CATDROP, OnSelchangeCat)
	ON_CBN_SELCHANGE(IDC_SUBDROP, OnSelchangeSub)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEMS, OnItemchangedItems)
END_MESSAGE_MAP()

BOOL CItemsTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_ITEMS")));
	CDialogPage::OnInitDialog();
	m_clcItems.InsertColumn(0, _T("Item Description"), LVCFMT_LEFT, 124, -1);
	m_clcItems.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 124, -1);
	FillCategory();
	cMulDisplay.InitDisplay();
	//cMulDisplay.DrawAnim(2);
	//cMulDisplay.CenterArt();
	//cMulDisplay.DrawArt(1145);
	//cMulDisplay.SetScale(1);
	//cMulDisplay.DrawMap(4);
	//TempX = 1700;
	//cMulDisplay.DisplayObj();

	return TRUE;
}

void CItemsTab::FillCategory()
{
	//Fill Combobox

	Table TBCats = Axis->DBData.QuerySQL(_T("SELECT Cat FROM Item GROUP BY Cat ORDER BY Cat"));
	TBCats.ResetRow();
	while (TBCats.GoNext())
		m_ccbCategories.AddString(TBCats.GetValue(_T("Cat")));

	m_ccbCategories.SetCurSel(0);
	OnSelchangeCat();

	UpdateData(FALSE);
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
	Table TBSubs = Axis->DBData.QuerySQL(CSQL(_T("SELECT Cat,Sub FROM Item WHERE Cat = '%1' GROUP BY Sub ORDER BY Sub"), csCat));
	TBSubs.ResetRow();
	while (TBSubs.GoNext())
		m_ccbSubsections.AddString(TBSubs.GetValue(_T("Sub")));

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

	Table TBDecs = Axis->DBData.QuerySQL(CSQL(_T("SELECT * FROM Item WHERE Cat = '%1' AND Sub = '%2' ORDER BY Desc"), csCat, csSub));
	TBDecs.ResetRow();
	int iCount = 0;
	while (TBDecs.GoNext())
	{
		m_clcItems.InsertItem(iCount, TBDecs.GetValue(_T("Desc")), iCount);
		CString cs_ItemID = TBDecs.GetValue(_T("Def"));
		cs_ItemID.MakeLower();
		m_clcItems.SetItemText(iCount, 1, cs_ItemID);
		iCount++;
	}
}

void CItemsTab::OnItemchangedItems(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		int iSelIndex = m_clcItems.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
		{
			//this->m_csItemID.SetWindowText("");
			//m_csItemIDDec.SetWindowText("");
			cMulDisplay.DrawArt(-1,0);
			return;
		}
		/*m_clcItems.SetHotItem(iSelIndex);
		m_clcItems.GetItemText(iSelIndex, 1);
		Table TBDecs = Axis->DBData.QuerySQL(CSQL(_T("SELECT Def,ID FROM Item WHERE Def = '%1'"), csCat, csSub));
		cMulDisplay.DrawArt(ahextoi(TBDecs.GetValue(_T("ID"))), 0);*/
		/*CSObject * pObject = (CSObject *) this->m_clcItems.GetItemData(iSelIndex);
		if (!pObject)
			return;
		if (pObject->m_csFilename == "")
			return;*/

		/*CString cs_ItemID = pObject->m_csID;
		cs_ItemID.MakeLower();
		this->m_csItemID.SetWindowText(cs_ItemID);*/

		/*DWORD wDisp = -1;
		if (alltoi(pObject->m_csID, pObject->m_csValue) != 0)
			wDisp = (DWORD)alltoi(pObject->m_csID, pObject->m_csValue);
		if (wDisp == 0xFFFFFFFF)
		{
			if (alltoi(pObject->m_csDisplay, pObject->m_csValue) != 0)
				wDisp = (DWORD)alltoi(pObject->m_csDisplay, pObject->m_csValue);
		}
		DWORD wDispCol = 0;
		if (alltoi(pObject->m_csColor, pObject->m_csValue) != 0)
			wDispCol = (DWORD)alltoi(pObject->m_csColor, pObject->m_csValue);
		CString cs_ItemIDDec;
		cs_ItemIDDec.Format("ID:0x%X    Hue:0x%X", wDisp, wDispCol);
		this->m_csItemIDDec.SetWindowText(cs_ItemIDDec);*/

		/*if (Main->m_dwShowItems)
		{
			WORD wID = -1;
			if (alltoi(pObject->m_csDisplay, pObject->m_csValue) != 0)
				wID = (WORD)alltoi(pObject->m_csDisplay, pObject->m_csValue);

			if (pObject->m_bType == TYPE_MULTI)
				m_Display.SetArtType(0);
			else
				m_Display.SetArtType(1);

			m_Display.SetArtIndex(wID);

			WORD wColor = 0;
			if (alltoi(pObject->m_csColor, pObject->m_csValue) != 0)
				wColor = (WORD)alltoi(pObject->m_csColor, pObject->m_csValue);

			m_Display.SetArtColor(wColor);
		}*/

		return;
	}
	//this->m_csItemID.SetWindowText("");
	//m_csItemIDDec.SetWindowText("");
	cMulDisplay.DrawArt(-1, 0);
	*pResult = 0;
}