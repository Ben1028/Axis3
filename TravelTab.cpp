
#include "stdafx.h"
#include "Axis3.h"
#include "TravelTab.h"
#include "SearchCritDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTravelTab, CDialogPage)

CTravelTab::CTravelTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CTravelTab::IDD, AfxGetApp()->LoadIcon(IDI_WORLD), pParent)
{
	bPointSort = false;
	bDescSort = false;
	bSearch = false;
}


CTravelTab::~CTravelTab()
{
}

void CTravelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP, cMapDisplay);
	DDX_Control(pDX, IDC_LOCATIONS, m_clcLocations);
	DDX_Control(pDX, IDC_CATDROP, m_ccbCategories);
	DDX_Control(pDX, IDC_SUBDROP, m_ccbSubsections);
	DDX_Control(pDX, IDC_ZOOMIN, cb_zoomin);
	DDX_Control(pDX, IDC_ZOOMOUT, cb_zoomout);
	DDX_Control(pDX, IDC_FINDAREA, cb_findarea);
	DDX_Control(pDX, IDC_XSCROLL, m_csbXScroll);
	DDX_Control(pDX, IDC_YSCROLL, m_csbYScroll);
	DDX_Control(pDX, IDC_COORDX, m_ceX);
	DDX_Control(pDX, IDC_COORDY, m_ceY);
	DDX_Control(pDX, IDC_COORDZ, m_ceZ);
	DDX_Control(pDX, IDC_COORDM, m_ceM);
}


BEGIN_MESSAGE_MAP(CTravelTab, CDialogPage)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_CATDROP, OnSelchangeCat)
	ON_CBN_SELCHANGE(IDC_SUBDROP, OnSelchangeSub)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LOCATIONS, OnItemchangedLocations)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LOCATIONS, OnColumnclickLocations)
	ON_NOTIFY(NM_DBLCLK, IDC_LOCATIONS, OnDblclkLocation)
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_WHERE, OnWhere)
	ON_BN_CLICKED(IDC_ZOOMOUT, OnZoomout)
	ON_BN_CLICKED(IDC_ZOOMIN, OnZoomin)
	ON_BN_CLICKED(IDC_FINDAREA, OnFindarea)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_COORDX, OnChangeX)
	ON_EN_CHANGE(IDC_COORDY, OnChangeY)
	ON_EN_CHANGE(IDC_COORDZ, OnChangeZ)
	ON_EN_CHANGE(IDC_COORDM, OnChangeM)
END_MESSAGE_MAP()

BOOL CTravelTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("Travel")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	m_clcLocations.InsertColumn(0, CMsg(_T("Description")), LVCFMT_LEFT, 230, -1);
	m_clcLocations.InsertColumn(1, CMsg(_T("Location")), LVCFMT_LEFT, 100, -1);
	m_clcLocations.InsertColumn(2, CMsg(_T("Areadef")), LVCFMT_LEFT, 0, -1);

	FillCategory();

	cMapDisplay.InitDisplay(true);
	cMapDisplay.DrawMap(0);

	m_csbXScroll.SetScrollRange(0, cMapDisplay.Objbmp.GetWidth()-1);
	m_csbYScroll.SetScrollRange(0, cMapDisplay.Objbmp.GetHeight()-1);

	CString csP = m_clcLocations.GetItemText(0, 1);
	int curPos = 0;
	int iX = _ttoi(csP.Tokenize(_T(","), curPos));
	int iY = _ttoi(csP.Tokenize(_T(","), curPos));
	int iZ = _ttoi(csP.Tokenize(_T(","), curPos));
	int iM = _ttoi(csP.Tokenize(_T(","), curPos));

	m_ceX.SetWindowText(CFrmt(_T("%1!d!"), iX));
	m_ceY.SetWindowText(CFrmt(_T("%1!d!"), iY));
	m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), iZ));
	m_ceM.SetWindowText(CFrmt(_T("%1!d!"), iM));

	cMapDisplay.SetCenterX(iX);
	cMapDisplay.SetCenterY(iY);
	m_csbXScroll.SetScrollPos(iX);
	m_csbYScroll.SetScrollPos(iY);

	cMapDisplay.DisplayObj();

	initialized = TRUE;

	return TRUE;
}

