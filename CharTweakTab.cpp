
#include "stdafx.h"
#include "Axis3.h"
#include "CharTweakTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCharTweakTab, CDialogPage)

CCharTweakTab::CCharTweakTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CCharTweakTab::IDD, AfxGetApp()->LoadIcon(IDI_EDITCHAR), pParent)
{

}


CCharTweakTab::~CCharTweakTab()
{
}

void CCharTweakTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharTweakTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CCharTweakTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_PLAYER_TWEAK")));
	CDialogPage::OnInitDialog();

	return TRUE;
}
