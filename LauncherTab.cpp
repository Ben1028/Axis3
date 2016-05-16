
#include "stdafx.h"
#include "Axis3.h"
#include "LauncherTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CLauncherTab, CDialogPage)

CLauncherTab::CLauncherTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CLauncherTab::IDD, AfxGetApp()->LoadIcon(IDI_LAUNCHER), pParent)
{

}


CLauncherTab::~CLauncherTab()
{
}

void CLauncherTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLauncherTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CLauncherTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_LAUNCHER")));
	CDialogPage::OnInitDialog();

	return TRUE;
}
