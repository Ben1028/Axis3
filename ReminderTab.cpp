
#include "stdafx.h"
#include "Axis3.h"
#include "ReminderTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CReminderTab, CDialogPage)

CReminderTab::CReminderTab(CWnd* pParent /*=NULL*/)
	: CDialogPage(CReminderTab::IDD, AfxGetApp()->LoadIcon(IDI_REMINDER), pParent)
{
	iReminder = 0;
	m_bStartup = 0;
	bEditor = false;
}


CReminderTab::~CReminderTab()
{
}

void CReminderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WEEKDAY, m_ccbWeekday);
	DDX_Control(pDX, IDC_STARTUP, m_cbStartup);
	DDX_Check(pDX, IDC_STARTUP, m_bStartup);
	DDX_Control(pDX, IDC_ADD, m_cbAdd);
	DDX_Control(pDX, IDC_EDIT, m_cbEdit);
	DDX_Control(pDX, IDC_DELETE, m_cbDelete);
	DDX_Control(pDX, IDC_SAVE, m_cbSave);
	DDX_Control(pDX, IDC_CANCEL, m_cbCancel);
	DDX_Control(pDX, IDC_MONTHLY, m_crMonthly);
	DDX_Control(pDX, IDC_WEEKLY, m_crWeekly);
	DDX_Control(pDX, IDC_DAILY, m_crDaily);
	DDX_Control(pDX, IDC_ONCE, m_crOnce);
	DDX_Control(pDX, IDC_NEVER, m_crNever);
	DDX_Control(pDX, IDC_DATEPICKER, m_cdtpDatePicker);
	DDX_Control(pDX, IDC_TIMEPICKER, m_cdtpTimePicker);
	DDX_Control(pDX, IDC_LIST, m_clcList);
	DDX_Control(pDX, IDC_TITLE, m_ceTitle);
	DDX_Control(pDX, IDC_FRAMETEXT, m_ceText);
}


BEGIN_MESSAGE_MAP(CReminderTab, CDialogPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, OnListChange)
	ON_BN_CLICKED(IDC_MONTHLY, OnMonthly)
	ON_BN_CLICKED(IDC_WEEKLY, OnWeekly)
	ON_BN_CLICKED(IDC_DAILY, OnDaily)
	ON_BN_CLICKED(IDC_ONCE, OnOnce)
	ON_BN_CLICKED(IDC_NEVER, OnNever)
	ON_BN_CLICKED(IDC_STARTUP, OnStartup)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CANCEL, OnCancelReminder)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CReminderTab::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_REMINDER")));
	CDialogPage::OnInitDialog();
	SetLanguage();
	Axis->DBLng.CommitTransaction();

	tNow = CTime::GetCurrentTime();

	m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);
	m_crNever.SetCheck(1);
	m_cdtpTimePicker.SetFormat(_T("h':'mm tt"));
	m_cdtpDatePicker.SetFormat(_T("ddd, MMMM d,  yyyy"));
	CTime tMax(2100,1,1,0,0,0);
	m_cdtpDatePicker.SetRange(&tNow, &tMax);

	m_clcList.InsertColumn(0, CMsg(_T("IDS_TITLE")), LVCFMT_LEFT, 100, -1);
	m_clcList.InsertColumn(1,CMsg( _T("IDS_TYPE")), LVCFMT_LEFT, 66, -1);
	LoadReminders();

	m_ceText.GetWindowRect(crText);
	ScreenToClient(crText);
	m_ceTitle.GetWindowRect(crTitle);
	ScreenToClient(crTitle);
	m_clcList.GetWindowRect(crList);
	ScreenToClient(crList);
	m_cdtpDatePicker.GetWindowRect(crDatePicker);
	ScreenToClient(crDatePicker);
	//GetDlgItem(IDC_GR_REMIND)->GetWindowRect(crGrRemind);
	//ScreenToClient(crGrRemind);
	initialized = TRUE;

	return TRUE;
}

