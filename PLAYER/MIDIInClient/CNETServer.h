#ifndef CNETServerH
#define CNETServerH

#include "CNETSystem.h"

typedef void (* TConnectionCallback)(int _iClientSocket);

class CNETServer
{
	public:	 CNETServer() {};
			~CNETServer() {};

			static void Init(unsigned int _uiServerPort);
			static void Loop();
			static bool bSendInfo(int _uiID,void *pBuff,unsigned int _uiSize);
			static bool bGetInfo (int _uiID,void *pBuff,unsigned int &_ruiSize);
			static void SetConnectionCallback(TConnectionCallback _pCB);

	protected:

			static bool					bNetInit;			
			static char					szServerName[MAXC_SNAME];
			static unsigned int			uiServerIP;
			static unsigned int			uiServerPort;
			static int					iServerSocket;
			static TConnectionCallback	pConnectionCallback;
};

#endif