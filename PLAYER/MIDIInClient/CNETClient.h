#ifndef CNETClientH
#define CNETClientH

#define	CLST_DISCONNECTED	0
#define	CLST_CONNECTED		1
#define	CLST_REGISTERED		2

class CNETClient
{
	public:	 CNETClient();
			~CNETClient();

			bool	bConnect (unsigned int _uiServerIP,unsigned int _uiServerPort);
			bool	bConnect (char *_szServerName,unsigned int _uiServerPort);

			bool	bGetInfo (void *_pBuff,int &_riSize);
			bool	bSendInfo(void *_pBuff,int _iSize);			
			int		iGetState();

	protected:
			int				iState;
			int				iClientSocket;
};

inline CNETClient::iGetState()
{
	return(iState);
}
#endif