void CReminderTab::SetLanguage()
{
	m_ccbWeekday.ResetContent();
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY1")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY2")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY3")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY4")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY5")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY6")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY7")));

	GetDlgItem(IDC_GR_REMIND)->SetWindowText(CMsg(_T("IDS_REMIND_ME")));
	GetDlgItem(IDC_GR_TIME)->SetWindowText(CMsg(_T("IDS_TIME")));
	GetDlgItem(IDC_ST_TITLE)->SetWindowText(CMsg(_T("IDS_TITLE")));

	GetDlgItem(IDC_ADD)->SetWindowText(CMsg(_T("IDS_ADD")));
	GetDlgItem(IDC_EDIT)->SetWindowText(CMsg(_T("IDS_EDIT")));
	GetDlgItem(IDC_DELETE)->SetWindowText(CMsg(_T("IDS_DELETE")));
	GetDlgItem(IDC_SAVE)->SetWindowText(CMsg(_T("IDS_SAVE")));
	GetDlgItem(IDC_CANCEL)->SetWindowText(CMsg(_T("IDS_CANCEL")));

	GetDlgItem(IDC_STARTUP)->SetWindowText(CMsg(_T("IDS_REM_STARTUP")));

	GetDlgItem(IDC_MONTHLY)->SetWindowText(CMsg(_T("IDS_MONTHLY")));
	GetDlgItem(IDC_WEEKLY)->SetWindowText(CMsg(_T("IDS_WEEKLY")));
	GetDlgItem(IDC_DAILY)->SetWindowText(CMsg(_T("IDS_DAILY")));
	GetDlgItem(IDC_ONCE)->SetWindowText(CMsg(_T("IDS_ONCE")));
	GetDlgItem(IDC_NEVER)->SetWindowText(CMsg(_T("IDS_NEVER")));
}

