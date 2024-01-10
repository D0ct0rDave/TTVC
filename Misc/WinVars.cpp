#include "winvars.h"

unsigned int	guiKeyFlags = 0;
				
HINSTANCE		ghInst;
HWND			ghMainWnd,ghMDIClWnd,ghBEdWnd,ghBRepWnd,ghBSeqWnd,ghPattEdWnd;
HACCEL			ghAccel;
				
bool			gbMainWndVis   = true;
bool			gbBEdWndVis    = false;
bool			gbBRepWndVis   = true;
bool			gbBSeqWndVis   = false;
bool			gbPattEdWndVis = true;
				
char			gsz_APP_NAME[MAX_LOADSTRING];			// Application title
char			gsz_SURE_TO_INIT_SONG[MAX_LOADSTRING];
int				giScrTX;
int				giScrTY;
int				giBRepTX = 224;
int				giBRepTY = 268;
int				giBSeqTX = 224;
int				giBSeqTY = 232;
int				giBEdTX  = 418;
int				giBEdTY  = 280; 
				
int				giBRepWTX = giBRepTX + 6;
int				giBRepWTY = giBRepTY + 25;
int				giBSeqWTX = giBSeqTX + 6;
int				giBSeqWTY = giBSeqTY + 25;
int				giBEdWTX  = giBEdTX + 6;
int				giBEdWTY  = giBEdTY + 25;
				
				