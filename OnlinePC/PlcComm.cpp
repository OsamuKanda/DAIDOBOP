// PlcComm.cpp : 実装ファイル
#include "PlcComm.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include <process.h>
#include <afxmt.h>


///////////////////////////////////////////////////////////////////////////////
// 定数・定義

#define	PLC_DATA_MAX		70			// 最大データ数
#define	PLC_CMD_MAX			100			// 最大コマンド数
#define	PLC_1DATA_MAX		480			// 最大書込みワード数
#define	PLC_RW_WAIT			3000		// PLC読み書き待ち時間(msec)
//#define	PLC_RW_WAIT			10000		// PLC接続待ち時間(msec)

// コマンド情報の構造体宣言
typedef struct
{
	NmPlcCommand	nmCmd;
	WORD			wData[PLC_1DATA_MAX];
	int				nSize;
}CmdInfo;


///////////////////////////////////////////////////////////////////////////////
// 外部変数

// 親ポインタ
static COnlinePCDlg	*m_pParent;

// Appコンフィグ
static CConfig&		m_conf = theApp.m_cnf;

// コマンドバッファ用クリティカルセクション
static CCriticalSection csPlcCmd;

// PLCコマンドバッファ
static CmdInfo		cmd[PLC_CMD_MAX];
static int			nCmdNum = 0;		// コマンド数
static int			nCmdIdx = 0;		// コマンドバッファIndex


///////////////////////////////////////////////////////////////////////////////
// 外部関数

// PLCデータスレッド
void PlcDataThread( void *p );
// PLCコマンドスレッド
void PlcCmdThread( void *p );


///////////////////////////////////////////////////////////////////////////////
// クラス実装

// 構築
CPlcComm::CPlcComm()
{
	// 初期化
	m_pParent = NULL;
}

// 破棄
CPlcComm::~CPlcComm()
{
}

// PLC通信の初期化	
BOOL CPlcComm::Initialize(void *pThis)
{
	m_pParent = (COnlinePCDlg *)pThis;

	// IPアドレス読み込み
	if( FALSE == m_conf.ReadBosysNetwork(&m_stNetInfo) )
	{
		AfxMessageBox(_T("IP情報読み込み異常"));
	}

	// PLC通信スレッド起動
	m_thData = _beginthread(PlcDataThread, 0, this);
	m_thCmd = _beginthread(PlcCmdThread, 0, this);

	return TRUE;
}

