
#pragma once

#include <comcat.h>
#include <objsafe.h>

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

class ArtIdx
{
public:
	DWORD dwLookup;
	DWORD dwSize;
	DWORD dwUnknown;
};

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

class CMultiRec
{
public:
	WORD wIndex;
	short x;
	short y;
	short z;
	DWORD dwFlags;
};

/////////////////////////////////////////////////////////////////////////////
// CUOArt class

class CUOArt : public CWnd
{
// Constructor
public:
	CUOArt();
	~CUOArt();

// Implementation
protected:
/*	void RedrawArt(CPaintDC* pdc, const CRect& rcBounds);
	void DrawArt(short sArtType, CRect bounds, DWORD dwArtIndex, short dx, short dy, short dz, WORD wColor = 0, WORD wFlags = 0);
	void DrawRoomview(CRect bounds, WORD wArtHeight);
	void DrawSwing(CRect bounds, WORD wArtHeight, int idirection = 0);
	void SetInvalid();
	DWORD BlendColors(WORD wBaseColor, WORD wAppliedColor, bool bBlendMode);
	DWORD ScaleColor(WORD wColor);

	BYTE m_tiledata [0x191800];
	WORD m_wArtWidth[0x10000];
	WORD m_wArtHeight[0x10000];
	bool m_bArtDataLoaded;
	void LoadArtData();
	void LoadTiledata();

	CByteArray m_bPixels;
	CByteArray m_bBackGround;
	CRect m_rcSize;

	bool m_bIsValid;
	short m_sArtType;
	long m_xOffset;
	long m_yOffset;
	WORD m_wAppliedColour;
	WORD m_wDrawFlags;
	DWORD m_dwArtIndex;
	DWORD m_dwBkColor;

#define F_ROOMVIEW		0x0001
#define F_ENABLESWING   0x0002
#define F_BG_GRASS		0x0004
#define F_SWINGNORTH	0x0010
#define F_SWINGWEST		0x0020
#define F_SWINGNW		0x0100
#define F_SWINGNE		0x0200
#define F_SWINGSW		0x0400
#define F_SWINGSE		0x0800
*/
	//{{AFX_MSG(CUOArt)
//	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/*
// Attributes
public:

	int m_wFrame;

	short GetArtType();
	void SetArtType(short);
	long GetArtIndex();
	void SetArtIndex(long);
	short GetArtColor();
	void SetArtColor(short);
	long GetXOffset();
	void SetXOffset(long);
	long GetYOffset();
	void SetYOffset(long);
	short GetDrawFlags();
	void SetDrawFlags(short);
	void ReloadArt();

	void SetObject(short type, long index);
	void SetOffset(long x, long y);
	long yScroll(long offset);
	long xScroll(long offset);
	void Scroll(long x, long y);
	long GetBkColor();
	void SetBkColor(long color);

	CBitmap * OnCreateIcon(int iBitWidth, int iBitHeight, int iID, WORD wAppliedColor, int iType = 1);

	enum ArtTypes
	{
		ART_MULTI,
		ART_ITEM,
		ART_LANDSCAPE,
		ART_NPC,
		ART_NPC2,
		ART_NPC3,
		ART_NPC4,
		ART_NPC5,
		ART_LIGHTS,
	};*/
};


/*class CBitmapDC : public CDC
{

public:

  CBitmapDC(int width, int height, COLORREF background = RGB(255,255,255));

  virtual ~CBitmapDC();
  CBitmap *Close();

private:

  CBitmap *m_pBitmap;
  CBitmap *m_pOldBmp;
};*/