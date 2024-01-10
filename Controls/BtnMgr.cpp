//------------------------------------------------------------------------------
#include "BtnMgr.h"
//------------------------------------------------------------------------------
CBtnMgr::CBtnMgr()
{
	memset(this,0,sizeof(CBtnMgr));
}
//------------------------------------------------------------------------------
CBtnMgr::CBtnMgr(int _iMaxButtons)
{
	Init(_iMaxButtons);
}
//------------------------------------------------------------------------------
CBtnMgr::~CBtnMgr()
{
	if (poButt) delete []poButt;
}
//------------------------------------------------------------------------------
void CBtnMgr::Init(int _iMaxButtons)
{
	if (poButt)
	{
		delete []poButt;
		poButt = NULL;
	}
	
	iMax = _iMaxButtons;
	poButt = new TButton[_iMaxButtons];
	iNum = 0;
	iHigButt = -1;
}
//------------------------------------------------------------------------------
int	 CBtnMgr::iAddButton(int _iX,int _iY,
						 int _iTX,int _iTY,
						 CBMPResource *_poPressSh,	int	_iPressShCX,int _iPressShCY,
						 CBMPResource *_poRelSh,	int	_iRelShCX,	int _iRelShCY,
						 CBMPResource *_poHigSh,	int	_iHigShCX,	int _iHigShCY,
						 CBMPResource *_poDisSh,	int	_iDisShCX,	int _iDisShCY,
						 EButtonType  _eType,
						 EButtonState _eState,
						 int _iGroup,
						 TButtonAction _pPushAction,
						 TButtonAction _pReleaseAction,
						 HWND	_hWnd)
{
	TButton *poB = poButt + iNum;
	
	poB->iX  = _iX;
	poB->iY  = _iY;
	poB->iTX = _iTX;
	poB->iTY = _iTY;

	poB->poPressSh  = _poPressSh;
	poB->iPressShCX = _iPressShCX;
	poB->iPressShCY = _iPressShCY;

	poB->poRelSh  = _poRelSh;
	poB->iRelShCX = _iRelShCX;
	poB->iRelShCY = _iRelShCY;

	poB->poHigSh  = _poHigSh;
	poB->iHigShCX = _iHigShCX;
	poB->iHigShCY = _iHigShCY;

	poB->poDisSh  = _poDisSh;
	poB->iDisShCX = _iDisShCX;
	poB->iDisShCY = _iDisShCY;

	poB->eType	= _eType;
	poB->eState = _eState;
	poB->iGroup = _iGroup;
	poB->pPushAction = _pPushAction;
	poB->pReleaseAction = _pReleaseAction;
	poB->hWnd = _hWnd;
	
	iNum++;
	return(iNum-1);
}
//------------------------------------------------------------------------------
void CBtnMgr::Draw()
{
	for (int i=0;i<iNum;i++)
	{
		TButton *poB = poButt + i;
		HDC hDC = GetDC(poB->hWnd);

		switch (poB->eState)
		{
			case eBSPressed:
			BitBlt(hDC,poB->iX,poB->iY,poB->iTX,poB->iTY,poB->poPressSh->GetDC(),poB->iPressShCX,poB->iPressShCY,SRCCOPY);
			break;

			case eBSReleased:
			if ((poB->poHigSh) && (iHigButt == i))
				BitBlt(hDC,poB->iX,poB->iY,poB->iTX,poB->iTY,poB->poHigSh->GetDC(),poB->iHigShCX,poB->iHigShCY,SRCCOPY);
			else
				BitBlt(hDC,poB->iX,poB->iY,poB->iTX,poB->iTY,poB->poRelSh->GetDC(),poB->iRelShCX,poB->iRelShCY,SRCCOPY);
			break;

			case eBSDisabled:
			if (poB->poDisSh)
				BitBlt(hDC,poB->iX,poB->iY,poB->iTX,poB->iTY,poB->poDisSh->GetDC(),poB->iDisShCX,poB->iDisShCY,SRCCOPY);
			else
				BitBlt(hDC,poB->iX,poB->iY,poB->iTX,poB->iTY,poB->poRelSh->GetDC(),poB->iRelShCX,poB->iRelShCY,SRCCOPY);
			break;
		}

		ReleaseDC(poB->hWnd,hDC);
	}
}
//------------------------------------------------------------------------------
inline bool bMouseOver(int _iX,int _iY,int _iBX,int _iBY,int _iBTX,int _iBTY)
{
	return ( (_iX>=_iBX) && (_iY>=_iBY) && (_iX<=(_iBX+_iBTX)) && (_iY<=(_iBY+_iBTY)) );
}
//------------------------------------------------------------------------------
void RefreshButton(TButton *_poButt)
{
	RECT Area;

	// Update old highlighted area
	Area.left  = _poButt->iX;
	Area.right = _poButt->iX+_poButt->iTX;
	Area.top   = _poButt->iY;
	Area.bottom= _poButt->iY+_poButt->iTY;

	InvalidateRect(_poButt->hWnd,&Area,false);
}
//------------------------------------------------------------------------------
void CBtnMgr::MouseDown(int _iX,int _iY)
{
	for (int i=0;i<iNum;i++)
	{
		TButton *poB = poButt + i;
		if (poB->eState != eBSDisabled)
		{
			if (bMouseOver(_iX,_iY,poB->iX,poB->iY,poB->iTX,poB->iTY))
			{
				if (poB->eState == eBSReleased)
				{	
					if (poB->iGroup)
					{
						for (int j=0;j<iNum;j++)
						{
							if (i == j) continue;

							if ((poButt[j].iGroup == poB->iGroup) && (poButt[j].eState == eBSPressed))
							{
								poButt[j].eState = eBSReleased;
								if (poButt[j].pReleaseAction) poButt[j].pReleaseAction();
								RefreshButton(&poButt[j]);
							}
						}
					}

					poB->eState = eBSPressed;	
					if (poB->pPushAction) poB->pPushAction();
					RefreshButton(poB);
				}
				else
				{
					if ((poB->eState == eBSPressed) && (poB->eType == eBTSwitch) && (poB->iGroup == 0))
					{
						poB->eState = eBSReleased;
						if (poB->pReleaseAction) poB->pReleaseAction();					
						RefreshButton(poB);
					}
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void CBtnMgr::MouseMove(int _iX,int _iY)
{
	int iNewHigButt = -1;

	for (int i=0;i<iNum;i++)
	{
		TButton *poB = poButt + i;

		if (poB->eState == eBSReleased)
			if (bMouseOver(_iX,_iY,poB->iX,poB->iY,poB->iTX,poB->iTY))
			{
				iNewHigButt = i;
				break;
			}
	}

	if (iNewHigButt != iHigButt)
	{
		// Update old highlighted area		
		if (iHigButt != -1)
			RefreshButton(&poButt[iHigButt]);


		iHigButt = iNewHigButt;

		// Update new highlighted area
		if (iHigButt != -1)
			RefreshButton(&poButt[iHigButt]);
	}
}
//------------------------------------------------------------------------------
void CBtnMgr::MouseUp  (int _iX,int _iY)
{
	for (int i=0;i<iNum;i++)
	{
		TButton *poB = poButt + i;

		if ((poB->eState == eBSPressed) && (poB->eType != eBTSwitch))
		{
			poB->eState = eBSReleased;
			if (poB->pReleaseAction) poB->pReleaseAction();
			RefreshButton(poB);
		}
	}
}
//------------------------------------------------------------------------------
void CBtnMgr::Timer()
{
	for (int i=0;i<iNum;i++)
	{
		TButton *poB = poButt + i;
	
		if (poB->eState == eBSPressed)
		{
			switch (poB->eType)
			{
				case eBTOneShot:
					poB->eState = eBSReleased;
					RefreshButton(poB);
					if (poB->pReleaseAction) poB->pReleaseAction();
				break;
				
				case eBTRepeat:
					if (poB->pPushAction) poB->pPushAction();
				break;
			}
		}
	}		
}

void CBtnMgr::PushButton(int _iButton)
{
	MouseDown(poButt[_iButton].iX,poButt[_iButton].iY);
	MouseUp  (poButt[_iButton].iX,poButt[_iButton].iY);
}
//------------------------------------------------------------------------------