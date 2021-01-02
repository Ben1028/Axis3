
#include "stdafx.h"
#include "Axis3.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSearchCritDlg::CSearchCritDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchCritDlg::IDD, pParent)
{
	bArea = 0;
	m_crPref = 0;
}


void CSearchCritDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ID_SEARCH, m_crPref);
	DDX_Control(pDX, IDC_SEARCH_VALUE, m_ceValue);
}


BEGIN_MESSAGE_MAP(CSearchCritDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ID_SEARCH, OnSearchId)
	ON_BN_CLICKED(IDC_DESC_SEARCH, OnSearchDescription)
	ON_BN_CLICKED(IDC_DEF_SEARCH, OnSearchDefname)
END_MESSAGE_MAP()

BOOL CSearchCritDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Search Criteria")));

	GetDlgItem(IDC_STATICSEARCH)->SetWindowText(CMsg(_T("Search by")));
	GetDlgItem(IDC_DEF_SEARCH)->SetWindowText(CMsg(_T("Defname")));
	GetDlgItem(IDC_DESC_SEARCH)->SetWindowText(CMsg(_T("Description")));
	GetDlgItem(IDC_STATICVALUE)->SetWindowText(CMsg(_T("Pattern")));
	GetDlgItem(IDC_STATICINFO)->SetWindowText(CMsg(_T("Pattern search:\r\n% = Any amount of characters or none\r\n_ = Any 1 character")));
	GetDlgItem(IDOK)->SetWindowText(CMsg(_T("OK")));

	if (bArea)
	{
		GetDlgItem(IDC_ID_SEARCH)->SetWindowText(CMsg(_T("Coordinate")));
		m_ceValue.SetWindowText(GetSettingString(_T("SearchAreaPattern")));
	}
	else
	{
		GetDlgItem(IDC_ID_SEARCH)->SetWindowText(CMsg(_T("ID")));
		m_ceValue.SetWindowText(GetSettingString(_T("SearchObjectPattern")));
	}

	Axis->DBLng.CommitTransaction();
	m_crPref = GetSettingNum(_T("SearchCriteria"));

	UpdateData(FALSE);
	return TRUE;
}

void CSearchCritDlg::OnSearchId() 
{
	m_crPref = 0;
	SetSettingNum(_T("SearchCriteria"), m_crPref);
}

void CSearchCritDlg::OnSearchDefname() 
{
	m_crPref = 1;
	SetSettingNum(_T("SearchCriteria"), m_crPref);
}

void CSearchCritDlg::OnSearchDescription() 
{
	m_crPref = 2;
	SetSettingNum(_T("SearchCriteria"), m_crPref);
}

void CSearchCritDlg::OnOK() 
{
	CString csValue;
	m_ceValue.GetWindowText(csValue);
	if (bArea)
	{
		SetSettingString(_T("SearchAreaPattern"), csValue);
	}
	else
	{
		SetSettingString(_T("SearchObjectPattern"), csValue);
	}
	CDialogEx::OnOK();
}
