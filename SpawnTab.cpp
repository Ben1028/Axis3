
#include "stdafx.h"
#include "Axis3.h"
#include "SpawnTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSpawnTab, CDialogPage)

CSpawnTab::CSpawnTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CSpawnTab::IDD, AfxGetApp()->LoadIcon(IDI_CHAR), pParent)
{

}


CSpawnTab::~CSpawnTab()
{
}

void CSpawnTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpawnTab, CDialogPage)
END_MESSAGE_MAP()

BOOL CSpawnTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_SPAWN")));
	CDialogPage::OnInitDialog();

	return TRUE;
}
