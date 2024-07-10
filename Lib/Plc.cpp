// Plc.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//
#include "plc.h"
#include <atlstr.h>

//**************************************************
//  �֐���	�F CPlc�i�R���X�g���N�^�j
//  ���e	�F CPlc�����o�������Ȃ�
//  ����	�F ...
//  �߂�l	�F ...
//**************************************************
CPlc::CPlc()
{
	// �N���e�B�J���Z�N�V�����I�u�W�F�N�g����
	InitializeCriticalSection(&m_rwLock);
}

//**************************************************
//  �֐���	�F ~CPlc�i�f�X�g���N�^�j
//  ���e	�F CPlc�I������
//  ����	�F ...
//  �߂�l	�F ...
//**************************************************
CPlc::~CPlc()
{
	m_sock.Close();

	// �N���e�B�J���Z�N�V�����I�u�W�F�N�g�폜
	DeleteCriticalSection(&m_rwLock);
}

//**************************************************
//  �֐���	�F Create
//  ���e	�F Create
//  ����	�F ...
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::Create( LPCTSTR lpIP, int nPort )
{
	CString		str = lpIP;
	CStringA	strA = str;

	m_dwIP		= (DWORD)inet_addr(strA);
	m_nPort		= nPort;

	return TRUE;
}

//**************************************************
//  �֐���	�F Create
//  ���e	�F Create
//  ����	�F ...
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::Create( DWORD dwIP, int nPort )
{
	m_dwIP		= (DWORD)dwIP;
	m_nPort		= nPort;

	return TRUE;
}

