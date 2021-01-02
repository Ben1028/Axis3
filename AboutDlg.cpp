#include "stdafx.h"
#include "Axis3.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LICENSE, m_ceLicense);
	DDX_Control(pDX, IDC_COPYRIGHT, m_ceCopyright);
	DDX_Control(pDX, IDOK, cb_ok);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	/*SetWindowText(CMsg("IDS_ABOUTBOX"));
	cb_ok.SetWindowText(CMsg("OK"));*/

	/*CString lf, lf2;
	lf.Format("%c%c", 0x0d, 0x0a);
	lf2.Format("%c%c%c%c", 0x0d, 0x0a, 0x0d, 0x0a);

	CString csCopyright = Main->GetVersionTitle();
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY1,CTime::GetCurrentTime().GetYear());
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY2,Main->GetBuildTimestamp());
	csCopyright += lf;
	csCopyright += CFMsg(IDS_COPY3,SERVER_VERSION);
	csCopyright += lf2;
	csCopyright += CMsg(IDS_COPY4);
	csCopyright += lf;
	csCopyright += CMsg(IDS_COPY5);
	csCopyright += lf;
	csCopyright += CMsg(IDS_COPY6);
	m_ceCopyright.SetWindowText(csCopyright);

	CString csLicense = CMsg(IDS_LICENCE1);
	csLicense += lf2;
	csLicense += CMsg(IDS_LICENCE2);
	csLicense += lf2;
	csLicense += CMsg(IDS_LICENCE3);
	m_ceLicense.SetWindowText(csLicense);*/

	return TRUE;
}
