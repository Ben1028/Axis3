
#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "CommandButton.h"

IMPLEMENT_DYNAMIC(CComButton, CButton)

CComButton::CComButton()
{
	m_pOldDragCapture = NULL;
    m_bDragging = false;
}

CComButton::~CComButton()
{
}

BEGIN_MESSAGE_MAP(CComButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CComButton::OnBnClicked)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CComButton::OnBnClicked()
{
	SendToUO(csCommand);
}

void CComButton::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	ClientToScreen(&point);
	HMENU hMenu = ::CreatePopupMenu();
	if (NULL != hMenu)
	{
		::AppendMenu(hMenu, MF_STRING, 2, CMsg(_T("IDS_BUTTONEDIT")));
		::AppendMenu(hMenu, MF_STRING, 1, CMsg(_T("IDS_BUTTONREMOVE")));

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
				CCommandPage * pComPage = (CCommandPage *)GetParent();
				pComPage->RemoveButton(GetDlgCtrlID());
				break;
			}
		case 2:
			{
				CCommandPage * pComPage = (CCommandPage *)GetParent();
				pComPage->EditButton(GetDlgCtrlID());
				break;
			}
		}
		::DestroyMenu(hMenu);
	}
}

void CComButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON)
	{
		if(m_pOldDragCapture == NULL)
		{
			m_pOldDragCapture = SetCapture();        
			GetCursorPos(&m_ptStartDrag);
			CRect rect;
			GetWindowRect(&rect);
			m_ptStartPos = rect.TopLeft();
		}
		m_bDragging = true;
		CRect rect;
		::GetWindowRect(m_hWnd,&rect);		
		CPoint ptMouse;
		GetCursorPos(&ptMouse);
		CSize sizeDiff = ptMouse - m_ptStartDrag;
		CSize sizeMove = m_ptStartPos-rect.TopLeft();
		rect.OffsetRect(sizeMove);						
		rect.OffsetRect(sizeDiff.cx, sizeDiff.cy);

		GetParent()->ScreenToClient(&rect);			
		MoveWindow(rect);		
	}
	CButton::OnMouseMove(nFlags, point);
}

void CComButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_pOldDragCapture != NULL)
	{		
		ReleaseCapture();
		m_pOldDragCapture = NULL;
	}

	if(m_bDragging)
	{
		m_bDragging = false;
		GetParent()->Invalidate();

		CRect rect;
		GetWindowRect(&rect);
		CPoint m_ptEndPos = rect.TopLeft();
		INT_PTR iNewX, iNewY;
		iNewX = (m_ptEndPos.x - m_ptStartPos.x);
		(iNewX >= 0) ? (iNewX+=41) : (iNewX-=41);
		iNewX = (iNewX/81)*5;
		iNewY = (m_ptEndPos.y - m_ptStartPos.y);
		(iNewY >= 0) ? (iNewY+=14) : (iNewY-=14);
		iNewY = iNewY/28;
		CCommandPage * pComPage = (CCommandPage *)GetParent();
		pComPage->Reposition(GetDlgCtrlID(),iNewX+iNewY);
	}

	CButton::OnLButtonUp(nFlags, point);
}