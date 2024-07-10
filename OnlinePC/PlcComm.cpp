// PlcComm.cpp : �����t�@�C��
#include "PlcComm.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include <process.h>
#include <afxmt.h>


///////////////////////////////////////////////////////////////////////////////
// �萔�E��`

#define	PLC_DATA_MAX		70			// �ő�f�[�^��
#define	PLC_CMD_MAX			100			// �ő�R�}���h��
#define	PLC_1DATA_MAX		480			// �ő发���݃��[�h��
#define	PLC_RW_WAIT			3000		// PLC�ǂݏ����҂�����(msec)
//#define	PLC_RW_WAIT			10000		// PLC�ڑ��҂�����(msec)

// �R�}���h���̍\���̐錾
typedef struct
{
	NmPlcCommand	nmCmd;
	WORD			wData[PLC_1DATA_MAX];
	int				nSize;
}CmdInfo;


///////////////////////////////////////////////////////////////////////////////
// �O���ϐ�

// �e�|�C���^
static COnlinePCDlg	*m_pParent;

// App�R���t�B�O
static CConfig&		m_conf = theApp.m_cnf;

// �R�}���h�o�b�t�@�p�N���e�B�J���Z�N�V����
static CCriticalSection csPlcCmd;

// PLC�R�}���h�o�b�t�@
static CmdInfo		cmd[PLC_CMD_MAX];
static int			nCmdNum = 0;		// �R�}���h��
static int			nCmdIdx = 0;		// �R�}���h�o�b�t�@Index


///////////////////////////////////////////////////////////////////////////////
// �O���֐�

// PLC�f�[�^�X���b�h
void PlcDataThread( void *p );
// PLC�R�}���h�X���b�h
void PlcCmdThread( void *p );


///////////////////////////////////////////////////////////////////////////////
// �N���X����

// �\�z
CPlcComm::CPlcComm()
{
	// ������
	m_pParent = NULL;
}

// �j��
CPlcComm::~CPlcComm()
{
}

// PLC�ʐM�̏�����	
BOOL CPlcComm::Initialize(void *pThis)
{
	m_pParent = (COnlinePCDlg *)pThis;

	// IP�A�h���X�ǂݍ���
	if( FALSE == m_conf.ReadBosysNetwork(&m_stNetInfo) )
	{
		AfxMessageBox(_T("IP���ǂݍ��ُ݈�"));
	}

	// PLC�ʐM�X���b�h�N��
	m_thData = _beginthread(PlcDataThread, 0, this);
	m_thCmd = _beginthread(PlcCmdThread, 0, this);

	return TRUE;
}

