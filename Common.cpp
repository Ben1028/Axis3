
#include "StdAfx.h"
#include "Axis3.h"
#include "Common.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString ErrorString(DWORD err)
{
	CString Error;
	LPTSTR s;
	if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, (LPTSTR)&s, 0, NULL) == 0)
	{ /* failed */
		CString t;
		t.Format(_T("Unknown error 0x%08x (%d)"), err, LOWORD(err));
		Error = t;
	}else
	{ /* success */
		LPTSTR p = _tcschr(s, _T('\r'));
		if(p != NULL)
		{ /* lose CRLF */
			*p = _T('\0');
		} 
		Error = s;
		::LocalFree(s);
	}
	return Error;
}


//******************
//String Encryption
CString Encrypt(CString csValue)
{
	CString csKey = _T("EtvFwERkLdFmclLKSwqaPiWxClkDfoybZpIWOtgmH");
	CString csEncrypted = csValue;

	for (int i = 0; i < csValue.GetLength(); i++)
	{
		csKey.SetAt(i, csKey[i] ^ 128);
		csEncrypted.SetAt(i, csValue[i] ^ csKey[i]);
	}
	return csEncrypted;
}

//******************
//Find UO client
BOOL CALLBACK FindUOClient(HWND hWnd, LPARAM lParam)
{
	CHWNDProcess * pClient = (CHWNDProcess *) lParam;
	pClient->pWnd = CWnd::FromHandle(hWnd);
	CString csTitle;
	pClient->pWnd->GetWindowText(csTitle);

	CString csClient = GetSettingString(_T("UOTitle"));
	if ((csTitle.Find(_T("Ultima Online")) != -1) || (csTitle.Find(_T("UOSA")) != -1) || (!csClient.IsEmpty() && (csTitle.Find(csClient) != -1)))
	{
		pClient->hwndProcess = hWnd;
		return FALSE;
	}

	pClient->hwndProcess = NULL;
	return TRUE;
}

//******************
//Format from language file or plain text
CMsg::CMsg(LPCTSTR lpszFormat, bool bFormat,...)
{
	CString sLangString = LoadLang(lpszFormat);
	if(bFormat)
	{
		va_list argList;
		va_start(argList, bFormat);
		LPTSTR lpszTemp;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
			sLangString, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
			lpszTemp == NULL)
		{
			AfxThrowMemoryException();
		}

		CString::operator=(lpszTemp);

		LocalFree(lpszTemp);
		va_end(argList);
	}
	else
		CString::operator=(sLangString);
}

//******************
//Format from plain text
CFrmt::CFrmt(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	LPTSTR lpszTemp;

	if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
		lpszTemp == NULL)
	{
		AfxThrowMemoryException();
	}

	CString::operator=(lpszTemp);

	LocalFree(lpszTemp);
	va_end(argList);
}

//******************
//Format from language CMD file or plain text with CommandPrefix
CCmd::CCmd(LPCTSTR lpszFormat, bool bFormat, ...)
{
	CString sCMDString = GetSettingString(_T("CommandPrefix")) + LoadCMD(lpszFormat);

	if (bFormat)
	{
		va_list argList;
		va_start(argList, bFormat);
		LPTSTR lpszTemp;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			sCMDString, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
			lpszTemp == NULL)
		{
			AfxThrowMemoryException();
		}

		CString::operator=(lpszTemp);

		LocalFree(lpszTemp);
		va_end(argList);
	}
	else
		CString::operator=(sCMDString);
}

//******************
//Format from language CMD file or plain text without CommandPrefix
CSpk::CSpk(LPCTSTR lpszFormat, bool bFormat, ...)
{
	CString sCMDString = LoadCMD(lpszFormat);

	if (bFormat)
	{
		va_list argList;
		va_start(argList, bFormat);
		LPTSTR lpszTemp;

		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			sCMDString, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
			lpszTemp == NULL)
		{
			AfxThrowMemoryException();
		}

		CString::operator=(lpszTemp);

		LocalFree(lpszTemp);
		va_end(argList);
	}
	else
		CString::operator=(sCMDString);
}

//******************
//SQL Formated Strings
CString SQLStrip(CString csString)
{
	csString.Replace(_T("'"), _T("''"));
	return csString;
}

