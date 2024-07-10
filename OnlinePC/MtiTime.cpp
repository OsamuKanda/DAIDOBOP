#include <windows.h>
#include "MtiTime.h"

// -----------------
// メソッド宣言
// -----------------

// コンストラクタ
CMultiTimer::CMultiTimer()
{
	m_ID		= 0;		// タイマーIDの初期化
	m_uiTime	= 0;		// タイマー周期
}

// デストラクタ
CMultiTimer::~CMultiTimer()
{
	// タイマーが起動されているなら
	if(m_ID)
	{
		// デバイスをクローズする
		KillTimer();
		m_ID = 0;
	}
}

// ----------------------------------------------------------------------------
//	関数名	：SetTimer
//	内容	：タイマーを起動する
//	引数	：UINT	uiTime			－	タイマー周期(ms)
//			：LPTIMECALLBACK lptc	－	タイマーコールバック関数アドレス
//			：LPVOID lpArg			－	タイマー関数への引数
//	戻り値	：TRUE：正常
//			：FALSE：異常
// ----------------------------------------------------------------------------
BOOL CMultiTimer::SetTimer(UINT uiTime, LPTIMECALLBACK lptc, LPVOID lpArg)
{
	TIMECAPS tc;

	// タイマーデバイスの存在チェック
	if( timeGetDevCaps( &tc, sizeof( TIMECAPS ) ) == TIMERR_NOCANDO ) 
	{
		return FALSE;
	}

	// タイマー値のチェック
	if ( tc.wPeriodMin > uiTime || tc.wPeriodMax < uiTime ) 
	{
		return FALSE;
	}

	// 最小タイマ解像度を設定します
	if( TIMERR_NOERROR != timeBeginPeriod(uiTime) )
	{
		return FALSE;
	}
	m_uiTime = uiTime;

	// タイマーイベントを起動する
	m_ID = timeSetEvent(uiTime, 1, lptc, (DWORD)lpArg, TIME_PERIODIC);

	// タイマーの起動が異常なら
	if(!m_ID)
	{
		return FALSE;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
//	関数名	：KillTimer
//	内容	：タイマーを終了する
//	引数	：なし
//	戻り値	：TRUE：正常
//			：FALSE：異常
// ----------------------------------------------------------------------------
BOOL CMultiTimer::KillTimer(void)
{
	// タイマーイベントを終了する
	if( timeKillEvent(m_ID) == TIMERR_NOCANDO )
	{
		return FALSE;
	}

	// タイマーを終了する
	if( m_uiTime > 0 )
	{
		timeBeginPeriod(m_uiTime);
	}

	return TRUE;
}
