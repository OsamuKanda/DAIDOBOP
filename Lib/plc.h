// Plc.h : PLC�N���X��`
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

/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
/* PLC�N���X��`��																	*/
/* �|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�| */
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
	CPlc();												// �R���X�g���N�^
	~CPlc();											// �f�X�g���N�^

	BOOL	Create(LPCTSTR, int);						// PLC�ʐM�N���X�쐬
	BOOL	Create( DWORD dwIP, int nPort );			// PLC�ʐM�N���X�쐬2
	BOOL	ReConnect(void);							// �Đڑ�
	BOOL	WriteW( BYTE, UINT, PWORD, WORD, DWORD);	// ���[�h������
	BOOL	WriteDW( BYTE, UINT, PDWORD, WORD, DWORD);	// �_�u�����[�h������
	BOOL	ReadW( BYTE, UINT, PWORD, WORD, DWORD);		// ���[�h�Ǎ���
	BOOL	ReadDW( BYTE, UINT, PDWORD, WORD, DWORD);	// �_�u�����[�h�Ǎ���
	BOOL	ChkConnect( BYTE, UINT, PWORD, DWORD );		// �ڑ��`�F�b�N
	int		GetError(void);								// �G���[�ԍ���Ԃ�

private:

};

#endif