//**************************************************
//  �֐���	�F ReConnect
//  ���e	�F ReConnect
//  ����	�F ...
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::ReConnect( void )
{
	m_sock.Close();									// �\�P�b�g�ؒf

	if( FALSE == m_sock.Create() )					// �\�P�b�g�쐬
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	if( FALSE == m_sock.Connect( m_dwIP, m_nPort) )	// �Đڑ�
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//  �֐���	�F WriteW
//  ���e	�F ���[�h�ꊇ��������
//  ����	�F PBYTE nDev		-	�f�o�C�X�R�[�h
//			�F UINT	uiDevNo		-	�f�o�C�X�m�n
//			�F PWORD pwData		-	�f�[�^
//			�F WORD wCnt		-	��
//			�F DWORD dwTime		-	�^�C���A�E�g����(ms)
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::WriteW( BYTE nDev, UINT uiDevNo, PWORD pwData, WORD wCnt, DWORD dwTime)
{
	int nSize;

	// �N���e�B�J���Z�N�V�����ɂ��r������(�C���X�^���X�̏��Ŏ��Ɏ����A�����b�N�j
	CCriticalLock	lock(&m_rwLock);

	// ����M�G���A���N���A
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// �R�}���h���b�Z�[�W�Ƀw�b�_���Z�b�g
	nSize = SetWriteCmdW(m_byBufW, nDev, uiDevNo, wCnt);

	// �R�}���h���b�Z�[�W�Ƀf�[�^���Z�b�g
	for( int i = 0; i < wCnt ; i++ )
		PcToPlc(pwData[i], &m_byBufW[POS_DATA_CMD + i * 2], 2);

	// �T�b��M�҂�
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC�ɃR�}���h���b�Z�[�W���M
	if( FALSE == m_sock.Send( m_byBufW, nSize) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ��M�҂�
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC���烌�X�|���X��M
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ���X�|���X�f�[�^���`�F�b�N
	if( 0 > GetWriteResW( m_byBufR ) )
	{
		m_Err = -1;
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//  �֐���	�F WriteDW
//  ���e	�F �_�u�����[�h�ꊇ��������
//  ����	�F PBYTE nDev		-	�f�o�C�X�R�[�h
//			�F UINT	uiDevNo		-	�f�o�C�X�m�n
//			�F PWORD pwData		-	�f�[�^
//			�F WORD wCnt		-	��
//			�F DWORD dwTime		-	�^�C���A�E�g����(ms)
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::WriteDW( BYTE nDev, UINT uiDevNo, PDWORD pdwData, WORD wCnt, DWORD dwTime)
{
	int nSize;

	// �N���e�B�J���Z�N�V�����ɂ��r������(�C���X�^���X�̏��Ŏ��Ɏ����A�����b�N�j
	CCriticalLock	lock(&m_rwLock);

	// ����M�G���A���N���A
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// �R�}���h���b�Z�[�W�Ƀw�b�_���Z�b�g
	nSize = SetWriteCmdW(m_byBufW, nDev, uiDevNo, wCnt * 2);

	// �R�}���h���b�Z�[�W�Ƀf�[�^���Z�b�g
	for( int i = 0; i < wCnt ; i++ )
		PcToPlc(pdwData[i], &m_byBufW[POS_DATA_CMD + i * 4], 4);

	// �T�b��M�҂�
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC�ɃR�}���h���b�Z�[�W���M
	if( FALSE == m_sock.Send( m_byBufW, nSize) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ��M�҂�
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC���烌�X�|���X��M
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ���X�|���X�f�[�^���`�F�b�N
	if( 0 > GetWriteResW( m_byBufR ) )
	{
		m_Err = -2;
		return FALSE;
	}

	return TRUE;

}

//**************************************************
//  �֐���	�F ReadW
//  ���e	�F ���[�h�ꊇ�Ǎ���
//  ����	�F PBYTE nDev		-	�f�o�C�X�R�[�h
//			�F UINT	uiDevNo		-	�f�o�C�X�m�n
//			�F PWORD pwData		-	�f�[�^
//			�F WORD wCnt		-	��
//			�F DWORD dwTime		-	�^�C���A�E�g����(ms)
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::ReadW( BYTE nDev, UINT uiDevNo, PWORD pwData, WORD wCnt, DWORD dwTime)
{
	int nDataPos;

	// �N���e�B�J���Z�N�V�����ɂ��r������(�C���X�^���X�̏��Ŏ��Ɏ����A�����b�N�j
	CCriticalLock	lock(&m_rwLock);

	// ����M�G���A���N���A
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// �R�}���h���b�Z�[�W�Ƀw�b�_���Z�b�g
	nDataPos = SetReadCmdW(m_byBufW, nDev, uiDevNo, wCnt);

	// �T�b��M�҂�
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC�ɃR�}���h���b�Z�[�W���M
	if( FALSE == m_sock.Send( m_byBufW, POS_DATA_CMD) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// �T�b��M�҂�
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC���烌�X�|���X��M
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES + wCnt * sizeof(WORD)) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ���X�|���X�f�[�^���`�F�b�N
	if( 0 > ( nDataPos = GetReadResW( m_byBufR , wCnt * sizeof(WORD) ) ) )
	{
		m_Err = -3;
		return FALSE;
	}

	// ��M�f�[�^���擾
	for( int i = 0; i < wCnt ; i++ )
//		PcToPlc(pwData[i], &m_byBufR[nDataPos + i * 2], 2);
		pwData[i] = PlcToPc( &m_byBufR[nDataPos + i * 2], 2);

	return TRUE;
}

//**************************************************
//  �֐���	�F ReadDW
//  ���e	�F ���[�h�ꊇ�Ǎ���
//  ����	�F PBYTE nDev		-	�f�o�C�X�R�[�h
//			�F UINT	uiDevNo		-	�f�o�C�X�m�n
//			�F PWORD pwData		-	�f�[�^
//			�F WORD wCnt		-	��
//			�F DWORD dwTime		-	�^�C���A�E�g����(ms)
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::ReadDW( BYTE nDev, UINT uiDevNo, PDWORD pdwData, WORD wCnt, DWORD dwTime)
{
	int nDataPos;

	// �N���e�B�J���Z�N�V�����ɂ��r������(�C���X�^���X�̏��Ŏ��Ɏ����A�����b�N�j
	CCriticalLock	lock(&m_rwLock);

	// ����M�G���A���N���A
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// �R�}���h���b�Z�[�W�Ƀw�b�_���Z�b�g
	nDataPos = SetReadCmdW(m_byBufW, nDev, uiDevNo, wCnt * 2);

	// ���M�҂�
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC�ɃR�}���h���b�Z�[�W���M
	if( FALSE == m_sock.Send( m_byBufW, POS_DATA_CMD) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ��M�҂�
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLC���烌�X�|���X��M
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES + wCnt * sizeof(DWORD)) )
		return FALSE;

	// ���X�|���X�f�[�^���`�F�b�N
	if( 0 > ( nDataPos = GetReadResW( m_byBufR, wCnt * sizeof(DWORD) ) ) )
	{
		m_Err = -4;
		return FALSE;
	}

	// ��M�f�[�^���擾
	for( int i = 0; i < wCnt ; i++ )
//		PcToPlc(pdwData[i], &m_byBufR[nDataPos + i * 4], 4);
		pdwData[i] = PlcToPc( &m_byBufR[nDataPos + i * 4], 4);

	return TRUE;
}

//**************************************************
//  �֐���	�F ChkConnect
//  ���e	�F �R�l�N�V�����`�F�b�N
//  ����	�F DWORD dwTime		-	�^�C���A�E�g����(ms)
//  �߂�l	�F ...
//**************************************************
BOOL CPlc::ChkConnect( BYTE nDev, UINT uiDevNo, PWORD pwData, DWORD dwTime )
{
	// �R�l�N�V�������픻��i�w���V�[���M�j

	if( FALSE == WriteW( nDev, uiDevNo, pwData, 1, dwTime) )
	{
		// �ُ�
		m_Err = m_sock.GetError();
		return FALSE;
	}
	return TRUE;
}

//**************************************************
//  �֐���	�F GetError
//  ���e	�F �G���[�ԍ���Ԃ�
//  ����	�F �Ȃ�
//  �߂�l	�F �G���[�ԍ�
//**************************************************
int CPlc::GetError(void)
{
	return m_Err;
}

