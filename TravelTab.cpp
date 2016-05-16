
#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "TravelTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTravelTab, CDialogPage)

CTravelTab::CTravelTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CTravelTab::IDD, AfxGetApp()->LoadIcon(IDI_WORLD), pParent)
{

}


CTravelTab::~CTravelTab()
{
}

void CTravelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP, cMapDisplay);
}


BEGIN_MESSAGE_MAP(CTravelTab, CDialogPage)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CTravelTab::OnInitDialog() 
{
	SetWindowText(CMsg(_T("IDS_TRAVEL")));
	CDialogPage::OnInitDialog();

	cMapDisplay.InitDisplay();
	cMapDisplay.DrawMap(4);
	cMapDisplay.SetScale(0);
	cMapDisplay.DisplayObj();
	SetFocus();

	return TRUE;
}

BOOL CTravelTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UNREFERENCED_PARAMETER(nFlags);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if ( rect.PtInRect(pt) )
	{
		if(zDelta > 0)
			cMapDisplay.SetScale(cMapDisplay.GetScale()+1);
		else
			cMapDisplay.SetScale(cMapDisplay.GetScale()-1);
		cMapDisplay.DisplayObj();
	}
	return true;
}

void CTravelTab::OnLButtonDown(UINT nFlags, CPoint pt) 
{
	UNREFERENCED_PARAMETER(nFlags);
	ClientToScreen(&pt);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if ( rect.PtInRect(pt) )
	{
		this->SetFocus();
	}
}