// PLC�f�[�^�X���b�h�i�|�[�����O�p�j
void PlcDataThread( void *p )
{
	CPlcComm	*pT = (CPlcComm *)p;
	ViewData	*vd = GetSharedMemory2();
	WORD		data[PLC_1DATA_MAX];
	BOOL		bFirstErr = FALSE;
	int			i, nRegNo;

	// Predic�̋N���҂�
	while( !m_pParent->m_isAppEnd && ((m_pParent->m_pDlgPredic == NULL) || 
									  (m_pParent->m_pDlgPredic != NULL && m_pParent->m_pDlgPredic->IsRun() == FALSE)) ) 
		Sleep(100);

	// �\�P�b�g��Ԃ����������A�\�P�b�g�쐬
	pT->m_nmPoll = PLC_CLOSE;
	pT->m_plcd.Create( pT->m_stNetInfo.dwIP, pT->m_stNetInfo.dwPortD );

	// �A�v���I���܂Ń|�[�����O
	while(!m_pParent->m_isAppEnd)
	{
		// PLC���ڑ��Ȃ�
		if( pT->m_nmPoll == PLC_CLOSE )
		{
			pT->m_nmPoll = PLC_WAIT;

			// PLC�Đڑ����ُ�Ȃ�
			if( !pT->m_plcd.ReConnect() )
			{
				// �����҂��Ă�蒼��
				pT->m_nmPoll = PLC_CLOSE;
				if( !bFirstErr )
				{
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�ǂݍ��ݏ����ڑ��ُ�"));
					bFirstErr = TRUE;
				}
				Sleep(10000);
				continue;
			}

			// �ڑ�����
			pT->m_nmPoll = PLC_CONNECTED;
			if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
				m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�ǂݍ��ݏ����J�n"));
			bFirstErr = FALSE;
		}

		// �ڑ���ԂȂ�
		if( pT->m_nmPoll == PLC_CONNECTED )
		{
			///////////////////////////////////////////////////////////////////
			// �f�[�^��ǂݍ��݂P�i���уf�[�^�j
			if( !pT->m_plcd.ReadW(DEV_D, pT->m_stNetInfo.dwAddrR, data, PLC_DATA_MAX, PLC_RW_WAIT) )
			{
				// �ُ�Ȃ��蒼��
				pT->m_nmPoll = PLC_CLOSE;
				if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
					m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�ǂݍ��ݏ����ُ�"));
				Sleep(10000);
				continue;
			}

			///////////////////////////////////////////////////////////////////
			ViewLock();

			nRegNo = 0;
			for(i = 0; i < 20;i++)
			{
				if( i % 2 == 0 )
					vd->cGradeCode[i]	= data[nRegNo]	& 0xff;
				else
					vd->cGradeCode[i]	= (data[nRegNo++]>>8) & 0xff;
			}
			vd->cGradeCode[20]	= 0;
			vd->cHeatNo[0]		= data[nRegNo]			& 0xff;
			vd->cHeatNo[1]		= (data[nRegNo++]>>8)	& 0xff;
			vd->cHeatNo[2]		= data[nRegNo]			& 0xff;
			vd->cHeatNo[3]		= (data[nRegNo++]>>8)	& 0xff;
			vd->cHeatNo[4]		= data[nRegNo]			& 0xff;
			vd->cHeatNo[5]		= (data[nRegNo++]>>8)	& 0xff;
			vd->cHeatNo[6]		= data[nRegNo]			& 0xff;
			vd->cHeatNo[7]		= (data[nRegNo++]>>8)	& 0xff;
			vd->cHeatNo[8]		= 0;
			vd->isLdOn			= PLC_SHtoINT(data[nRegNo++]);
			vd->isHeatDataChg	= PLC_SHtoINT(data[nRegNo++]);
			vd->nTdWeidht		= PLC_SHtoINT(data[nRegNo++]);
			vd->isCharge		= PLC_SHtoINT(data[nRegNo++]);
			vd->cStandardNo[0]	= data[nRegNo]			& 0xff;
			vd->cStandardNo[1]	= (data[nRegNo++]>>8)	& 0xff;
			vd->cStandardNo[2]	= data[nRegNo]			& 0xff;
			vd->cStandardNo[3]	= (data[nRegNo++]>>8)	& 0xff;
			vd->cStandardNo[4]	= data[nRegNo]			& 0xff;
			vd->cStandardNo[5]	= (data[nRegNo++]>>8)	& 0xff;
			vd->cStandardNo[6]	= 0;
			vd->nPtnNo			= PLC_SHtoINT(data[nRegNo++])-1;
			vd->nCastLengthS	= PLC_DWORD(data[nRegNo]); nRegNo += 2;
			vd->nCastLengthC	= PLC_SHtoINT(data[nRegNo++]);
			vd->nMdCoolWtA		= PLC_SHtoINT(data[nRegNo++]);
			vd->nMdCoolWtC		= PLC_SHtoINT(data[nRegNo++]);
			vd->nMdCoolWtD		= PLC_SHtoINT(data[nRegNo++]);
			vd->nMdCoolWtTempD	= PLC_SHtoINT(data[nRegNo++]);
			vd->isBoRst			= PLC_SHtoINT(data[nRegNo++]);
			vd->nSnOpen			= PLC_SHtoINT(data[nRegNo++]);
			vd->nCastSpeed		= PLC_SHtoINT(data[nRegNo++]);
			vd->nMdLevel		= PLC_SHtoINT(data[nRegNo++]);
			vd->isAutoLvCtrl	= PLC_SHtoINT(data[nRegNo++]);
			vd->isPrOn			= PLC_SHtoINT(data[nRegNo++]);
			vd->isL1Err			= PLC_SHtoINT(data[nRegNo++]);
			for(i = 0; i < NumTCTap;i++)
				vd->nTemp[i]	= PLC_SHtoINT(data[nRegNo++]);
			vd->nPlcErr1		= PLC_SHtoINT(data[nRegNo++]);
			vd->nPlcErr2		= PLC_SHtoINT(data[nRegNo++]);

			ViewUnlock();
			///////////////////////////////////////////////////////////////////

			// �����҂�
			Sleep(200);
		}
	}
}

