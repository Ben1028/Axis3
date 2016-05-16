#include "stdafx.h"
#include "Axis2.h"
#include "Axis2Dlg.h"
#include "RemoteConsole.h"
#include "RemoteConsoleLoginDlg.h"

CRemoteConsole::CRemoteConsole(HWND hWndParent)
{
	Main->m_DlgGeneral->m_pRConsole = this;
	this->m_parentHWnd = hWndParent;
	m_bCommandPending = false;
	m_bIsConnected = false;
	m_bIsInitialized = false;
	m_bIsAlive = false;
	m_bContinue = true;
	m_csCommand = "";
	m_dwThreadID = 0;
	m_csPort = "";
	m_csAddress = "";
	m_csAccount = "";
	m_csPassword = "";
	CString csSavePath;
	// Try to find out where to connect to
	CRemoteConsoleLoginDlg dlg;
	if ( dlg.DoModal() != IDOK )
		return;
	m_csAddress = dlg.csAddress;
	m_csPort = dlg.csPort;
	// check the IP
	if ((m_csAddress == "")||(m_csPort == ""))
	{
		AfxMessageBox(CMsg(_T("IDS_ERROR_NOIP")), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	m_csAccount = dlg.csAccount;
	m_csPassword = dlg.csPassword;

	m_pThread = AfxBeginThread(&MainThread, this);
	while (!m_bIsInitialized)
		Sleep(50);
	return;
}

CRemoteConsole::~CRemoteConsole()
{
	/*m_bContinue = false;
	int i = 0;
	while ( m_bIsAlive && i < 100)
	{
			Sleep(100);
			i++;
	}
	
	Main->m_DlgGeneral->m_pRConsole = NULL;*/
}

void CRemoteConsole::Broadcast(CString csMessage)
{
	/*this->m_csCommand.Format(_T("B %s%c"), csMessage, 0x0d);
	this->m_bCommandPending = true;
	if (this->IsConnected())
	{
		int i = 0;
		while(this->m_bCommandPending && i < 100)
		{
			Sleep(100);
			i++;
		}
	}*/
}

void CRemoteConsole::Reconnect()
{
	/*this->m_bContinue = true;
	m_pThread = AfxBeginThread(&MainThread, this);
	while (!m_bIsInitialized)
		Sleep(50);
	int i = 0;
	while (!m_bIsConnected)
	{
		Sleep(50);
		i++;
		if ( i > 10 )
			break;
	}
	if ( m_bIsConnected  && m_parentHWnd != NULL)
		SendMessage(m_parentHWnd, WM_RCONSOLERECONNECT, (WPARAM) this, 0);*/

}

void CRemoteConsole::Command(CString csMessage)
{
	/*this->m_csCommand.Format(_T("%s%c"), csMessage, 0x0d);
	this->m_bCommandPending = true;
	if (this->IsConnected())
	{
		int i = 0;
		while(this->m_bCommandPending && i < 100)
		{
			Sleep(100);
			i++;
		}
	}*/
}
