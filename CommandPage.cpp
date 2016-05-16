
#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "CommandButton.h"
#include "CommandPage.h"

IMPLEMENT_DYNAMIC(CCommandPage, CDialogEx)

CCommandPage::CCommandPage(UINT ID, CWnd* pParent /*=NULL*/)
	: CDialogEx(ID, pParent)
{
}


CCommandPage::~CCommandPage()
{
	ClearPtrArray(&ptrButtonArray);
}

void CCommandPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCommandPage, CDialogEx)
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CCommandPage::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	SetDlgCtrlID(Main->iDlgCtrlID);
	Main->iDlgCtrlID++;

	Table TB = Main->m_DlgCommands->DB.QuerySQL(CMsg(_T("SELECT * FROM %1"),true,csTabText));
	TB.ResetRow();
	while(TB.GoNext())
		CreateButton(TB.GetValue(_T("Name")),TB.GetValue(_T("Command")));

	return TRUE;
}

BOOL CCommandPage::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if((pMsg->wParam == VK_ESCAPE)||(pMsg->wParam == VK_RETURN ))
		{return TRUE;}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CCommandPage::OnRButtonDown(UINT /*nFlags*/, CPoint point) 
{
	ClientToScreen(&point);
	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		::AppendMenu(hMenu, MF_STRING, 1, CMsg(_T("IDS_TABEDIT")));
		::AppendMenu(hMenu, MF_STRING, 2, CMsg(_T("IDS_TABREMOVE")));

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
				Main->m_DlgCommands->EditTab();
				break;
			}
		case 2:
			{
				Main->m_DlgCommands->RemoveTab();
				break;
			}
		}
		::DestroyMenu(hMenu);
	}
}

HBRUSH CCommandPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//Set Color of Tab's Background
	CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return ::CreateSolidBrush(Main->crBlueBkg);
}

bool CCommandPage::CreateButton(CString sCaption, CString sCommand)
{
	if(GetButtonCount() >= 30)
		return false;
	CRect rc;
	GetClientRect(&rc);
	int x = rc.left + MARGIN_LEFT;
	int y = rc.top + MARGIN_TOP;

	CDC* dcPtr = GetDC();
	CFont* fontPtr = GetFont();
	fontPtr = dcPtr->SelectObject(fontPtr);
	CSize sz = dcPtr->GetTextExtent(sCaption);
	fontPtr = dcPtr->SelectObject(fontPtr);
	ReleaseDC(dcPtr);
	sz.cy += 10;
	sz.cx = 72;
	CComButton* pButton = new CComButton;
	pButton->Create(sCaption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_TABSTOP,CRect(x,y,x+sz.cx,y+sz.cy),this,Main->iDlgCtrlID);
	pButton->SetFont(GetFont());
	pButton->ShowWindow(SW_SHOW);
	pButton->csCommand=sCommand;
	ptrButtonArray.Add(pButton);
	Main->iDlgCtrlID++;
	SortButtons();
	return true;
}

void CCommandPage::SortButtons()
{
	INT_PTR iCount = GetButtonCount();
	int X, Y;
	CRect rcTab;
	GetClientRect(&rcTab);
	X = rcTab.left + MARGIN_LEFT;
	Y = rcTab.top + MARGIN_TOP;

	for(INT_PTR i=0; i<iCount; i++)
	{
		CComButton* pItem = ptrButtonArray.GetAt(i);
		CRect rc;
		pItem->GetClientRect(&rc);
		pItem->MoveWindow(X, Y, rc.right, rc.bottom);
		Y += 28;
		if (Y > rcTab.bottom - rc.bottom)
		{
			Y = rcTab.top + MARGIN_TOP;
			X += 81;
		}
	}
}

void CCommandPage::SortDB()
{
	INT_PTR iCount = GetButtonCount();
	Main->m_DlgCommands->DB.BeginTransaction();
	Main->m_DlgCommands->DB.ExecuteSQL(CSQL(_T("DELETE FROM %1"),csTabText));
	for(INT_PTR i=0; i<iCount; i++)
	{
		CComButton* pItem = ptrButtonArray.GetAt(i);
		CString csName;
		pItem->GetWindowText(csName);
		Main->m_DlgCommands->DB.ExecuteSQL(CSQL(_T("INSERT INTO %1 VALUES('%2','%3')"),csTabText,csName,pItem->csCommand));
	}
	Main->m_DlgCommands->DB.CommitTransaction();

}

void CCommandPage::Reposition(int nID, INT_PTR iOffset)
{
	INT_PTR iCount = GetButtonCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		CComButton* pItem = ptrButtonArray.GetAt(i);
		if(pItem->GetDlgCtrlID() == nID)
		{
			if(i+iOffset >= iCount)
			{
				ptrButtonArray.RemoveAt(i);
				ptrButtonArray.Add(pItem);
			}
			else if(i+iOffset < 0)
			{
				ptrButtonArray.RemoveAt(i);
				ptrButtonArray.InsertAt(0,pItem);
			}
			else
			{	
				ptrButtonArray.RemoveAt(i);
				ptrButtonArray.InsertAt(i+iOffset,pItem);
			}
			SortButtons();
			SortDB();
			return;
		}
	}
}

void CCommandPage::RemoveButton(int nID)
{
	Main->m_DlgCommands->OnBnClickedtabCancel();
	Main->m_DlgCommands->OnBnClickedbuttonCancel();
	INT_PTR iCount = GetButtonCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		CComButton* pItem = ptrButtonArray.GetAt(i);
		if(pItem->GetDlgCtrlID() == nID)
		{
			CString csKey;
			pItem->GetWindowText(csKey);
			Main->m_DlgCommands->DB.ExecuteSQL(CSQL(_T("DELETE FROM %1 WHERE Name = '%2'"),csTabText,csKey));
			pItem->DestroyWindow();
			delete pItem;
			pItem = NULL;
			ptrButtonArray.RemoveAt(i);
			SortButtons();
			return;
		}
	}
}

void CCommandPage::EditButton(int nID)
{
	INT_PTR iCount = GetButtonCount();
	for(INT_PTR i=0; i<iCount; i++)
	{
		CComButton* pItem = ptrButtonArray.GetAt(i);
		if(pItem->GetDlgCtrlID() == nID)
		{
			Main->m_DlgCommands->pEditButton = pItem;
			Main->m_DlgCommands->EditButton();
			return;
		}
	}
}