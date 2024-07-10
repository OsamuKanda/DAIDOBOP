/*
	ƒƒbƒZ[ƒWƒEƒBƒ“ƒhƒE‹@”\ƒ‰ƒCƒuƒ‰ƒŠ
	
*/

#ifndef QUE_H
#define QUE_H

#include <afxtempl.h>
#include "sub.h"


/* |||||||||||||||||||||||||||||||||||||||| */
/* ƒLƒ…[ƒNƒ‰ƒX’è‹`•”																*/
/* |||||||||||||||||||||||||||||||||||||||| */

class CQue
{
	CList<BYTE>			m_list;				// ƒŠƒXƒgƒNƒ‰ƒX
	INT					m_nMax;				// ƒLƒ…[“àƒf[ƒ^Å‘å”
	INT					m_nCount;			// ƒLƒ…[“àƒf[ƒ^”
	CRITICAL_SECTION	m_Critical;			// ƒNƒŠƒeƒBƒJƒ‹ƒZƒNƒVƒ‡ƒ“ƒIƒuƒWƒFƒNƒg

public:
	CQue(INT);								// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	~CQue();								// ƒfƒXƒgƒ‰ƒNƒ^

	UINT	AddData(LPBYTE, INT);
	INT		GetData(LPBYTE, INT);
	INT		GetCount( void );
	void	Reflesh( void );
};

#endif