void CTravelTab::SetLanguage()
{
	GetDlgItem(IDC_STATICMAP)->SetWindowText(CMsg(_T("Map")));
	GetDlgItem(IDC_STATICSUB)->SetWindowText(CMsg(_T("Sub")));
	GetDlgItem(IDC_STATICX)->SetWindowText(CMsg(_T("X")));
	GetDlgItem(IDC_STATICY)->SetWindowText(CMsg(_T("Y")));
	GetDlgItem(IDC_STATICZ)->SetWindowText(CMsg(_T("Z")));
	GetDlgItem(IDC_STATICM)->SetWindowText(CMsg(_T("M")));

	GetDlgItem(IDC_WHERE)->SetWindowText(CMsg(_T("Where")));
	GetDlgItem(IDC_GO)->SetWindowText(CMsg(_T("Go")));
	GetDlgItem(IDC_SEND)->SetWindowText(CMsg(_T("Send")));

	cb_zoomin.InitButton(IDI_ZOOMIN, CMsg(_T("Zoom In")));
	cb_zoomout.InitButton(IDI_ZOOMOUT, CMsg(_T("Zoom Out")));
	cb_findarea.InitButton(IDI_FIND, CMsg(_T("Find Area")));
}

BOOL CTravelTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	UNREFERENCED_PARAMETER(nFlags);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if ( rect.PtInRect(pt) )
	{
		if (zDelta > 0)
			OnZoomin();
		else
			OnZoomout();
		cMapDisplay.DisplayObj();
	}
	return true;
}

void CTravelTab::OnRButtonDown(UINT nFlags, CPoint pt)
{
	ClientToScreen(&pt);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if (rect.PtInRect(pt))
	{
		xscroll = pt.x - rect.left;
		yscroll = pt.y - rect.top;
	}
	else
	{
		ScreenToClient(&pt);
		CDialogPage::OnRButtonDown(nFlags, pt);
	}
}

void CTravelTab::OnMouseMove(UINT nFlags, CPoint pt)
{
	ClientToScreen(&pt);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if ((rect.PtInRect(pt)) && (nFlags & MK_RBUTTON))
	{
		short x, y;
		x = (short)pt.x - (short)rect.left - (short)xscroll;
		y = (short)pt.y - (short)rect.top - (short)yscroll;
		xscroll = pt.x - rect.left;
		yscroll = pt.y - rect.top;

		x = min(cMapDisplay.GetCenterX() - (x * cMapDisplay.GetScale()), cMapDisplay.Objbmp.GetWidth() - ((rect.Width()/2) * cMapDisplay.GetScale()));
		x = max((rect.Width() / 2) * cMapDisplay.GetScale(), x);
		y = min(cMapDisplay.GetCenterY() - (y * cMapDisplay.GetScale()), cMapDisplay.Objbmp.GetHeight() - ((rect.Height()/2) * cMapDisplay.GetScale()));
		y = max((rect.Height() / 2) * cMapDisplay.GetScale(), y);
		cMapDisplay.SetCenterX(x);
		cMapDisplay.SetCenterY(y);
		x = cMapDisplay.GetCenterX();
		y = cMapDisplay.GetCenterY();

		m_ceX.SetWindowText(CFrmt(_T("%1!d!"), x));
		m_ceY.SetWindowText(CFrmt(_T("%1!d!"), y));
		m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), cMapDisplay.GetMapHeight(x, y)));

		m_csbXScroll.SetScrollPos(x);
		m_csbYScroll.SetScrollPos(y);

		cMapDisplay.DisplayObj();
	
	}
}


void CTravelTab::OnLButtonDown(UINT nFlags, CPoint pt)
{
	UNREFERENCED_PARAMETER(nFlags);
	ClientToScreen(&pt);
	CRect rect;
	cMapDisplay.GetWindowRect(&rect);
	if (rect.PtInRect(pt))
	{
		short x, y;
		x = cMapDisplay.GetCenterX() - (cMapDisplay.idispcenterX - (pt.x - rect.left)) * cMapDisplay.GetScale();
		y = cMapDisplay.GetCenterY() - (cMapDisplay.idispcenterY - (pt.y - rect.top)) * cMapDisplay.GetScale();
		cMapDisplay.SetCenterX(x);
		cMapDisplay.SetCenterY(y);

		x = cMapDisplay.GetCenterX();
		y = cMapDisplay.GetCenterY();

		m_ceX.SetWindowText(CFrmt(_T("%1!d!"), x));
		m_ceY.SetWindowText(CFrmt(_T("%1!d!"), y));
		m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), cMapDisplay.GetMapHeight(x,y)));

		m_csbXScroll.SetScrollPos(x);
		m_csbYScroll.SetScrollPos(y);

		cMapDisplay.DisplayObj();
	}
}

