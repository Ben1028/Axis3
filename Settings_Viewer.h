
#pragma once

class CSettingViewer : public CDialogEx
{
public:
	CSettingViewer(CWnd* pParent = NULL);

	DWORD m_dwItemBGColor;

	enum { IDD = IDD_SETTING_VIEWER };
	BOOL	m_bItemViewScale;
	BOOL	m_bMultiViewScale;
	CStatic	m_csBGColor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();
	afx_msg void OnResetItemCache();
	afx_msg void OnResetMultiCache();
	afx_msg void OnResetMapCache();
	afx_msg void OnBGColor();
	afx_msg void OnPaint();
	afx_msg void OnScaleItems();
	afx_msg void OnScaleMultis();
	afx_msg void OnResetBGColor();
	DECLARE_MESSAGE_MAP()

};