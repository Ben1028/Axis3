
#pragma once

#include "RemoteConsoleDlg.h"
#include "DialogPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGeneralTab dialog

class CGeneralTab : public CDialogPage
{
	DECLARE_DYNCREATE(CGeneralTab)

// Construction
public:
	CGeneralTab(CWnd* pParent = NULL);
	virtual ~CGeneralTab();
	void SetLanguage();
	CRemoteConsoleDlg m_rcDlg;
	void HitConsole() { OnRconsole(); };

// Dialog Data
	enum { IDD = IDD_GENERAL };
	CSliderCtrl	m_cscSlider;
	CStatic	m_csSliderValue;
	CButton	m_cbRemoteConsole;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

// Implementation
protected:

	virtual BOOL OnInitDialog();

	afx_msg void OnRconsole();
	afx_msg void OnBnClickedAdmin();
	afx_msg void OnBnClickedInfo();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedClients();
	afx_msg void OnBnClickedServinfo();
	afx_msg void OnBnClickedVersion();

	afx_msg void OnBnClickedLink();
	afx_msg void OnBnClickedFlip();
	afx_msg void OnBnClickedShrink();
	afx_msg void OnBnClickedDupe();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedNuke();

	afx_msg void OnBnClickedBuy();
	afx_msg void OnBnClickedSell();
	afx_msg void OnBnClickedInventory();
	afx_msg void OnBnClickedPurchases();
	afx_msg void OnBnClickedSamples();
	afx_msg void OnBnClickedRestock();
	afx_msg void OnBnClickedSnow();
	afx_msg void OnBnClickedRain();
	afx_msg void OnBnClickedDry();
	afx_msg void OnBnClickedLightlevel();
	afx_msg void OnBnClickedInvul();
	afx_msg void OnBnClickedAllmove();
	afx_msg void OnBnClickedInvisible();
	afx_msg void OnBnClickedFix();
	afx_msg void OnBnClickedTele();
	afx_msg void OnBnClickedHearall();
	afx_msg void OnBnClickedGmtoggle();
	afx_msg void OnBnClickedDetail();
	afx_msg void OnBnClickedNightsight();
	afx_msg void OnBnClickedDebug();

	afx_msg void OnBnClickedJail();
	afx_msg void OnBnClickedForgive();
	afx_msg void OnBnClickedKill();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnBnClickedResurrect();

	afx_msg void OnBnClickedPageon();
	afx_msg void OnBnClickedPagelist();
	afx_msg void OnBnClickedPagegoplayer();
	afx_msg void OnBnClickedPagedisconnect();
	afx_msg void OnBnClickedPageban();
	afx_msg void OnBnClickedPageoff();
	afx_msg void OnBnClickedPagequeue();
	afx_msg void OnBnClickedPagegoorigin();
	afx_msg void OnBnClickedPagejail();
	afx_msg void OnBnClickedPagedelete();

	afx_msg void OnBnClickedWorldsave();
	afx_msg void OnBnClickedSavestatics();
	afx_msg void OnBnClickedResync();
	afx_msg void OnBnClickedRestockall();

	DECLARE_MESSAGE_MAP()
};
