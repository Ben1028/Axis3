
#include "stdafx.h"
#include "Axis3.h"
#include "ItemsTweakTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CItemTweakTab, CDialogPage)

CItemTweakTab::CItemTweakTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CItemTweakTab::IDD, AfxGetApp()->LoadIcon(IDI_EDITITEM), pParent)
{

}


CItemTweakTab::~CItemTweakTab()
{
}

void CItemTweakTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CItemTweakTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CItemTweakTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("Item Tweak")));
	CDialogPage::OnInitDialog();

	return TRUE;
}
