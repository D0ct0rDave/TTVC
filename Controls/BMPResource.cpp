
#include "BMPResource.h"


CBMPResource::CBMPResource()
{
	hDC  = 0;
	hBMP = 0;
}

CBMPResource::~CBMPResource()
{
	// Freeing stuff here
	// if (hDC)	ReleaseDC(hDC);
	// if (hBMP)	
}

void CBMPResource::Load(char *_szFilename,HWND _hWnd,HINSTANCE _hInst)
{
	DWORD err;
	hBMP = (HBITMAP)LoadImage(_hInst,_szFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	err = GetLastError();

	if (! hBMP) return;

	HDC hWndDC = ::GetDC(_hWnd);
	if (! hWndDC) return;

	/*
	BOOL GetBitmapDimensionEx(
	  HBITMAP hBitmap,     // handle to bitmap
	  LPSIZE lpDimension   // address of structure receiving dimensions
	);
	*/

	BITMAP bmp;
	if (GetObject(hBMP,sizeof(BITMAP),&bmp))
	{
		iTX = bmp.bmWidth;
		iTY = bmp.bmHeight;		
	}

	hDC  = CreateCompatibleDC(hWndDC);
	SelectObject(hDC,hBMP);


	::ReleaseDC(_hWnd,hWndDC);
}
