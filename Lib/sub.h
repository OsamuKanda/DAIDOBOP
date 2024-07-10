/*
	�T�u�@�\���C�u����
	
*/

#ifndef SUB_H
#define SUB_H

/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
/* �N���e�B�J�����b�N�N���X��`��																*/
/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */

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