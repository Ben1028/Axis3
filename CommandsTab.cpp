
#include "stdafx.h"
#include "Axis3.h"
#include "CommandsTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCommandsTab, CDialogPage)

CCommandsTab::CCommandsTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CCommandsTab::IDD, AfxGetApp()->LoadIcon(IDI_COMMANDS), pParent)
{
	pEditButton = NULL;
}

CCommandsTab::~CCommandsTab()
{
	ClearPtrArray(&p_DlgTabs);
	DB.Close();
	pEditButton = NULL;
}

void CCommandsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLABEL, ceTabName);
	DDX_Control(pDX, IDC_BUTTONLABEL, ceButtonName);
	DDX_Control(pDX, IDC_COM, creButtonCommand);

	DDX_Control(pDX, IDC_ADDBUTTON, cbAddButton);
	DDX_Control(pDX, IDC_BUTTONCANCEL, cbCancelButton);
	DDX_Control(pDX, IDC_BUTTONOK, cbButtonOk);
	DDX_Control(pDX, IDC_ADDTAB, cbAddTab);
	DDX_Control(pDX, IDC_TABCANCEL, cbCancelTab);
	DDX_Control(pDX, IDC_TABOK, cbTabOk);
}

BEGIN_MESSAGE_MAP(CCommandsTab, CDialogPage)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnBnClickedAddbutton)
	ON_BN_CLICKED(IDC_ADDTAB, OnBnClickedAddtab)
	ON_BN_CLICKED(IDC_TABOK, OnBnClickedtabOK)
	ON_BN_CLICKED(IDC_TABCANCEL, OnBnClickedtabCancel)
	ON_BN_CLICKED(IDC_BUTTONOK, OnBnClickedbuttonOK)
	ON_BN_CLICKED(IDC_BUTTONCANCEL, OnBnClickedbuttonCancel)
	ON_WM_SIZE()

END_MESSAGE_MAP()

BOOL CCommandsTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Commands")));
	CDialogPage::OnInitDialog();

	ModifyStyle(0,WS_CLIPCHILDREN);
	m_ComTabCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),3001);

	DB.Open(CFrmt(_T("%1Command"), Axis->m_csRootDirectory));
	LoadTabs();
	SetTabsPosition();
	m_ComTabCtrl.SetBehavior(TAB_BEHAVIOR_SCALE);
	m_ComTabCtrl.SetNotifyManager(this);
	m_ComTabCtrl.SetLayout(TAB_LAYOUT_BOTTOM);
	m_ComTabCtrl.Update();
	m_ComTabCtrl.SetSel(m_ComTabCtrl.GetFirstEnableTab());

	GetDlgItem(IDC_COM_MACRO)->SetWindowText(CMsg(_T("Command / Macro")));
	GetDlgItem(IDC_ADDBUTTON)->SetWindowText(CMsg(_T("Add Button")));
	GetDlgItem(IDC_ADDTAB)->SetWindowText(CMsg(_T("Add Tab")));
	cbCancelTab.InitButton(IDI_CANCEL,CMsg(_T("Cancel")));
	cbTabOk.InitButton(IDI_OK,CMsg(_T("OK")));
	cbCancelButton.InitButton(IDI_CANCEL,CMsg(_T("Cancel")));
	cbButtonOk.InitButton(IDI_OK,CMsg(_T("OK")));
	Axis->DBLng.CommitTransaction();

	m_ComTabCtrl.GetWindowRect(crComTabCtrl);
	ScreenToClient(crComTabCtrl);
	creButtonCommand.GetWindowRect(crButtonCommand);
	ScreenToClient(crButtonCommand);
	ceTabName.GetWindowRect(crTabName);
	ScreenToClient(crTabName);
	cbAddTab.GetWindowRect(crAddTab);
	ScreenToClient(crAddTab);
	cbCancelTab.GetWindowRect(crCancelTab);
	ScreenToClient(crCancelTab);
	cbTabOk.GetWindowRect(crTabOk);
	ScreenToClient(crTabOk);

	initialized = TRUE;

	return TRUE;
}

void CCommandsTab::OnTabSelected(TabCtrl * /*pCtrl*/, HTAB hTab)
{
	CCommandPage * pComPage = p_DlgTabs.GetAt(m_ComTabCtrl.GetIndex(hTab));
	pComPage->SortButtons();
	OnBnClickedtabCancel();
	OnBnClickedbuttonCancel();
}

void CCommandsTab::SetTabsPosition()
{	CWnd *pBaseWnd = GetDlgItem(IDC_COMTAB);

	if(pBaseWnd!=NULL)
	{	
		CRect rcTab;
		pBaseWnd->GetWindowRect(&rcTab);
		ScreenToClient(&rcTab);
		rcTab.right = max(rcTab.left,rcTab.right);
		rcTab.bottom = max(rcTab.top,rcTab.bottom);
		m_ComTabCtrl.MoveWindow(&rcTab);
	}
}


