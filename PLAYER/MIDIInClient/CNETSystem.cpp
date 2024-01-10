// ----------------------------------------------------------------------------
#include "CNETSystem.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#endif
// ----------------------------------------------------------------------------

#define CONID (('C' << 24) | ('O' << 16) | ('N' << 8) | ('N' << 0))

// ----------------------------------------------------------------------------
#define SOCK_CONNTYPE SOCK_DGRAM
#define SOCK_PROTO	  IPPROTO_UDP
// ----------------------------------------------------------------------------
//#define SOCK_CONNTYPE SOCK_STREAM
//#define SOCK_PROTO	  IPPROTO_TCP

inline void Block(int _iSocket)
{
	unsigned long pars = 0;
	ioctlsocket(_iSocket,FIONBIO,&pars);
}

inline void Unblock(int _iSocket)
{
	unsigned long pars = 1;
	ioctlsocket(_iSocket,FIONBIO,&pars);
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
typedef struct{
	
	int				iSocket;
	SOCKADDR_IN		InAddr;
	bool			bValid;

}TClientData;

#define MAX_CLIENTS 256
TClientData goCD[MAX_CLIENTS];

TClientData	goClient;
// ----------------------------------------------------------------------------
int iInsertClient(SOCKADDR_IN *_pInAddr,int _iSocket)
{
	for (int i=0;i<MAX_CLIENTS;i++)
	{
		if (! goCD[i].bValid)
		{			
			goCD[i].iSocket= _iSocket;
			goCD[i].bValid	= true;
			goCD[i].InAddr	= *_pInAddr;
			return(i);
		}
	}

	return(-1);
}
// ----------------------------------------------------------------------------
inline bool INTPendingInfo(unsigned int _iSocket,SOCKADDR_IN *_pInAddr)
{
	int		iSize;
	int		iRes;
	char	cBuffer[MAX_PACKET_SIZE];
	int		iAddrSize = sizeof(SOCKADDR); 

	iRes = recvfrom(
					_iSocket,				// the socket
					(char *)cBuffer,		// data buffer
					MAX_PACKET_SIZE,		// size of the data
					MSG_PEEK,				// flags
					(LPSOCKADDR)_pInAddr,	// addr to send
					&iAddrSize				// sizeof addr structure
					);
	
	if (iRes == SOCKET_ERROR)
	{
		int i = WSAGetLastError();	
	}

	return(iRes>0);
}
// ----------------------------------------------------------------------------
inline int INTSend(TClientData  *_pCD,void *_pBuf,int _iSize)
{
	int iRes = sendto(
					_pCD->iSocket,				// the socket
					(char *)_pBuf,				// data buffer
					_iSize,						// size of the data
					0,							// flags
					(LPSOCKADDR)&_pCD->InAddr,	// addr to send
					sizeof(struct sockaddr)		// sizeof addr structure
					);
					
						
	if (iRes == SOCKET_ERROR)
		int i = WSAGetLastError();	
	
	return(iRes);
}
// ----------------------------------------------------------------------------
inline int INTRecv(TClientData  *_pCD,void *_pBuf,int _iSize)
{
	int iAddrSize = sizeof(SOCKADDR);

	int iRes = recvfrom(
					_pCD->iSocket,				// the socket
					(char *)_pBuf,				// data buffer
					_iSize,						// size of the data
					0,							// flags
					(LPSOCKADDR)&_pCD->InAddr,	// addr to send
					&iAddrSize					// sizeof addr structure
					);

	if (iRes == SOCKET_ERROR)
		int i = WSAGetLastError();	
	
	return(iRes);
}
// ----------------------------------------------------------------------------
void CNETSystem::Init()
{
	// Init socket system: This call should be placed in networking module not here
	#ifdef WIN32
	WSADATA		WSAData;
    LPWSADATA	lpWSAData = &WSAData;

	WSAStartup(2,lpWSAData);
	
	// reset client data structure
	memset(goCD,0,sizeof(TClientData)*MAX_CLIENTS);
	#endif
}
// ----------------------------------------------------------------------------
void CNETSystem::Finish()
{
	#ifdef WIN32
	WSACleanup ();
	#endif
}
// ----------------------------------------------------------------------------
// Usado por el servidor
int CNETSystem::CreateServer(unsigned int &_uiServerIP,char *_szServerName,unsigned int _uiPort)
{
	// ---------------------------------------
	// Server initialization
	// ---------------------------------------
	int					iServerSocket;
	struct sockaddr_in	indir;
	int					i;

	// create a socket
	if ( (iServerSocket=socket(AF_INET,SOCK_CONNTYPE, SOCK_PROTO)) <0)
		return(-1);	
	
	// set socket to nonblocking mode
	unsigned long pars = 1;
	Unblock(iServerSocket);
	
	GetHostName(_szServerName);
	_uiServerIP = uiGetHostByName(_szServerName);

	// fill internet socket structure
	indir.sin_port        = htons(_uiPort);
	indir.sin_addr.s_addr = INADDR_ANY; // _uiServerIP;
	indir.sin_family      = AF_INET;
	for(i=0;i<8;i++) indir.sin_zero[i] = 0;
	
	// link socket with file descriptor
	if ( (i=bind(iServerSocket,(struct sockaddr *) &indir,sizeof(indir))) <0)
	{
		closesocket(iServerSocket);
		return(-1);
	}

	return(iServerSocket);
}
// ----------------------------------------------------------------------------
int CNETSystem::AcceptConnections(int _iServerSocket)
{
	// Look for client requests
	SOCKADDR_IN		sClDir;

	if(! INTPendingInfo(_iServerSocket,&sClDir) )
		return(-1);

	int			_CONID;
	TClientData oCD;
		
	oCD.InAddr   = sClDir;
	oCD.iSocket = _iServerSocket;
	oCD.bValid   = true;

	INTRecv(&oCD,&_CONID,4);
	if (_CONID != CONID) 
		return(-1);

	// Insert new client
	return( iInsertClient(&sClDir,_iServerSocket) );
}
// ----------------------------------------------------------------------------
void CNETSystem::GetHostName(char *_szHostName)
{
	gethostname(_szHostName,MAXC_SNAME);
}
// ----------------------------------------------------------------------------
unsigned int CNETSystem::uiGetHostByName(char *_szHostName)
{	
	struct hostent		*st;

	if ( (st=gethostbyname(_szHostName)) == 0) return(-1);
	return ( *(unsigned int*)(st->h_addr_list[0]) );
}
// ----------------------------------------------------------------------------
// Usado por el servidor
int CNETSystem::WriteToClient(int _iClSocket,void *_pBuff,int _iSize)
{
	int iRes;

	// Block(_iClSocket);
		iRes = INTSend(&goCD[_iClSocket],(char *)&_iSize,4);
		iRes = INTSend(&goCD[_iClSocket],(char *)_pBuff ,_iSize);
	// Unblock(_iClSocket);
	return ( iRes );
}
// ----------------------------------------------------------------------------
int CNETSystem::ReadFromClient(int _iClSocket,void *_pBuff,int _iSize)
{
	int iSize,iRes;
	
	Block(_iClSocket);
	iRes = INTRecv(&goCD[_iClSocket],(char *)&iSize,4);
	iRes = INTRecv(&goCD[_iClSocket],(char *)_pBuff,iSize);
	Unblock(_iClSocket);

	return(iRes);
}
// ----------------------------------------------------------------------------
bool CNETSystem::bPendingInfo(int _iClSocket)
{
	if (_iClSocket != (int)&goClient) return(false);
	return( INTPendingInfo(goClient.iSocket,&goClient.InAddr) );
}

// ----------------------------------------------------------------------------
int CNETSystem::ConnectToServer(char *_szServerName,unsigned int _uiPort)
{
	unsigned int uiServerIP = uiGetHostByName(_szServerName);
	if (! uiServerIP) return(-1);
	return ( ConnectToServer(uiServerIP,_uiPort) );
}
// ----------------------------------------------------------------------------
int CNETSystem::ConnectToServer(unsigned int _uiServerIP,unsigned int _uiPort)
{
	int	iClientSocket;
	if ( (iClientSocket= socket(AF_INET,SOCK_CONNTYPE, SOCK_PROTO)) <0)
		return(-1);
	
	// set socket to nonblocking mode
	Unblock(iClientSocket);

	#ifdef _DEBUG 
	#ifdef WIN32
	int i = WSAGetLastError();
	#endif
	#endif

	goClient.bValid					= true;
	goClient.iSocket				= iClientSocket;

	goClient.InAddr.sin_family		= AF_INET;
	goClient.InAddr.sin_port		= htons(_uiPort);
	goClient.InAddr.sin_addr.s_addr	= _uiServerIP;
	for(int j=0;j<8;j++)
		goClient.InAddr.sin_zero[j]=0;

	unsigned int _CONID = CONID;
	INTSend(&goClient,&_CONID,4);

	return ( (int)&goClient );
}
// ----------------------------------------------------------------------------
int CNETSystem::WriteToServer (int _iClSocket,void *_pBuff,int _iSize)
{
	if (_iClSocket != (int)&goClient) return(-1);

	int iRes;
	int nFromLen;

	// Block(_iClSocket);

		iRes = INTSend(&goClient,(char *)&_iSize,4);
		iRes = INTSend(&goClient,(char *)_pBuff ,_iSize);

	// Unblock(_iClSocket);
	return ( iRes );
}
// ----------------------------------------------------------------------------
int CNETSystem::ReadFromServer(int _iClSocket,void *_pBuff,int _iSize)
{
	int iSize,iRes;
	
	Block(goClient.iSocket);

	iRes = INTRecv(&goClient,(char *)&_iSize,4);
	iRes = INTRecv(&goClient,(char *)_pBuff ,_iSize);

	Unblock(goClient.iSocket);

	return(iRes);

}
// ----------------------------------------------------------------------------
void CNETSystem::Disconnect(int _iClSocket)
{
	closesocket( goClient.iSocket );
}
// ----------------------------------------------------------------------------