//******************
//Center New Window with Main Dialog
void CenterWindowEx(HWND h_wind)
{
	CRect rectFrame, rectDlg;
	CWnd* pMainWnd = AfxGetMainWnd();
	if(pMainWnd != NULL)
	{
		pMainWnd->GetClientRect(rectFrame);
		pMainWnd->ClientToScreen(rectFrame);
		::GetWindowRect(h_wind, rectDlg);
		int nXPos = rectFrame.left + (rectFrame.Width() / 2) - ((rectDlg.Width()+8) / 2);
		int nYPos = rectFrame.top + (rectFrame.Height() / 2) - ((rectDlg.Height()+32) / 2);
		::SetWindowPos(h_wind, HWND_TOP, nXPos, nYPos, (rectDlg.Width()+8), (rectDlg.Height()+32), SWP_NOCOPYBITS);
	}

}

//******************
//Load Language String
CString LoadLang(LPCTSTR sLangID)
{	
	//Check Chosen Language
	if (Axis->csLangCode != _T("ENU"))
	{
		Table TBLang = Axis->DBLng.QuerySQL(CFrmt(_T("SELECT * FROM %1 WHERE ID = '%2'"), Axis->csLangCode, SQLStrip(sLangID)));
		if (TBLang.GetRowCount() != 0)
			return TBLang.GetValue(_T("String"));
	}

	//Default to English and save String in list if not found
	Table TBLang = Axis->DBLng.QuerySQL(CFrmt(_T("SELECT * FROM IDList WHERE ID = '%1'"), SQLStrip(sLangID)));
	if (!TBLang.GetRowCount())
		Axis->DBLng.ExecuteSQL(CFrmt(_T("INSERT INTO IDList VALUES('%1')"), SQLStrip(sLangID)));
	return sLangID;
}

CString LoadCMD(LPCTSTR sCMDID)
{
	//Check Chosen Command
	Table TBCMD = Axis->DBLng.QuerySQL(CFrmt(_T("SELECT * FROM CMD WHERE ID = '%1'"), SQLStrip(sCMDID)));
	if (TBCMD.GetRowCount() != 0)
		return TBCMD.GetValue(_T("String"));

	//Command not found, save it!
	Axis->DBLng.ExecuteSQL(CFrmt(_T("INSERT INTO CMD VALUES('%1','%1')"), SQLStrip(sCMDID)));
	return sCMDID;
}

//******************
//Get Setting String/Num
CString GetSettingString(LPCTSTR sKey)
{
	//Check Profile's Saved Settings
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings_Profile_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if (TBSettings.GetRowCount() != 0)
	{
		return TBSettings.GetValue(_T("Value"));
	}
	//Get Default setting if not found
	return GetDefaultString(sKey);
}

int GetSettingNum(LPCTSTR sKey)
{
	return _ttoi(GetSettingString(sKey));
}

//******************
//Set Setting String/Num
void SetSettingString(LPCTSTR sKey, LPCTSTR sValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings_Profile_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if (TBSettings.GetRowCount() != 0)
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("UPDATE Settings_Profile_%3 SET Value = '%1' WHERE ID = '%2'"), sValue, sKey, Axis->csProfile));
	}
	else
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("INSERT INTO Settings_Profile_%3 VALUES('%1','%2')"), sKey, sValue, Axis->csProfile));
	}
}

void SetSettingNum(LPCTSTR sKey, int iValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings_Profile_%2 WHERE ID = '%1'"), sKey, Axis->csProfile));
	if (TBSettings.GetRowCount() != 0)
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("UPDATE Settings_Profile_%3 SET Value = '%1!d!' WHERE ID = '%2'"), iValue, sKey, Axis->csProfile));
	}
	else
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("INSERT INTO Settings_Profile_%3 VALUES('%1','%2!d!')"), sKey, iValue, Axis->csProfile));
	}
}

//******************
//Get Default Setting/Num
CString GetDefaultString(LPCTSTR sKey)
{
	//Get Default setting
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings WHERE ID = '%1'"),sKey));
	if (TBSettings.GetRowCount() != 0)
	{
		return TBSettings.GetValue(_T("Value"));
	}
	//Setting not found!
	return _T("Settings Error");
}

int GetDefaultNum(LPCTSTR sKey)
{
	return _ttoi(GetDefaultString(sKey));
}

