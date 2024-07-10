/*
	ƒTƒu‹@”\ƒ‰ƒCƒuƒ‰ƒŠ
	
*/

#ifndef SUB_H
#define SUB_H

/* |||||||||||||||||||||||||||||||||||||||| */
/* ƒNƒŠƒeƒBƒJƒ‹ƒƒbƒNƒNƒ‰ƒX’è‹`•”																*/
/* |||||||||||||||||||||||||||||||||||||||| */

class CCriticalLock
{
	CRITICAL_SECTION	*m_Object;
public:
	CCriticalLock(CRITICAL_SECTION *Object)
	{
		m_Object = Object;
		if( m_Object->LockCount )
			EnterCriticalSection(m_Object);
	};
	~CCriticalLock()
	{
		if( m_Object->LockCount )
			LeaveCriticalSection(m_Object);
	};
};

#endif