// Plc.h : PLCƒNƒ‰ƒX’è‹`
//

#ifndef _PLC_H
#define _PLC_H

#include "winsock.h"
#include "socket.h"
#include "aj71qe.h"
#include "que.h"

#define MAX_BUFFER	1500
#define	TYPE_WORD	1
#define TYPE_DWORD	2
#define	PLC_INT(x)			(int)(*(int*)(&x))
#define	PLC_SHtoINT(x)		(int)(*(short*)(&x))
#define	PLC_BIT_SH(x,y)		((int)(*(short*)(&x))>>y&0x1)
#define	PLC_BIT_INT(x,y)	((int)(*(int*)(&x))>>y&0x1)
#define	PLC_DWORD(x)		(DWORD)(*(DWORD*)(&x))

/* |||||||||||||||||||||||||||||||||||||||| */
/* PLCƒNƒ‰ƒX’è‹`•”																	*/
/* |||||||||||||||||||||||||||||||||||||||| */
class CPlc 
{
public:
	CSocket1			m_sock;
	CRITICAL_SECTION	m_rwLock;
	DWORD				m_dwIP;
	int					m_nPort;
	BYTE				m_byBufR[MAX_BUFFER];
	BYTE				m_byBufW[MAX_BUFFER];
	int					m_Err;

public:
	CPlc();												// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	~CPlc();											// ƒfƒXƒgƒ‰ƒNƒ^

	BOOL	Create(LPCTSTR, int);						// PLC’ÊMƒNƒ‰ƒXì¬
	BOOL	Create( DWORD dwIP, int nPort );			// PLC’ÊMƒNƒ‰ƒXì¬2
	BOOL	ReConnect(void);							// ÄÚ‘±
	BOOL	WriteW( BYTE, UINT, PWORD, WORD, DWORD);	// ƒ[ƒh‘ž‚Ý
	BOOL	WriteDW( BYTE, UINT, PDWORD, WORD, DWORD);	// ƒ_ƒuƒ‹ƒ[ƒh‘ž‚Ý
	BOOL	ReadW( BYTE, UINT, PWORD, WORD, DWORD);		// ƒ[ƒh“Çž‚Ý
	BOOL	ReadDW( BYTE, UINT, PDWORD, WORD, DWORD);	// ƒ_ƒuƒ‹ƒ[ƒh“Çž‚Ý
	BOOL	ChkConnect( BYTE, UINT, PWORD, DWORD );		// Ú‘±ƒ`ƒFƒbƒN
	int		GetError(void);								// ƒGƒ‰[”Ô†‚ð•Ô‚·

private:

};

#endif