void CTravelTab::FillCategory()
{
	m_ccbCategories.ResetContent();
	Table TBCats = Axis->DBData.QuerySQL(_T("SELECT Cat FROM Map GROUP BY Cat ORDER BY Cat"));
	TBCats.ResetRow();
	while (TBCats.GoNext())
	{
		m_ccbCategories.AddString(TBCats.GetValue(_T("Cat")));
	}

	m_ccbCategories.SetCurSel(0);
	OnSelchangeCat();
}

void CTravelTab::OnSelchangeCat()
{
	UpdateData(TRUE);
	m_ccbSubsections.ResetContent();

	int iSel = m_ccbCategories.GetCurSel();
	if (iSel == -1)
	{
		UpdateData(FALSE);
		return;
	}

	CString csCat;
	m_ccbCategories.GetLBText(iSel, csCat);
	Table TBSubs = Axis->DBData.QuerySQL(CFrmt(_T("SELECT Cat,Sub FROM Map WHERE Cat = '%1' GROUP BY Sub ORDER BY Sub"), SQLStrip(csCat)));
	TBSubs.ResetRow();
	while (TBSubs.GoNext())
	{
		m_ccbSubsections.AddString(TBSubs.GetValue(_T("Sub")));
	}

	m_ceM.SetWindowText(CFrmt(_T("%1!d!"), iSel));
	cMapDisplay.DrawMap(iSel);
	cMapDisplay.DisplayObj();

	m_ccbSubsections.SetCurSel(0);
	OnSelchangeSub();

	UpdateData(FALSE);
}

void CTravelTab::OnSelchangeSub()
{
	UpdateData(TRUE);

	m_clcLocations.SetHotItem(-1);
	m_clcLocations.DeleteAllItems();

	int iSel = m_ccbCategories.GetCurSel();
	CString csCat, csSub;
	m_ccbCategories.GetLBText(iSel, csCat);
	iSel = m_ccbSubsections.GetCurSel();
	m_ccbSubsections.GetLBText(iSel, csSub);

	Table TBDecs = Axis->DBData.QuerySQL(CFrmt(_T("SELECT * FROM Map WHERE Cat = '%1' AND Sub = '%2' ORDER BY Desc"), SQLStrip(csCat), SQLStrip(csSub)));
	csSortQuerry = CFrmt(_T("SELECT * FROM Map WHERE Cat = '%1' AND Sub = '%2' ORDER BY"), SQLStrip(csCat), SQLStrip(csSub));
	TBDecs.ResetRow();
	int iCount = 0;
	while (TBDecs.GoNext())
	{
		m_clcLocations.InsertItem(iCount, TBDecs.GetValue(_T("Desc")), iCount);
		m_clcLocations.SetItemText(iCount, 1, TBDecs.GetValue(_T("Point")));
		m_clcLocations.SetItemText(iCount, 2, TBDecs.GetValue(_T("Areadef")));
		iCount++;
	}
	bSearch = false;
}

void CTravelTab::OnItemchangedLocations(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		int iSelIndex = m_clcLocations.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex != -1)
		{
			m_clcLocations.SetHotItem(iSelIndex);

			CString csP = m_clcLocations.GetItemText(iSelIndex, 1);

			int curPos = 0;
			int iX = _ttoi(csP.Tokenize(_T(","), curPos));
			int iY = _ttoi(csP.Tokenize(_T(","), curPos));
			int iZ = _ttoi(csP.Tokenize(_T(","), curPos));
			int iM = _ttoi(csP.Tokenize(_T(","), curPos));

			m_ceX.SetWindowText(CFrmt(_T("%1!d!"), iX));
			m_ceY.SetWindowText(CFrmt(_T("%1!d!"), iY));
			m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), iZ));
			m_ceM.SetWindowText(CFrmt(_T("%1!d!"), iM));

			cMapDisplay.SetCenterX(iX);
			cMapDisplay.SetCenterY(iY);
			m_csbXScroll.SetScrollPos(iX);
			m_csbYScroll.SetScrollPos(iY);
			cMapDisplay.DisplayObj();

			if (bSearch)
			{
				Table TBInfo = Axis->DBData.QuerySQL(CFrmt(_T("SELECT Cat,Sub FROM Map WHERE Def = '%1'"), m_clcLocations.GetItemText(iSelIndex, 1)));
				TBInfo.ResetRow();
				if (TBInfo.GoNext())
				{
					CString csCat = TBInfo.GetValue(_T("Cat"));
					m_ccbCategories.SelectString(0, csCat);
					Table TBSubs = Axis->DBData.QuerySQL(CFrmt(_T("SELECT Cat,Sub FROM Map WHERE Cat = '%1' GROUP BY Sub ORDER BY Sub"), SQLStrip(csCat)));
					TBSubs.ResetRow();
					m_ccbSubsections.ResetContent();
					while (TBSubs.GoNext())
					{
						m_ccbSubsections.AddString(TBSubs.GetValue(_T("Sub")));
					}
					m_ccbSubsections.SelectString(0, TBInfo.GetValue(_T("Sub")));
				}
			}
			return;
		}
	}
	*pResult = 0;
}

