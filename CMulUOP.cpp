#include "StdAfx.h"
#include "Axis3.h"
#include "Axis3Dlg.h"
#include "Common.h"
#include "CMulUOP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMulUOP, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CMulUOP::CMulUOP()
{
	//SetScale();
	bAutoScale = false;
}

CMulUOP::~CMulUOP()
{
	CleanHashArray(m_aMapHash);
	CleanHashArray(m_aStaticHash);
}

void CMulUOP::InitDisplay()
{
	CRect rcBounds;
	GetClientRect(&rcBounds);
	Drawbmp.Create(rcBounds.Width(), rcBounds.Height());
	Drawbmp.Clear(GetColorRef(GetSettingNum(_T("ViewerBGColor"))));
}

void CMulUOP::OnSize(UINT nType, int cx, int cy)
{
	CRect rcBounds;
	GetClientRect(&rcBounds);
	Drawbmp.Create(rcBounds.Width(), rcBounds.Height());
	DisplayObj();

	CWnd::OnSize(nType, cx, cy);
}

void CMulUOP::SetInvalid()
{  
	Invalidate();
	UpdateWindow();
}

void CMulUOP::OnPaint() 
{
	CPaintDC pdc(this);
	CRect rcBounds;
	GetClientRect(&rcBounds);
	if ( rcBounds.Width() != m_rcSize.Width() || rcBounds.Height() != m_rcSize.Height() )
	{
		SetInvalid();
		m_rcSize = rcBounds;
	}
	Drawbmp.Draw(pdc);
}

void CMulUOP::DisplayObj()
{
	Drawbmp.Clear(GetColorRef(GetSettingNum(_T("ViewerBGColor"))));
	int diWidth, diHeight, siWidth, siHeight;
	int sx = 0;
	int sy = 0;

	if (bAutoScale)
	{
		diWidth = Drawbmp.GetWidth();
		diHeight = Drawbmp.GetHeight();
		siWidth = Objbmp.GetWidth();
		siHeight = Objbmp.GetHeight();
	}
	else
	{
		sx = max(sx, (Objbmp.GetWidth() - Drawbmp.GetWidth()) / 2);
		sy = max(sy, (Objbmp.GetHeight() - Drawbmp.GetHeight()) / 2);
		siWidth = diWidth = min(Objbmp.GetWidth(), Drawbmp.GetWidth());
		siHeight = diHeight = min(Objbmp.GetHeight(), Drawbmp.GetHeight());
	}

	int dx = (Drawbmp.GetWidth() - diWidth) / 2;
	int dy = (Drawbmp.GetHeight() - diHeight) / 2;

	Drawbmp.DrawTransparent(dx, dy, diWidth, diHeight, Objbmp, sx, sy, siWidth, siHeight, RGB(0, 0, 0));

	SetInvalid();
}
//void CMulUOP::DisplayObj(int sx, int sy, bool bAlfa, bool bReset)
//{
//	sx = max(sx,(Drawbmp.GetWidth()/2)*flArtScale);
//	sx = min(sx,((Objbmp.GetWidth()/flArtScale) - (Drawbmp.GetWidth()/2))*flArtScale);
//	sx = max(sx - ((Drawbmp.GetWidth()/2)*flArtScale),0);
//	sy = max(sx,(Drawbmp.GetHeight()/2)*flArtScale);
//	sy = min(sy,((Objbmp.GetHeight()/flArtScale) - (Drawbmp.GetHeight()/2))*flArtScale);
//	sy = max(sy - ((Drawbmp.GetHeight()/2)*flArtScale),0);
//
//	int iWidth = min((Objbmp.GetWidth()/flArtScale),Drawbmp.GetWidth());
//	int iHeight = min((Objbmp.GetHeight()/flArtScale),Drawbmp.GetHeight());
//
//	int dx = (Drawbmp.GetWidth()-iWidth)/2;
//	int dy = (Drawbmp.GetHeight()-iHeight)/2;
//
//	if (bReset)
//		Drawbmp.Clear(GetColorRef(GetSettingNum(_T("ViewerBGColor"))));
//
//	if (bAlfa)
//		Drawbmp.DrawTransparent(dx, dy, iWidth, iHeight, Objbmp, sx, sy, iWidth, iHeight, RGB(0,0,0));
//	else
//		Drawbmp.Draw(dx, dy, iWidth, iHeight, Objbmp, sx, sy, iWidth, iHeight);
//	SetInvalid();
//}

//void CMulUOP::SetScale(float flScale)
//{
//	flArtScale = max(flScale,0.01);
//}

void CMulUOP::SetAutoScale(bool bScale)
{
	bAutoScale = bScale;
}

float CMulUOP::GetScale()
{
	return flArtScale;
}

void CMulUOP::SetCenterX(int x)
{
	iCenterX = max(x,0);
	iCenterX = min(x,Objbmp.GetWidth());
}

int CMulUOP::GetCenterX()
{
	return iCenterX;
}

void CMulUOP::SetCenterY(int y)
{
	iCenterY = max(y,0);
	iCenterY = min(y,Objbmp.GetHeight());
}

int CMulUOP::GetCenterY()
{
	return iCenterY;
}

CBitmapEx CMulUOP::DrawError()
{
	CBitmapEx ErrorPic;
	ErrorPic.Create(70, 30);
	ErrorPic.Clear(GetColorRef(GetSettingNum(_T("ViewerBGColor"))));
	ErrorPic.DrawTextW(0, 0, _T("ERROR!"), _RGB(235, 0, 0), 100, _T("Times New Roman"), 12, 1);
	return ErrorPic;
}

