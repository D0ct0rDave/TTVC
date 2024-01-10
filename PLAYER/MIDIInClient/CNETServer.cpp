#include "CNETServer.h"
#include <stdlib.h>

char				CNETServer::szServerName[MAXC_SNAME];
unsigned int		CNETServer::uiServerIP			= 0;
unsigned int		CNETServer::uiServerPort		= 0;
int					CNETServer::iServerSocket		= 0;
bool				CNETServer::bNetInit			= false;
TConnectionCallback	CNETServer::pConnectionCallback = NULL;

void CNETServer::SetConnectionCallback(TConnectionCallback _pCB)
{
	pConnectionCallback = _pCB;
}

void CNETServer::Init(unsigned int _uiServerPort)
{
	uiServerPort = _uiServerPort;
	iServerSocket = CNETSystem::CreateServer(uiServerIP,szServerName,_uiServerPort);
	bNetInit	  = (iServerSocket != -1);
}

void CNETServer::Loop()
{
	if (! bNetInit) return;

	// Look for new posible clients
	int iClientSocket = CNETSystem::AcceptConnections(iServerSocket);

	if (iClientSocket >= 0)
	{
		if (pConnectionCallback)
			pConnectionCallback(iClientSocket);		
	}
}


bool CNETServer::bSendInfo(int _uiID,void *pBuff,unsigned int _uiSize)
{
	if (! bNetInit) return(false);
	return( CNETSystem::WriteToClient(_uiID,pBuff,_uiSize) == _uiSize);
}

bool CNETServer::bGetInfo (int _uiID,void *pBuff,unsigned int &_ruiSize)
{	
	if (! bNetInit) return(false);
	if (! CNETSystem::bPendingInfo(_uiID)) return(false);

	_ruiSize = CNETSystem::ReadFromClient(_uiID,pBuff,_ruiSize);
	return( _ruiSize > 0 );
}
