
#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAxis3Dlg * Main;

CAxis3Dlg::CAxis3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAxis3Dlg::IDD, pParent)
{
	Main = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	iDlgCtrlID = 3002;
	hbWhite = ::CreateSolidBrush(RGB(249, 249, 251));
	hbBlue = ::CreateSolidBrush(RGB(242, 252, 255));

	m_iReceiveTimeout = 60000;
}

void CAxis3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

CAxis3Dlg::~CAxis3Dlg()
{
	m_nid.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &m_nid);

	//delete tabs
	delete m_DlgGeneral;
	delete m_DlgTravel;
	delete m_DlgSpawn;
	delete m_DlgCharTweak;
	delete m_DlgITems;
	delete m_DlgItemTweak;
	delete m_DlgAccount;
	delete m_DlgMisc;
	delete m_DlgLauncher;
	delete m_DlgCommands;
	delete m_DlgReminder;
}

BEGIN_MESSAGE_MAP(CAxis3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDOK, OnOk)
	ON_COMMAND(IDCLOSE, OnClose)
	ON_COMMAND(ID_SETTINGS_PROFILE, OnProfile)
	ON_COMMAND(ID_EXIT_CLOSEAXIS, OnExitCloseaxis)
	ON_COMMAND(ID_HELP_ABOUTAxis3, OnHelpAboutAxis3)
	ON_COMMAND(ID_HELP_DOCUMENTATION, OnHelpDocumentation)
	ON_COMMAND(ID_SETTINGS_GENERAL, OnSettingDefault)
	ON_COMMAND(ID_DATAEDIT_DEFAULTSETTINGS, OnDataDefaultSettings)
	ON_COMMAND(ID_DATAEDIT_LANGUAGE, OnDataLanguage)
	ON_COMMAND(ID_SETTINGS_VIEWER, OnSettingViewer)
END_MESSAGE_MAP()

