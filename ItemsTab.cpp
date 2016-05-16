
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
	DDX_Control(pDX, IDC_CATTREE, m_ctcCategories);
	DDX_Control(pDX, IDC_ART, cMulDisplay);
}


BEGIN_MESSAGE_MAP(CItemsTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CItemsTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_ITEMS")));
	CDialogPage::OnInitDialog();
	FillCategoryTree();
	cMulDisplay.InitDisplay();
	//cMulDisplay.DrawAnim(2);
	//cMulDisplay.CenterArt();
	//cMulDisplay.DrawArt(1145);
	//cMulDisplay.SetScale(1);
	cMulDisplay.DrawMap(4);
	//TempX = 1700;
	cMulDisplay.DisplayObj();

	return TRUE;
}

/*void CItemsTab::OnTest()
{
	cMulDisplay.SetScale(cMulDisplay.GetScale()+1);
	cMulDisplay.DisplayObj();
}*/

void CItemsTab::FillCategoryTree()
{
	HTREEITEM CategoryParent = NULL;
	HTREEITEM SubsectionParent = NULL;
	TV_INSERTSTRUCT InsertItem;

	m_ctcCategories.DeleteAllItems();
	if(Axis->DBData.IsOpen())
	{
		Table TBTree = Axis->DBData.QuerySQL(_T("SELECT Cat,Sub,Desc FROM Item ORDER BY Cat,Sub,Desc"));
		TBTree.ResetRow();
		LPCTSTR stCat = _T(" ");
		LPCTSTR stSub = _T(" ");
		while(TBTree.GoNext())
		{
			if (lstrcmpi(stCat,TBTree.GetValue(_T("Cat"))))
			{
				stCat = TBTree.GetValue(_T("Cat"));
				InsertItem.item.mask = TVIF_TEXT;
				InsertItem.item.pszText = LPTSTR(stCat);
				InsertItem.item.cchTextMax = strlen((char *)stCat);
				InsertItem.hParent = NULL;
				InsertItem.hInsertAfter = TVI_SORT;
				CategoryParent = m_ctcCategories.InsertItem(&InsertItem);
				stSub = _T(" ");
			}

			if (lstrcmpi(stSub,TBTree.GetValue(_T("Sub"))))
			{
				stSub = TBTree.GetValue(_T("Sub"));
				InsertItem.item.mask = TVIF_TEXT;
				InsertItem.item.pszText = LPTSTR(stSub);
				InsertItem.item.cchTextMax = strlen((char *)stSub);
				InsertItem.hParent = CategoryParent;
				InsertItem.hInsertAfter = TVI_SORT;
				SubsectionParent = m_ctcCategories.InsertItem(&InsertItem);
			}

			LPCTSTR stDesc = TBTree.GetValue(_T("Desc"));
			InsertItem.item.mask = TVIF_TEXT;
			InsertItem.item.pszText = LPTSTR(stDesc);
			InsertItem.item.cchTextMax = strlen((char *)stDesc);
			InsertItem.hParent = SubsectionParent;
			InsertItem.hInsertAfter = TVI_SORT;
			m_ctcCategories.InsertItem(&InsertItem);
		}
	}
}