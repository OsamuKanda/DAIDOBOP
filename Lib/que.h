/*
	���b�Z�[�W�E�B���h�E�@�\���C�u����
	
*/

#ifndef QUE_H
#define QUE_H

#include <afxtempl.h>
#include "sub.h"


/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
/* �L���[�N���X��`��																*/
/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */

class CQue
{
	CList<BYTE>			m_list;				// ���X�g�N���X
	INT					m_nMax;				// �L���[���f�[�^�ő吔
	INT					m_nCount;			// �L���[���f�[�^��
	CRITICAL_SECTION	m_Critical;			// �N���e�B�J���Z�N�V�����I�u�W�F�N�g

public:
	CQue(INT);								// �R���X�g���N�^
	~CQue();								// �f�X�g���N�^

	UINT	AddData(LPBYTE, INT);
	INT		GetData(LPBYTE, INT);
	INT		GetCount( void );
	void	Reflesh( void );
};

#endif