CBitmapEx CMulUOP::GetArtBMP(CFile &fData, ArtIdx indexRec, DWORD dwArtIndex, WORD wAppliedColor)
{
	CBitmapEx ArtBMP;
	try
	{
		BYTE * bData = NULL;
		DWORD dwOffset = 4;
		fData.Seek(indexRec.dwLookup, CFile::begin);
		bData = new BYTE[indexRec.dwSize];
		fData.Read(&bData[0], indexRec.dwSize);

		WORD wArtWidth, wArtHeight;
		memcpy(&wArtWidth, &bData[dwOffset], 2);
		dwOffset += 2;
		memcpy(&wArtHeight, &bData[dwOffset], 2);
		dwOffset += 2;
		if (wArtWidth == 0 || wArtWidth > 1024 || wArtHeight == 0 || wArtHeight > 1024)
			throw 1;

		ArtBMP.Create(wArtWidth, wArtHeight);
		ArtBMP.Clear(_RGB(0, 0, 0));

		WORD * lineStart = new WORD[wArtHeight];
		for (int i = 0; i < wArtHeight; i++)
		{
			memcpy(&lineStart[i], &bData[dwOffset], 2);
			dwOffset += 2;
		}
		DWORD dataStart = dwOffset;
		int x, y;
		x = y = 0;
		while (y < wArtHeight)
		{
			WORD xOffset, xRun;
			memcpy(&xOffset, &bData[dwOffset], 2);
			dwOffset += 2;
			memcpy(&xRun, &bData[dwOffset], 2);
			dwOffset += 2;
			if ((xRun + xOffset) > 2048)
				break;
			else
			{
				if ((xRun + xOffset) != 0)
				{
					x += xOffset;
					WORD * wColor = new WORD[xRun];
					memcpy(&wColor[0], &bData[dwOffset], 2 * xRun);
					dwOffset += 2 * xRun;
					for (int iCount = 0; iCount < xRun; iCount++)
					{
						int X, Y;
						X = x + iCount;
						Y = y;
						if (X >= 0 && X < ArtBMP.GetWidth() && Y >= 0 && Y < ArtBMP.GetHeight())
						{
							DWORD dwColor = BlendColors(wColor[iCount], wAppliedColor, false);
							BYTE r, g, b;
							b = (BYTE)((dwColor >> 16) & 0xFF);
							g = (BYTE)((dwColor >> 8) & 0xFF);
							r = (BYTE)((dwColor) & 0xFF);
							ArtBMP.SetPixel(X, Y, _RGB(r, g, b));
						}
					}
					delete[] wColor;
					x += xRun;
				}
				else
				{
					x = 0;
					y++;
					dwOffset = lineStart[y] * 2 + dataStart;
				}
			}
		}
		delete[] lineStart;
		delete[] bData;
	}
	catch (...)
	{
	}
	return ArtBMP;
}

