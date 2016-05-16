
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
		CString fmt;
		CString t;
		fmt.LoadString(IDS_UNKNOWN_ERROR);
		t.Format(fmt, err, LOWORD(err));
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

	CString csClient = GetSettingString(_T("CustomClientName"));
	if ((csTitle.Find(_T("Ultima Online")) != -1) || (csTitle.Find(_T("UOSA")) != -1) || (!csClient.IsEmpty() && (csTitle.Find(csClient) != -1)))
	{
		pClient->hwndProcess = hWnd;
		return FALSE;
	}

	pClient->hwndProcess = NULL;
	return TRUE;
}

//******************
//Find Axis
BOOL CALLBACK FindAxis(HWND hWnd, LPARAM lParam)
{
	CHWNDProcess * pAxis = (CHWNDProcess *) lParam;
	pAxis->pWnd = CWnd::FromHandle(hWnd);
	CString csTitle;
	pAxis->pWnd->GetWindowText(csTitle);
	if (csTitle.Find(Axis->GetVersionTitle()) != -1)
	{
		pAxis->hwndProcess = hWnd;
		return FALSE;
	}

	pAxis->hwndProcess = NULL;
	return TRUE;
}

//******************
//Format from string table
CMsg::CMsg(UINT nFormatID, bool bFormat,...)
{
	CString strFormat;
	if (!strFormat.LoadString(nFormatID))
	{
		TRACE(_T("Unknown resource string id : %d\n"),nFormatID);
		ASSERT(false);
		CString::operator=(_T("Error!"));
	}
	else if(bFormat)
	{
		  va_list argList;
		  va_start(argList, bFormat);
		  LPTSTR lpszTemp;
		  if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
								strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
				lpszTemp == NULL)
		  {
			  AfxThrowMemoryException();
		  }
		  CString::operator=(lpszTemp);

		  LocalFree(lpszTemp);
		  va_end(argList);
	}else{
		LoadString(nFormatID);
	}
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
//SQL Formated Strings
CSQL::CSQL(LPCTSTR lpszFormat,...)
{
		va_list argList;
		va_start(argList, lpszFormat);
		LPTSTR lpszTemp;
		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
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
	Table TBLang = Axis->DBLng.QuerySQL(CSQL(_T("SELECT * FROM %1 WHERE ID = '%2'"),Axis->csLangCode,sLangID));
	if(TBLang.GetRowCount() != 0)
		return TBLang.GetValue(_T("String"));
	//Default to English if not found
	TBLang = Axis->DBLng.QuerySQL(CSQL(_T("SELECT * FROM ENU WHERE ID = '%1'"),sLangID));
	if(TBLang.GetRowCount() != 0)
		return TBLang.GetValue(_T("String"));
	//String not found!
	return sLangID;
}

//******************
//Get Setting String
CString GetSettingString(LPCTSTR sKey)
{
	//Check Profile's Saved Settings
	Table TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if(TBSettings.GetRowCount() != 0)
		return TBSettings.GetValue(_T("Value"));
	//Get Default setting if not found
	TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings WHERE ID = '%1'"),sKey));
	if(TBSettings.GetRowCount() != 0)
		return TBSettings.GetValue(_T("Value"));
	//Setting not found!
	return _T("Settings Error");
}

//******************
//Set Setting String
void SetSettingValue(LPCTSTR sKey, LPCTSTR sValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if(TBSettings.GetRowCount() != 0)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("UPDATE Settings_%3 SET Value = '%1' WHERE ID = '%2'"),sValue,sKey,Axis->csProfile));
	else
		Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO Settings_%3 VALUES('%1','%2')"),sKey,sValue,Axis->csProfile));
}

//******************
//Get Default Setting
CString GetDefaultString(LPCTSTR sKey)
{
	//Get Default setting
	Table TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings WHERE ID = '%1'"),sKey));
	if(TBSettings.GetRowCount() != 0)
		return TBSettings.GetValue(_T("Value"));
	//Setting not found!
	return _T("Settings Error");
}