void CReminderTab::OnMonthly()
{
	iReminder = 4;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(true);
	m_ccbWeekday.ResetContent();

	for (int i = 1; i <= 31; i++)
		m_ccbWeekday.AddString(CMsg(_T("IDS_DECIMAL"),true,i));

	m_ccbWeekday.SetCurSel(tNow.GetDay()-1);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnWeekly()
{
	iReminder = 3;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(true);
	m_ccbWeekday.ResetContent();
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY1")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY2")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY3")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY4")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY5")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY6")));
	m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY7")));
	m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnDaily()
{
	iReminder = 2;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnOnce()
{
	iReminder = 1;
	m_cbStartup.EnableWindow(true);
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(true);
}

void CReminderTab::OnNever()
{
	iReminder = 0;
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
}

void CReminderTab::OnStartup()
{
	UpdateData();
	m_cdtpTimePicker.EnableWindow(m_bStartup ? false : true);
}

void CReminderTab::OnAdd()
{
	tNow = CTime::GetCurrentTime();
	m_cdtpTimePicker.SetTime(&tNow);
	m_ceText.SetReadOnly(0);
	m_ceTitle.SetReadOnly(0);
	m_ceText.SetWindowText(_T(""));
	m_ceTitle.SetWindowText(_T(""));
	m_cbSave.EnableWindow(true);
	m_cbCancel.EnableWindow(true);
	m_cbAdd.EnableWindow(false);
	m_crMonthly.EnableWindow(true);
	m_crWeekly.EnableWindow(true);
	m_crDaily.EnableWindow(true);
	m_crOnce.EnableWindow(true);
	m_crNever.EnableWindow(true);
	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
	switch(iReminder)
	{
	case 0:
		OnNever();
		break;
	case 1:
		OnOnce();
		break;
	case 2:
		OnDaily();
		break;
	case 3:
		OnWeekly();
		break;
	case 4:
		OnMonthly();
		break;
	}

}

void CReminderTab::OnEdit()
{
	bEditor = true;
	m_ceText.SetReadOnly(0);
	m_ceTitle.SetReadOnly(0);
	m_cbSave.EnableWindow(true);
	m_cbCancel.EnableWindow(true);
	m_cbAdd.EnableWindow(false);
	m_crMonthly.EnableWindow(true);
	m_crWeekly.EnableWindow(true);
	m_crDaily.EnableWindow(true);
	m_crOnce.EnableWindow(true);
	m_crNever.EnableWindow(true);

	if (m_crNever.GetCheck())
	{
		m_cbStartup.EnableWindow(false);
		m_cdtpTimePicker.EnableWindow(false);
		m_ccbWeekday.EnableWindow(false);
		m_cdtpDatePicker.EnableWindow(false);
	}
	else
	{
		m_cbStartup.EnableWindow(true);
		m_cdtpTimePicker.EnableWindow(m_cbStartup.GetCheck() ? false : true);
		m_ccbWeekday.EnableWindow((m_crWeekly.GetCheck()) || (m_crMonthly.GetCheck()));
		m_cdtpDatePicker.EnableWindow(m_crOnce.GetCheck());
	}

	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
}

void CReminderTab::OnDelete()
{
	int iIndex = m_clcList.GetSelectionMark();
	CString csTitle = m_clcList.GetItemText(iIndex,0);
	Axis->DBSettings.ExecuteSQL(CSQL(_T("DELETE FROM Reminders WHERE Title = '%1'"),csTitle));
	m_clcList.DeleteItem(iIndex);
	m_cbEdit.EnableWindow(false);
	m_cbDelete.EnableWindow(false);
}

void CReminderTab::OnSave()
{
	CString csTitle, csText;
	m_ceTitle.GetWindowText(csTitle);
	m_ceText.GetWindowText(csText);
	int iStartup = 0;
	int iHour = 0;
	int iMin = 0;
	int iDate = 0;

	if(csTitle == "")
	{
		AfxMessageBox(CMsg(_T("IDS_REM_TITLE_EMPTY")), MB_ICONSTOP);
		return;
	}
	if(csText == "")
	{
		AfxMessageBox(CMsg(_T("IDS_REM_TEXT_EMPTY")), MB_ICONSTOP);
		return;
	}

	if (bEditor)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("DELETE FROM Reminders WHERE Title = '%1'"),m_clcList.GetItemText(m_clcList.GetSelectionMark(),0)));

	Table TBReminders = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Reminders WHERE Title = '%1'"),csTitle));
	if(TBReminders.GetRowCount() != 0)
	{
		//This Reminder already exists, stop here.
		AfxMessageBox(CMsg(_T("IDS_REM_EXISTS"),true,csTitle), MB_ICONSTOP);
		return;
	}

	if (iReminder)
	{
		CTime tReminder;
		if (!m_cbStartup.GetCheck())
		{
			m_cdtpTimePicker.GetTime(tReminder);
			iHour = tReminder.GetHour();
			iMin = tReminder.GetMinute();
		}
		else
			iStartup = 1;

		switch(iReminder)
		{
		case 1:
			{
			m_cdtpDatePicker.GetTime(tReminder);
			iDate = (int)tReminder.GetTime();
			break;
			}
		case 3:
		case 4:
			iDate = m_ccbWeekday.GetCurSel()+1;
		}
	}

	Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO Reminders VALUES('%1','%2','%3!d!','%4!d!','%5!d!','%6!d!','%7!d!')"),csTitle,csText,iReminder,iStartup,iDate,iHour,iMin));

	CString csType;
	switch(iReminder)
	{
	case 0:
		csType = CMsg(_T("IDS_NEVER"));
		break;
	case 1:
		csType = CMsg(_T("IDS_ONCE"));
		break;
	case 2:
		csType = CMsg(_T("IDS_DAILY"));
		break;
	case 3:
		csType = CMsg(_T("IDS_WEEKLY"));
		break;
	case 4:
		csType = CMsg(_T("IDS_MONTHLY"));
		break;
	}

	if (bEditor)
	{
		int iIndex = m_clcList.GetSelectionMark();
		m_clcList.SetItemText(iIndex, 0, csTitle);
		m_clcList.SetItemText(iIndex, 1, csType);
	}
	else
	{
		int iIndex = m_clcList.GetItemCount();
		m_clcList.InsertItem(iIndex, csTitle, 0);
		m_clcList.SetItemText(iIndex, 1, csType);
		m_clcList.SetItemState( iIndex, LVIS_SELECTED, LVIS_SELECTED );
		m_clcList.EnsureVisible( iIndex, FALSE );
		m_clcList.SetHotItem( iIndex );
	}


	m_ceText.SetReadOnly(1);
	m_ceTitle.SetReadOnly(1);
	m_cbSave.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbAdd.EnableWindow(true);
	m_crMonthly.EnableWindow(false);
	m_crWeekly.EnableWindow(false);
	m_crDaily.EnableWindow(false);
	m_crOnce.EnableWindow(false);
	m_crNever.EnableWindow(false);
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
	m_cbEdit.EnableWindow(true);
	m_cbDelete.EnableWindow(true);
	bEditor = false;
}

