// Chart3DDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "TMapEx.h"
#include "TMap2.h"
#include "TMap2If.h"

static CConfig		&cnf	= theApp.m_cnfBoexe;
#define	PartitionCount	10

// CTMapDlg ダイアログ

IMPLEMENT_DYNAMIC(CTMapDlg, CWinFormsDialog<TMap::TMapControl>)

CTMapDlg::CTMapDlg(CWnd* pParent, BOOL isRec)
{
	m_isRecord = isRec;
	m_pDlgTcView = NULL;
	memset(m_iData, 0, sizeof(m_iData));
	m_bBatchStop		= FALSE;
	m_bAutoRun			= FALSE;
	m_nRecCur			= 0;
	m_nRecCnt			= 0;
	m_nStartTime		= 0;
	m_nOffsetTime		= 0;
	m_pBatch			= NULL;

	// create dialog.
	Create(CTMapDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CTMapDlg::~CTMapDlg()
{
}

void CTMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTMapDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_MESSAGE(WM_MAPDATA, OnAddone)
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_MESSAGE(WM_RECSTART,  OnRecStart)
	ON_MESSAGE(WM_RECSTOP,	 OnRecStop)
	ON_MESSAGE(WM_BATCHSTOP, OnBatchStop)
	ON_MESSAGE(WM_TMAP_SCR,  OnTMapScroll)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CTMapDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CTMapDlg::OnOk(){}

//It evades ending by ESC Key.
void CTMapDlg::OnCancel(){}

void CTMapDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CTMapDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// ダイアログの初期化
BOOL CTMapDlg::OnInitDialog() 
{
	CWinFormsDialog<TMap::TMapControl>::OnInitDialog();
	
	// SYSTEMメニュー削除
	::SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, WS_POPUP);
	ModifyStyle(WS_SYSMENU, 0, 0);

	// メニュー追加
    m_menu.LoadMenu( IDR_CHART3D_MENU );
    SetMenu( &m_menu );

	// 履歴子画面表示
	if( m_isRecord )
	{
		m_pDlgRec = new CTMapEx(this);
		m_pDlgRec->SetWindowPos( &wndTop, 75, 890, 0, 0, SWP_NOSIZE );
		m_pDlgRec->ShowWindow(SW_HIDE);
	}

	// タイマー開始
	SetTimer(1, 500, NULL);

	return TRUE;
}

// WM_SHOWWINDOW proc.
void CTMapDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// show? 
	if( bShow )
	{
		// 履歴子画面表示
		if( m_isRecord )
		{
			m_pDlgRec->ShowWindow(SW_SHOW);
			m_nRecCnt = m_pBatch->GetRecordCount();
			GetScr(m_pDlgRec)->SetScrollRange(0, m_nRecCnt-1);
		}
	}
	// hide?
	else
	{
		// 履歴子画面表示
		if( m_isRecord )
			m_pDlgRec->ShowWindow(SW_HIDE);
	}
}

// selected 'TC Position' menu.
void CTMapDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

//
CDialog *GetTMapDlg(CDialog *parent, BOOL isRec)
{
	return new CTMapDlg(parent, isRec);
}

// tcno->MapNo変換テーブル
static int tc2map[28] = 
{
	0,
	1,
	3,
	4,
	6,
	7,
	9,
	10,
	11,
	12,
	13,
	15,
	16,
	17,
	18,
	19,
	21,
	22,
	24,
	25,
	27,
	28,
	29,
	30,
	31,
	33,
	34,
	35
};

LRESULT CTMapDlg::OnAddone(WPARAM wParam, LPARAM lParam) 
{
	int		nData[NumADChannel];
	int		nDataMap[36];
	int		nSetMap[22][121];
	int		i, j, m, ii, jj;

	// スクロール関係
	if( m_isRecord )
	{
		m_nRecCur = (int)wParam;
		GetScr(m_pDlgRec)->SetScrollPos(m_nRecCur);
	}

	// データ取得
	memcpy(nData, (int *)lParam, sizeof(nData));

	// グラフ用平均処理
	SetAverageData(nData, nDataMap);
	GetAverageData(nDataMap);

	// グラフコントロールにアクセス
	TMap::TMapControl	^cc = (TMap::TMapControl	^)GetControl();

	// グラフ用に並べ替え
	int	l[22] = {0,1,3,4,6,7,9,10,12,13,15,16,18,19,21,22,24,25,27,28,30,31};
	for( m = 0; m < 22; m++ )
	{
		for( i = 0; i < 11; i++ )
		{
			for( j = 0; j < 11; j++ )
			{
				ii = (m%2) * 11 + i;
				jj = (m/2) * 11 + j;
				nSetMap[ii][jj] = m_iData[l[m]][i][j];
			}
		}
	}

	// 表示ラインのデータをセット
	cc->setData((int*)nSetMap);

	return 0;
}