BOOL CAxis3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyle(0,WS_CLIPCHILDREN);
	m_TabCtrl.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),3000);

	//Create All Tabs
	m_DlgGeneral = new CGeneralTab;
	m_DlgTravel = new CTravelTab;
	m_DlgSpawn = new CSpawnTab;
	m_DlgCharTweak = new CCharTweakTab;
	m_DlgITems = new CItemsTab;
	m_DlgItemTweak = new CItemTweakTab;
	m_DlgAccount = new CAccountTab;
	m_DlgMisc = new CMiscTab;
	m_DlgLauncher = new CLauncherTab;
	m_DlgCommands = new CCommandsTab;
	m_DlgReminder = new CReminderTab;

	m_DlgGeneral->Create(m_DlgGeneral->IDD,&m_TabCtrl);
	m_DlgTravel->Create(m_DlgTravel->IDD,&m_TabCtrl);
	m_DlgSpawn->Create(m_DlgSpawn->IDD,&m_TabCtrl);
	m_DlgCharTweak->Create(m_DlgCharTweak->IDD,&m_TabCtrl);
	m_DlgITems->Create(m_DlgITems->IDD,&m_TabCtrl);
	m_DlgItemTweak->Create(m_DlgItemTweak->IDD,&m_TabCtrl);
	m_DlgAccount->Create(m_DlgAccount->IDD,&m_TabCtrl);
	m_DlgMisc->Create(m_DlgMisc->IDD,&m_TabCtrl);
	m_DlgLauncher->Create(m_DlgLauncher->IDD,&m_TabCtrl);
	m_DlgCommands->Create(m_DlgCommands->IDD,&m_TabCtrl);
	m_DlgReminder->Create(m_DlgReminder->IDD,&m_TabCtrl);

	SetTabsPosition();
	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCALE);
	m_TabCtrl.SetNotifyManager(this);
	m_TabCtrl.Update();

	//Select each tab for proper initilisation
	for(int x = m_TabCtrl.GetCount(); x>0;x--)
		m_TabCtrl.SetSel(m_TabCtrl.GetTab(x-1));

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, CMsg(_T("&About Axis3")));
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_nid.cbSize = sizeof(m_nid);
	m_nid.hWnd = this->GetSafeHwnd();
	m_nid.uID = 1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_USER;
	m_nid.hIcon = m_hIcon;
	_tcscpy_s(m_nid.szTip,ARRAYSIZE(m_nid.szTip), Axis->GetVersionTitle());
	Shell_NotifyIcon(NIM_ADD, &m_nid);

	GetWindowRect(&MinSize);
    ScreenToClient(&MinSize);
	m_TabCtrl.GetWindowRect(&TabSize);
    ScreenToClient(&TabSize);
    initialized = TRUE;

	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	pMenu->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, CMsg(_T("&Settings")));
		CMenu* pSub = pMenu->GetSubMenu(0);
		pSub->ModifyMenu(ID_SETTINGS_PROFILE, MF_STRING, ID_SETTINGS_PROFILE, CMsg(_T("&Profile")));
		pSub->ModifyMenu(ID_SETTINGS_GENERAL, MF_STRING, ID_SETTINGS_GENERAL, CMsg(_T("&General")));
		pSub->ModifyMenu(ID_SETTINGS_VIEWER, MF_STRING, ID_SETTINGS_VIEWER, CMsg(_T("&Viewer")));

	pMenu->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 0, CMsg(_T("Data &Edit")));
		pSub = pMenu->GetSubMenu(1);
		pSub->ModifyMenu(ID_DATAEDIT_DEFAULTSETTINGS, MF_STRING, ID_DATAEDIT_DEFAULTSETTINGS, CMsg(_T("De&fault Settings")));
		pSub->ModifyMenu(ID_DATAEDIT_LANGUAGE, MF_STRING, ID_DATAEDIT_LANGUAGE, CMsg(_T("&Language")));

	pMenu->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 0, CMsg(_T("&Help")));
		pSub = pMenu->GetSubMenu(2);
		pSub->ModifyMenu(ID_HELP_ABOUTAxis3, MF_STRING, ID_HELP_ABOUTAxis3, CMsg(_T("&About Axis3")));
		pSub->ModifyMenu(ID_HELP_DOCUMENTATION, MF_STRING, ID_HELP_DOCUMENTATION, CMsg(_T("&Documentation")));

	pMenu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 0, CMsg(_T("E&xit")));
		pSub = pMenu->GetSubMenu(3);
		pSub->ModifyMenu(ID_EXIT_CLOSEAXIS, MF_STRING, ID_EXIT_CLOSEAXIS, CMsg(_T("Close A&xis3")));

	if (GetSettingNum(_T("DevMode")) != 1)
	{
		pMenu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
	}
	AfxGetMainWnd()->DrawMenuBar();

	SetWindowPos(GetSettingNum(_T("AlwaysOnTop")) ? &CWnd::wndTopMost : &CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}

void CAxis3Dlg::SetTabsPosition()
{	CWnd *pBaseWnd = GetDlgItem(IDC_TAB);

	if(pBaseWnd!=NULL)
	{	
		CRect rcTab;
		pBaseWnd->GetWindowRect(&rcTab);
		ScreenToClient(&rcTab);
		rcTab.right = max(rcTab.left,rcTab.right);
		rcTab.bottom = max(rcTab.top,rcTab.bottom);
		m_TabCtrl.MoveWindow(&rcTab);
	}
}

void CAxis3Dlg::OnTabSelected(TabCtrl * /*pCtrl*/, HTAB /*hTab*/)
{
	Main->m_DlgCommands->OnBnClickedtabCancel();
	Main->m_DlgCommands->OnBnClickedbuttonCancel();
}

void CAxis3Dlg::OnOk()
{
}

void CAxis3Dlg::OnClose()
{
	CDialogEx::OnOK();
}


void CAxis3Dlg::OnExitCloseaxis()
{
	EndDialog(IDOK);
}


