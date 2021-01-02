
#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "DialogPage.h"

IMPLEMENT_DYNAMIC(CDialogPage, CDialogEx)

CDialogPage::CDialogPage(UINT ID, HICON h_Icon ,CWnd* pParent /*=NULL*/)
	: CDialogEx(ID, pParent)
{
	initialized = FALSE;
	m_hIcon = h_Icon;
}


CDialogPage::~CDialogPage()
{
}

void CDialogPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogPage, CDialogEx)
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDOK, OnOk)
	ON_COMMAND(IDCLOSE, OnClose)
END_MESSAGE_MAP()

void CDialogPage::PreSubclassWindow() 
{
	ModifyStyleEx(WS_EX_DLGMODALFRAME, 0);
	ModifyStyle(WS_CHILD, WS_POPUP);

	CDialogEx::PreSubclassWindow();
}

BOOL CDialogPage::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	//Set CtrlID of Tab Pages
	iCtrlID = Main->iDlgCtrlID;
	SetDlgCtrlID(Main->iDlgCtrlID);
	Main->iDlgCtrlID++;

	//Set Icon and Position of Tab
	SetIcon(m_hIcon, FALSE);
	CenterWindow();

	GetWindowRect(&MinSize);
    ScreenToClient(&MinSize);
	GetClientRect(&MinDock);
    ScreenToClient(&MinDock);

	//Dock the Page to the Main Dialog
	OnCancel();

	return TRUE;
}

void CDialogPage::OnRButtonDown(UINT /*nFlags*/, CPoint point) 
{
	//Dock/Undock Menu
	ClientToScreen(&point);
	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		DWORD dwCurStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		if (dwCurStyle & WS_CHILD)
			::AppendMenu(hMenu, MF_STRING, 1, CMsg(_T("Undock Tab")));
		else
			::AppendMenu(hMenu, MF_STRING, 2, CMsg(_T("Dock Tab")));
		int sel = ::TrackPopupMenuEx(hMenu, 
				TPM_CENTERALIGN | TPM_RETURNCMD,
				point.x,
				point.y,
				m_hWnd,
				NULL);
		switch (sel)
		{
		case 1:
			{
				//Undock Tab from Main Dialog
				//Change Style/Parent/Caption to Dialog form  WS_EX_APPWINDOW
				ModifyStyle(WS_CHILD, WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME);
				::SetWindowText(GetSafeHwnd(), Main->m_TabCtrl.GetTabText(Main->m_TabCtrl.GetSel()));
				::SetParent(GetSafeHwnd(), NULL);
				//Position Window
				CenterWindowEx(m_hWnd);
				//Remove Tab from Main Dialog
				Main->m_TabCtrl.Delete(Main->m_TabCtrl.GetSel());
				Main->m_TabCtrl.Update();

			break;
			}
		case 2:
			{
				//Dock Tab to Main Dialog
				OnCancel();
			break;
			}
		}
		::DestroyMenu(hMenu);
	}
}

void CDialogPage::OnCancel()
{
	//Dock Tab to Main Dialog
	//Hide Window
	::ShowWindow(GetSafeHwnd(),SW_HIDE);
	//Reset Style/Parent/ID/Caption to Tab form
	ModifyStyle(WS_THICKFRAME | WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, WS_CHILD);
	::ShowWindow(GetSafeHwnd(), SW_RESTORE);
	::SetParent(GetSafeHwnd(), Main->m_TabCtrl.GetSafeHwnd());
	SetDlgCtrlID(iCtrlID);
	CString csCaption;
	GetWindowText(csCaption);
	//Add Tab to Main Dialog
	Main->m_TabCtrl.SetSel(Main->m_TabCtrl.AddEx(GetSafeHwnd(),csCaption,-1));
	Main->m_TabCtrl.Update();

	return;
}

HBRUSH CDialogPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//Set Color of Tab's Background
	CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	switch (nCtlColor) 
	{
		case CTLCOLOR_EDIT:
		case CTLCOLOR_LISTBOX:
			return Main->hbWhite;
		default:
			return Main->hbBlue;
	}
}

void CDialogPage::OnOk()
{
}

void CDialogPage::OnClose()
{
	CDialogEx::OnOK();
}

void CDialogPage::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if(initialized)
	{
		lpMMI->ptMinTrackSize.x = MinSize.Width();
		lpMMI->ptMinTrackSize.y = MinSize.Height();
	}   
    
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}