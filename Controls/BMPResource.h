#ifndef BMPResourceH
#define BMPResourceH

#include <windows.h>

class CBMPResource
{
	public:	 CBMPResource();
			~CBMPResource();

			void Load(char *_szFilename,HWND _hWnd,HINSTANCE _hInst);
			HDC GetDC();
			int iGetTX();
			int iGetTY();

	private:
			HDC			hDC;
			HBITMAP		hBMP;
			int			iTX;
			int			iTY;
};

inline HDC CBMPResource::GetDC()
{
	return(hDC);
}

inline int CBMPResource::iGetTX()
{
	return(iTX);
}

inline int CBMPResource::iGetTY()
{
	return(iTY);
}

#endif