void CTravelTab::OnDblclkLocation(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnGo();
	*pResult = 0;
}

void CTravelTab::OnColumnclickLocations(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString csType, csOrder;
	if (pNMListView->iSubItem == 0)
	{
		csType = _T("Desc");
		bDescSort = !bDescSort;
		csOrder = (bDescSort ? _T("DESC") : _T("ASC"));
	}
	if (pNMListView->iSubItem == 1)
	{
		csType = _T("Point");
		bPointSort = !bPointSort;
		csOrder = (bPointSort ? _T("DESC") : _T("ASC"));
	}
	Table TBSort;
	TBSort = Axis->DBData.QuerySQL(CFrmt(_T("%1 %2 %3"), csSortQuerry, csType, csOrder));
	TBSort.ResetRow();

	if (TBSort.GetRowCount())
	{
		m_clcLocations.SetHotItem(-1);
		m_clcLocations.DeleteAllItems();

		int iCount = 0;
		while (TBSort.GoNext())
		{
			m_clcLocations.InsertItem(iCount, TBSort.GetValue(_T("Desc")), iCount);
			m_clcLocations.SetItemText(iCount, 1, TBSort.GetValue(_T("Point")));
			m_clcLocations.SetItemText(iCount, 2, TBSort.GetValue(_T("Areadef")));
			iCount++;
		}
	}
	*pResult = 0;
}

void CTravelTab::OnGo()
{
	CString csx,csy,csz,csm;
	m_ceX.GetWindowText(csx);
	m_ceY.GetWindowText(csy);
	m_ceZ.GetWindowText(csz);
	m_ceM.GetWindowText(csm);
	SendToUO(CCmd(_T("go %1,%2,%3,%4"),true, csx, csy, csz, csm));
}

void CTravelTab::OnSend()
{
	CString csx, csy, csz, csm;
	m_ceX.GetWindowText(csx);
	m_ceY.GetWindowText(csy);
	m_ceZ.GetWindowText(csz);
	m_ceM.GetWindowText(csm);
	SendToUO(CCmd(_T("xgo %1,%2,%3,%4"), true, csx, csy, csz, csm));
}

void CTravelTab::OnWhere()
{
	SendToUO(CCmd(_T("where")));
}

void CTravelTab::OnZoomout()
{
	cMapDisplay.SetScale(cMapDisplay.GetScale() * 2);
	cMapDisplay.DisplayObj();
}

void CTravelTab::OnZoomin()
{
	cMapDisplay.SetScale(cMapDisplay.GetScale() / 2);
	cMapDisplay.DisplayObj();
}

void CTravelTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->m_hWnd == m_csbXScroll.m_hWnd)
	{
		int minpos;
		int maxpos;
		pScrollBar->GetScrollRange(&minpos, &maxpos);
		int curpos = pScrollBar->GetScrollPos();
		switch (nSBCode)
		{
		case SB_TOP:
			curpos = minpos;
			break;
		case SB_BOTTOM:
			curpos = maxpos;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEUP:
			if (curpos > minpos)
				curpos--;
			break;
		case SB_LINEDOWN:
			if (curpos < maxpos)
				curpos++;
			break;
		case SB_THUMBPOSITION:
			curpos = nPos;
			break;
		case SB_THUMBTRACK:
			curpos = nPos;
			break;
		}

		pScrollBar->SetScrollPos(curpos);
		cMapDisplay.SetCenterX(curpos);
		cMapDisplay.DisplayObj();
		m_ceX.SetWindowText(CFrmt(_T("%1!d!"),curpos));
	}
}