void CReminderTab::OnCancelReminder()
{
	m_ceText.SetReadOnly(1);
	m_ceTitle.SetReadOnly(1);
	m_cbSave.EnableWindow(false);
	m_cbCancel.EnableWindow(false);
	m_cbAdd.EnableWindow(true);
	m_crMonthly.EnableWindow(false);
	m_crWeekly.EnableWindow(false);
	m_crDaily.EnableWindow(false);
	m_crOnce.EnableWindow(false);
	m_crNever.EnableWindow(false);
	m_cbStartup.EnableWindow(false);
	m_cdtpTimePicker.EnableWindow(false);
	m_ccbWeekday.EnableWindow(false);
	m_cdtpDatePicker.EnableWindow(false);
	m_cbEdit.EnableWindow(bEditor);
	m_cbDelete.EnableWindow(bEditor);
	bEditor = false;
}

void CReminderTab::LoadReminders()
{
	Table TBReminders = Axis->DBSettings.QuerySQL(_T("SELECT * FROM Reminders"));
	TBReminders.ResetRow();
	int iIndex = 0;
	CString csType;
	while(TBReminders.GoNext())
	{
		switch(_ttoi(TBReminders.GetValue(_T("Type"))))
		{
			case 0:
				csType = CMsg(_T("IDS_NEVER"));
				break;
			case 1:
				csType = CMsg(_T("IDS_ONCE"));
				break;
			case 2:
				csType = CMsg(_T("IDS_DAILY"));
				break;
			case 3:
				csType = CMsg(_T("IDS_WEEKLY"));
				break;
			case 4:
				csType = CMsg(_T("IDS_MONTHLY"));
				break;
		}
		m_clcList.InsertItem(iIndex, TBReminders.GetValue(_T("Title")), 0);
		m_clcList.SetItemText(iIndex, 1, csType);
		iIndex++;
	}
}