//******************
//Set Default Setting String/Num
void SetDefaultString(LPCTSTR sKey, LPCTSTR sValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings WHERE ID = '%1'"),sKey));
	if (TBSettings.GetRowCount() != 0)
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("UPDATE Settings SET Value = '%1' WHERE ID = '%2'"), sValue, sKey));
	}
	else
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("INSERT INTO Settings VALUES('%1','%2')"), sKey, sValue));
	}
}

void SetDefaultNum(LPCTSTR sKey, int iValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings WHERE ID = '%1'"), sKey));
	if (TBSettings.GetRowCount() != 0)
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("UPDATE Settings SET Value = '%1!d!' WHERE ID = '%2'"), iValue, sKey));
	}
	else
	{
		Axis->DBSettings.ExecuteSQL(CFrmt(_T("INSERT INTO Settings VALUES('%1','%2!d!')"), sKey, iValue));
	}
}

//******************
//Get Custom Mulpath
CString GetMulPath(LPCTSTR sKey)
{
	//Check Saved Settings
	Table TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM Settings_Profile_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if(TBSettings.GetRowCount() != 0)
		return TBSettings.GetValue(_T("Value"));
	//Get Default setting if not found
	TBSettings = Axis->DBSettings.QuerySQL(CFrmt(_T("SELECT * FROM MulPath WHERE Name = '%1'"),sKey));
	if(TBSettings.GetRowCount() != 0)
		return CFrmt(_T("%1%2"),GetSettingString(_T("MulPath")),TBSettings.GetValue(_T("File")));
	//Setting not found!
	return _T("GetPath Error");
}

//******************
//Clear Saved Setting
void ClearSetting(LPCTSTR sKey)
{
	//Removing saved setting will reset it to default
	Axis->DBSettings.QuerySQL(CFrmt(_T("DELETE FROM Settings_Profile_%2 WHERE ID = '%1'"), sKey, Axis->csProfile));
}

//******************
//Cleanup Pointer Arrays
void ClearPtrArray(CPtrArray *pArray)
{
	for ( int i = 0; i < pArray->GetSize(); i++ )
	{
		CWnd * pWnd = (CWnd *) pArray->GetAt(i);
		if ( pWnd )
		{
			pWnd->DestroyWindow();
			delete pWnd;
			pWnd = NULL;
		}
	}
	pArray->RemoveAll();
}

//******************
//Start Command thread (Used for Macros with delays)
bool SendToUO(LPCTSTR Cmd)
{
	CString* t = new CString(Cmd);
	AfxBeginThread(SendToClient,t);
	return true;
}

//******************
//UO command thread
UINT SendToClient(LPVOID pParam)
{
	CString* Cmd = static_cast<CString*>(pParam);

	if (GetSettingNum(_T("DevMode")) == 1)
	{
		AfxMessageBox(*Cmd);
	}
	else
	{
		CHWNDProcess * pClient = new CHWNDProcess;
			pClient->hwndProcess = NULL;
			EnumWindows(FindUOClient, (LPARAM)pClient);
			if (pClient->hwndProcess)
			{
				// Found the client window
				CString sToken;
				int curPos = 0;

				sToken = Cmd->Tokenize(_T("\r\n"), curPos);
				while (sToken != _T(""))
				{
					const std::regex pattern("[sS][lL][eE][eE][pP]\\(\\d{1,}\\)");
					CT2CA pszString(sToken);
					std::string text(pszString);
					if (std::regex_match(text, pattern))
					{
						sToken = sToken.SpanExcluding(_T(")"));
						sToken = sToken.Mid(sToken.Find(_T("(")) + 1);
						DWORD dwMilsec = _ttoi(sToken);
						Sleep(dwMilsec);
					}
					else
					{
						for (int i = 0; i < sToken.GetLength(); i++)
						{
							pClient->pWnd->SendMessage(WM_CHAR, sToken[i], 0);
						}
						pClient->pWnd->SendMessage(WM_KEYDOWN, VK_RETURN, 0x001C0001);
						pClient->pWnd->SendMessage(WM_CHAR, VK_RETURN, 0x001C0001);
						pClient->pWnd->SendMessage(WM_KEYUP, VK_RETURN, 0x001C0001);
						pClient->pWnd->SetForegroundWindow();
					}
					sToken = Cmd->Tokenize(_T("\r\n"), curPos);
				}
			}
		delete pClient;
	}
	delete Cmd;
	return 0;
}

