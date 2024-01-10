//------------------------------------------------------------------------------
#ifndef BtnMgrH
#define BtnMgrH


//------------------------------------------------------------------------------
// TODO:
// Hacer un singleton
// Lanzar acciones cuando los botones sean "activado" desde el gestor
// Activado significa 
//		- pulsado por primera vez, o 
//		- En cada repeticion, o
// Lanzar acciones cuando el boton sea "desactivado"
//		- si se vuelve a pulsar y cambia el estado
// Agrupar botones de modo que si uno se pulsa el resto se libera,
// en ese caso, se debe lanzar la accion de boton desactivado.
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//Botones General

#include <Windows.h>
#include "BMPResource.h"

#define		MAX_BOTONES		128


typedef void (* TButtonAction)(void);

typedef enum {eBSPressed,eBSReleased,eBSDisabled} EButtonState;
typedef enum {eBTOneShot,eBTRepeat  ,eBTSwitch  } EButtonType;


typedef struct{

	int					iX;
	int					iY;

	int					iTX;
	int					iTY;
		
	TButtonAction		pPushAction;
	TButtonAction		pReleaseAction;
	EButtonState		eState;	
	EButtonType			eType;

	int					iGroup;
	HWND				hWnd;
	
	CBMPResource		*poPressSh;
	int					iPressShCX;
	int					iPressShCY;

    CBMPResource		*poRelSh;
	int					iRelShCX;
	int					iRelShCY;              

	CBMPResource		*poDisSh;
	int					iDisShCX;
	int					iDisShCY;

	CBMPResource		*poHigSh;
	int					iHigShCX;
	int					iHigShCY;


}TButton;

class CBtnMgr
{
	public:	CBtnMgr();
			CBtnMgr(int _iMaxButtons);
			~CBtnMgr();

			void Init(int _iMaxButtons);
			int	 iAddButton(int _iX,int _iY,
							int _iTX,int _iTY,
							CBMPResource *_poPressSh=NULL,		int	_iPressShCX=0,		int _iPressShCY=0,
							CBMPResource *_poRelSh=NULL,	int	_iRelShCX=0,	int _iRelShCY=0,
							CBMPResource *_poHigSh=NULL,	int	_iHigShCX=0,	int _iHigShCY=0,
							CBMPResource *_poDisSh=NULL,	int	_iDisShCX=0,	int _iDisShCY=0,
							EButtonType	 _eType  = eBTOneShot,
							EButtonState _eState = eBSPressed,
							int _iGroup = 0,
							TButtonAction _pPushAction = NULL,
							TButtonAction _pReleaseAction = NULL,
							HWND	_hWnd = 0);
			void Draw();
			
			void MouseDown(int _iX,int _iY);
			void MouseMove(int _iX,int _iY);
			void MouseUp  (int _iX,int _iY);
			void Timer    ();

			void PushButton(int _iButton);

		private:
			TButton	*poButt;
			int		iMax;
			int		iNum;

			int		iHigButt;
};


//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