void CMulUOP::DrawArt(DWORD dwArtIndex, WORD wAppliedColor)
{
	//Check if we have a cache that matches
	if (Objbmp.Load(CMsg(_T("IDS_ART_FILENAME"), true, Axis->csProfile, dwArtIndex, wAppliedColor)))
		return;

	dwArtIndex += 0x4000;
	CString csDataFile, csIndexFile;
	CFile fData, fIndex;
	ArtIdx indexRec;

	if (Axis->IsArtUOP)
	{
		csDataFile = GetMulPath(_T("ArtUOP"));
		if ( !fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			return;
		ArtAddress pArt = FindArtEntry(dwArtIndex);
		indexRec.dwLookup = (DWORD)pArt.qwAdress;
		indexRec.dwSize = pArt.dwCompressedSize;
	}
	else
	{
		csDataFile = GetMulPath(_T("ArtMul"));
		csIndexFile = GetMulPath(_T("ArtIDX"));
		if ( !fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			return;

		if ( !fIndex.Open(csIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		{
			fData.Close();
			return;
		}
		if (fIndex.Seek(dwArtIndex * sizeof(ArtIdx), CFile::begin) > fIndex.GetLength())
		{
			fIndex.Close();
			fData.Close();
			return;
		}
		fIndex.Read(&indexRec, sizeof(ArtIdx));
	}

	if (( indexRec.dwLookup != 0xFFFFFFFF )&&( indexRec.dwSize > 0 ))
	{
		Objbmp.Create(GetArtBMP(fData, indexRec, dwArtIndex, wAppliedColor));
		//Cache Image for later use
		Objbmp.Save(CMsg(_T("IDS_ART_FILENAME"), true, Axis->csProfile, dwArtIndex, wAppliedColor));
	}
	else
	{
		Objbmp.Create(DrawError());
	}
}

void CMulUOP::DrawMulti(DWORD dwArtIndex)
{
	//Check if we have a cache that matches
	if (Objbmp.Load(CMsg(_T("IDS_MULTI_FILENAME"), true, Axis->csProfile, dwArtIndex)))
		return;

	CString csDataFile, csIndexFile;
	CFile fData, fIndex;
	ArtIdx indexRec;

	csDataFile = GetMulPath(_T("MultiMul"));
	csIndexFile = GetMulPath(_T("MultiIDX"));
	if (!fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		return;

	if (!fIndex.Open(csIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		fData.Close();
		return;
	}
	if (fIndex.Seek(dwArtIndex * sizeof(ArtIdx), CFile::begin) > fIndex.GetLength())
	{
		fIndex.Close();
		fData.Close();
		return;
	}
	fIndex.Read(&indexRec, sizeof(ArtIdx));

	CString csArtDataFile, csArtIndexFile;
	CFile fArtData, fArtIndex;
	ArtIdx ArtindexRec;
	if (Axis->IsArtUOP)
	{
		csArtDataFile = GetMulPath(_T("ArtUOP"));
		if (!fArtData.Open(csArtDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;
	}
	else
	{
		csArtDataFile = GetMulPath(_T("ArtMul"));
		csArtIndexFile = GetMulPath(_T("ArtIDX"));
		if (!fArtData.Open(csArtDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		if (!fArtIndex.Open(csArtIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		{
			fArtData.Close();
			return;
		}
	}

	if ((indexRec.dwLookup != 0xFFFFFFFF) && (indexRec.dwSize > 0))
	{
		try
		{
			WORD wItemCount = 0;
			CMultiRec ** items = NULL;
			UINT uMultiRecSize = sizeof(CMultiRec);

			fData.Seek(indexRec.dwLookup, CFile::begin);
			if (!Axis->IsArtPostHS)
			{
				uMultiRecSize = (sizeof(CMultiRec) - sizeof(DWORD));
			}

			wItemCount = (WORD)(indexRec.dwSize / uMultiRecSize);
			items = new CMultiRec *[wItemCount];
			for (WORD i = 0; i < wItemCount; i++)
			{
				CMultiRec * pItem = new CMultiRec;
				fData.Read(pItem, uMultiRecSize);
				items[i] = pItem;
			}

			//Sort Items
			int x1, x2, y1, y2;
			x1 = x2 = y1 = y2 = 0;
			for (WORD i = 0; i < wItemCount; i++)
			{
				if (items[i]->x < x1)
					x1 = items[i]->x;
				else if (items[i]->x > x2)
					x2 = items[i]->x;
				if (items[i]->y < y1)
					y1 = items[i]->y;
				else if (items[i]->y > y2)
					y2 = items[i]->y;
			}
			int X, Y;
			X = x2 - x1 + 1;
			Y = y2 - y1 + 1;
			CPtrList * grid = new CPtrList[X * Y];
			for (int i = 0; i < wItemCount; i++)
			{
				int x, y;
				// We need to get the item height from the tiledata.
				BYTE bItemHeight;
				if (Axis->IsArtPostHS)
				{
					bItemHeight = Axis->m_staticdata[(items[i]->wIndex / 32) * 1316 + 4 + ((items[i]->wIndex % 32) * 41) + 20];
				}
				else
				{
					bItemHeight = Axis->m_staticdata[(items[i]->wIndex / 32) * 1188 + 4 + ((items[i]->wIndex % 32) * 37) + 16];
				}
				items[i]->dwFlags = bItemHeight;
				x = items[i]->x - x1;
				y = items[i]->y - y1;
				int index = (y * X) + x;
				//First item in the list.
				if (grid[index].IsEmpty())
					grid[index].AddHead((void *)items[i]);
				else
				{
					//Compare the heights of the items....
					bool bInserted = false;
					POSITION pos = grid[index].GetHeadPosition();
					while (pos)
					{
						POSITION oldPos = pos;
						CMultiRec * pTest = (CMultiRec *)grid[index].GetNext(pos);
						if ((items[i]->x > pTest->x) || (items[i]->y < pTest->y))
							continue;
						else if ((items[i]->x == pTest->x) && (items[i]->y == pTest->y) && ((items[i]->dwFlags + items[i]->z) >= (pTest->dwFlags + pTest->z)))
							continue;
						else
						{
							// Insert it here.
							grid[index].InsertBefore(oldPos, (void *)items[i]);
							bInserted = true;
							break;
						}
					}
					if (!bInserted)
						grid[index].AddTail((void *)items[i]);
				}
			}
			// Create Temp bitmap
			int MaxX, MaxY, MinX, MinY;
			MaxX = MaxY = 0;
			MinX = MinY = 1600;
			Objbmp.Create(2000, 2000);
			Objbmp.Clear(_RGB(0, 0, 0));

			for (int y = y1; y <= y2; y++)
			{
				for (int x = x1; x <= x2; x++)
				{
					X = x - x1;
					Y = y - y1;
					int index = ((y - y1) * (x2 - x1 + 1)) + (x - x1);
					if (!grid[index].IsEmpty())
					{
						POSITION pos = grid[index].GetHeadPosition();
						while (pos)
						{
							CMultiRec * pItem = (CMultiRec *)grid[index].GetNext(pos);
							if (!pItem)
								continue;
							if (Axis->IsArtUOP)
							{
								ArtAddress pArt = FindArtEntry(pItem->wIndex + 0x4000);
								ArtindexRec.dwLookup = (DWORD)pArt.qwAdress;
								ArtindexRec.dwSize = pArt.dwCompressedSize;
							}
							else
							{
								if (fArtIndex.Seek((pItem->wIndex + 0x4000) * sizeof(ArtIdx), CFile::begin) > fArtIndex.GetLength())
								{
									continue;
								}
								fArtIndex.Read(&ArtindexRec, sizeof(ArtIdx));
							}

							if ((ArtindexRec.dwLookup != 0xFFFFFFFF) && (ArtindexRec.dwSize > 0))
							{
								WORD wAppliedColor = 0;
								if (Axis->IsArtPostHS)
								{
									wAppliedColor = (WORD)pItem->dwHue;
								}
								CBitmapEx BmpTemp = GetArtBMP(fArtData, ArtindexRec, pItem->wIndex + 0x4000, wAppliedColor);
								int DX = (Objbmp.GetWidth() / 2) + ((pItem->x - pItem->y) * 22);
								int DY = (Objbmp.GetHeight() / 2) + ((pItem->x + pItem->y) * 22) - BmpTemp.GetHeight() - (pItem->z * 4);

								Objbmp.DrawTransparent(DX, DY, BmpTemp.GetWidth(), BmpTemp.GetHeight(), BmpTemp, 0, 0, BmpTemp.GetWidth(), BmpTemp.GetHeight(), RGB(0, 0, 0));
								if ((DX+BmpTemp.GetWidth()) > MaxX)
								{
									MaxX = (DX + BmpTemp.GetWidth());
								}
								else if (DX < MinX)
								{
									MinX = DX;
								}
								else if ((DY + BmpTemp.GetHeight()) > MaxY)
								{
									MaxY = (DY + BmpTemp.GetHeight());
								}
								else if (DY < MinY)
								{
									MinY = DY;
								}	
							}
						}
					}
				}
			}
			for (int i = 0; i < wItemCount; i++)
			{
				CMultiRec * pItem = items[i];
				delete pItem;
			}
			delete[] items;
			delete[] grid;
			Objbmp.Crop(MinX, MinY, MaxX - MinX, MaxY - MinY);
			//Cache Image for later use
			Objbmp.Save(CMsg(_T("IDS_MULTI_FILENAME"), true, Axis->csProfile, dwArtIndex));
		}
		catch (...)
		{
		}
	}
	else
	{
		Objbmp.Create(DrawError());
	}
}

void CMulUOP::DrawAnim(DWORD dwAnimIndex, WORD wAppliedColor, int iFrame)
{
	if ( dwAnimIndex >= 0x1000 )
		return;

	// See if we find this guy in body.bef
	int i = Axis->m_daBodydef.Find((WORD) dwAnimIndex);
	if ( i != -1 )
	{
		CDefObj * pDef = (CDefObj *) Axis->m_daBodydef.GetAt(i);
		dwAnimIndex = pDef->wID;
	}
	int iMul = 1;
	// Check to see which anim*.mul
	int j = Axis->m_daBodyConv.Find((WORD) dwAnimIndex);
	if ( j != -1 )
	{
		CDefObj * pMulDef = (CDefObj *) Axis->m_daBodyConv.GetAt(j);
		dwAnimIndex = pMulDef->wID;
		iMul = pMulDef->iMul;
	}

	CString csDataFile, csIndexFile;
	CFile fData, fIndex;
	ArtIdx indexRec;
	switch ( iMul )
	{
	case 1:
		if ( dwAnimIndex < 0xC8 )		//High Details
			dwAnimIndex *= 110;
		else if ( dwAnimIndex < 0x190 )	//Low Details
			dwAnimIndex = ( dwAnimIndex - 0xC8 ) * 65 + 22000;
		else							//Humans - Wearables
			dwAnimIndex = ( dwAnimIndex - 0x190) * 175 + 35000;
		csDataFile = GetMulPath(_T("AnimMul"));
		csIndexFile = GetMulPath(_T("AnimIDX"));
		break;
	case 2:
		if ( dwAnimIndex == 0x44 )		//Kirin Fix
			dwAnimIndex = 13420;
		else if ( dwAnimIndex < 0xC8 )	//High Details
			dwAnimIndex *= 110;
		else							//Low Details
			dwAnimIndex = ( dwAnimIndex - 0xC8 ) * 65 + 22000;
		csDataFile = GetMulPath(_T("AnimMul2"));
		csIndexFile = GetMulPath(_T("AnimIDX2"));
		break;
	case 3:
		if ( dwAnimIndex == 0x5F )		//Turkey Fix
			dwAnimIndex = 15175;
		else if ( dwAnimIndex < 0x190 )	//High Details
			dwAnimIndex *= 110;
		else if ( dwAnimIndex < 0x258 )	//Low Details
			dwAnimIndex = ( dwAnimIndex - 0x190 ) * 65 + 44000;
		else							//Humans - Wearables
			dwAnimIndex = ( dwAnimIndex - 0x258 ) * 175 + 70000;
		csDataFile = GetMulPath(_T("AnimMul3"));
		csIndexFile = GetMulPath(_T("AnimIDX3"));
		break;
	case 4:
		if ( dwAnimIndex < 0xC8 )		//High Details
			dwAnimIndex *= 110;
		else if ( dwAnimIndex < 0x190 )	//Low Details
			dwAnimIndex = ( dwAnimIndex - 0xC8 ) * 65 + 22000;
		else							//Humans - Wearables
			dwAnimIndex = ( dwAnimIndex - 0x190) * 175 + 35000;
		csDataFile = GetMulPath(_T("AnimMul4"));
		csIndexFile = GetMulPath(_T("AnimIDX4"));
		break;
	case 5:
		if ( dwAnimIndex == 0x22 )		//Chimera Fix
			dwAnimIndex = 11210;
		else if ( dwAnimIndex < 0xC8 )	//High Details
			dwAnimIndex *= 110;
		else if ( dwAnimIndex < 0x190 )	//Low Details
			dwAnimIndex = ( dwAnimIndex - 0xC8 ) * 65 + 22000;
		else							//Humans - Wearables
			dwAnimIndex = ( dwAnimIndex - 0x190) * 175 + 35000;
		csDataFile = GetMulPath(_T("AnimMul5"));
		csIndexFile = GetMulPath(_T("AnimIDX5"));
		break;
	}
	dwAnimIndex += abs(iFrame);

	if ( !fData.Open(csDataFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		return;

	if ( !fIndex.Open(csIndexFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		fData.Close();
		return;
	}
	if (fIndex.Seek(dwAnimIndex * sizeof(ArtIdx), CFile::begin) > fIndex.GetLength())
	{
		fIndex.Close();
		fData.Close();
		return;
	}
	fIndex.Read(&indexRec, sizeof(ArtIdx));

	if (( indexRec.dwLookup != 0xFFFFFFFF )&&( indexRec.dwSize > 0 ))
	{
		try
		{
			BYTE * bData = NULL;
			DWORD dwOffset = 0;
			fData.Seek(indexRec.dwLookup, CFile::begin);
			bData = new BYTE [indexRec.dwSize];
			fData.Read(&bData[0], indexRec.dwSize);
			WORD wPalette[256];
			memcpy(&wPalette[0], &bData[dwOffset], sizeof(WORD) * 256);
			dwOffset += sizeof(WORD) * 256;
			DWORD dwFrameCount;
			memcpy(&dwFrameCount, &bData[dwOffset], 4);
			dwOffset += 4;
			DWORD * frameOffsets = new DWORD [dwFrameCount];
			memcpy(&frameOffsets[0], &bData[dwOffset], sizeof(DWORD) * dwFrameCount);
			// Read the frame we want
			dwOffset = 256 * sizeof(WORD) + frameOffsets[0];
			short imageCenterX, width, height;
			memcpy(&imageCenterX, &bData[dwOffset], 2);
			dwOffset += 4;
			memcpy(&width, &bData[dwOffset], 2);
			dwOffset += 2;
			memcpy(&height, &bData[dwOffset], 2);
			dwOffset += 2;
			WORD previousLine = 0xFF;
			width += 4;
			height += 4;
			imageCenterX += 2;
			Objbmp.Create(width, height);
			Objbmp.Clear(_RGB(0,0,0));

			int Y = 2;
			while ( true )
			{
				short header, offset;
				memcpy(&header, &bData[dwOffset], 2);
				dwOffset += 2;
				memcpy(&offset, &bData[dwOffset], 2);
				dwOffset += 2;
				if ( ( header == 0x7FFF ) || ( offset == 0x7FFF) )
					break;
				WORD wRunLength = header & 0x0FFF;
				WORD wLineNum = (header >> 12) & 0x000f;
				WORD wTmp = offset & 0x8000;
				offset = (wTmp) | (offset >> 6);
				int X = 0;
				if ( ( previousLine != 0xFF ) && ( wLineNum != previousLine ) )
					Y++;
				previousLine = wLineNum;
				for ( int i = 0; i < wRunLength; i++ )
				{
					BYTE bIndex = 0;
					memcpy(&bIndex, &bData[dwOffset], 1);
					dwOffset++;
					X = (iFrame > 0) ? ((Objbmp.GetWidth()-imageCenterX) - (offset+i)) : (imageCenterX + (offset+i));
					if ( X >= 0 && X < Objbmp.GetWidth() && Y >= 0 && Y < Objbmp.GetHeight() )
					{
						DWORD dwColor = BlendColors(wPalette[bIndex], wAppliedColor, false);
						BYTE r, g, b;
						b = (BYTE) ((dwColor >> 16) & 0xFF);
						g = (BYTE) ((dwColor >> 8) & 0xFF);
						r = (BYTE) ((dwColor) & 0xFF);
						Objbmp.SetPixel( X, Y, _RGB(r,g,b));
					}

				}
			}
			delete [] frameOffsets;
			delete [] bData;
		}
		catch(...)
		{
		}
	}
}

//*************
//Map
void CMulUOP::DrawMap(int iMap)
{
	bool bDrawStatics = (GetSettingNum(_T("DrawMapStatics")) != 0);
	bool bDrawMapDifs = ((GetSettingNum(_T("DrawMapDif")) != 0) && (!Axis->IsMapUOP));
	bool bDrawStaticDifs = (bDrawMapDifs && bDrawStatics);

	//Check if we have a cache that matches the settings
	if (Objbmp.Load(CMsg(_T("IDS_MAP_FILENAME"), true, Axis->csProfile, iMap, bDrawStatics, bDrawMapDifs, bDrawStaticDifs)))
		return;

	//Check if we can draw the Map
	CFile fData, fStatic, fStaidx;
	CFile fMapDifl, fMapDif, fStaDifl, fStaDifi, fStaDif;
	CString csMapFileName;
	if (Axis->IsMapUOP)
		csMapFileName = GetMulPath(CMsg(_T("Map%1!d!UOP"),true,iMap));
	else
		csMapFileName = GetMulPath(CMsg(_T("Map%1!d!Mul"),true,iMap));

	if ( !fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
	{
		if ((iMap != 1)&&(Axis->IsMapUOP))
			return;

		csMapFileName = GetMulPath(_T("Map0Mul"));
		if ( !fData.Open(csMapFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			return;
	}

	//Check if we want and can draw map statics
	if (bDrawStatics)
	{
		CString csStaticIdxName = GetMulPath(CMsg(_T("Staidx%1!d!"),true,iMap));
		CString csStaticFileName = GetMulPath(CMsg(_T("Statics%1!d!"),true,iMap));
		if ( !fStatic.Open(csStaticFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			bDrawStatics = false;
		else
		{
			if ( !fStaidx.Open(csStaticIdxName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			{
				fStatic.Close();
				bDrawStatics = false;
			}
		}
	}

	//Check if we want and can draw Map Difs
	if (bDrawMapDifs)
	{
		CString csMapDifFileName = GetMulPath(CMsg(_T("MapDif%1!d!"),true,iMap));
		CString csMapDiflFileName = GetMulPath(CMsg(_T("MapDifL%1!d!"),true,iMap));
		if ( !fMapDif.Open(csMapDifFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			bDrawMapDifs = false;
		else
		{
			if ( !fMapDifl.Open(csMapDiflFileName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			{
				fMapDif.Close();
				bDrawMapDifs = false;
			}
			else
			{
				LoadPatchHash(csMapDiflFileName, m_aMapHash);
				paMapHash = &m_aMapHash;
			}
		}
	}

	//Check if we want and can draw Statics Difs
	if (bDrawStaticDifs)
	{
		CString csStaDifName = GetMulPath(CMsg(_T("Stadif%1!d!"),true,iMap));
		CString csStaDifiName = GetMulPath(CMsg(_T("StadifI%1!d!"),true,iMap));
		CString csStaDiflName = GetMulPath(CMsg(_T("StadifL%1!d!"),true,iMap));
		if ( !fStaDif.Open(csStaDifName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			bDrawStaticDifs = false;
		else
		{
			if ( !fStaDifl.Open(csStaDiflName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
			{
				fStaDif.Close();
				bDrawStaticDifs = false;
			}
			else
			{
				if ( !fStaDifi.Open(csStaDifiName, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
				{
					fStaDif.Close();
					fStaDifl.Close();
					bDrawStaticDifs = false;
				}
				else
				{
					LoadPatchHash(csStaDiflName, m_aStaticHash);
					paStaticHash = &m_aStaticHash;
				}
			}
		}
	}

	//Check if we have a cache that matches the new info
	if (Objbmp.Load(CMsg(_T("IDS_MAP_FILENAME"), true, Axis->csProfile, iMap, bDrawStatics, bDrawMapDifs, bDrawStaticDifs)))
		return;

	//Get Map Size
	int width = 6144;
	int height = 4096;

	switch (iMap)
	{
	case 0:
	case 1:
		if (Axis->IsMapML)
			width = 7168;
		break;
	case 2:
		width = 2304;
		height = 1600;
		break;
	case 3:
		width = 2560;
		height = 2048;
		break;
	case 4:
		width = 1448;
		height = 1448;
		break;
	case 5:
		width = 1280;
		break;
	}

	//Get UOP Map Data
	if (Axis->IsMapUOP)
	{
		CString csMapPattern = csMapFileName.Mid(csMapFileName.ReverseFind('\\')+1);
		csMapPattern = csMapPattern.SpanExcluding(_T("."));
		csMapPattern.MakeLower();

		DWORD dwUOPHashLo, dwUOPHashHi, dwCompressedSize, dwHeaderLenght;
		DWORD dwFilesInBlock, dwTotalFiles, dwLoop;
		DWORD64 dwUOPPtr;

		fData.Seek(sizeof(DWORD)*3, CFile::begin);
		fData.Read(&dwUOPHashLo, sizeof(DWORD));
		fData.Read(&dwUOPHashHi, sizeof(DWORD));
		dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
		fData.Seek(sizeof(DWORD), CFile::current);
		fData.Read(&dwTotalFiles, sizeof(DWORD));
		fData.Seek(dwUOPPtr, CFile::begin);
		dwLoop = dwTotalFiles;

		while (dwUOPPtr > 0)
		{
			fData.Read(&dwFilesInBlock, sizeof(DWORD));
			fData.Read(&dwUOPHashLo, sizeof(DWORD));
			fData.Read(&dwUOPHashHi, sizeof(DWORD));
			fData.Seek(dwUOPPtr + (sizeof(DWORD)*3), CFile::begin);
			dwUOPPtr = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;

			while ((dwFilesInBlock > 0)&&(dwTotalFiles > 0))
			{
				dwTotalFiles--;
				dwFilesInBlock--;

				fData.Read(&dwUOPHashLo, sizeof(DWORD));
				fData.Read(&dwUOPHashHi, sizeof(DWORD));
				fData.Read(&dwHeaderLenght, sizeof(DWORD));
				fData.Read(&dwCompressedSize, sizeof(DWORD));

				MapAddress * pMapAddress = new (MapAddress);
				pMapAddress->qwAdress = (((__int64)dwUOPHashHi << 32) + dwUOPHashLo) + dwHeaderLenght;

				fData.Seek(sizeof(DWORD), CFile::current);
				fData.Read(&dwUOPHashLo, sizeof(DWORD));
				fData.Read(&dwUOPHashHi, sizeof(DWORD));
				__int64 qwHash = ((__int64)dwUOPHashHi << 32) + dwUOPHashLo;
					
				CString csFile;
				for (unsigned int x = 0; x < dwLoop; x++)
				{
					if (HashFileName(CMsg(_T("IDS_MAPUOP_FILE"),true,csMapPattern,x)) == qwHash)
					{
						pMapAddress->dwFirstBlock = x*4096;
						pMapAddress->dwLastBlock = (x*4096)+(dwCompressedSize / 196)-1;
						m_aUopAddress.SetAtGrow(x,pMapAddress);
						break;
					}
				}
					
				fData.Seek(sizeof(WORD)*3, CFile::current);
			}

			fData.Seek(dwUOPPtr, CFile::begin);
		}
	}

	//Draw Map
	int xBlocks = width/8;
	int yBlocks = height/8;
	Objbmp.Create(width, height);
	Objbmp.Clear(_RGB(0,0,0));
	MapBlock bData;
	StaticIdx indexRec;
	try
	{
		for ( int bx = 0; bx < xBlocks; bx++ )
		{
			for ( int by = 0; by < xBlocks; by++ )
			{
				StaticData statCells[64];
				if (bDrawStatics)
				{
					memset(&statCells[0], 0x00, (sizeof(StaticData) * 64));
					bool bPatched = false;
					if (bDrawStaticDifs)
					{
						DWORD dwPatchOffset = FindPatch(*paStaticHash, (bx * yBlocks)+by);
						if ( dwPatchOffset != 0xFFFFFFFF )
						{
							fStaDifi.Seek(dwPatchOffset * sizeof(StaticIdx), CFile::begin);
							fStaDifi.Read(&indexRec, sizeof(StaticIdx));
							bPatched = true;
						}
						else
						{
							fStaidx.Seek(((bx * yBlocks)+by) * sizeof(StaticIdx), CFile::begin);
							fStaidx.Read(&indexRec, sizeof(StaticIdx));
						}
					}
					else
					{
						fStaidx.Seek(((bx * yBlocks)+by) * sizeof(StaticIdx), CFile::begin);
						fStaidx.Read(&indexRec, sizeof(StaticIdx));
					}

					if ( indexRec.dwStart != 0xFFFFFFFF )
					{
						int numStatics = indexRec.dwLength / sizeof(StaticData);
						if (bPatched)
							fStaDif.Seek(indexRec.dwStart, CFile::begin);
						else
							fStatic.Seek(indexRec.dwStart, CFile::begin);
						for ( int m = 0; m < numStatics; m++ )
						{
							StaticData objects;
							if (bPatched)
								fStaDif.Read(&objects, sizeof(StaticData));
							else
								fStatic.Read(&objects, sizeof(StaticData));
							int statBlock = objects.bXoff + (objects.bYoff * 8);
							if ( statBlock > 64 )
								continue;
							objects.bJunk1 = 0xFF;

							if ( statCells[statBlock].bJunk1 == 0xFF )
							{
								if ( statCells[statBlock].bAlt <= objects.bAlt )
									memcpy(&statCells[statBlock], &objects, sizeof(StaticData));
							}
							else
								memcpy(&statCells[statBlock], &objects, sizeof(StaticData));
						}
					}
				}

				if (bDrawMapDifs)
				{
					DWORD dwPatchOffset = FindPatch(*paMapHash, (bx * yBlocks)+by);
					if ( dwPatchOffset != 0xFFFFFFFF )
					{
						fMapDif.Seek(dwPatchOffset * sizeof(MapBlock), CFile::begin);
						fMapDif.Read(&bData, sizeof(MapBlock));
					}
					else
					{
						fData.Seek(((bx * yBlocks)+by) * sizeof(MapBlock), CFile::begin);
						fData.Read(&bData, sizeof(MapBlock));
					}
				}
				else
				{
					if (Axis->IsMapUOP)
						fData.Seek(FindMapBlock((bx * yBlocks)+by), CFile::begin);
					else
						fData.Seek(((bx * yBlocks)+by) * sizeof(MapBlock), CFile::begin);

					fData.Read(&bData, sizeof(MapBlock));
				}

				for ( int cy = 0; cy < 8; cy++ )
				{
					for ( int cx = 0; cx < 8; cx++ )
					{
						int X = (bx*8)+cx;
						int Y = (by*8)+cy;
						int iCellIndex = (cy * 8) + cx;
						WORD wColor = ((WORD(bData.cells[iCellIndex].bColorHi)) << 8) | (WORD(bData.cells[iCellIndex].bColorLo));

						if ((bDrawStatics) && (statCells[iCellIndex].bJunk1 == 0xFF) && (statCells[iCellIndex].bAlt >= bData.cells[iCellIndex].bAltitude) )
							wColor = (((WORD(statCells[iCellIndex].bColorHi)) << 8) | (WORD(statCells[iCellIndex].bColorLo))) + 0x4000;

						DWORD dwColor = Axis->m_dwColorMap[wColor];

						BYTE r, g, b;
						b = (BYTE) ((dwColor >> 16) & 0xFF);
						g = (BYTE) ((dwColor >> 8) & 0xFF);
						r = (BYTE) ((dwColor) & 0xFF);
						Objbmp.SetPixel( X, Y, _RGB(r,g,b));
					}
				}
			}
		}
	}
	catch(...)
	{
	}
	fData.Close();
	if (bDrawStatics)
	{
		fStaidx.Close();
		fStatic.Close();
	}
	if (bDrawMapDifs)
	{
		fMapDifl.Close();
		fMapDif.Close();
	}
	if (bDrawStaticDifs)
	{
		fStaDifl.Close();
		fStaDifi.Close();
		fStaDif.Close();
	}

	//Save Map for future uses
	Objbmp.Save(CMsg(_T("IDS_MAP_FILENAME"),true,Axis->csProfile,iMap,bDrawStatics,bDrawMapDifs,bDrawStaticDifs));
}

//*************
//Hues
DWORD CMulUOP::BlendColors(WORD wBaseColor, WORD wAppliedColor, bool bBlendMode)
{
	if ( wAppliedColor == 0 || wAppliedColor > ( Axis->m_aHueGroups.GetUpperBound() * 8 ))
		return ScaleColor(wBaseColor);
	wAppliedColor--;
	DWORD dwBase = ScaleColor(wBaseColor);

	WORD r, g, b;
	r = (WORD) (( dwBase & 0x00FF0000 ) >> 16);
	g = (WORD) (( dwBase & 0x0000FF00 ) >> 8);
	b = (WORD) ( dwBase & 0x000000FF );
	if ( bBlendMode && r != g && r != b)
		return dwBase;
	WORD wOutput = ( wBaseColor >> 10 );
	wOutput += ( wBaseColor >> 5 ) & 0x1f;
	wOutput += ( wBaseColor ) & 0x1f;
	wOutput /= 3;

	int groupIndex = wAppliedColor / 8;
	int hueIndex = wAppliedColor % 8;
	CHueGroup * pGroup = (CHueGroup *) Axis->m_aHueGroups.GetAt(groupIndex);
	if ( pGroup == NULL )
		return dwBase;
	CHueEntry Hue = (CHueEntry) pGroup->Hues[hueIndex];
	return ScaleColor(Hue.wColorTable[wOutput]);
}

//*************
//UOP manipuation
ArtAddress CMulUOP::FindArtEntry(DWORD dwIndex)
{
	CString csArtPattern = GetMulPath(_T("ArtUOP"));
	csArtPattern = csArtPattern.Mid(csArtPattern.ReverseFind('\\')+1);
	csArtPattern = csArtPattern.SpanExcluding(_T("."));
	csArtPattern.MakeLower();
	DWORD64 qwFileHash = HashFileName(CMsg(_T("IDS_ARTUOP_FILE"),true,csArtPattern,dwIndex));
	DWORD64 qwNullFileHash = HashFileName(CMsg(_T("IDS_ARTUOP_FILE"),true,csArtPattern,0x4000));
	ArtAddress* pArtNull = NULL;
	for ( int i = 0; i < Axis->m_aUopAddress.GetSize(); i++ )
	{
		ArtAddress* pArt = (ArtAddress *) Axis->m_aUopAddress.GetAt(i);
		if ( pArt )
		{
			if (qwFileHash == pArt->qwHash)
				return *pArt;
			if (qwNullFileHash == pArt->qwHash)
				pArtNull = pArt;
		}
	}
return *pArtNull;
}

LONGLONG CMulUOP::FindMapBlock(unsigned int iBlock)
{
	for ( int i = 0; i < m_aUopAddress.GetSize(); i++ )
	{
		MapAddress * pMapAddress = (MapAddress *) m_aUopAddress.GetAt(i);
		if (!pMapAddress)
			continue;
		if (( iBlock <= pMapAddress->dwLastBlock ) && ( iBlock >= pMapAddress->dwFirstBlock ))
			return pMapAddress->qwAdress + ((iBlock - pMapAddress->dwFirstBlock)*196);
	}
	return 0;
}

//*************
// CDefArray
int CDefArray::Find(WORD wFind)
{
	if ( this->GetSize() == 0 )
		return -1;
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CDefObj * pDef = (CDefObj *) this->GetAt(iIndex);
		WORD wExisting = pDef->wValue;
		if ( wExisting == wFind )
			return (int)iIndex;
		if ( wExisting > wFind )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return -1;
}

int CDefArray::Insert(CDefObj * pDef)
{
	if ( pDef->wValue == 0 )
	{
		delete pDef;
		return -1;
	}
	INT_PTR iLower = 0;
	INT_PTR iUpper = this->GetUpperBound();
	if ( iUpper == -1 )
	{
		this->InsertAt(0, pDef);
		return 0;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		CDefObj * pTest = (CDefObj *) this->GetAt(iIndex);
		if ( pTest->wValue > pDef->wValue)
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
	this->InsertAt(iIndex, (void *) pDef);
	return (int)iIndex;
}

//*************
//Dif Patches

BOOL CMulUOP::LoadPatchHash(CString sFile, CPtrArray &array)
{
	CleanHashArray(array);
	CFile file;
	if ( !file.Open(sFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone) )
		return false;
	DWORD i = 0;
	while ( TRUE )
	{
		DWORD dwIndex;
		if ( ! file.Read(&dwIndex, sizeof(DWORD)) )
			break;
		InsertHash(array, dwIndex, i);
		i++;
	}
	file.Close();	
	return true;
}

void CMulUOP::CleanHashArray(CPtrArray &array)
{
	for ( int i = 0; i < array.GetSize(); i++ )
	{
		PatchHash * pHash = (PatchHash*) array.GetAt(i);
		if ( pHash )
			delete pHash;
	}
	array.RemoveAll();
}

void CMulUOP::InsertHash(CPtrArray &array, DWORD lookup, DWORD index)
{

	PatchHash * pNewHash = new PatchHash;
	pNewHash->dwIndex = index;
	pNewHash->dwLookup = lookup;

	INT_PTR iLower = 0;
	INT_PTR iUpper = array.GetUpperBound();
	if ( iUpper == -1 )
	{
		array.InsertAt(0, pNewHash);
		return;
	}
	INT_PTR iIndex = 0;
	INT_PTR iCompare = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		PatchHash * pTest = (PatchHash *) array.GetAt(iIndex);
		if ( pTest->dwLookup > pNewHash->dwLookup )
		{
			iCompare = 0;
			iUpper = iIndex - 1;
		}
		else
		{
			if ( pTest->dwLookup == pNewHash->dwLookup )
			{
				delete pTest;
				array.SetAt(iIndex, pNewHash);
				return;
			}
			iCompare = 1;
			iLower = iIndex + 1;
		}
	}
	iIndex += iCompare;
	array.InsertAt(iIndex, (void *) pNewHash);
	return;
}

DWORD CMulUOP::FindPatch(CPtrArray &array, DWORD lookup)
{
	if ( array.GetSize() == 0 )
		return 0xFFFFFFFF;
	INT_PTR iLower = 0;
	INT_PTR iUpper = array.GetUpperBound();
	INT_PTR iIndex = 0;
	while ( iLower <= iUpper )
	{
		iIndex = (iUpper + iLower ) / 2;
		PatchHash * pTest = (PatchHash *) array.GetAt(iIndex);
		if ( pTest->dwLookup == lookup )
			return pTest->dwIndex;
		if ( pTest->dwLookup > lookup )
			iUpper = iIndex - 1;
		else
			iLower = iIndex + 1;
	}
	return 0xFFFFFFFF;
}