//******************
//UOP/Mul
DWORD ScaleColor(WORD wColor)
{
	DWORD dwNewColor;	
	dwNewColor = ((((((wColor >> 10) & 0x01f) * 0x0ff / 0x01f))
		| (((((wColor >> 5) & 0x01f) * 0x0ff / 0x01f)) << 8)
		| ((((wColor & 0x01f) * 0x0ff / 0x01f)) << 16)));
	return (dwNewColor);
}

UINT LoadUOPArtData(LPVOID pParam)
{
	CString csMulFile = GetMulPath(_T("ArtUOP"));
	CFile cfArtMul;
	if ( cfArtMul.Open(csMulFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		Log(CFrmt(_T("UOP Art format detected!")));
		Log(CFrmt(_T("Loading file: %1"), csMulFile));
		Axis->IsArtUOP = true;
		DWORD dwUOPHashLo, dwUOPHashHi, dwCompressedSize, dwHeaderLenght;
		DWORD dwFilesInBlock, dwTotalFiles;
		DWORD64 dwUOPPtr;
		cfArtMul.Seek((sizeof(DWORD)*3), CFile::begin);
		cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
		cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
		dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
		cfArtMul.Seek(sizeof(DWORD), CFile::current);
		cfArtMul.Read(&dwTotalFiles, sizeof(DWORD));
		cfArtMul.Seek(dwUOPPtr, CFile::begin);

		while (dwUOPPtr > 0)
		{
			cfArtMul.Read(&dwFilesInBlock, sizeof(DWORD));
			cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
			cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
			dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;


			while ((dwFilesInBlock > 0)&&(dwTotalFiles > 0))
			{
				dwTotalFiles--;
				dwFilesInBlock--;
				ArtAddress * pArtAddress = new (ArtAddress);

				cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
				cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
				cfArtMul.Read(&dwHeaderLenght, sizeof(DWORD));
				pArtAddress->qwAdress = (((__int64)dwUOPHashHi << 32) + dwUOPHashLo) + dwHeaderLenght;

				cfArtMul.Read(&dwCompressedSize, sizeof(DWORD));
				pArtAddress->dwCompressedSize = dwCompressedSize;

				cfArtMul.Seek(sizeof(DWORD), CFile::current);
				cfArtMul.Read(&dwUOPHashLo, sizeof(DWORD));
				cfArtMul.Read(&dwUOPHashHi, sizeof(DWORD));
				pArtAddress->qwHash = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
						
				Axis->m_aUopAddress.Add(pArtAddress);
						
				cfArtMul.Seek(sizeof(DWORD)+sizeof(WORD), CFile::current);
			}
			cfArtMul.Seek(dwUOPPtr, CFile::begin);
		}
	}
	return 0;
}

void UnLoadUOPArtData()
{
	for ( int i = 0; i < Axis->m_aUopAddress.GetSize(); i++ )
	{
		ArtAddress * pArtAddress = (ArtAddress *) Axis->m_aUopAddress.GetAt(i);
		if ( pArtAddress )
			delete pArtAddress;
	}
	Axis->m_aUopAddress.RemoveAll();
}

UINT LoadHues(LPVOID pParam)
{
	CFile cfHues;
	CString csHueFile = GetMulPath(_T("Hues"));
	if ( cfHues.Open( csHueFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone ) )
	{
		Log(CFrmt(_T("Loading file: %1"), csHueFile));
		int iBytesRead = 1;
		while ( iBytesRead )
		{
			CHueGroup * pHueGroup = new (CHueGroup);
			iBytesRead = cfHues.Read( pHueGroup, sizeof(CHueGroup) );
			if ( iBytesRead )
			{
				Axis->m_aHueGroups.Add( pHueGroup );
			}
			else
				delete pHueGroup;

		}
		cfHues.Close();
	}
	else
	{
		Log(CFrmt(_T("ERROR: Failed to load file: %1"), csHueFile));
		return 1;
	}
	return 0;
}

void UnLoadHues()
{
	for ( int i = 0; i < Axis->m_aHueGroups.GetSize(); i++ )
	{
		CHueGroup * pHueGroup = (CHueGroup *) Axis->m_aHueGroups.GetAt(i);
		if ( pHueGroup )
			delete pHueGroup;
	}
	Axis->m_aHueGroups.RemoveAll();
}

UINT LoadBodyDef(LPVOID pParam)
{
	CString csPath = GetMulPath(_T("BodyDef"));
	CStdioFile csfInput;

	if (csfInput.Open(csPath, CFile::modeRead | CFile::shareDenyNone))
	{
		Log(CFrmt(_T("Loading file: %1"), csPath));
		BOOL bStatus = TRUE;
		while (bStatus)
		{
			CString csLine;
			bStatus = csfInput.ReadString(csLine);
			if (!bStatus)
				break;
			csLine = csLine.SpanExcluding(_T("#"));
			csLine.Trim();
			if (csLine != "")
			{
				CDefObj * pDef = new CDefObj;
				CString csTemp;
				csTemp = csLine.SpanExcluding(_T(" \t"));
				pDef->wValue = (WORD)_ttoi(csTemp);

				csTemp = csLine.Mid(csLine.Find('{') + 1);
				csTemp = csTemp.SpanExcluding(_T("}"));
				pDef->wID = (WORD)_ttoi(csTemp);

				Axis->m_daBodydef.Insert(pDef);
			}
		}
	}
	else
	{
		Log(CFrmt(_T("ERROR: Failed to load file: %1"), csPath));
		return 1;
	}
	return 0;
}

void UnLoadBodyDef()
{
	for ( int i = 0; i <= Axis->m_daBodydef.GetUpperBound(); i++ )
	{
		CDefObj * pDef = (CDefObj *) Axis->m_daBodydef.GetAt(i);
		delete pDef;
	}
	Axis->m_daBodydef.RemoveAll();
}

UINT LoadBodyConvert(LPVOID pParam)
{
	CString csPath = GetMulPath(_T("BodyConv"));
	CStdioFile csfInput;

	if (csfInput.Open(csPath, CFile::modeRead | CFile::shareDenyNone))
	{
		Log(CFrmt(_T("Loading file: %1"), csPath));
		BOOL bStatus = TRUE;
		while (bStatus)
		{
			CString csLine;
			bStatus = csfInput.ReadString(csLine);
			if (!bStatus)
				break;
			csLine = csLine.SpanExcluding(_T("#"));
			csLine.Trim();
			if (csLine != "")
			{
				CDefObj * pDef = new CDefObj;
				CString csTemp;
				int iLength, iMul = 2;
				csTemp = csLine.SpanExcluding(_T(" \t"));
				iLength = csTemp.GetLength() + 1;
				if (_ttoi(csTemp) == 0)
				{
					delete pDef;
					continue;
				}
				pDef->wValue = (WORD)_ttoi(csTemp);

				while (1)
				{
					csTemp = csLine.Mid(iLength);
					csTemp = csTemp.SpanExcluding(_T(" \t"));
					iLength += csTemp.GetLength() + 1;
					if (csTemp == "")
					{
						pDef->wID = pDef->wValue;
						pDef->iMul = 1;
						break;
					}
					if (csTemp != "-1")
					{
						pDef->wID = (WORD)_ttoi(csTemp);
						pDef->iMul = iMul;
						break;
					}
					iMul++;
				}
				Axis->m_daBodyConv.Insert(pDef);
			}
		}
	}
	else
	{
		Log(CFrmt(_T("ERROR: Failed to load file: %1"), csPath));
		return 1;
	}
	return 0;
}

void UnLoadBodyConvert()
{
	for ( int i = 0; i <= Axis->m_daBodyConv.GetUpperBound(); i++ )
	{
		CDefObj * pDef = (CDefObj *) Axis->m_daBodyConv.GetAt(i);
		delete pDef;
	}
	Axis->m_daBodyConv.RemoveAll();
}

UINT LoadRadarcol(LPVOID pParam)
{
	CFile cfRadarFile;
	CString csRadarFileName = GetMulPath(_T("Radarcol"));
	memset(&Axis->m_dwColorMap[0], 0, sizeof(Axis->m_dwColorMap));
	if (cfRadarFile.Open(csRadarFileName, CFile::modeRead | CFile::shareDenyNone))
	{
		Log(CFrmt(_T("Loading file: %1"), csRadarFileName));
		WORD wColorArray[65536];
		memset(&wColorArray, 0x00, sizeof(wColorArray));
		cfRadarFile.Read(&wColorArray[0], sizeof(wColorArray));
		cfRadarFile.Close();
		for (long i = 0; i < 65536; i++)
			Axis->m_dwColorMap[i] = ScaleColor(wColorArray[i]);
	}
	else
	{
		Log(CFrmt(_T("ERROR: Failed to load file: %1"), csRadarFileName));
		return 1;
	}
	return 0;
}

UINT LoadTiledata(LPVOID pParam)
{
	CFile cfTiledata;
	CString csFile = GetMulPath(_T("Tiledata"));
	if (cfTiledata.Open(csFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		Log(CFrmt(_T("Loading file: %1"), csFile));
		//Land data not needed at the moment
		//Axis->m_landdata.SetSize(0x78800);
		//cfTiledata.Read(Axis->m_landdata.GetData(), Axis->m_landdata.GetSize());
		Axis->m_staticdata.SetSize(static_cast<INT_PTR>(cfTiledata.GetLength()- 0x78800));
		cfTiledata.Seek(0x78800, CFile::begin);
		cfTiledata.Read(Axis->m_staticdata.GetData(), Axis->m_staticdata.GetSize());
		cfTiledata.Close();
		if (Axis->m_staticdata.GetSize() == 0x292000)
		{
			Axis->IsArtPostHS = true;
			Log(CFrmt(_T("HS Art format detected!")));
		}
	}
	else
	{
		Log(CFrmt(_T("ERROR: Failed to load file: %1"),csFile));
	}
	return 0;
}

void DetectMapFormat()
{
	CFile fData;
	CString csMapFileName = GetMulPath(_T("Map0UOP"));
	CFileException ex;

	if (fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		Log(CFrmt(_T("UOP Map format detected!")));
		Log(CFrmt(_T("ML Map format detected!")));
		Axis->IsMapUOP = true;
		Axis->IsMapML = true;
		fData.Close();
		return;
	}

	csMapFileName = GetMulPath(_T("Map0Mul"));
	if ( fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		if (fData.GetLength() > 89915000)
		{
			Log(CFrmt(_T("ML Map format detected!")));
			Axis->IsMapML = true;
		}
		fData.Close();
	}
}

__int64 HashFileName(CString csFile)
{
	UINT eax, ecx, edx, ebx, esi, edi;

	eax = ecx = edx = ebx = esi = edi = 0;
	ebx = edi = esi = (__int32) csFile.GetLength() + 0xDEADBEEF;

	int i = 0;

	for ( i = 0; i + 12 < csFile.GetLength(); i += 12 )
	{
		edi = (__int32) ( ( csFile[ i + 7 ] << 24 ) | ( csFile[ i + 6 ] << 16 ) | ( csFile[ i + 5 ] << 8 ) | csFile[ i + 4 ] ) + edi;
		esi = (__int32) ( ( csFile[ i + 11 ] << 24 ) | ( csFile[ i + 10 ] << 16 ) | ( csFile[ i + 9 ] << 8 ) | csFile[ i + 8 ] ) + esi;
		edx = (__int32) ( ( csFile[ i + 3 ] << 24 ) | ( csFile[ i + 2 ] << 16 ) | ( csFile[ i + 1 ] << 8 ) | csFile[ i ] ) - esi;

		edx = ( edx + ebx ) ^ ( esi >> 28 ) ^ ( esi << 4 );
		esi += edi;
		edi = ( edi - edx ) ^ ( edx >> 26 ) ^ ( edx << 6 );
		edx += esi;
		esi = ( esi - edi ) ^ ( edi >> 24 ) ^ ( edi << 8 );
		edi += edx;
		ebx = ( edx - esi ) ^ ( esi >> 16 ) ^ ( esi << 16 );
		esi += edi;
		edi = ( edi - ebx ) ^ ( ebx >> 13 ) ^ ( ebx << 19 );
		ebx += esi;
		esi = ( esi - edi ) ^ ( edi >> 28 ) ^ ( edi << 4 );
		edi += ebx;
	}

	if ( csFile.GetLength() - i > 0 )
	{
		switch ( csFile.GetLength() - i )
		{
			case 12:
				esi += (__int32) csFile[ i + 11 ] << 24;
				//goto case 11;
			case 11:
				esi += (__int32) csFile[ i + 10 ] << 16;
				//goto case 10;
			case 10:
				esi += (__int32) csFile[ i + 9 ] << 8;
				//goto case 9;
			case 9:
				esi += (__int32) csFile[ i + 8 ];
				//goto case 8;
			case 8:
				edi += (__int32) csFile[ i + 7 ] << 24;
				//goto case 7;
			case 7:
				edi += (__int32) csFile[ i + 6 ] << 16;
				//goto case 6;
			case 6:
				edi += (__int32) csFile[ i + 5 ] << 8;
				//goto case 5;
			case 5:
				edi += (__int32) csFile[ i + 4 ];
				//goto case 4;
			case 4:
				ebx += (__int32) csFile[ i + 3 ] << 24;
				//goto case 3;
			case 3:
				ebx += (__int32) csFile[ i + 2 ] << 16;
				//goto case 2;
			case 2:
				ebx += (__int32) csFile[ i + 1 ] << 8;
				//goto case 1;
			case 1:		
				ebx += (__int32) csFile[ i ];
				break;			
		}

		esi = ( esi ^ edi ) - ( ( edi >> 18 ) ^ ( edi << 14 ) );
		ecx = ( esi ^ ebx ) - ( ( esi >> 21 ) ^ ( esi << 11 ) );
		edi = ( edi ^ ecx ) - ( ( ecx >> 7 ) ^ ( ecx << 25 ) );
		esi = ( esi ^ edi ) - ( ( edi >> 16 ) ^ ( edi << 16 ) );
		edx = ( esi ^ ecx ) - ( ( esi >> 28 ) ^ ( esi << 4 ) );
		edi = ( edi ^ edx ) - ( ( edx >> 18 ) ^ ( edx << 14 ) );
		eax = ( esi ^ edi ) - ( ( edi >> 8 ) ^ ( edi << 24 ) );

		return ( (__int64) edi << 32 ) | eax;
	}

	return ( (__int64) esi << 32 ) | eax;
}

long ahextoi(CString csHex)
{
	return _tcstol(csHex, NULL, 16);
}

UINT isStrType(CString csString)
{
	if (csString.Left(1) == _T("0"))
	{
		if (ahextoi(csString) != 0)
			return 1;
		else
			return 2;
	}
	else 
	{
		if (csString.SpanIncluding(_T("0123456789")) == csString)
			return 0;
		else
			return 2;
	}
}

void Log(CString csData)
{
	if (Axis->logFile.m_hFile != CFile::hFileNull)
	{
		Axis->logFile.WriteString(CFrmt(_T("%1 \r\n"),csData));
	}
}

CString GetEditString(CWnd &ceText)
{
	CString csData;
	ceText.GetWindowText(csData);
	return csData;
}

int GetEditNum(CWnd &ceText)
{
	CString csData;
	ceText.GetWindowText(csData);
	return _ttoi(csData);
}

DWORD GetColorRef(DWORD dwColRef)
{
	return _RGB(GetRValue(dwColRef), GetGValue(dwColRef), GetBValue(dwColRef));
}

void ClearDirectory(CString csPath)
{
	WIN32_FIND_DATA info;
	HANDLE hp;
	hp = FindFirstFile(CFrmt(_T("%1/*.*"), csPath), &info);
	do
	{
		DeleteFile(CFrmt(_T("%1/%2"), csPath, info.cFileName));

	} while (FindNextFile(hp, &info));
	FindClose(hp);
}

void CreateDirectoryTree(CString csPath)
{
	CreateDirectory(csPath, NULL);
	csPath += _T("/Art");
	CreateDirectory(csPath, NULL);

	CreateDirectory(csPath + _T("/Static"), NULL);
	CreateDirectory(csPath + _T("/Multi"), NULL);
	CreateDirectory(csPath + _T("/Map"), NULL);
}

void DeleteDirectoryTree(CString csPath)
{
	CString csArtPath = csPath += _T("/Art");
	ClearDirectory(csArtPath + _T("/Static"));
	RemoveDirectory(csArtPath + _T("/Static"));
	ClearDirectory(csArtPath + _T("/Multi"));
	RemoveDirectory(csArtPath + _T("/Multi"));
	ClearDirectory(csArtPath + _T("/Map"));
	RemoveDirectory(csArtPath + _T("/Map"));
	RemoveDirectory(csArtPath);

	ClearDirectory(csPath);
	RemoveDirectory(csPath);
}