#include "DialogPage.h"
#include "AccountTab.h"
#include "CharTweakTab.h"
#include "CommandsTab.h"
#include "GeneralTab.h"
#include "ItemsTab.h"
#include "ItemsTweakTab.h"
#include "LauncherTab.h"
#include "MiscTab.h"
#include "ReminderTab.h"
#include "SpawnTab.h"
#include "TravelTab.h"
#include "ProfileDlg.h"
#include "SettingEdit.h"
#include "LangEdit.h"
#include "Settings_Viewer.h"
#include "Settings_General.h"

#pragma once

// CAxis3Dlg dialog
class CAxis3Dlg : public CDialogEx, 
	public TabCtrlNotify
{
public:
	CAxis3Dlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CAxis3Dlg();
	TabCtrlEx<TabCtrlStyle_VS2008_client_blue> m_TabCtrl;
	int iDlgCtrlID;
	HBRUSH hbWhite;
	HBRUSH hbBlue;

	CRect MinSize;
	CRect TabSize;
    BOOL initialized;

	NOTIFYICONDATA m_nid;

	CGeneralTab * m_DlgGeneral;
	CTravelTab * m_DlgTravel;
	CSpawnTab * m_DlgSpawn;
	CCharTweakTab * m_DlgCharTweak;
	CItemsTab * m_DlgITems;
	CItemTweakTab * m_DlgItemTweak;
	CAccountTab * m_DlgAccount;
	CMiscTab * m_DlgMisc;
	CLauncherTab * m_DlgLauncher;
	CCommandsTab * m_DlgCommands;
	CReminderTab * m_DlgReminder;

	//Profile variables
	CString m_csAddress;
	CString m_csPort;
	CString m_csAccount;
	CString m_csPassword;
	CString m_csPath;
	int m_iReceiveTimeout;

private:
	void SetTabsPosition();
	virtual void OnTabSelected(TabCtrl *pCtrl, HTAB hTab);


// Dialog Data
	enum { IDD = IDD_Axis3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);


// Implementation
protected:
	HICON m_hIcon;

	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOk();
	afx_msg void OnClose();
	afx_msg void OnProfile();
	afx_msg void OnExitCloseaxis();
	afx_msg void OnHelpAboutAxis3();
	afx_msg void OnHelpDocumentation();
	afx_msg void OnSettingDefault();
	afx_msg void OnDataDefaultSettings();
	afx_msg void OnDataLanguage();
	afx_msg void OnSettingViewer();

	DECLARE_MESSAGE_MAP()
};

extern CAxis3Dlg * Main;

UINT ProfileThread(LPVOID lpParam);
UINT ProfileLocalThread(LPVOID lpParam);