void CAxis3Dlg::OnProfile()
{
	CProfileDlg dlg;
		if (dlg.DoModal() != IDOK)
		{
			if (dlg.m_bSelect)
				m_DlgITems->FillCategory();
			return;
		}
		if (dlg.m_bLocal)
		{
			m_csPath = dlg.csPath;
			if (m_csPath == "")
			{
				AfxMessageBox(CMsg(_T("Spheretable.scp not found!")), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			AfxBeginThread(&ProfileLocalThread, this);
		}
		else
		{
			m_csAddress = dlg.csAddress;
			m_csPort = dlg.csPort;
			if ((m_csAddress == "") || (m_csPort == ""))
			{
				AfxMessageBox(CMsg(_T("Couldn't determine the IP address for the remote server.")), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			m_csAccount = dlg.csAccount;
			m_csPassword = dlg.csPassword;

			AfxBeginThread(&ProfileThread, this);
		}
}


void CAxis3Dlg::OnHelpAboutAxis3()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CAxis3Dlg::OnHelpDocumentation()
{
	HtmlHelp((DWORD_PTR)_T("Axis3.chm::/Welcome.htm"), HH_DISPLAY_TOPIC);
}

void CAxis3Dlg::OnSettingDefault()
{
	CSettingsGeneral dlg;
	if (dlg.DoModal() != IDOK)
		return;
}

void CAxis3Dlg::OnDataDefaultSettings()
{
	CSettingEditDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
}

void CAxis3Dlg::OnDataLanguage()
{
	CLangEditDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;
}

void CAxis3Dlg::OnSettingViewer()
{
	CSettingViewer dlg;
	if (dlg.DoModal() != IDOK)
		return;
}



//***********************************
//***********************************

HBRUSH CAxis3Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//Set Color of Tab's Background
	CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	::SetBkMode(pDC->m_hDC,TRANSPARENT);
	return Main->hbWhite;
}

LRESULT CAxis3Dlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_SYSCOMMAND)
	{
		if (wParam == SC_MINIMIZE)
		{
			ShowWindow(SW_SHOWMINIMIZED);
			return TRUE;
		}
	}
	return CDialogEx::DefWindowProc(message, wParam, lParam);
}

void CAxis3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_CLOSE)
		{
			if (!GetSettingNum(_T("SysClose")))
			{
				ShowWindow(SW_HIDE);
				return;
			}
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CAxis3Dlg::OnSize(UINT nType, int cx, int cy) 
{
	CRect crNew;
	GetWindowRect(crNew);
	int dx = crNew.Width()-MinSize.Width();
	int dy = crNew.Height()-MinSize.Height();

	if (m_TabCtrl)
		m_TabCtrl.MoveWindow(TabSize.left,TabSize.top,TabSize.Width()+dx,TabSize.Height()+dy);

	CDialogEx::OnSize(nType, cx, cy);
}

void CAxis3Dlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if(initialized)
	{
		lpMMI->ptMinTrackSize.x = MinSize.Width();
		lpMMI->ptMinTrackSize.y = MinSize.Height();
	}   
    
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CAxis3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CAxis3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT ProfileLocalThread(LPVOID lpData)
{
	Axis->DBData.Close();
	CAxis3Dlg * pMain = (CAxis3Dlg *)lpData;
	LPTSTR szCmdline = _tcsdup(TEXT("AxisDBCreator.exe " + pMain->m_csPath + " " + Axis->csProfile + "\\Data.db"));

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, szCmdline, NULL, NULL, FALSE, CREATE_NEW_PROCESS_GROUP|CREATE_NO_WINDOW, NULL, Axis->m_csRootDirectory, &si, &pi))
	{
		AfxMessageBox(CMsg(_T("ERROR (%1!d!) Failed Starting Application %2"), true, GetLastError(), szCmdline));
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	Axis->DBData.Open(CFrmt(_T("%1/Data.db"), Axis->csProfile));
	pMain->m_DlgITems->FillCategory();
	return 0;
}