void CCommandsTab::OnBnClickedAddbutton()
{
	OnBnClickedtabCancel();
	OnBnClickedbuttonCancel();
	CString csLabel = GetEditString(ceButtonName);
	CString csCommand = GetEditString(creButtonCommand);

	if (csLabel == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a Label")), MB_ICONSTOP);
		return;
	}

	Table TB = DB.QuerySQL(CFrmt(_T("SELECT * FROM %1 WHERE Name LIKE '%2'"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel()),csLabel));
	if(TB.GetRowCount()>0)
	{
		AfxMessageBox(CMsg(_T("Button '%1' already exists"),true,csLabel), MB_ICONSTOP);
		return;
	}

	if (csCommand == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a command")), MB_ICONSTOP);
		return;
	}

	CCommandPage * pComPage = p_DlgTabs.GetAt(m_ComTabCtrl.GetIndex(m_ComTabCtrl.GetSel()));
	if(pComPage->CreateButton(csLabel,csCommand))
		DB.ExecuteSQL(CFrmt(_T("INSERT INTO %1 VALUES('%2','%3')"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel()),csLabel,csCommand));
	else
		AfxMessageBox(CMsg(_T("Tab '%1' is full!"),true,m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel())), MB_ICONSTOP);
}


void CCommandsTab::OnBnClickedAddtab()
{
	OnBnClickedtabCancel();
	OnBnClickedbuttonCancel();
	CString csLabel = GetEditString(ceTabName);

	if (csLabel == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a Label")), MB_ICONSTOP);
		return;
	}

	Table TB = DB.QuerySQL(CFrmt(_T("SELECT * FROM TABSLIST WHERE TabName LIKE '%1'"),csLabel));
	if(TB.GetRowCount()>0)
	{
		AfxMessageBox(CMsg(_T("Tab '%1' already exists"),true,csLabel), MB_ICONSTOP);
		return;
	}

	DB.ExecuteSQL(CFrmt(_T("CREATE TABLE '%1' (Name TEXT, Command TEXT)"),csLabel));
	DB.ExecuteSQL(CFrmt(_T("INSERT INTO TABSLIST VALUES('%1')"),csLabel));

	CCommandPage * m_DlgTab = new CCommandPage;
	m_DlgTab->csTabText = csLabel;
	m_DlgTab->Create(m_DlgTab->IDD,&m_ComTabCtrl);
	m_ComTabCtrl.Add(*m_DlgTab,csLabel,-1);
	p_DlgTabs.Add(m_DlgTab);
	m_ComTabCtrl.Update();
}

void CCommandsTab::LoadTabs()
{
	Table TB = DB.QuerySQL(_T("SELECT * FROM TABSLIST"));
	TB.ResetRow();
	while(TB.GoNext())
	{
		CCommandPage * m_DlgTab = new CCommandPage;
		m_DlgTab->csTabText = TB.GetValue(_T("TabName"));
		m_DlgTab->Create(m_DlgTab->IDD,&m_ComTabCtrl);
		m_ComTabCtrl.Add(*m_DlgTab,TB.GetValue(_T("TabName")),-1);
		p_DlgTabs.Add(m_DlgTab);
		m_ComTabCtrl.Update();
	}
}

void CCommandsTab::RemoveTab()
{
	OnBnClickedtabCancel();
	OnBnClickedbuttonCancel();
	if (m_ComTabCtrl.GetCount() > 1)
	{
		INT_PTR pID = m_ComTabCtrl.GetIndex(m_ComTabCtrl.GetSel());
		CCommandPage * pComPage = p_DlgTabs.GetAt(pID);
		DB.ExecuteSQL(CFrmt(_T("DELETE FROM TABSLIST WHERE TabName = '%1'"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel())));
		DB.ExecuteSQL(CFrmt(_T("DROP TABLE %1"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel())));
		pComPage->DestroyWindow();
		delete pComPage;
		pComPage = NULL;
		p_DlgTabs.RemoveAt(pID);
		m_ComTabCtrl.Delete(m_ComTabCtrl.GetSel());
		m_ComTabCtrl.Update();
	}else
	{
		AfxMessageBox(CMsg(_T("Can't delete the last tab")), MB_ICONSTOP);
	}
}

void CCommandsTab::EditTab()
{
	OnBnClickedbuttonCancel();
	ceTabName.SetWindowText(m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel()));
	cbAddTab.ShowWindow(SW_HIDE);
	cbCancelTab.ShowWindow(SW_SHOW);
	cbTabOk.ShowWindow(SW_SHOW);
}

