#pragma once

class CDialogPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPage)

public:
	CDialogPage(){};
	CDialogPage(UINT ID, HICON h_Icon, CWnd* pParent = NULL);
	virtual ~CDialogPage();
	virtual void OnCancel();
	virtual void PreSubclassWindow();
	int iCtrlID;

	CRect MinSize;
	CRect MinDock;
    BOOL initialized;

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOk();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
