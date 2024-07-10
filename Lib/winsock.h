///////////////////////////////////////////////////////////////////////////////////
//	Windows Socket Initialize Program
//	File Name	ÅF WinSock.h
//	Contents	ÅF Windows Socket Initialize Define Header
//				ÅF
//	Trend		ÅF2000/01/20 Create ,K.Tanno
//				ÅF
//				ÅF
///////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <afxsock.h>

/* Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å| */
/* Windows Socket Class Define																*/
/* Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å|Å| */
class CWinSock 
{
	WSADATA		wsaData;
	int			m_Err;	

public:
	~CWinSock();								// Destruct
	BOOL	InitWinSock(BYTE, BYTE);			// Initialize Windows Socket
	int		GetError(void);
};