// PLCデータスレッド（ポーリング用）
void PlcDataThread( void *p )
{
	CPlcComm	*pT = (CPlcComm *)p;
	ViewData	*vd = GetSharedMemory2();
	WORD		data[PLC_1DATA_MAX];
	BOOL		bFirstErr = FALSE;
	int			i, nRegNo;

	// Predicの起動待ち
	while( !m_pParent->m_isAppEnd && ((m_pParent->m_pDlgPredic == NULL) || 
									  (m_pParent->m_pDlgPredic != NULL && m_pParent->m_pDlgPredic->IsRun() == FALSE)) ) 
		Sleep(100);

	// ソケット状態を初期化し、ソケット作成
	pT->m_nmPoll = PLC_CLOSE;
	pT->m_plcd.Create( pT->m_stNetInfo.dwIP, pT->m_stNetInfo.dwPortD );

	// アプリ終了までポーリング
	while(!m_pParent->m_isAppEnd)
	{
		// PLC未接続なら
		if( pT->m_nmPoll == PLC_CLOSE )
		{
			pT->m_nmPoll = PLC_WAIT;

			// PLC再接続し異常なら
			if( !pT->m_plcd.ReConnect() )
			{
				// 少し待ってやり直し
				pT->m_nmPoll = PLC_CLOSE;
				if( !bFirstErr )
				{
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC読み込み処理接続異常"));
					bFirstErr = TRUE;
				}
				Sleep(10000);
				continue;
			}

			// 接続完了
			pT->m_nmPoll = PLC_CONNECTED;
			if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
				m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC読み込み処理開始"));
			bFirstErr = FALSE;
		}

		// 接続状態なら
		if( pT->m_nmPoll == PLC_CONNECTED )
		{
			///////////////////////////////////////////////////////////////////
			// データを読み込み１（実績データ）
			if( !pT->m_plcd.ReadW(DEV_D, pT->m_stNetInfo.dwAddrR, data, PLC_DATA_MAX, PLC_RW_WAIT) )
			{
				// 異常ならやり直し
				pT->m_nmPoll = PLC_CLOSE;
				if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
					m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC読み込み処理異常"));
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

			// 少し待ち
			Sleep(200);
		}
	}
}

// PLCコマンドスレッド
void PlcCmdThread( void *p )
{
	CPlcComm *pT = (CPlcComm *)p;
	NmPlcCommand	nmCmd;
	WORD			*pBuf;
	BOOL			bFirstErr = FALSE;
	int				nIdx, nSize, nHealthyTrig = 0;

	// Predicの起動待ち
	while( !m_pParent->m_isAppEnd && ((m_pParent->m_pDlgPredic == NULL) || 
									  (m_pParent->m_pDlgPredic != NULL && m_pParent->m_pDlgPredic->IsRun() == FALSE)) ) 
		Sleep(100);

	// ソケット状態を初期化し、ソケット作成
	pT->m_nmCmd = PLC_CLOSE;
	pT->m_plcc.Create( pT->m_stNetInfo.dwIP, pT->m_stNetInfo.dwPortC );

	while(!m_pParent->m_isAppEnd)
	{
		// PLC未接続なら
		if( pT->m_nmCmd == PLC_CLOSE )
		{
			// 少し待って
			pT->m_nmCmd = PLC_WAIT;

			// PLC再接続し異常なら
			if( !pT->m_plcc.ReConnect() )
			{
				// やり直し
				pT->m_nmCmd = PLC_CLOSE;
				if( !bFirstErr )
				{
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLC書き込み処理接続異常"));
					bFirstErr = TRUE;
				}
				Sleep(10000);
				continue;
			}

			// 接続完了
			pT->m_nmCmd = PLC_CONNECTED;
			if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
				m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLCコマンド処理開始"));
			bFirstErr = FALSE;

			///////////////////////////////////////////////////////////////////
			csPlcCmd.Lock();

			// 溜まってる通信データを破棄する
			nCmdNum = 0;

			csPlcCmd.Unlock();
			///////////////////////////////////////////////////////////////////
		}

		// 接続状態なら
		if( pT->m_nmCmd == PLC_CONNECTED )
		{
			///////////////////////////////////////////////////////////////////
			csPlcCmd.Lock();

			// データがあるなら
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
			// データがないなら
			else
				// コマンドなし
				nmCmd = idCmdNone;

			csPlcCmd.Unlock();
			///////////////////////////////////////////////////////////////////

			// コマンドがあるなら
			if( nmCmd != idCmdNone )
			{
				// コマンド処理
				if( !pT->CommandProc(nmCmd, pBuf, nSize) )
				{
					pT->m_nmCmd = PLC_CLOSE;
					if( !m_pParent->m_isAppEnd && m_pParent->m_pDlgPredic != NULL )
						m_pParent->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, 0, (LPARAM)_T("PLCコマンド処理異常"));
					Sleep(10000);
				}
			}
		}

		// 待ち
		Sleep(100);
	}
}

// PLCコマンド処理
BOOL CPlcComm::CommandProc(NmPlcCommand nmCmd, WORD *pVal, int nSize)
{
	int		nCnt = nSize / sizeof(WORD);

	switch(nmCmd)
	{
	case idCmdSend:
		if( !m_plcc.WriteW(DEV_D, pVal[0], &pVal[1], nCnt-1, PLC_RW_WAIT) )
		{
			// 異常終了
			return FALSE;
		}
		break;
	}

	return TRUE;
}

// PLCコマンドスレッドへコマンド登録
BOOL SendPlc(NmPlcCommand nmCmd, void *pDat, int nSize)
{
	// サイズが異常なら終了
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

	// 正常終了
	return TRUE;
}

