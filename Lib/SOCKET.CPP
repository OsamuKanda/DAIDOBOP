///////////////////////////////////////////////////////////////////////////////////
//	Windows Socket Program
//	File Name	： Socket.cpp
//	Contents	： Windows Socket
//				：
//	Trend		： 2000/01/20 Create ,K.Tanno V1.0
//				： 2001/04/16 Update ,K.Tanno V1.1
//				： 2001/04/23 Add	 ,K.Tanno V1.1
//				：
///////////////////////////////////////////////////////////////////////////////////

#include	"socket.h"						



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： CSocket1
//  Contents	： Construct
//  Argument	： ...
//  Return Value： ...
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSocket1::CSocket1(void)
{
	m_sock = INVALID_SOCKET;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： ~CSocket1
//  Contents	： Destruct
//  Argument	： ...
//  Return Value： ...
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSocket1::~CSocket1(void)
{
	// if the socket wasn't closed,
	if( INVALID_SOCKET != m_sock )
		Close();						// close socket
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Create
//  Contents	： Create Socket
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Create(void)
{	
	// create socket
	m_sock = socket(AF_INET,					// address family
					SOCK_STREAM,				// socket type streem
					0);							// protocol(case of AF_INET is zero)

	// check socket
	if(INVALID_SOCKET == m_sock)
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}

	// socket ok
	return TRUE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Create
//  Contents	： Create Socket
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void CSocket1::Create(SOCKET Sock)
{	
	// create socket
	m_sock = Sock;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Close
//  Contents	： Close Socket
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Close(void)
{	
	// if socket wasn't created,
	if( INVALID_SOCKET == m_sock )				
		return TRUE;
	
	// shutdown with no send 
	if(SOCKET_ERROR == shutdown(m_sock,1))
	{
		m_Err = WSAGetLastError();
// V1.1 delete:ここで関数を抜けるとclosesocketされずハンドルリークする
//		return FALSE;									// Error
	}

	// close socket
	if(SOCKET_ERROR == closesocket(m_sock))
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}
	
	m_sock = INVALID_SOCKET;					

	return TRUE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Bind
//  Contents	： Bind Socket
//  Argument	： lpIpAddr - IP Address
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Bind(LPSTR lpIpAddr, int nPort)
{
	SOCKADDR_IN	addr;								// socket information
	int			addrSize = sizeof(addr);

	// Setup Bind Data
	ZeroMemory(&addr,sizeof(addr));					
	addr.sin_family			= AF_INET;				
	addr.sin_port			= htons(nPort);			
	addr.sin_addr.s_addr	= inet_addr(lpIpAddr);	

	// Bind Socket
	if(SOCKET_ERROR == bind(m_sock,						// Socket
							(LPSOCKADDR)&addr,			// Bind Data
							addrSize))					// Bind Data Size
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}

	// Check Port
	if(SOCKET_ERROR == getsockname(	m_sock,				// Socket
									(LPSOCKADDR)&addr,	// Bind Data
									&addrSize))			// Bind Data Size
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}

	// Get Port
	m_port = addr.sin_port;

	return TRUE;										// OK
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Bind
//  Contents	： Bind Socket
//  Argument	： lpIpAddr - IP Address
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Bind(DWORD dwIP, int nPort)
{
	SOCKADDR_IN	addr;								// socket information
	int			addrSize = sizeof(addr);

	// Setup Bind Data
	ZeroMemory(&addr,sizeof(addr));					
	addr.sin_family			= AF_INET;				
	addr.sin_port			= htons(nPort);			
	addr.sin_addr.s_addr	= dwIP;	

	// Bind Socket
	if(SOCKET_ERROR == bind(m_sock,						// Socket
							(LPSOCKADDR)&addr,			// Bind Data
							addrSize))					// Bind Data Size
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}
	// Check Port
	if(SOCKET_ERROR == getsockname(	m_sock,				// Socket
									(LPSOCKADDR)&addr,	// Bind Data
									&addrSize))			// Bind Data Size
	{
		m_Err = WSAGetLastError();	
		return FALSE;									// Error
	}

	// Get Port
	m_port = addr.sin_port;

	return TRUE;										// OK
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Listen
//  Contents	： Listen 
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Listen(int backlog)
{
	// Listen
	if(0 != listen(	m_sock,						// Socket Handle
					backlog))					// Max Connect Count
	{
		m_Err = WSAGetLastError();	
		return FALSE;							// Error
	}

	return TRUE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Accept
//  Contents	： Accept 
//  Argument	： None
//  Return Value： New Socket Handle
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SOCKET CSocket1::Accept(void)
{
	// 内部変数定義
	SOCKADDR_IN From;		// 接続相手のｱﾄﾞﾚｽ情報
	int Len = sizeof(From);	// ｱﾄﾞﾚｽ情報領域のｻｲｽﾞ
	SOCKET	newSock;
	
	// Accept Socket
	if(INVALID_SOCKET == (newSock = accept(	m_sock,				// Socket Handle
											(LPSOCKADDR)&From,	// Accept Information
											&Len)))				// Accept Information Size
	{
		m_Err = WSAGetLastError();	
		return INVALID_SOCKET;									// Error
	}

	return newSock;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Connect
//  Contents	： Connect 
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Connect(DWORD dwIP, int nPort)
{
	SOCKADDR_IN	addr;
												
	ZeroMemory(&addr,sizeof(addr));	
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(nPort);
	addr.sin_addr.s_addr	= dwIP;
	
	if(SOCKET_ERROR == connect(	m_sock,				// Socket Handle
								(LPSOCKADDR)&addr,	// Socket Information
								sizeof(addr)))		// Socket Information Size
	{
		m_Err = WSAGetLastError();	
		return FALSE;								// Error
	}

	return TRUE;									// Ok
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Recv
//  Contents	： Recv 
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Recv(LPVOID lpBuf,int	iLength)
{
	int		iOneRecv;									// Data Length of Once
	int		iSum = 0;									// Data Length of Sum
	LPSTR	lpStr = (LPSTR)lpBuf;						

	do													
	{
		if(SOCKET_ERROR == (iOneRecv = recv(m_sock,			// Socket Handle
											lpStr + iSum,	// Recv Buffer
											iLength - iSum,	// Recv Buffer Size
											0)))			// Flag
		{
			m_Err = WSAGetLastError();	
			return FALSE;									// FALSE
		}
		
		iSum += iOneRecv;									// Add Data Length

	}while(iLength > iSum && iOneRecv != 0);				

	if(0 == iOneRecv)
		return FALSE;

	return TRUE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： Send
//  Contents	： Send 
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::Send(LPVOID	lpBuf,int	iLength)
{
	// 内部変数定義
	int		iOneSend;									// Data Length of Once
	int		iSum = 0;									// Data Length of Sum
	LPSTR	lpStr = (LPSTR)lpBuf;						
	
	do													
	{
		
		if(SOCKET_ERROR == (iOneSend = send(m_sock,			// Socket Handle
											lpStr + iSum,	// Recv Buffer
											iLength - iSum,	// Recv Buffer Size
											0)))			// Flag
		{
			m_Err = WSAGetLastError();	
			return FALSE;									// FALSE
		}
		
		iSum += iOneSend;									// Add Data Length

	}while(iLength > iSum);								

	return TRUE;											// TRUE
}

// V1.1 add:書き込み用Select
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： SelectW
//  Contents	： Select for Write
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::SelectW( DWORD dwTime )
{
    fd_set fds;
    struct timeval tv;

    // SELECTに送信可能確認のフラグをセット
    FD_ZERO(&fds);
    FD_SET(m_sock, &fds);

    // タイムアウト時間の設定
    tv.tv_sec	= dwTime / 1000;
    tv.tv_usec	= dwTime % 1000;

    // 送信前に呼び出す
	if( 0 >= select(1, NULL, &fds, NULL, &tv) )
	{
		m_Err = WSAGetLastError();
		return FALSE;
	}

	return TRUE;
}

// V1.1 add:読み込み用Select
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： SelectR
//  Contents	： Select for Read
//  Argument	： None
//  Return Value： TRUE:OK, FALSE:Error
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CSocket1::SelectR( DWORD dwTime )
{
    fd_set	fds;
    struct	timeval tv;

    // SELECTに送信可能確認のフラグをセット
    FD_ZERO(&fds);
    FD_SET(m_sock, &fds);

    // タイムアウト時間の設定
    tv.tv_sec	= dwTime / 1000;
    tv.tv_usec	= dwTime % 1000;

    // 送信前に呼び出す
	if( 0 >= select(1, &fds, NULL, NULL, &tv) )
	{
		m_Err = WSAGetLastError();
		return FALSE;
	}

	return TRUE;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： GetSocket
//  Contents	： Get Socket Handle
//  Argument	： None
//  Return Value： Socket Handle
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
SOCKET CSocket1::GetSocket(void)
{
	return m_sock;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： GetPortNo
//  Contents	： Get Port No
//  Argument	： None
//  Return Value： Port No
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CSocket1::GetPortNo(void)
{
	return m_port;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Func Name	： GetError
//  Contents	： Get Error Code
//  Argument	： None
//  Return Value： Error Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CSocket1::GetError(void)
{
	return m_Err;
}