UINT ProfileThread(LPVOID lpData)
{
	Axis->DBData.Close();
	CAxis3Dlg * pMain = (CAxis3Dlg *) lpData;
	CSocket sock;
	BYTE szBuffer[8*MAX_BUFFER];
	int iSize;
	sock.m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock.m_hSocket == INVALID_SOCKET)
	{
		AfxMessageBox(CMsg(_T("Error creating socket: dwErr = %1!ld!"),true, WSAGetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		return 1;
	}
	if (!sock.Connect(pMain->m_csAddress, _ttoi(pMain->m_csPort)))
	{
		AfxMessageBox(CMsg(_T("Could not connect to the remote console --\r\nLast Error was '%1'\r\nHost = % 2, Port = % 3\r\nThis is usually because the Sphere server isn't running."),true, ErrorString(GetLastError()) , pMain->m_csAddress, pMain->m_csPort));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		return 2;
	}
	// Log on.
	Sleep( 100 );
	CString cAt((char) 0x40);
	if (!sock.Send(cAt, 1))
	{
		AfxMessageBox(CMsg(_T("Error while sending login info to the remote console: dwErr = %1!ld!\r\nIs the Sphere server running ?"),true, GetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		return 3;
	}
	// Receive the response
	int i = 0;
	while ( true )
	{
		FD_SET fd;
		struct timeval timeout;
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if ( rc < 0 )
		{
			char szErrBuffer[MAX_BUFFER];
			strerror_s(szErrBuffer,MAX_BUFFER,WSAGetLastError());
			AfxMessageBox(CMsg(_T("Socket error on select(): %1!ld! - %2"),true, WSAGetLastError(), szErrBuffer));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			return 4;
		}
		if ( rc == 0 && i++ > pMain->m_iReceiveTimeout )
		{
			AfxMessageBox(CMsg(_T("Timeout exceeded while receiving data from remote server.")));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			return 5;
		}
		if ( rc > 0 )
		{
			sock.Receive(&szBuffer, sizeof(szBuffer));
			//This is the size of the file to receive
			iSize = atoi((const char*)szBuffer);
			break;
		}
	}

	// Send the username?
	if ( pMain->m_csAccount != "" )
	{
		Sleep( 100 );
		CString csAcct = pMain->m_csAccount + (char)0x0A;
		if ( !sock.Send(csAcct, csAcct.GetLength()* sizeof(TCHAR)))
		{
			AfxMessageBox(CMsg(_T("Error while sending account to the remote console: dwErr = %1!ld!\r\nIs the Sphere server still running ?"),true, GetLastError()));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			return 6;
		}
	}
	// Send the password
	Sleep( 100 );
	CString csPass = pMain->m_csPassword + (char)0x0A;
	if ( !sock.Send(csPass, csPass.GetLength()* sizeof(TCHAR)))
	{
		AfxMessageBox(CMsg(_T("Error while sending password to the remote console: dwErr = %1!ld!\r\nIs the Sphere server still running ?"),true, GetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		return 7;
	}

	LPCTSTR pFileName = _T("temp.db");
	LPCTSTR pDBName = CFrmt(_T("%1/Data.db"),Axis->csProfile);

	CFile fData;
	if ( !fData.Open(pFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone) )
		return 8;

	// Here's the main loop
	while (true)
	{
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		FD_SET fd;
		struct timeval timeout;
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if (rc > 0)
		{
			int nBytes = sock.Receive(&szBuffer, sizeof(szBuffer));

			if (nBytes <= 0)
				break;

			while(szBuffer[0] == 0x22)
			{
				CString csBuf(szBuffer);

				//check if it's really a message
				if (csBuf.Find(_T("MSG:")) != -1)
					break;

				csBuf = csBuf.Mid(5);
				csBuf = csBuf.SpanExcluding(_T("\""));

				//remove that string from the buffer
				for (int pos = 1; pos < nBytes; pos++)
				{
					if (szBuffer[pos] == 0x22)
					{
						pos++;
						for (int j = 0; j < nBytes; j++)
						{
							if (j >= (nBytes-pos))
								szBuffer[j] = 0x00;
							else
								szBuffer[j] = szBuffer[pos+j];
						}
						nBytes -= pos;
						break;
					}
				}

				AfxMessageBox(csBuf);
			}
				
			//write to file
			fData.Write(szBuffer, nBytes);
			iSize -= nBytes;

			if (iSize <= 0)
			{
				//close and rename database
				Axis->DBData.Close();
				fData.Close();
				if ( fData.Open(pDBName, CFile::modeRead | CFile::shareDenyNone) )
				{
					fData.Close();
					try{CFile::Remove(pDBName);}
					catch (CFileException* pEx){pEx->Delete();}
				}
				try{CFile::Rename(pFileName,pDBName);}
				catch (CFileException* pEx){pEx->Delete();}
				break;
				Axis->DBData.Open(pDBName);
			}
		}
		if (rc < 0)
		{
			AfxMessageBox(CMsg(_T("Connection lost.")));
			break;
		}
	}
	//Close and remove temp file if it had failed to complete
	fData.Close();
	try{CFile::Remove(pFileName);}
	catch (CFileException* pEx){pEx->Delete();}

	closesocket(sock.m_hSocket);
	sock.m_hSocket = INVALID_SOCKET;
	pMain->m_DlgITems->FillCategory();
	return 0;
}