void CCommandsTab::OnBnClickedtabOK()
{
	INT_PTR pID = m_ComTabCtrl.GetIndex(m_ComTabCtrl.GetSel());
	CCommandPage * pComPage = p_DlgTabs.GetAt(pID);
	CString csLabel = GetEditString(ceTabName);

	if (csLabel == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a Label")), MB_ICONSTOP);
		return;
	}

	if (csLabel==pComPage->csTabText)
	{
		OnBnClickedtabCancel();
		return;
	}

	Table TB = DB.QuerySQL(CFrmt(_T("SELECT * FROM TABSLIST WHERE TabName LIKE '%1'"),csLabel));
	if(TB.GetRowCount()>0)
	{
		AfxMessageBox(CMsg(_T("Tab '%1' already exists"),true,csLabel), MB_ICONSTOP);
		return;
	}
	DB.ExecuteSQL(CFrmt(_T("UPDATE TABSLIST SET TabName = '%1' WHERE TabName = '%2'"),csLabel,pComPage->csTabText));
	DB.ExecuteSQL(CFrmt(_T("ALTER TABLE '%1' RENAME TO '%2'"),pComPage->csTabText,csLabel));
	
	pComPage->csTabText = csLabel;
	m_ComTabCtrl.SetTabText(m_ComTabCtrl.GetSel(),csLabel);
	m_ComTabCtrl.Update();
	OnBnClickedtabCancel();
}

void CCommandsTab::OnBnClickedtabCancel()
{
	cbAddTab.ShowWindow(SW_SHOW);
	cbCancelTab.ShowWindow(SW_HIDE);
	cbTabOk.ShowWindow(SW_HIDE);
}

void CCommandsTab::EditButton()
{
	OnBnClickedtabCancel();
	creButtonCommand.SetWindowText(pEditButton->csCommand);
	CString csTitle;
	pEditButton->GetWindowText(csTitle);
	ceButtonName.SetWindowText(csTitle);
	cbAddButton.ShowWindow(SW_HIDE);
	cbCancelButton.ShowWindow(SW_SHOW);
	cbButtonOk.ShowWindow(SW_SHOW);
}

void CCommandsTab::OnBnClickedbuttonCancel()
{
	pEditButton = NULL;
	cbAddButton.ShowWindow(SW_SHOW);
	cbCancelButton.ShowWindow(SW_HIDE);
	cbButtonOk.ShowWindow(SW_HIDE);
}

void CCommandsTab::OnBnClickedbuttonOK()
{
	CString csTitle;
	pEditButton->GetWindowText(csTitle);
	CString csLabel = GetEditString(ceButtonName);
	CString csCommand = GetEditString(creButtonCommand);
	if (csLabel == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a Label")), MB_ICONSTOP);
		return;
	}

	if ((csLabel==csTitle)&&(csCommand==pEditButton->csCommand))
	{
		OnBnClickedbuttonCancel();
		return;
	}

	Table TB = DB.QuerySQL(CFrmt(_T("SELECT * FROM %1 WHERE Name LIKE '%2'"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel()),csLabel));
	if((TB.GetRowCount()>0)&&(csLabel!=csTitle))
	{
		AfxMessageBox(CMsg(_T("Button '%1' already exists"),true,csLabel), MB_ICONSTOP);
		return;
	}

	if (csCommand == "")
	{
		AfxMessageBox(CMsg(_T("You must enter a command")), MB_ICONSTOP);
		return;
	}

	DB.ExecuteSQL(CFrmt(_T("UPDATE %1 SET Name = '%2',Command = '%3' WHERE Name = '%4'"),m_ComTabCtrl.GetTabText(m_ComTabCtrl.GetSel()),csLabel,csCommand,csTitle));

	pEditButton->SetWindowText(csLabel);
	pEditButton->csCommand = csCommand;
	m_ComTabCtrl.Update();

	OnBnClickedbuttonCancel();
}

void CCommandsTab::OnSize(UINT nType, int cx, int cy) 
{
	CRect crNew;
	GetWindowRect(crNew);
	int dx = crNew.Width()-MinSize.Width();
	int dy = crNew.Height()-MinSize.Height();
	DWORD dwCurStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	if (dwCurStyle & WS_CHILD)
	{
		dx = crNew.Width()-MinDock.Width();
		dy = crNew.Height()-MinDock.Height();
	}

	if(initialized)
	{
		m_ComTabCtrl.MoveWindow(crComTabCtrl.left,crComTabCtrl.top,crComTabCtrl.Width()+dx,crComTabCtrl.Height()+dy);
		if(m_ComTabCtrl.GetSel())
		{
			CCommandPage * pComPage = p_DlgTabs.GetAt(m_ComTabCtrl.GetIndex(m_ComTabCtrl.GetSel()));
			pComPage->SortButtons();
		}
		creButtonCommand.MoveWindow(crButtonCommand.left,crButtonCommand.top,crButtonCommand.Width(),crButtonCommand.Height()+dy);
		ceTabName.MoveWindow(crTabName.left,crTabName.top+dy,crTabName.Width(),crTabName.Height());
		cbAddTab.MoveWindow(crAddTab.left,crAddTab.top+dy,crAddTab.Width(),crAddTab.Height());
		cbCancelTab.MoveWindow(crCancelTab.left,crCancelTab.top+dy,crCancelTab.Width(),crCancelTab.Height());
		cbTabOk.MoveWindow(crTabOk.left,crTabOk.top+dy,crTabOk.Width(),crTabOk.Height());
	}

	CDialogEx::OnSize(nType, cx, cy);
}