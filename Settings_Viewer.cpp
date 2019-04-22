#include "stdafx.h"
#include "Axis3.h"
#include "Settings_Viewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSettingViewer::CSettingViewer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingViewer::IDD, pParent)
{
	m_bItemViewScale = GetSettingNum(_T("ViewerScaleItems"));
	m_bMultiViewScale = GetSettingNum(_T("ViewerScaleMultis"));
	m_dwItemBGColor = GetSettingNum(_T("ViewerBGColor"));
}

void CSettingViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_ITEMVIEW_SCALE, m_bItemViewScale);
	DDX_Check(pDX, IDC_MULTIVIEW_SCALE, m_bMultiViewScale);
	DDX_Control(pDX, IDC_VIEWERBG_COLOR, m_csBGColor);

}


BEGIN_MESSAGE_MAP(CSettingViewer, CDialogEx)
	ON_BN_CLICKED(IDC_RESET_ITEM_CACHE, OnResetItemCache)
	ON_BN_CLICKED(IDC_RESET_MULTI_CACHE, OnResetMultiCache)
	ON_BN_CLICKED(IDC_RESET_MAP_CACHE, OnResetMapCache)
	ON_BN_CLICKED(IDC_VIEWERBG_COLOR, OnBGColor)
	ON_BN_CLICKED(IDC_ITEMVIEW_SCALE, OnScaleItems)
	ON_BN_CLICKED(IDC_MULTIVIEW_SCALE, OnScaleMultis)
	ON_BN_CLICKED(IDC_RESET_BGCOL, OnResetBGColor)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CSettingViewer::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_SETTING_VIEWER")));

	GetDlgItem(IDC_RESET_ITEM_CACHE)->SetWindowText(CMsg(_T("IDS_RESET_ITEM_CACHE")));
	GetDlgItem(IDC_RESET_MULTI_CACHE)->SetWindowText(CMsg(_T("IDS_RESET_MULTI_CACHE")));
	GetDlgItem(IDC_RESET_MAP_CACHE)->SetWindowText(CMsg(_T("IDS_RESET_MAP_CACHE")));
	GetDlgItem(IDC_ITEMVIEW_SCALE)->SetWindowText(CMsg(_T("IDS_ITEM_SCALE")));
	GetDlgItem(IDC_MULTIVIEW_SCALE)->SetWindowText(CMsg(_T("IDS_MULTI_SCALE")));
	GetDlgItem(IDC_VIEWERBGCOL)->SetWindowText(CMsg(_T("IDS_VIEWER_BGCOL")));
	Axis->DBLng.CommitTransaction();
	UpdateData(false);
	return TRUE;
}

void CSettingViewer::OnResetItemCache()
{
	if ( AfxMessageBox(CMsg(_T("IDS_CONFIRM_ITEMCACHE")), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL )
		return;
	CString csPath = CMsg(_T("IDS_ART_FILENAME"));
	ClearDirectory(csPath.Left(csPath.ReverseFind('/')));
}

void CSettingViewer::OnResetMultiCache()
{
	if (AfxMessageBox(CMsg(_T("IDS_CONFIRM_MULTICACHE")), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return;
	CString csPath = CMsg(_T("IDS_MULTI_FILENAME"));
	ClearDirectory(csPath.Left(csPath.ReverseFind('/')));
}

void CSettingViewer::OnResetMapCache()
{
	if (AfxMessageBox(CMsg(_T("IDS_CONFIRM_MAPCACHE")), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return;
	CString csPath = CMsg(_T("IDS_MAP_FILENAME"));
	ClearDirectory(csPath.Left(csPath.ReverseFind('/')));
}

void CSettingViewer::OnResetBGColor()
{
	ClearSetting(_T("ViewerBGColor"));
	m_dwItemBGColor = GetSettingNum(_T("ViewerBGColor"));
	SendMessage(WM_PAINT);
}

void CSettingViewer::OnScaleItems()
{
	UpdateData();
	SetSettingNum(_T("ViewerScaleItems"), m_bItemViewScale);
}

void CSettingViewer::OnScaleMultis()
{
	UpdateData();
	SetSettingNum(_T("ViewerScaleMultis"), m_bMultiViewScale);
}

void CSettingViewer::OnBGColor() 
{
	CColorDialog colors;
	if ( colors.DoModal() == IDOK )
		m_dwItemBGColor = colors.GetColor();
	SendMessage( WM_PAINT );
	SetSettingNum(_T("ViewerBGColor"), m_dwItemBGColor);
}

void CSettingViewer::OnPaint() 
{
	CPaintDC dc(this);

	CDC * pDC = m_csBGColor.GetDC();
	CRgn rgn;
	CRect rect;
	m_csBGColor.GetWindowRect( &rect );
	CBrush * pNewBrush = new (CBrush);
	pNewBrush->CreateSolidBrush( (COLORREF) m_dwItemBGColor );
	CBrush * pOldBrush = pDC->SelectObject(pNewBrush);
	CRect cr;
	m_csBGColor.GetClientRect(&cr);
	rgn.CreateRectRgnIndirect(cr);
	pDC->SelectClipRgn(&rgn);
	pDC->FillRect(cr, pNewBrush);
	pDC->SelectObject(pOldBrush);
	delete (pNewBrush);
	rgn.DeleteObject();
	m_csBGColor.ReleaseDC( pDC );

}