// ----------------------------------------------------------------------------
#ifndef CNETSystemH
#define CNETSystemH

// ----------------------------------------------------------------------------
#define MAXC_SNAME		256
#define MAX_CONNECTIONS	128
#define MAX_PACKET_SIZE	65536
// ----------------------------------------------------------------------------

class CNETSystem
{
	public:	 CNETSystem() {};
			~CNETSystem() {};

			static void Init();
			static void Finish();

			// Usado por el servidor
			static int CreateServer		(unsigned int &_uiServerIP,char *_szServerName,unsigned int _uiPort);
			static int WriteToClient	(int _iClSocket,void *_pBuff,int _iSize);
			static int ReadFromClient	(int _iClSocket,void *_pBuff,int _iSize);
			static int AcceptConnections(int _iServerSocket);
			static bool bPendingInfo	(int _iClSocket);

			// Usado por el cliente
			static int  ConnectToServer	(char *_szServerName,unsigned int _uiPort);
			static int  ConnectToServer	(unsigned int _uiServerIP,unsigned int _uiPort);
			static void Disconnect		(int _iClSocket);
			static int  WriteToServer	(int _iClSocket,void *_pBuff,int _iSize);
			static int  ReadFromServer	(int _iClSocket,void *_pBuff,int _iSize);

			// Misc
			static  void GetHostName(char *_szHostName);
			static  unsigned int uiGetHostByName(char *_szHostName);
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------