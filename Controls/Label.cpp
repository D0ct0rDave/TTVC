#include "Label.h"
#include <stdlib.h>
#include <stdio.h>

void CLabel::Init(HWND _hWnd,int _iX,int _iY,char *_szFont,int _iFontSize)
{
	hWnd = _hWnd;
	iX = _iX;
	iY = _iY;
	iFontSize = _iFontSize;

	Font = CreateFont(iFontSize,0,
					  0,0,
					  FW_BOLD, // FW_DONTCARE,
					  false,	// italic
					  false,	// underline
					  false,	// strike out
					  DEFAULT_CHARSET,
					  OUT_DEFAULT_PRECIS,
					  CLIP_DEFAULT_PRECIS,
					  PROOF_QUALITY, // DEFAULT_QUALITY,
					  FF_MODERN | DEFAULT_PITCH,
					  _szFont);
}

void CLabel::SetColor(int _iColor)
{
	iColor = _iColor;
}

void CLabel::SetText (char *_szString)
{
	// Invalidate old area (when old text is larger than new text)
	InvalidateRect(hWnd,&Area,false);

	strncpy(szStr,_szString,1024);
	szStr[1023] = 0;
	iLen        = strlen(szStr);

	Area.left   = iX;
	Area.top    = iY;
	Area.right  = iX + iFontSize*iLen;
	Area.bottom = iY + iFontSize;
	
	// Invalidate new area (when new text is larger than old text)
	InvalidateRect(hWnd,&Area,false);
}

void CLabel::SetValue(int _iValue)
{
	char szString[1024];
	sprintf(szString,"%d",_iValue);
	SetText(szString);
}

void CLabel::Draw()
{
	HDC hDC = GetDC(hWnd);
	
	Draw(hDC);

	ReleaseDC(hWnd,hDC);	
}

void CLabel::Draw(HDC _hDC)
{
	// DrawText(hDC,szString,strlen(szString),&UpdRect, DT_CENTER);
	SelectObject(_hDC,Font);
	SetBkMode   (_hDC, TRANSPARENT); 
	SetTextColor(_hDC,iColor);
	// SetBkColor(hDC,
	TextOut(_hDC,iX,iY,szStr,iLen);
}
