
// Axis3.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CAxis3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CAxis3App * Axis;
CAxis3App theApp;

CAxis3App::CAxis3App()
{
	//__________________________________________________
	//Memory Leak Debug Code
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag( tmpFlag );
	//__________________________________________________
	
	EnableHtmlHelp();
	Axis = this;

	IsArtUOP = false;
	IsArtPostHS = false;
	IsMapUOP = false;
	IsMapML = false;
	m_dwColorMap = new DWORD[65536];
}

BOOL CAxis3App::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxInitRichEdit2();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	//Open Log file
	if (logFile.Open(CMsg(_T("AxisLogs/Axislog-%1.txt"), true, CTime::GetCurrentTime().Format("%Y%m%d")), CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyNone))
	{
		logFile.SeekToEnd();
	}
	Log(CMsg(_T("Axis3 Launched: %1"), true, CTime::GetCurrentTime().Format("%H:%M:%S")));

	// Set Application Root Path
	TCHAR str[MAX_PATH];
	GetModuleFileName(NULL, str, MAX_PATH);
	CString strDir(str);
	m_csRootDirectory = strDir.Left(strDir.ReverseFind(_T('\\'))+1);

	//Load Default Settings File
	CString csPath = CMsg(_T("%1Settings"), true, m_csRootDirectory);
	Log(CMsg(_T("Return %2!d! - Opening Default Settings: %1"), true, csPath, DBSettings.Open(csPath)));

	//Load Profile
	csProfile = GetDefaultString(_T("Profile"));
	csPath = CMsg(_T("%1/Data.db"), true, csProfile);
	Log(CMsg(_T("Return %2!d! - Opening Profile Data: %1"), true, csPath, DBData.Open(csPath)));

	//Load Language File
	csLangCode = GetSettingString(_T("LangCode"));
	csPath = CMsg(_T("%1Lang"), true, m_csRootDirectory);
	Log(CMsg(_T("Return %2!d! - Opening Lang File: %1"), true, csPath, DBLng.Open(csPath)));

	//Check if first time running
	if ((GetSettingString(_T("MulPath")) == _T("")) || (GetSettingString(_T("UOClient")) == _T("")))
	{
		CSettingsGeneral dlg;
		dlg.DoModal();
		//Not allowed to run without a mulpath set
		if (GetSettingString(_T("MulPath")) == _T(""))
		{
			AfxMessageBox(CMsg(_T("IDS_ERROR_MULPATH")), MB_ICONERROR);
			return FALSE;
		}
		if (GetSettingString(_T("UOClient")) == _T(""))
		{
			AfxMessageBox(CMsg(_T("IDS_ERROR_CLIENT")), MB_ICONERROR);
			return FALSE;
		}
		SetDefaultString(_T("MulPath"), GetSettingString(_T("MulPath")));
		SetDefaultString(_T("UOClient"), GetSettingString(_T("UOClient")));
	}

	AfxBeginThread(LoadUOPArtData, nullptr);
	AfxBeginThread(LoadHues, nullptr);
	AfxBeginThread(LoadBodyDef, nullptr);
	AfxBeginThread(LoadBodyConvert, nullptr);
	AfxBeginThread(LoadRadarcol, nullptr);
	AfxBeginThread(LoadTiledata, nullptr);
	DetectMapFormat();

	CAxis3Dlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	DBLng.Close();
	DBSettings.Close();
	DBData.Close();
	return TRUE;
}

BOOL CAxis3App::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

CString CAxis3App::GetVersionTitle()
{
	CString csExe = AfxGetAppName();
	csExe += ".exe";
	DWORD dwHandle = 0;
	DWORD dwSize = GetFileVersionInfoSize(csExe, &dwHandle);
	char * pBuffer = new char [dwSize];
	GetFileVersionInfo(csExe, dwHandle, dwSize, (LPVOID) pBuffer);
	unsigned int iDataSize = 80;
	LPVOID pData;
	VerQueryValue(pBuffer, _T("\\StringFileInfo\\040904b0\\ProductVersion"), &pData, &iDataSize);
	CString csVersionInfo = CMsg(_T("%1"),true,pData);

	delete [] pBuffer;
	
	csVersionInfo.Replace(_T(","), _T("."));
	csVersionInfo.Replace(_T(" "), _T(""));

	return CMsg(_T("IDS_AXIS_VERSION"),true,csVersionInfo);
}


CString CAxis3App::GetBuildTimestamp()
{
	return CMsg(_T("%1 %2"), __DATE__, __TIME__);
}

CAxis3App::~CAxis3App()
{
	UnLoadUOPArtData();
	UnLoadHues();
	UnLoadBodyDef();
	UnLoadBodyConvert();
	delete m_dwColorMap;
	m_staticdata.RemoveAll();
	Log(_T("\r\n"));
	logFile.Close();
}