// setm_iData.
void CTMapDlg::SetAverageData(int *data, int *datam)
{
	int i;

	// 今回データ取得(マップ配列に入れる）
	for( i = 0; i < 28; i++ )
	{
		datam[tc2map[i]] = data[i];
	}

	// データの無いところの平均処理
	datam[2]	= 1*(datam[11]-datam[35])/4+datam[35];
	datam[5]	= 3*(datam[11]-datam[35])/4+datam[35];
	datam[8]	= 2*(datam[11]-datam[35])/4+datam[35];
	datam[14]	= (datam[17]+datam[11])/2;
	datam[20]	= 1*(datam[29]-datam[17])/4+datam[17];
	datam[23]	= 3*(datam[29]-datam[17])/4+datam[17];
	datam[26]	= 2*(datam[29]-datam[17])/4+datam[17];
	datam[32]	= (datam[35]+datam[29])/2;
}

// get Average Data.
void CTMapDlg::GetAverageData(int *pData)
{
	int	i, j;

	// 配列変数の初期化
	memset( m_iData, 0, sizeof( m_iData ) );
	for( i = 0; i < 11; i++ )
	{
		for( j = 0; j < TapRaw-1; j++)
			CalcAverageData(i * TapRaw + j, pData);
	}
}

// calc Average Data.
void CTMapDlg::CalcAverageData(int f, int *pData)
{
	int j, k, z;
	float a1, a2, a3, q, r, s;

	/*データ点数を設定*/
	z = PartitionCount + 1;

	/*各ポイント値を設定*/
	m_iData[f][0][0] = pData[f];				// 中心点
	m_iData[f][0][z - 1] = pData[f + 3];		// 右横
	m_iData[f][z - 1][0] = pData[f + 1];		// 真下
	m_iData[f][z - 1][z - 1] = pData[f + 4];	// 右斜め下

	/*p[f]-p[f + 3]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 3] - pData[f]) / z;

	/*p[f + 1]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 1]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（横方向）のポイント値を設定*/
		m_iData[f][0][j] = int(a1 * j + pData[f]);
		m_iData[f][z - 1][j] = int(a2 * j + pData[f + 1]);
	}

	/*p[f]-p[f + 1]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 1] - pData[f]) / z;

	/*p[f + 3]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 3]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（縦方向）のポイント値を設定*/
		m_iData[f][j][0] = int(a1 * j + pData[f]);
		m_iData[f][j][z - 1] = int(a2 * j + pData[f + 3]);
	}

	/*p[f]-p[f + 4]ポイント間の傾きを計算*/
	a3 = (float)(pData[f + 4] - pData[f]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*p[f]-p[f + 3]ポイント間の傾きを計算*/
		a1 = (float)(m_iData[f][j][z - 1] - m_iData[f][j][0]) / z;

		for(k = 1;k < z - 1;k++)
		{
			/*p[f]-p[f + 1]ポイント間の傾きを計算*/
			a2 = (float)(m_iData[f][z - 1][k] - m_iData[f][0][k]) / z;

			/*p[f]-p[f + 3]ポイント間のポイント値を計算*/
			q = a1 * k + m_iData[f][j][0];

			/*p[f]-p[f + 1]ポイント間のポイント値を計算*/
			r = a2 * j + m_iData[f][0][k];

			/*対角線上のデータの場合*/
			if (k == j)
			{
				/*p[f]-p[f + 4]ポイント間のポイント値を計算*/
				s = a3 * k + pData[f];

				/*3つのポイント値の平均をポイント値として設定*/
				m_iData[f][j][k] = int((q + r + s) / 3);
			}
			/*対角線上以外のデータの場合*/
			else
			{
				/*2つのポイント値の平均をポイント値として設定*/
				m_iData[f][j][k] = int((q + r) / 2);
			}
		}
	}
}

void SetLogInfo(CDialog *pThis, StLogInfo *pInfo)
{
	CTMapDlg	*pT = (CTMapDlg *)pThis;
	pT->m_pBatch = pInfo->pBatch;
	pT->m_nStartTime = pInfo->nStartTime;
	pT->m_nOffsetTime = pInfo->nOffsetTime;
	pT->m_pBatch->SetLogPointer(pT->m_nOffsetTime);
}

