#include "CNETClient.h"
#include "CNETSystem.h"

CNETClient::CNETClient()
{
	iState = CLST_DISCONNECTED;
}

CNETClient::~CNETClient()
{
}

bool CNETClient::bConnect(unsigned int _uiServerIP,unsigned int _uiServerPort)
{
	// CNETSystem::GetHostName(
	if ( (iClientSocket = CNETSystem::ConnectToServer(_uiServerIP,_uiServerPort)) > -1) 
	{
		iState = CLST_CONNECTED;
		return(true);
	}
	else
		return(false);
}

bool CNETClient::bConnect(char *_szServerName,unsigned int _uiServerPort)
{
	if ( (iClientSocket = CNETSystem::ConnectToServer(_szServerName,_uiServerPort)) > -1)		
	{
		iState = CLST_CONNECTED;
		return(true);
	}
	else
		return(false);
}

bool CNETClient::bSendInfo (void *_pBuff,int _iSize)
{
	return( CNETSystem::WriteToServer( iClientSocket,_pBuff,_iSize) == _iSize);
}

bool CNETClient::bGetInfo(void *_pBuff,int &_riSize)
{
	if (! CNETSystem::bPendingInfo(iClientSocket)) return(false);
	_riSize = CNETSystem::ReadFromServer(iClientSocket,_pBuff,_riSize);
	return(_riSize > 0);
}
