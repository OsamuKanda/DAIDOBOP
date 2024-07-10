#include <windows.h>
#include "MtiTime.h"

// -----------------
// ���\�b�h�錾
// -----------------

// �R���X�g���N�^
CMultiTimer::CMultiTimer()
{
	m_ID		= 0;		// �^�C�}�[ID�̏�����
	m_uiTime	= 0;		// �^�C�}�[����
}

// �f�X�g���N�^
CMultiTimer::~CMultiTimer()
{
	// �^�C�}�[���N������Ă���Ȃ�
	if(m_ID)
	{
		// �f�o�C�X���N���[�Y����
		KillTimer();
		m_ID = 0;
	}
}

// ----------------------------------------------------------------------------
//	�֐���	�FSetTimer
//	���e	�F�^�C�}�[���N������
//	����	�FUINT	uiTime			�|	�^�C�}�[����(ms)
//			�FLPTIMECALLBACK lptc	�|	�^�C�}�[�R�[���o�b�N�֐��A�h���X
//			�FLPVOID lpArg			�|	�^�C�}�[�֐��ւ̈���
//	�߂�l	�FTRUE�F����
//			�FFALSE�F�ُ�
// ----------------------------------------------------------------------------
BOOL CMultiTimer::SetTimer(UINT uiTime, LPTIMECALLBACK lptc, LPVOID lpArg)
{
	TIMECAPS tc;

	// �^�C�}�[�f�o�C�X�̑��݃`�F�b�N
	if( timeGetDevCaps( &tc, sizeof( TIMECAPS ) ) == TIMERR_NOCANDO ) 
	{
		return FALSE;
	}

	// �^�C�}�[�l�̃`�F�b�N
	if ( tc.wPeriodMin > uiTime || tc.wPeriodMax < uiTime ) 
	{
		return FALSE;
	}

	// �ŏ��^�C�}�𑜓x��ݒ肵�܂�
	if( TIMERR_NOERROR != timeBeginPeriod(uiTime) )
	{
		return FALSE;
	}
	m_uiTime = uiTime;

	// �^�C�}�[�C�x���g���N������
	m_ID = timeSetEvent(uiTime, 1, lptc, (DWORD)lpArg, TIME_PERIODIC);

	// �^�C�}�[�̋N�����ُ�Ȃ�
	if(!m_ID)
	{
		return FALSE;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
//	�֐���	�FKillTimer
//	���e	�F�^�C�}�[���I������
//	����	�F�Ȃ�
//	�߂�l	�FTRUE�F����
//			�FFALSE�F�ُ�
// ----------------------------------------------------------------------------
BOOL CMultiTimer::KillTimer(void)
{
	// �^�C�}�[�C�x���g���I������
	if( timeKillEvent(m_ID) == TIMERR_NOCANDO )
	{
		return FALSE;
	}

	// �^�C�}�[���I������
	if( m_uiTime > 0 )
	{
		timeBeginPeriod(m_uiTime);
	}

	return TRUE;
}
