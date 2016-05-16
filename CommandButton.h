#pragma once

#define IDC_RANGE_START 1000
#define IDC_RANGE_END	2000
#define MARGIN_TOP 7
#define MARGIN_LEFT 7

class CComButton : public CButton
{
	DECLARE_DYNAMIC(CComButton)

public:
	CComButton();
	virtual ~CComButton();
	CString csCommand;
	std::vector<DWORD> m_vtTopLeftControls;
	std::vector<DWORD> m_vtBottomRightControls;
	CWnd * m_pOldDragCapture;
	CPoint m_ptStartDrag,m_ptStartPos;  
    bool m_bDragging;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};