// PLC�R�}���h�X���b�h
void PlcCmdThread( void *p )
{
	CPlcComm *pT = (CPlcComm *)p;
	NmPlcCommand	nmCmd;
	WORD			*pBuf;
	BOOL			bFirstErr = FALSE;
	int				nIdx, nSize, nHealthyTrig = 0;

	// Predic�̋N���҂�
	while( !m_pParent->m_isAppEnd && ((m_pParent->m_pDlgPredic == NULL) || 
									  (m_pParent->m_pDlgPredic != NULL && m_pParent->m_pDlgPredic->IsRun() == FALSE)) ) 
		Sleep(100);

	// �\�P�b�g��Ԃ����������A�\�P�b�g�쐬
	pT->m_nmCmd = PLC_CLOSE;
	pT->m_plcc.Create( pT->m_stNetInfo.dwIP, pT->m_stNetInfo.dwPortC );

	while(!m_pParent->m_isAppEnd)
	{
		// PLC���ڑ��Ȃ�
		if( pT->m_nmCmd == PLC_CLOSE )
		{
			// �����҂���
			pT->m_nmCmd = PLC_WAIT;

			// PLC�Đڑ����ُ�Ȃ�
			if( !pT->m_plcc.ReConnect() )
			{
				// ��蒼��
				pT->m_nmCmd = PLC_CLOSE;
				if( !bFirstErr )
				{
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�������ݏ����ڑ��ُ�"));
					bFirstErr = TRUE;
				}
				Sleep(10000);
				continue;
			}

			// �ڑ�����
			pT->m_nmCmd = PLC_CONNECTED;
			if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
				m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�R�}���h�����J�n"));
			bFirstErr = FALSE;

			///////////////////////////////////////////////////////////////////
			csPlcCmd.Lock();

			// ���܂��Ă�ʐM�f�[�^��j������
			nCmdNum = 0;

			csPlcCmd.Unlock();
			///////////////////////////////////////////////////////////////////
		}

		// �ڑ���ԂȂ�
		if( pT->m_nmCmd == PLC_CONNECTED )
		{
			///////////////////////////////////////////////////////////////////
			csPlcCmd.Lock();

			// �f�[�^������Ȃ�
			if( nCmdNum > 0 )
			{
				nIdx = nCmdIdx - nCmdNum;
				if( nIdx < 0 )
					nIdx = PLC_CMD_MAX + nIdx;
				nmCmd = cmd[nIdx].nmCmd;
				pBuf = cmd[nIdx].wData;
				nSize = cmd[nIdx].nSize;
				nCmdNum--;
			}
			// �f�[�^���Ȃ��Ȃ�
			else
				// �R�}���h�Ȃ�
				nmCmd = idCmdNone;

			csPlcCmd.Unlock();
			///////////////////////////////////////////////////////////////////

			// �R�}���h������Ȃ�
			if( nmCmd != idCmdNone )
			{
				// �R�}���h����
				if( !pT->CommandProc(nmCmd, pBuf, nSize) )
				{
					pT->m_nmCmd = PLC_CLOSE;
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC�R�}���h�����ُ�"));
					Sleep(10000);
				}
			}
		}

		// �҂�
		Sleep(100);
	}
}

// PLC�R�}���h����
BOOL CPlcComm::CommandProc(NmPlcCommand nmCmd, WORD *pVal, int nSize)
{
	int		nCnt = nSize / sizeof(WORD);

	switch(nmCmd)
	{
	case idCmdSend:
		if( !m_plcc.WriteW(DEV_D, pVal[0], &pVal[1], nCnt-1, PLC_RW_WAIT) )
		{
			// �ُ�I��
			return FALSE;
		}
		break;
	}

	return TRUE;
}

// PLC�R�}���h�X���b�h�փR�}���h�o�^
BOOL SendPlc(NmPlcCommand nmCmd, void *pDat, int nSize)
{
	// �T�C�Y���ُ�Ȃ�I��
	if( nSize > PLC_1DATA_MAX * sizeof(WORD) ) 
		return FALSE;

	///////////////////////////////////
	csPlcCmd.Lock();

	cmd[nCmdIdx].nmCmd = nmCmd;
	memcpy(cmd[nCmdIdx].wData, pDat, nSize);
	cmd[nCmdIdx].nSize = nSize;
	nCmdIdx++;
	if( nCmdIdx >= PLC_CMD_MAX )
		nCmdIdx = 0;
	if( nCmdNum < PLC_CMD_MAX )
		nCmdNum++;

	csPlcCmd.Unlock();
	///////////////////////////////////

	// ����I��
	return TRUE;
}