void CTravelTab::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->m_hWnd == m_csbYScroll.m_hWnd)
	{
		int minpos;
		int maxpos;
		pScrollBar->GetScrollRange(&minpos, &maxpos);
		int curpos = pScrollBar->GetScrollPos();
		switch (nSBCode)
		{
		case SB_TOP:
			curpos = minpos;
			break;
		case SB_BOTTOM:
			curpos = maxpos;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEUP:
			if (curpos > minpos)
				curpos--;
			break;
		case SB_LINEDOWN:
			if (curpos < maxpos)
				curpos++;
			break;
		case SB_THUMBPOSITION:
			curpos = nPos;
			break;
		case SB_THUMBTRACK:
			curpos = nPos;
			break;
		}

		pScrollBar->SetScrollPos(curpos);
		cMapDisplay.SetCenterY(curpos);
		cMapDisplay.DisplayObj();
		m_ceY.SetWindowText(CFrmt(_T("%1!d!"), curpos));
	}
}

void CTravelTab::OnChangeX()
{
	CString csx;
	m_ceX.GetWindowText(csx);
	m_csbXScroll.SetScrollPos(_ttoi(csx));
	cMapDisplay.SetCenterX(_ttoi(csx));
	m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), cMapDisplay.GetMapHeight(_ttoi(csx), cMapDisplay.GetCenterY())));
	cMapDisplay.DisplayObj();
}

void CTravelTab::OnChangeY()
{
	CString csy;
	m_ceY.GetWindowText(csy);
	m_csbYScroll.SetScrollPos(_ttoi(csy));
	cMapDisplay.SetCenterY(_ttoi(csy));
	m_ceZ.SetWindowText(CFrmt(_T("%1!d!"), cMapDisplay.GetMapHeight(cMapDisplay.GetCenterX(), _ttoi(csy))));
	cMapDisplay.DisplayObj();
}

void CTravelTab::OnChangeZ()
{
	//Nothing to do here at the moment
}

void CTravelTab::OnChangeM()
{
	CString csm;
	m_ceM.GetWindowText(csm);
	cMapDisplay.DrawMap(_ttoi(csm));

	m_csbXScroll.SetScrollRange(0, cMapDisplay.Objbmp.GetWidth()-1);
	m_csbYScroll.SetScrollRange(0, cMapDisplay.Objbmp.GetHeight()-1);

	cMapDisplay.DisplayObj();
}

void CTravelTab::OnFindarea()
{
	CSearchCritDlg dlg;
	dlg.bArea = 1;
	if (dlg.DoModal() != IDOK)
		return;
	if (GetSettingString(_T("SearchAreaPattern")) != "")
	{
		Find();
	}
	return;
}

void CTravelTab::Find()
{
	CString csPattern = SQLStrip(GetSettingString(_T("SearchAreaPattern")));
	int iCriteria = GetSettingNum(_T("SearchCriteria"));
	Table TBSearch;

	switch (iCriteria)
	{
	case 0:
	{
		TBSearch = Axis->DBData.QuerySQL(CFrmt(_T("SELECT * FROM Map WHERE Point LIKE '%1' ORDER BY Desc"), csPattern));
		csSortQuerry = CFrmt(_T("SELECT * FROM Map WHERE Point LIKE '%1' ORDER BY"), csPattern);
		break;
	}
	case 1:
	{
		TBSearch = Axis->DBData.QuerySQL(CFrmt(_T("SELECT * FROM Map WHERE Areadef LIKE '%1' ORDER BY Desc"), csPattern));
		csSortQuerry = CFrmt(_T("SELECT * FROM Map WHERE Areadef LIKE '%1' ORDER BY"), csPattern);
		break;
	}
	case 2:
	{
		TBSearch = Axis->DBData.QuerySQL(CFrmt(_T("SELECT * FROM Map WHERE Desc LIKE '%1' ORDER BY Desc"), csPattern));
		csSortQuerry = CFrmt(_T("SELECT * FROM Map WHERE Desc LIKE '%1' ORDER BY"), csPattern);
		break;
	}
	}
	TBSearch.ResetRow();

	if (TBSearch.GetRowCount())
	{
		m_clcLocations.SetHotItem(-1);
		m_clcLocations.DeleteAllItems();

		int iCount = 0;
		while (TBSearch.GoNext())
		{
			m_clcLocations.InsertItem(iCount, TBSearch.GetValue(_T("Desc")), iCount);
			m_clcLocations.SetItemText(iCount, 1, TBSearch.GetValue(_T("Point")));
			m_clcLocations.SetItemText(iCount, 2, TBSearch.GetValue(_T("Areadef")));
			iCount++;
		}
		bSearch = true;
	}
	else
	{
		AfxMessageBox(CMsg(_T("No Areas Found!")));
	}
}