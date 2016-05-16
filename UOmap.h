
#pragma once


#include <comcat.h>
#include <objsafe.h>

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

class CDrawObject : public CObject
{
public:
	CDrawObject();
	CPoint m_pt;
	BYTE m_bDrawType;
	BYTE m_bSize;
	DWORD m_dwColor;
};

class CDrawRect : public CObject
{
public:
	CDrawRect();
	CRect m_rect;
	BYTE m_bDrawMode;
	DWORD m_dwColor;
};

struct PatchHash
{
	DWORD dwIndex;
	DWORD dwLookup;
};

#define UO_X_SIZE m_maxX
#define UO_Y_SIZE m_maxY
#define UO_X_CENTER (UO_X_SIZE / 2)
#define UO_Y_CENTER (UO_Y_SIZE / 2)
#define UO_X_BLOCKS (UO_X_SIZE / 8)
#define UO_Y_BLOCKS (UO_Y_SIZE / 8)
#define UO_X_CENTER_BLOCK (UO_X_CENTER / 8)
#define UO_Y_CENTER_BLOCK (UO_Y_CENTER / 8)

/////////////////////////////////////////////////////////////////////////////
// CUOMap wrapper class


class CUOMap : public CWnd
{
// Constructor
public:
	CUOMap();
	~CUOMap();

/*// Implementation

protected:
	void RedrawMap(CPaintDC* pdc, const CRect& rcBounds);
	DWORD ScaleColor(WORD wColor);
	void SetInvalid();
	void LoadRadarcol();
	void OpenMap();
	void CloseMap();

	BOOL LoadPatchHash(CString sFile, CPtrArray &array);
	void CleanHashArray(CPtrArray &array);
	void InsertHash(CPtrArray &array, DWORD lookup, DWORD index);
	DWORD FindPatch(CPtrArray &array, DWORD lookup);

	DWORD * m_dwColorMap;

	CObArray m_aDrawObjects;
	CObArray m_aDrawRects;
	CRect m_rcSize;
	CByteArray m_bMapArray;

	CPtrArray m_aMapHash;
	CPtrArray m_aStaticHash;
	CPtrArray * paMapHash;
	CPtrArray * paStaticHash;

	HANDLE hMapFile;
	HANDLE hStaIdxFile;
	HANDLE hStaticFile;
	HANDLE hMapDifFile;
	HANDLE hMapDiflFile;
	HANDLE hStaDifFile;
	HANDLE hStaDifiFile;
	HANDLE hStaDiflFile;

	HANDLE hMapFileMapping;
	HANDLE hStaIdxFileMapping;
	HANDLE hStaticFileMapping;
	HANDLE hMapDifFileMapping;
	HANDLE hMapDiflFileMapping;
	HANDLE hStaDifFileMapping;
	HANDLE hStaDifiFileMapping;
	HANDLE hStaDiflFileMapping;

	PBYTE pbMapFile;
	PBYTE pbStaIdxFile;
	PBYTE pbStaticFile;
	PBYTE pbMapDifFile;
	PBYTE pbMapDiflFile;
	PBYTE pbStaDifFile;
	PBYTE pbStaDifiFile;
	PBYTE pbStaDiflFile;

	bool m_bIsValid;
	WORD m_wDrawFlags;
	short m_xCenter;
	short m_yCenter;
	short m_zoomLevel;
	short m_mulfile;
	bool iLoaded;
	BOOL iMapPatch;
	BOOL iStaticPatch;
	int m_maxX;
	int m_maxY;

#define F_DRAWSTATICS	0x0001
#define F_DRAWDIFS		0x0002



	afx_msg void OnPaint();
	*/
	DECLARE_MESSAGE_MAP()

/*
// Attributes
public:

	void ReSetCenter(short x, short y);
	short GetZoomLevel();
	void SetZoomLevel(short);
	short GetXCenter();
	void SetXCenter(short);
	short GetYCenter();
	void SetYCenter(short);
	short GetWidth();
	void SetWidth(short propVal);
	short GetHeight();
	void SetHeight(short propVal);
	void ZoomIn();
	void ZoomOut();
	void SetCenter(short x, short y);
	void Scroll(short direction, short distance);
	short GetMapFile();
	void SetMapFile(short propVal);
	void LoadMap(short m, short w, short h);
	long GetCenterBlock();
	short GetMapHeight(short x, short y);
	void GetCenter(short* x, short* y);
	short GetDrawFlags();
	void SetDrawFlags(short);

	void CtrlToMap(short* x, short* y);
	void MapToCtrl(short* x, short* y);
	short CtrlToMapX(short x);
	short CtrlToMapY(short y);
	short MapToCtrlX(short x);
	short MapToCtrlY(short y);

	void GetDisplaySize(short* x, short* y);
	short GetDisplayHeight();
	short GetDisplayWidth();

	long AddDrawObject(short x, short y, short type, short size, long color);
	BOOL RemoveDrawObject(short x, short y, short type, short size, long color);
	BOOL RemoveDrawObjectAt(long index);
	void RemoveDrawObjects();

	long AddDrawRect(short xleft, short ytop, short width, short height, short mode, long color);
	BOOL RemoveDrawRect(short xleft, short ytop, short width, short height, short mode, long color);
	BOOL RemoveDrawRectAt(long index);
	void RemoveDrawRects();

	void UpdateMap();

	enum
	{
		DRAW_NONE,
		DRAW_PLUS,
		DRAW_SQUARE,
		DRAW_DIAMOND,
		DRAW_DOT,
		DRAW_CIRCLE,
		DRAW_TRIANGLE,

		DRAW_TYPE_QTY
	} DRAW_TYPE;

	enum
	{
		DRAW_FILL,
		DRAW_BLEND,
		DRAW_OUTLINE,

		DRAW_MODE_QTY
	} DRAW_MODE;

	enum {
		SCROLL_N = 0,
		SCROLL_NE,
		SCROLL_E,
		SCROLL_SE,
		SCROLL_S,
		SCROLL_SW,
		SCROLL_W,
		SCROLL_NW
	};*/
};
