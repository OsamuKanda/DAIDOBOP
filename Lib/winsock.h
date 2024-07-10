///////////////////////////////////////////////////////////////////////////////////
//	Windows Socket Initialize Program
//	File Name	�F WinSock.h
//	Contents	�F Windows Socket Initialize Define Header
//				�F
//	Trend		�F2000/01/20 Create ,K.Tanno
//				�F
//				�F
///////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <afxsock.h>

/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
/* Windows Socket Class Define																*/
/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
class CWinSock 
{
	WSADATA		wsaData;
	int			m_Err;	

public:
	~CWinSock();								// Destruct
	BOOL	InitWinSock(BYTE, BYTE);			// Initialize Windows Socket
	int		GetError(void);
};
