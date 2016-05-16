#pragma once

class CRemoteConsole
{
public:
	void Command(CString csMessage);
	void Reconnect();
	void Broadcast(CString csMessage);
	CRemoteConsole(HWND hWndParent = NULL);
	virtual ~CRemoteConsole();
	bool IsConnected() {return m_bIsConnected;};
	CString m_csCommand;

	bool m_bIsConnected;
	bool m_bIsInitialized;
	HWND  m_parentHWnd;
	DWORD m_dwThreadID;
	HANDLE m_hThread;
	bool m_bIsAlive;
protected:
	bool m_bLogging;
	CWinThread * m_pThread;
};
