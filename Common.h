#pragma once

#include "CMulUOP.h"

#define GETNONWHITESPACE( pStr ) while ( pStr[0] == ' ' || pStr[0] == '\t' ) pStr ++;

static int WM_RCONSOLEMESSAGE = WM_USER + 100;

//*****************
// Find Process
class CHWNDProcess
{
public:
	HWND hwndProcess;
	CWnd * pWnd;
};

BOOL CALLBACK FindUOClient(HWND hWnd, LPARAM lParam);
BOOL CALLBACK FindAxis(HWND hWnd, LPARAM lParam);

//*****************
//String Table Class
class CMsg : public CString  
{
public :
  CMsg(LPCTSTR pszFormatstr, bool bFormat = false,...);
  CMsg(UINT nFormatID, bool bFormat = false,...);
};

class CCmd : public CString
{
public:
	CCmd(LPCTSTR pszFormatstr, bool bFormat = false, ...);
};

class CSQL : public CString  
{
public :
  CSQL(LPCTSTR pszFormatstr,...);
};

//*****************
//Advanced String Array
class CAdvStringArray : public CStringArray
{
public:
	int Insert(CString csName);
	int Find(CString csName);
	CString GetValue(CString csKey);
};

//*****************
// Misc functions
CString ErrorString(DWORD err);
CString Encrypt(CString csValue);
void CenterWindowEx(HWND h_wind);
CString LoadLang(LPCTSTR sLangID);
CString LoadCMD(LPCTSTR sCMDID);
CString GetSettingString(LPCTSTR sKey);
CString GetDefaultString(LPCTSTR sKey);
void SetSettingValue(LPCTSTR sKey, LPCTSTR sValue);
void SetDefaultValue(LPCTSTR sKey, LPCTSTR sValue);
CString GetMulPath(LPCTSTR sKey);
//void SetMulPath(LPCTSTR sKey, LPCTSTR sValue);
void ClearPtrArray(CPtrArray *pArray);
UINT SendToClient(LPVOID pParam);
bool CommandToUO(LPCTSTR Cmd);
bool SendToUO(LPCTSTR Cmd);

DWORD ScaleColor(WORD wColor);
void LoadUOPArtData();
void LoadHues();
void UnLoadUOPArtData();
void UnLoadHues();
void LoadBodyDef();
void UnLoadBodyDef();
void LoadBodyConvert();
void UnLoadBodyConvert();
void LoadRadarcol();
void DetectMapFormat();
__int64 HashFileName(CString csFile);

long ahextoi(CString csHex);
UINT isStrType(CString csString);