// 
LRESULT CTMapDlg::OnRecStart(WPARAM, LPARAM lParam) 
{
	int	nCurRec;

	if( !m_bBatchStop || m_bAutoRun )
		return 0;

	// 
	nCurRec = m_pBatch->GetCurrentRecord();

	// setup log
	m_pBatch->SetLogPointer(nCurRec);

	// auto run flag on.
	m_bAutoRun = TRUE;

	return 0;
}

// 
LRESULT CTMapDlg::OnRecStop(WPARAM, LPARAM lParam) 
{
	if( !m_bBatchStop || !m_bAutoRun )
		return 0;

	m_bAutoRun = FALSE;

	return 0;
}

// 
LRESULT CTMapDlg::OnBatchStop(WPARAM, LPARAM lParam) 
{
	m_bBatchStop = TRUE;

	return 0;
}

LRESULT CTMapDlg::OnTMapScroll(WPARAM wParam, LPARAM lParam) 
{
	int	nSBCode = wParam;
	int	nPos	= lParam;

	if( m_bBatchStop && !m_bAutoRun )
	{
		int	nCurPos;
		nCurPos = GetScr(m_pDlgRec)->GetScrollPos();	
		switch(nSBCode)
		{
		case SB_LINELEFT:
			if( nCurPos > 0 )
			{
				nCurPos-=2;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_LINERIGHT:
			if( nCurPos < (m_nRecCnt - 1) )
			{
				nCurPos+=2;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_PAGELEFT:
			if( nCurPos > 0 )
			{
				nCurPos-=11;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_PAGERIGHT:
			if( nCurPos < (m_nRecCnt - 1) )
			{
				nCurPos+=9;
				if( nCurPos > m_nRecCnt - 1 )
					nCurPos = m_nRecCnt - 1;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_THUMBTRACK:
			if( nPos >= 0 && nPos < (m_nRecCnt - 1) )
			{
				nCurPos = nPos - 1;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_THUMBPOSITION:
			if( nPos >= 0 && nPos < (m_nRecCnt - 1) )
			{
				nCurPos = nPos - 1;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

void CTMapDlg::DispCurrentMap(void)
{
	int		nData[NumADChannel];
	int		nRecCnt, nCurRec;

	SampledData	data;

	nRecCnt = m_pBatch->GetRecordCount();
	nCurRec = m_pBatch->GetCurrentRecord();
	if( nCurRec >= (nRecCnt - 1) )
		return;

	m_pBatch->GetSampledData(&data);
	//
	m_pBatch->SetAverageData(&data, nData);
	m_pBatch->LimitTempData(nData);

	SendMessage(WM_MAPDATA, (WPARAM)nCurRec, (LPARAM)nData);

	// disp current time
	DispCurrentTime();
}

void CTMapDlg::DispCurrentTime(void)
{
	static time_t	preTime;
	time_t			t;
	int				nCurRec;
	_TCHAR			buf[64];

	if( m_pBatch == NULL )
		return;

	// get current time.
	nCurRec = m_pBatch->GetCurrentRecord();
	t = (time_t)(m_nStartTime + nCurRec * m_pBatch->m_header.header.samplePeriod * 0.001);
	if (t != preTime)
	{
		// make up the display string.
//		_tcscpy(buf, _tctime(&t)),	buf[_tcslen(buf) - 1] = _T('\0');

		// clear displaying area.
		GetText(m_pDlgRec)->SetWindowText(GetTimeStr(t));

		// update current time.
		preTime	= t;
	}
}

LPCTSTR CTMapDlg::GetTimeStr(time_t tm)
{
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	tp	= localtime(&tm);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

// タイマー処理
void CTMapDlg::OnTimer(UINT nIDEvent)
{
	if( m_bBatchStop && m_bAutoRun )
	{
		DispCurrentMap();
	}
	else
		DispCurrentTime();

	CDialog::OnTimer(nIDEvent);
}

//
BOOL GetBatchStopReq(CDialog *pThis)
{
	CTMapDlg	*pT = (CTMapDlg *)pThis;
	return pT->m_bBatchStop;
}

void SetBatchStopReq(CDialog *pThis, BOOL bSet)
{
	CTMapDlg	*pT = (CTMapDlg *)pThis;
	pT->m_bBatchStop = bSet;
	pT->m_bAutoRun	 = FALSE;
}

