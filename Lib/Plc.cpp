// Plc.cpp : アプリケーション用のエントリ ポイントの定義
//
#include "plc.h"
#include <atlstr.h>

//**************************************************
//  関数名	： CPlc（コンストラクタ）
//  内容	： CPlcメンバ初期化など
//  引数	： ...
//  戻り値	： ...
//**************************************************
CPlc::CPlc()
{
	// クリティカルセクションオブジェクト生成
	InitializeCriticalSection(&m_rwLock);
}

//**************************************************
//  関数名	： ~CPlc（デストラクタ）
//  内容	： CPlc終了処理
//  引数	： ...
//  戻り値	： ...
//**************************************************
CPlc::~CPlc()
{
	m_sock.Close();

	// クリティカルセクションオブジェクト削除
	DeleteCriticalSection(&m_rwLock);
}

//**************************************************
//  関数名	： Create
//  内容	： Create
//  引数	： ...
//  戻り値	： ...
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
//  関数名	： Create
//  内容	： Create
//  引数	： ...
//  戻り値	： ...
//**************************************************
BOOL CPlc::Create( DWORD dwIP, int nPort )
{
	m_dwIP		= (DWORD)dwIP;
	m_nPort		= nPort;

	return TRUE;
}

//**************************************************
//  関数名	： ReConnect
//  内容	： ReConnect
//  引数	： ...
//  戻り値	： ...
//**************************************************
BOOL CPlc::ReConnect( void )
{
	m_sock.Close();									// ソケット切断

	if( FALSE == m_sock.Create() )					// ソケット作成
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	if( FALSE == m_sock.Connect( m_dwIP, m_nPort) )	// 再接続
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//  関数名	： WriteW
//  内容	： ワード一括書き込み
//  引数	： PBYTE nDev		-	デバイスコード
//			： UINT	uiDevNo		-	デバイスＮＯ
//			： PWORD pwData		-	データ
//			： WORD wCnt		-	数
//			： DWORD dwTime		-	タイムアウト時間(ms)
//  戻り値	： ...
//**************************************************
BOOL CPlc::WriteW( BYTE nDev, UINT uiDevNo, PWORD pwData, WORD wCnt, DWORD dwTime)
{
	int nSize;

	// クリティカルセクションによる排他制御(インスタンスの消滅時に自動アンロック）
	CCriticalLock	lock(&m_rwLock);

	// 送受信エリアをクリア
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// コマンドメッセージにヘッダ情報セット
	nSize = SetWriteCmdW(m_byBufW, nDev, uiDevNo, wCnt);

	// コマンドメッセージにデータをセット
	for( int i = 0; i < wCnt ; i++ )
		PcToPlc(pwData[i], &m_byBufW[POS_DATA_CMD + i * 2], 2);

	// ５秒受信待ち
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCにコマンドメッセージ送信
	if( FALSE == m_sock.Send( m_byBufW, nSize) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// 受信待ち
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCからレスポンス受信
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// レスポンスデータをチェック
	if( 0 > GetWriteResW( m_byBufR ) )
	{
		m_Err = -1;
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//  関数名	： WriteDW
//  内容	： ダブルワード一括書き込み
//  引数	： PBYTE nDev		-	デバイスコード
//			： UINT	uiDevNo		-	デバイスＮＯ
//			： PWORD pwData		-	データ
//			： WORD wCnt		-	数
//			： DWORD dwTime		-	タイムアウト時間(ms)
//  戻り値	： ...
//**************************************************
BOOL CPlc::WriteDW( BYTE nDev, UINT uiDevNo, PDWORD pdwData, WORD wCnt, DWORD dwTime)
{
	int nSize;

	// クリティカルセクションによる排他制御(インスタンスの消滅時に自動アンロック）
	CCriticalLock	lock(&m_rwLock);

	// 送受信エリアをクリア
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// コマンドメッセージにヘッダ情報セット
	nSize = SetWriteCmdW(m_byBufW, nDev, uiDevNo, wCnt * 2);

	// コマンドメッセージにデータをセット
	for( int i = 0; i < wCnt ; i++ )
		PcToPlc(pdwData[i], &m_byBufW[POS_DATA_CMD + i * 4], 4);

	// ５秒受信待ち
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCにコマンドメッセージ送信
	if( FALSE == m_sock.Send( m_byBufW, nSize) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// 受信待ち
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCからレスポンス受信
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// レスポンスデータをチェック
	if( 0 > GetWriteResW( m_byBufR ) )
	{
		m_Err = -2;
		return FALSE;
	}

	return TRUE;

}

//**************************************************
//  関数名	： ReadW
//  内容	： ワード一括読込み
//  引数	： PBYTE nDev		-	デバイスコード
//			： UINT	uiDevNo		-	デバイスＮＯ
//			： PWORD pwData		-	データ
//			： WORD wCnt		-	数
//			： DWORD dwTime		-	タイムアウト時間(ms)
//  戻り値	： ...
//**************************************************
BOOL CPlc::ReadW( BYTE nDev, UINT uiDevNo, PWORD pwData, WORD wCnt, DWORD dwTime)
{
	int nDataPos;

	// クリティカルセクションによる排他制御(インスタンスの消滅時に自動アンロック）
	CCriticalLock	lock(&m_rwLock);

	// 送受信エリアをクリア
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// コマンドメッセージにヘッダ情報セット
	nDataPos = SetReadCmdW(m_byBufW, nDev, uiDevNo, wCnt);

	// ５秒受信待ち
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCにコマンドメッセージ送信
	if( FALSE == m_sock.Send( m_byBufW, POS_DATA_CMD) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// ５秒受信待ち
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCからレスポンス受信
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES + wCnt * sizeof(WORD)) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// レスポンスデータをチェック
	if( 0 > ( nDataPos = GetReadResW( m_byBufR , wCnt * sizeof(WORD) ) ) )
	{
		m_Err = -3;
		return FALSE;
	}

	// 受信データを取得
	for( int i = 0; i < wCnt ; i++ )
//		PcToPlc(pwData[i], &m_byBufR[nDataPos + i * 2], 2);
		pwData[i] = PlcToPc( &m_byBufR[nDataPos + i * 2], 2);

	return TRUE;
}

//**************************************************
//  関数名	： ReadDW
//  内容	： ワード一括読込み
//  引数	： PBYTE nDev		-	デバイスコード
//			： UINT	uiDevNo		-	デバイスＮＯ
//			： PWORD pwData		-	データ
//			： WORD wCnt		-	数
//			： DWORD dwTime		-	タイムアウト時間(ms)
//  戻り値	： ...
//**************************************************
BOOL CPlc::ReadDW( BYTE nDev, UINT uiDevNo, PDWORD pdwData, WORD wCnt, DWORD dwTime)
{
	int nDataPos;

	// クリティカルセクションによる排他制御(インスタンスの消滅時に自動アンロック）
	CCriticalLock	lock(&m_rwLock);

	// 送受信エリアをクリア
	ZeroMemory( m_byBufW, sizeof(m_byBufW) );
	ZeroMemory( m_byBufR, sizeof(m_byBufR) );

	// コマンドメッセージにヘッダ情報セット
	nDataPos = SetReadCmdW(m_byBufW, nDev, uiDevNo, wCnt * 2);

	// 送信待ち
	if( FALSE == m_sock.SelectW( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCにコマンドメッセージ送信
	if( FALSE == m_sock.Send( m_byBufW, POS_DATA_CMD) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// 受信待ち
	if( FALSE == m_sock.SelectR( dwTime ) )
	{
		m_Err = m_sock.GetError();
		return FALSE;
	}

	// PLCからレスポンス受信
	if( FALSE == m_sock.Recv( m_byBufR, POS_DATA_RES + wCnt * sizeof(DWORD)) )
		return FALSE;

	// レスポンスデータをチェック
	if( 0 > ( nDataPos = GetReadResW( m_byBufR, wCnt * sizeof(DWORD) ) ) )
	{
		m_Err = -4;
		return FALSE;
	}

	// 受信データを取得
	for( int i = 0; i < wCnt ; i++ )
//		PcToPlc(pdwData[i], &m_byBufR[nDataPos + i * 4], 4);
		pdwData[i] = PlcToPc( &m_byBufR[nDataPos + i * 4], 4);

	return TRUE;
}

//**************************************************
//  関数名	： ChkConnect
//  内容	： コネクションチェック
//  引数	： DWORD dwTime		-	タイムアウト時間(ms)
//  戻り値	： ...
//**************************************************
BOOL CPlc::ChkConnect( BYTE nDev, UINT uiDevNo, PWORD pwData, DWORD dwTime )
{
	// コネクション正常判定（ヘルシー送信）

	if( FALSE == WriteW( nDev, uiDevNo, pwData, 1, dwTime) )
	{
		// 異常
		m_Err = m_sock.GetError();
		return FALSE;
	}
	return TRUE;
}

//**************************************************
//  関数名	： GetError
//  内容	： エラー番号を返す
//  引数	： なし
//  戻り値	： エラー番号
//**************************************************
int CPlc::GetError(void)
{
	return m_Err;
}

