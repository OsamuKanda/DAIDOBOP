/*=============================================================================
  �t�@�C����    : CMultiTimer.h
  ����			: �}���`���f�B�A�^�C�}�N���X(�p�����l�����Ă܂���)
===============================================================================
 history |   date     |     name     |              comment
---------+------------+--------------+-----------------------------------------
		 | 2002.03.XX |  K.Tanno     | Create.
=============================================================================*/
#ifndef	_INCCMULTITIMER_H
#define _INCCMULTITIMER_H


#include <mmsystem.h>


// -----------------
// �N���X�錾
// -----------------
class CMultiTimer
{
public:	
	CMultiTimer();									// �R���X�g���N�^
	~CMultiTimer();									// �f�X�g���N�^

private:
	INT		m_ID;									// �^�C�}�[ID
	INT		m_uiTime;								// �^�C�}�[����
	
public:
	BOOL SetTimer(UINT, LPTIMECALLBACK, LPVOID);	// �^�C�}�[�N��
	BOOL KillTimer();								// �^�C�}�[�I��
};

#endif // _INCCMULTITIMER_H

