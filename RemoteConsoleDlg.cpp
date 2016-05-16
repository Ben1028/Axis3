#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "RemoteConsoleLoginDlg.h"
#include "RemoteConsoleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CRemoteConsoleDlg, CDialogEx)

CRemoteConsoleDlg::CRemoteConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteConsoleDlg::IDD, pParent)
{
	CWaitCursor hourglass;

	m_bIsConnected = false;
	m_bContinue = false;
	m_bCommandPending = false;
	m_iReceiveTimeout = 60000;
	m_nIDTimer = 4000;
}

CRemoteConsoleDlg::~CRemoteConsoleDlg()
{
	m_bContinue = false;
	int i = 0;
	while (m_bIsConnected && i < 100)
	{
		Sleep(100);
		i++;
	}
}

void CRemoteConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RE_CONSOLE, m_creConsole);
	DDX_Control(pDX, IDC_CONSOLE_SEND, m_ceConsoleSend);
	DDX_Control(pDX, IDC_RECONNECT, m_cbReconnect);
	DDX_Control(pDX, IDC_BROADCAST, m_cbBroadcast);
	DDX_Check(pDX, IDC_BROADCAST, m_bBroadcast);
}


BEGIN_MESSAGE_MAP(CRemoteConsoleDlg, CDialog)
	ON_BN_CLICKED(IDC_RECONNECT, OnReconnect)
	ON_BN_CLICKED(IDC_BROADCAST, OnBroadcast)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteConsoleDlg message handlers

BOOL CRemoteConsoleDlg::OnInitDialog() 
{
	Axis->DBLng.BeginTransaction();
	SetWindowText(CMsg(_T("IDS_REMCONSOLE")));
	GetDlgItem(IDC_RECONNECT)->SetWindowText(CMsg(_T("IDS_RECONNECT")));
	Axis->DBLng.CommitTransaction();

	m_hIcon = AfxGetApp()->LoadIcon(IDI_COMMANDS);
	SetIcon(m_hIcon, FALSE);

	CDialogEx::OnInitDialog();
	m_creConsole.LimitText(0x7FFFFFFF);

	GetWindowRect(m_crOriginal);
	m_creConsole.GetWindowRect(m_crConsole);
	m_cbReconnect.GetWindowRect(m_crReConnect);
	m_ceConsoleSend.GetWindowRect(m_crCommandText);
	m_cbBroadcast.GetWindowRect(m_crBroadcast);

	ScreenToClient(m_crOriginal);
	ScreenToClient(m_crConsole);
	ScreenToClient(m_crReConnect);
	ScreenToClient(m_crCommandText);
	ScreenToClient(m_crBroadcast);

	return FALSE;
}