//******************
//Set Default Setting String
void SetDefaultValue(LPCTSTR sKey, LPCTSTR sValue)
{
	Table TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings WHERE ID = '%1'"),sKey));
	if(TBSettings.GetRowCount() != 0)
		Axis->DBSettings.ExecuteSQL(CSQL(_T("UPDATE Settings SET Value = '%1' WHERE ID = '%2'"),sValue,sKey));
	else
		Axis->DBSettings.ExecuteSQL(CSQL(_T("INSERT INTO Settings VALUES('%1','%2')"),sKey,sValue));
}

//******************
//Get Custom Mulpath
CString GetMulPath(LPCTSTR sKey)
{
	//Check Saved Settings
	Table TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM Settings_%2 WHERE ID = '%1'"),sKey,Axis->csProfile));
	if(TBSettings.GetRowCount() != 0)
		return TBSettings.GetValue(_T("Value"));
	//Get Default setting if not found
	TBSettings = Axis->DBSettings.QuerySQL(CSQL(_T("SELECT * FROM MulPath WHERE Name = '%1'"),sKey));
	if(TBSettings.GetRowCount() != 0)
		return CMsg(_T("%1%2"),true,GetSettingString(_T("MulPath")),TBSettings.GetValue(_T("File")));
	//Setting not found!
	return _T("GetPath Error");
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
//Send a command to UO Client
bool CommandToUO(LPCTSTR Cmd)
{
	SendToUO(CMsg(_T("%1%2"),true,GetSettingString(_T("CommandPrefix")),Cmd));
	return true;
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

#ifdef _DEBUG
	AfxMessageBox(*Cmd);
#else
	CHWNDProcess * pClient = new CHWNDProcess;
	pClient->hwndProcess = NULL;
	EnumWindows(FindUOClient,(LPARAM)pClient);
	if (pClient->hwndProcess)
	{
		// Found the client window
		CString sToken;
		int curPos = 0;

		sToken= Cmd->Tokenize(_T("\r\n"),curPos);
		while (sToken != _T(""))
		{
			const std::tr1::regex pattern ("[sS][lL][eE][eE][pP]\\(\\d{1,}\\)");
			CT2CA pszString (sToken);
			std::string text (pszString);
			if (std::tr1::regex_match(text, pattern))
			{
				sToken = sToken.SpanExcluding(_T(")"));
				sToken = sToken.Mid(sToken.Find(_T("("))+1);
				DWORD dwMilsec = _ttoi(sToken);
				Sleep(dwMilsec);
			}
			else
			{
				for (int i = 0; i < sToken.GetLength(); i++)
				{
					pClient->pWnd->SendMessage(WM_CHAR, sToken[i], 0);
				}
				pClient->pWnd->SendMessage(WM_CHAR, VK_RETURN, 0);
				pClient->pWnd->SetForegroundWindow();
			}
			sToken = Cmd->Tokenize(_T("\r\n"), curPos);
		}
	}
	delete pClient;
#endif

	delete Cmd;
	return 0;
}

//******************
//Advanced String Array
int CAdvStringArray::Find(CString csName)
{
	if ( this->GetSize() == 0 )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csExisting = this->GetAt(iIndex);
		if ( csExisting.CompareNoCase(csName) == 0 )
			return (int)iIndex;
		if ( csExisting.CompareNoCase(csName) > 0 )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return -1;
}

int CAdvStringArray::Insert(CString csName)
{
	if ( csName == "" )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, csName);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csTest = this->GetAt(iIndex);
		if ( csTest.CompareNoCase(csName) > 0 )
		{
			iCompare = 0;
			iUpper = iIndex - 1;
		}
		else
		{
			iCompare = 1;
			iLower = iIndex + 1;
		}
	}
	iIndex += iCompare;
	this->InsertAt(iIndex, csName);
	return (int)iIndex;
}

