#ifndef WinVarsH
#define WinVarsH

#include <afxwin.h>

#define				KEY_CONTROL		0x01
#define				KEY_SHIFT		0x02
#define				KEY_ALT			0x04
extern unsigned int	guiKeyFlags;

extern HWND			ghMainWnd,ghMDIClWnd,ghBEdWnd,ghBRepWnd,ghBSeqWnd,ghPattEdWnd;
extern bool			gbMainWndVis,gbBEdWndVis,gbBRepWndVis,gbBSeqWndVis,gbPattEdWndVis;
extern HINSTANCE	ghInst;					// Current app instance
extern HACCEL		ghAccel;

#define MAX_LOADSTRING 100
extern char			gsz_APP_NAME[MAX_LOADSTRING];			// Application title
extern char			gsz_SURE_TO_INIT_SONG[MAX_LOADSTRING];

extern int			giBRepTX,giBRepTY,giBSeqTX,giBSeqTY,giBEdTX,giBEdTY; 
extern int			giBRepWTX,giBRepWTY,giBSeqWTX,giBSeqWTY,giBEdWTX,giBEdWTY; 
extern int			giScrTX,giScrTY;

#endif