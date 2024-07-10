///////////////////////////////////////////////////////////////////////////////////
//	Windows Socket Program
//	File Name	�F Socket.h
//	Contents	�F Windows Socket
//				�F
//	Trend		�F 2000/01/20 Create ,K.Tanno
//				�F
//				�F
///////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <afxsock.h>

// �׽�錾---------------------------------------------------------------------------
class CSocket1
{
private:												
	SOCKET	m_sock;										// socket handle
	int		m_port;										// port
	int		m_Err;										// error code
	
public:
	CSocket1(void);
	~CSocket1(void);
	BOOL	Create(void);								// Create Socket
	void	Create(SOCKET);								// Create Socket
	BOOL	Close(void);								// Close Socket
	BOOL	Bind(LPSTR,int);							// Bind Socket
	BOOL	Bind(DWORD,int);							// Bind Socket
	BOOL	Listen(int);								// Bind Socket
	SOCKET	Accept(void);								// Acceot Socket
	BOOL	Connect(DWORD, int);						// Connect Socket
	BOOL	Recv(LPVOID, int);							// Recv Socket
	BOOL	Send(LPVOID, int);							// Send Socket
	SOCKET	GetSocket(void);							// Get Socket Handle
	int		GetPortNo(void);							// Get Port No
	int		GetError(void);								// Get Error Code
	BOOL	SelectW( DWORD );							// Select for Write
	BOOL	SelectR( DWORD );							// Select for Read
};