CString CAdvStringArray::GetValue(CString csKey)
{
	if ( this->GetSize() == 0 )
		return _T("Error!");
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CString csEntry = this->GetAt(iIndex);
		csEntry = csEntry.SpanExcluding(_T("="));
		if ( csEntry.CompareNoCase(csKey) == 0 )
		{
			csKey = this->GetAt(iIndex);
			CString csValue = csKey.Mid(csKey.Find(_T("=")) + 1);
			return csValue;
		}
		if ( csEntry.CompareNoCase(csKey) > 0 )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return csKey;
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

void LoadUOPArtData()
{
	CString csMulFile = GetMulPath(_T("ArtUOP"));
	CFile cfArtMul;
	if ( cfArtMul.Open(csMulFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
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

void LoadHues()
{
	CFile cfHues;
	CString csHueFile = GetMulPath(_T("Hues"));
	if ( cfHues.Open( csHueFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone ) )
	{
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

void LoadBodyDef()
{
	CString csPath = GetMulPath(_T("BodyDef"));
	CStdioFile csfInput;

		if ( !csfInput.Open(csPath, CFile::modeRead | CFile::shareDenyNone) )
			return;

	BOOL bStatus = TRUE;
	while ( bStatus )
	{
		CString csLine;
		bStatus = csfInput.ReadString(csLine);
		if ( !bStatus )
			break;
		csLine = csLine.SpanExcluding(_T("#"));
		csLine.Trim();
		if ( csLine != "" )
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

void UnLoadBodyDef()
{
	for ( int i = 0; i <= Axis->m_daBodydef.GetUpperBound(); i++ )
	{
		CDefObj * pDef = (CDefObj *) Axis->m_daBodydef.GetAt(i);
		delete pDef;
	}
	Axis->m_daBodydef.RemoveAll();
}

void LoadBodyConvert()
{
	CString csPath = GetMulPath(_T("BodyConv"));
	CStdioFile csfInput;

		if ( !csfInput.Open(csPath, CFile::modeRead | CFile::shareDenyNone) )
			return;

	BOOL bStatus = TRUE;
	while ( bStatus )
	{
		CString csLine;
		bStatus = csfInput.ReadString(csLine);
		if ( !bStatus )
			break;
		csLine = csLine.SpanExcluding(_T("#"));
		csLine.Trim();
		if ( csLine != "" )
		{
			CDefObj * pDef = new CDefObj;
			CString csTemp;
			int iLength, iMul = 2;
			csTemp = csLine.SpanExcluding(_T(" \t"));
			iLength = csTemp.GetLength()+1;
			if (_ttoi(csTemp)==0)
			{
				delete pDef;
				continue;
			}
			pDef->wValue = (WORD)_ttoi(csTemp);
			
			while(1)
			{
				csTemp = csLine.Mid(iLength);
				csTemp = csTemp.SpanExcluding(_T(" \t"));
				iLength += csTemp.GetLength()+1;
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

void UnLoadBodyConvert()
{
	for ( int i = 0; i <= Axis->m_daBodyConv.GetUpperBound(); i++ )
	{
		CDefObj * pDef = (CDefObj *) Axis->m_daBodyConv.GetAt(i);
		delete pDef;
	}
	Axis->m_daBodyConv.RemoveAll();
}

void LoadRadarcol()
{
	CFile cfRadarFile;
	CString csRadarFileName = GetMulPath(_T("Radarcol"));
	memset(&Axis->m_dwColorMap[0], 0, sizeof(Axis->m_dwColorMap));
	if (cfRadarFile.Open(csRadarFileName, CFile::modeRead | CFile::shareDenyNone))
	{
		WORD wColorArray[65536];
		memset(&wColorArray, 0x00, sizeof(wColorArray));
		cfRadarFile.Read(&wColorArray[0], sizeof(wColorArray));
		cfRadarFile.Close();
		for (long i = 0; i < 65536; i++)
			Axis->m_dwColorMap[i] = ScaleColor(wColorArray[i]);
	}
}

void DetectMapFormat()
{
	CFile fData;
	CString csMapFileName = GetMulPath(_T("Map0UOP"));
	if ( fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		Axis->IsMapUOP = true;
		Axis->IsMapML = true;
		fData.Close();
		return;
	}

	csMapFileName = GetMulPath(_T("Map0Mul"));
	if ( fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		if ( fData.GetLength() > 89915000 )
			Axis->IsMapML = true;
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