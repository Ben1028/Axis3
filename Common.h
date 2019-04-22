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
// Misc functions
CString ErrorString(DWORD err);
CString Encrypt(CString csValue);
void CenterWindowEx(HWND h_wind);
CString LoadLang(LPCTSTR sLangID);
CString LoadCMD(LPCTSTR sCMDID);
CString GetSettingString(LPCTSTR sKey);
int GetSettingNum(LPCTSTR sKey);
CString GetDefaultString(LPCTSTR sKey);
int GetDefaultNum(LPCTSTR sKey);
void SetSettingString(LPCTSTR sKey, LPCTSTR sValue);
void SetSettingNum(LPCTSTR sKey, int iValue);
void SetDefaultString(LPCTSTR sKey, LPCTSTR sValue);
void SetDefaultNum(LPCTSTR sKey, int iValue);
void ClearSetting(LPCTSTR sKey);
CString GetMulPath(LPCTSTR sKey);
//void SetMulPath(LPCTSTR sKey, LPCTSTR sValue);
void ClearPtrArray(CPtrArray *pArray);
UINT SendToClient(LPVOID pParam);
bool CommandToUO(LPCTSTR Cmd);
bool SendToUO(LPCTSTR Cmd);

DWORD ScaleColor(WORD wColor);
UINT LoadUOPArtData(LPVOID pParam);
UINT LoadHues(LPVOID pParam);
void UnLoadUOPArtData();
void UnLoadHues();
UINT LoadBodyDef(LPVOID pParam);
void UnLoadBodyDef();
UINT LoadBodyConvert(LPVOID pParam);
void UnLoadBodyConvert();
UINT LoadRadarcol(LPVOID pParam);
UINT LoadTiledata(LPVOID pParam);
void DetectMapFormat();
__int64 HashFileName(CString csFile);

long ahextoi(CString csHex);
UINT isStrType(CString csString);
void Log(CString csData);
CString SQLStrip(CString csString);

CString GetEditString(CWnd &ceText);
int GetEditNum(CWnd &ceText);
void ClearDirectory(CString csPath);
DWORD GetColorRef(DWORD dwColRef);
void CreateDirectoryTree(CString csPath);
void DeleteDirectoryTree(CString csPath);