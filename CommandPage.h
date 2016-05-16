#pragma once
#include "CommandButton.h"

class CCommandPage : public CDialogEx
{
	DECLARE_DYNAMIC(CCommandPage)

public:
	CTypedPtrArray<CPtrArray, CComButton*> ptrButtonArray;
	CString csTabText;

	CCommandPage(){};
	CCommandPage(UINT ID, CWnd* pParent = NULL);
	virtual ~CCommandPage();
	bool CreateButton(CString sCaption, CString sCommand);
	void RemoveButton(int nID);
	void EditButton(int nID);
	void SortButtons();
	void SortDB();
	void Reposition(int nID, INT_PTR iOffset);
	INT_PTR GetButtonCount(){ return ptrButtonArray.GetCount();}

	enum { IDD = IDD_COMPAGE };

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
};
