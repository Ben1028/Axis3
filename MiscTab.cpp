
#include "stdafx.h"
#include "Axis3.h"
#include "MiscTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMiscTab, CDialogPage)

CMiscTab::CMiscTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CMiscTab::IDD, AfxGetApp()->LoadIcon(IDI_MISC), pParent)
{

}


CMiscTab::~CMiscTab()
{
}

void CMiscTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMiscTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CMiscTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("Misc")));
	CDialogPage::OnInitDialog();

	return TRUE;
}