void CRemoteConsoleDlg::OnReconnect() 
{
	if (m_bIsConnected)
	{
		CWaitCursor hourglass;
		m_bContinue = false;
		m_cbReconnect.SetWindowText(CMsg(_T("IDS_RECONNECT")));
	}
	else
	{
		CRemoteConsoleLoginDlg dlg;
		if ( dlg.DoModal() != IDOK )
			return;
		m_csAddress = dlg.csAddress;
		m_csPort = dlg.csPort;
		if ((m_csAddress == "")||(m_csPort == ""))
		{
			AfxMessageBox(CMsg(_T("IDS_ERROR_NOIP")), MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		m_csAccount = dlg.csAccount;
		m_csPassword = dlg.csPassword;

		m_cbReconnect.SetWindowText(CMsg(_T("IDS_DISCONNECT")));
		SetTimer(m_nIDTimer, 1000, NULL);	
		AfxBeginThread(&ClientThread, this);
	}
}

void CRemoteConsoleDlg::OnTimer(UINT nIDEvent) 
{
	if (!m_bIsConnected)
	{
		m_cbReconnect.SetWindowText(CMsg(_T("IDS_RECONNECT")));
		KillTimer(nIDEvent);
	}
	SetWindowText(CMsg(_T("IDS_REMOTE_CONSOLE_TITLE"),true, m_csAccount, m_bIsConnected ? CMsg(_T("IDS_CONNECTED")) : CMsg(_T("IDS_NOT_CONNECTED"))));

	CDialog::OnTimer(nIDEvent);
}

void CRemoteConsoleDlg::OnConMessage(CString csText)
{
	CString sToken;
	int curPos = 0;
	int iScrollCnt = 0;
	sToken= csText.Tokenize(_T("\r\n"),curPos);
	while (sToken != _T(""))
	{
		sToken = csText.Tokenize(_T("\r\n"), curPos);
		iScrollCnt++;
	}

	if(m_creConsole)
	{
		int lastLine = m_creConsole.GetLineCount();
		int lastchar = m_creConsole.LineIndex(lastLine);
		m_creConsole.SetSel(lastchar, lastchar);
		m_creConsole.ReplaceSel(csText);
		m_creConsole.LineScroll(iScrollCnt);
	}
}

void CRemoteConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		Main->m_DlgGeneral->HitConsole();
		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

void CRemoteConsoleDlg::OnSize(UINT nType, int cx, int cy) 
{
	int dx, dy;
	dx = 0; dy = 0;
	CRect crNew;
	GetWindowRect(crNew);
	ScreenToClient(crNew);
	dx = crNew.Width() - m_crOriginal.Width();
	dy = crNew.Height() - m_crOriginal.Height();

	m_creConsole.MoveWindow(m_crConsole.left , m_crConsole.top, m_crConsole.Width() + dx, m_crConsole.Height() + dy);
	m_cbReconnect.MoveWindow(m_crReConnect.left + dx, m_crReConnect.top + dy, m_crReConnect.Width(), m_crReConnect.Height());
	m_ceConsoleSend.MoveWindow(m_crCommandText.left, m_crCommandText.top + dy, m_crCommandText.Width() + dx, m_crCommandText.Height());
	m_cbBroadcast.MoveWindow(m_crBroadcast.left, m_crBroadcast.top + dy, m_crBroadcast.Width(), m_crBroadcast.Height());

	CDialog::OnSize(nType, cx, cy);
}

void CRemoteConsoleDlg::OnBroadcast()
{
	UpdateData();
}

void CRemoteConsoleDlg::OnSendcommand() 
{
	CString csMessage;
	m_ceConsoleSend.GetWindowText(csMessage);

	if ( m_bBroadcast )
		m_csCommand.Format(_T("%s %s%c"), _T("B"), csMessage, 0x0d);
	else
		m_csCommand.Format(_T("%s%c"), csMessage, 0x0d);

	if ( m_csCommand.GetLength() > 76 )
	{
		AfxMessageBox(CMsg(_T("IDS_COMMAND_LONG")), MB_OK);
		return;
	}
	m_ceConsoleSend.SetWindowText(_T(""));
	m_bCommandPending = true;
	if (m_bIsConnected)
	{
		int i = 0;
		while(m_bCommandPending && i < 100)
		{
			Sleep(100);
			i++;
		}
	}
}

BOOL CRemoteConsoleDlg::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnSendcommand();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

UINT ClientThread(LPVOID lpData)
{
	CRemoteConsoleDlg * pConsole = (CRemoteConsoleDlg *) lpData;
	BYTE szBuffer[MAX_BUFFER];
	pConsole->m_bContinue = true;
	CSocket sock;
	sock.m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (sock.m_hSocket == INVALID_SOCKET)
	{
		pConsole->OnConMessage(CMsg(_T("IDS_ERROR_NOSOCKET"),true, WSAGetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		return 1;
	}
	if (!sock.Connect(pConsole->m_csAddress, _ttoi(pConsole->m_csPort)))
	{
		pConsole->OnConMessage(CMsg(_T("IDS_ERROR_NOCONNECT"),true, ErrorString(GetLastError()) , pConsole->m_csAddress, pConsole->m_csPort));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		return 2;
	}
	// Log on.
	Sleep( 100 );
	CString cSpace((char) 0x20);
	if (!sock.Send(cSpace, 1))
	{
		pConsole->OnConMessage(CMsg(_T("IDS_ERROR_LOGIN"),true, GetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		return 3;
	}
	// Receive the response
	int i = 0;
	while ( true )
	{
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		FD_SET fd;
		struct timeval timeout;
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if ( rc < 0 )
		{
			char szBuffer[MAX_BUFFER];
			strerror_s(szBuffer,MAX_BUFFER,WSAGetLastError());
			pConsole->OnConMessage(CMsg(_T("IDS_ERROR_SOCKETSELECT"),true, WSAGetLastError(), szBuffer));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			return 4;
		}
		if ( rc == 0 && i++ > pConsole->m_iReceiveTimeout )
		{
			pConsole->OnConMessage(CMsg(_T("IDS_ERROR_TIMEOUT")));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			return 5;
		}
		if ( rc > 0 )
			break;
	}
	// Send the username?
	if ( pConsole->m_csAccount != "" )
	{
		Sleep( 100 );
		CString csAcct = pConsole->m_csAccount + (char)0x0A;
		if ( !sock.Send(csAcct, csAcct.GetLength()* sizeof(TCHAR)))
		{
			pConsole->OnConMessage(CMsg(_T("IDS_ERROR_SENDACCOUNT"),true, GetLastError()));
			closesocket(sock.m_hSocket);
			sock.m_hSocket = INVALID_SOCKET;
			pConsole->m_bCommandPending = false;
			return 6;
		}
	}
	// Send the password
	Sleep( 100 );
	CString csPass = pConsole->m_csPassword + (char)0x0A;
	if ( !sock.Send(csPass, csPass.GetLength()* sizeof(TCHAR)))
	{
		pConsole->OnConMessage(CMsg(_T("IDS_ERROR_SENDPASSWORD"),true, GetLastError()));
		closesocket(sock.m_hSocket);
		sock.m_hSocket = INVALID_SOCKET;
		pConsole->m_bCommandPending = false;
		return 7;
	}
	pConsole->m_bIsConnected = true;

	// Here's the main loop
	while (pConsole->m_bContinue)
	{
		FD_SET fd;
		struct timeval timeout;
		memset(&szBuffer[0], 0x00, sizeof(szBuffer));
		FD_ZERO(&fd);
		FD_SET(sock.m_hSocket, &fd);
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		int rc = select(0, &fd, 0, 0, &timeout);
		if (rc > 0)
		{
			int nBytes = sock.Receive(&szBuffer[0], sizeof(szBuffer));
			if (nBytes <= 0)
			{
				pConsole->OnConMessage(CMsg(_T("IDS_ERROR_CONNECTIONLOST")));
				break;
			}
			CString csText(szBuffer);
			pConsole->OnConMessage(csText);
		}
		if (rc < 0)
		{
			pConsole->OnConMessage(CMsg(_T("IDS_ERROR_CONNECTIONLOST")));
			break;
		}
		if (pConsole->m_bCommandPending)
		{
			sock.Send( pConsole->m_csCommand, pConsole->m_csCommand.GetLength()* sizeof(TCHAR));
			pConsole->m_bCommandPending = false;
			pConsole->m_csCommand = "";
		}
	}

	// Disconnect the socket
	if ( !pConsole->m_bContinue)
		pConsole->OnConMessage(CMsg(_T("IDS_CONSOLE_DISCONECT")));

	closesocket(sock.m_hSocket);
	sock.m_hSocket = INVALID_SOCKET;
	pConsole->m_bCommandPending = false;
	pConsole->m_bIsConnected = false;
	return 0;
}