void CReminderTab::OnListChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uNewState & LVNI_SELECTED)
	{
		// this is the one that has been selected
		int iSelIndex = m_clcList.GetNextItem(-1, LVNI_SELECTED);
		if (iSelIndex == -1)
			return;

		m_clcList.SetHotItem(iSelIndex);

		tNow = CTime::GetCurrentTime();
		m_cdtpTimePicker.SetTime(&tNow);
		m_cdtpDatePicker.SetTime(&tNow);
		m_ccbWeekday.ResetContent();
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY1")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY2")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY3")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY4")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY5")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY6")));
		m_ccbWeekday.AddString(CMsg(_T("IDS_WEEKDAY7")));
		m_ccbWeekday.SetCurSel(tNow.GetDayOfWeek()-1);

		CString csTitle, csText;
		csTitle = m_clcList.GetItemText(iSelIndex,0);
		Table TBReminders = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Reminders WHERE Title = '%1'"),csTitle));
		if(TBReminders.GetRowCount() == 0)
			return;

		m_ceTitle.SetWindowText(csTitle);
		m_ceText.SetWindowText(TBReminders.GetValue(_T("Text")));
		
		int iType, iHour, iMin, iDate, iStartup;
		iType = _ttoi(TBReminders.GetValue(_T("Type")));
		iHour = _ttoi(TBReminders.GetValue(_T("Hour")));
		iMin = _ttoi(TBReminders.GetValue(_T("Min")));
		iDate = _ttoi(TBReminders.GetValue(_T("Date")));
		iStartup = _ttoi(TBReminders.GetValue(_T("Startup")));

		m_crNever.SetCheck(0);
		m_crOnce.SetCheck(0);
		m_crDaily.SetCheck(0);
		m_crWeekly.SetCheck(0);
		m_crMonthly.SetCheck(0);
		iReminder = iType;

		m_cbStartup.SetCheck(0);
		switch(iType)
		{
		case 0:
			break;
		default:
			{
				if (iStartup)
					m_cbStartup.SetCheck(1);
				else
				{
					CTime tOnce(tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),iHour,iMin,tNow.GetSecond());
					m_cdtpTimePicker.SetTime(&tOnce);
				}
			}
		}

		switch(iType)
		{
		case 0:
			m_crNever.SetCheck(1);
			break;
		case 1:
			{
				CTime tOnce((__time64_t)iDate);
				m_crOnce.SetCheck(1);
				m_cdtpDatePicker.SetTime(&tOnce);
			}
			break;
		case 2:
			m_crDaily.SetCheck(1);
			break;
		case 3:
			m_crWeekly.SetCheck(1);
			m_ccbWeekday.SetCurSel(iDate-1);
			break;
		case 4:
			{
				m_crMonthly.SetCheck(1);
				m_ccbWeekday.ResetContent();
				for (int i = 1; i <= 31; i++)
					m_ccbWeekday.AddString(CMsg(_T("IDS_DECIMAL"),true,i));
				m_ccbWeekday.SetCurSel(iDate-1);
			}
			break;
		}

		m_ceText.SetReadOnly(1);
		m_ceTitle.SetReadOnly(1);
		m_cbSave.EnableWindow(false);
		m_cbCancel.EnableWindow(false);
		m_cbAdd.EnableWindow(true);
		m_crMonthly.EnableWindow(false);
		m_crWeekly.EnableWindow(false);
		m_crDaily.EnableWindow(false);
		m_crOnce.EnableWindow(false);
		m_crNever.EnableWindow(false);
		m_cbStartup.EnableWindow(false);
		m_cdtpTimePicker.EnableWindow(false);
		m_ccbWeekday.EnableWindow(false);
		m_cdtpDatePicker.EnableWindow(false);
		m_cbEdit.EnableWindow(true);
		m_cbDelete.EnableWindow(true);
		bEditor = false;
		return;
	}
	*pResult = 0;
}

void CReminderTab::OnSize(UINT nType, int cx, int cy) 
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
		m_ceText.MoveWindow(crText.left,crText.top,crText.Width()+dx,crText.Height()+dy);
		m_ceTitle.MoveWindow(crTitle.left,crTitle.top,crTitle.Width()+dx,crTitle.Height());
		m_clcList.MoveWindow(crList.left,crList.top,crList.Width(),crList.Height()+dy);
		m_cdtpDatePicker.MoveWindow(crDatePicker.left,crDatePicker.top+dy,crDatePicker.Width(),crDatePicker.Height());
		//GetDlgItem(IDC_GR_REMIND)->MoveWindow(crGrRemind.left,crGrRemind.top+(dy/2),crGrRemind.Width(),crGrRemind.Height());
	}
	
	CDialogEx::OnSize(nType, cx, cy);
}