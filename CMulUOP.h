#pragma once

#include "BitmapEx.h"

//*****************
class CDefObj
{
public:
	WORD wValue;
	WORD wID;
	int iMul;
};

class CDefArray : public CPtrArray, public CDefObj
{
public:
	int Insert(CDefObj * pDef);
	int Find(WORD wFind);
};

//*****************
class ArtIdx
{
public:
	DWORD dwLookup;
	DWORD dwSize;
	DWORD dwUnknown;
};

//*****************
class ArtAddress
{
public:
	WORD wArtWidth;
	WORD wArtHeight;
	DWORD64 qwAdress;
	DWORD64 qwHash;
	DWORD dwCompressedSize;
};

//*****************
class CHueEntry
{
public:
	WORD wColorTable[32];
	WORD wTableStart;
	WORD wTableEnd;
	CHAR cName[20];
};

class CHueGroup
{
public:
	DWORD dwHeader;
	CHueEntry Hues[8];
};

//*****************
class MapCell
{
public:
	BYTE bColorLo;
	BYTE bColorHi;
	CHAR bAltitude;
};

class MapBlock
{
public:
	DWORD dwHeader;
	MapCell cells[64];
};

class MapAddress
{
public:
	DWORD dwFirstBlock;
	DWORD dwLastBlock;
	DWORD64 qwAdress;
};

//*****************
class StaticIdx
{
public:
	DWORD dwStart;
	DWORD dwLength;
	DWORD dwUnk;
};

class StaticData
{
public:
	BYTE bColorLo;
	BYTE bColorHi;
	BYTE bXoff;
	BYTE bYoff;
	CHAR bAlt;
	BYTE bJunk1;
	BYTE bJunk2;
};

//*****************
struct PatchHash
{
	DWORD dwIndex;
	DWORD dwLookup;
};

//*****************
//Mul and UOP manipulation class
class CMulUOP : public CWnd
{
public :
	CMulUOP();
	~CMulUOP();
	CBitmapEx Drawbmp;
	CBitmapEx Bkgbmp;
	CBitmapEx Objbmp;
	void DrawArt(DWORD dwArtIndex, WORD wAppliedColor = 0);
	void DrawAnim(DWORD dwAnimIndex, WORD wAppliedColor = 0, int wFrame = -1);
	void DrawMap(int iMap = 0);
	void DisplayObj(int x = 0, int y = 0, bool bAlfa = true, bool bReset = true);
	void InitDisplay();

	void SetScale(int iScale = 1);
	int GetScale();
	void SetCenterX(int x);
	int GetCenterX();
	void SetCenterY(int y);
	int GetCenterY();

protected:
	int iCenterX;
	int iCenterY;
	int iArtScale;
	COLORREF crBkg;
	CRect m_rcSize;
	void SetInvalid();
	DWORD BlendColors(WORD wBaseColor, WORD wAppliedColor, bool bBlendMode);

	ArtAddress FindArtEntry(DWORD dwIndex);
	CPtrArray m_aUopAddress;
	LONGLONG FindMapBlock(unsigned int iBlock);

	CPtrArray m_aMapHash;
	CPtrArray m_aStaticHash;
	CPtrArray * paMapHash;
	CPtrArray * paStaticHash;
	BOOL LoadPatchHash(CString sFile, CPtrArray &array);
	void CleanHashArray(CPtrArray &array);
	void InsertHash(CPtrArray &array, DWORD lookup, DWORD index);
	DWORD FindPatch(CPtrArray &array, DWORD lookup);

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};