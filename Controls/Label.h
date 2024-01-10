#ifndef CLabelH
#define CLabelH

#include <Windows.h>

class CLabel
{
	public:
		 CLabel(){ };
		~CLabel(){ };

		void Init	 (HWND _hWnd,int _iX,int _iY,char *_szFont,int _iFontSize);
		void SetColor(int _iColor);
		void SetText (char *_szString);
		void SetValue(int _iValue);
		void Draw	 (HDC _hDC);
		void Draw	 ();

	private:
		int			iColor;

		HFONT		Font;
		int			iX;
		int			iY;
		int			iFontSize;

		HWND		hWnd;
		HDC			hDC;

		RECT		Area;
		char		szStr[1024];
		int			iLen;
};

#endif