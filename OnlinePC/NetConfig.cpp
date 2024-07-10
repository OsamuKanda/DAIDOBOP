// SysConfig.cpp: CNetConfig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OnlinePC.h"
#include "NetConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/破棄
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// 名前	: CNetConfig
// 内容	: 構築
// 引数	: なし
// 戻値	: なし
//-----------------------------------------------------------------------------
CNetConfig::CNetConfig()
{
	// 初期化＆デフォルト値設定
	memset(&m_stLog, 0, sizeof(m_stLog));
	memset(m_szPath, 0, sizeof(m_szPath));
	strcpy(m_szPath, DEF_CONFIG_PATH);
}

//-----------------------------------------------------------------------------
// 名前	: ~CNetConfig
// 内容	: 破棄
// 引数	: なし
// 戻値	: なし
//-----------------------------------------------------------------------------
CNetConfig::~CNetConfig()
{
}

//-----------------------------------------------------------------------------
// 名前	: LoadConfig
// 内容	: コンフィグ情報の読み込み
// 引数	: なし
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::LoadConfig(void)
{
	StLogPrm	stLog;
	BOOL		bRet;

#define	GET_LOG_DW(x)	\
	bRet = GetDataDW(LOG_SECTION_NAME, #x, &stLog.dw##x);\
	if( !bRet ) return FALSE;

#define	GET_LOG_STR(x,c)	\
	bRet = GetDataStr(LOG_SECTION_NAME, #x, stLog.sz##x, c);\
	if( !bRet ) return FALSE;

	// Logパラメータ読み出し
	GET_LOG_STR(SysLogPath, MAX_LOG_PATH);
	GET_LOG_DW(SysLogDays);
	GET_LOG_DW(SysLogSw);

	// パラメータ採用
	m_stLog = stLog;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: SaveConfig
// 内容	: コンフィグ情報の保存
// 引数	: なし
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::SaveConfig(void)
{
	StLogPrm	stLog;
	BOOL		bRet;

#define	SET_LOG_DW(x)	\
	bRet = SetDataDW(LOG_SECTION_NAME, #x, stLog.dw##x);\
	if( !bRet ) return FALSE;

#define	SET_LOG_STR(x,c)	\
	bRet = SetDataStr(LOG_SECTION_NAME, #x, stLog.sz##x, c);\
	if( !bRet ) return FALSE;

	// get current parameter.
	stLog = m_stLog;

	// Logパラメータ読み出し
	SET_LOG_STR(SysLogPath, MAX_LOG_PATH);
	SET_LOG_DW(SysLogDays);
	SET_LOG_DW(SysLogSw);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: SetConfigPath
// 内容	: コンフィグ情報のパスを設定
// 引数	: pszPath	... パスのアドレス
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetConfigPath(LPCSTR pszPath)
{
	int	len, max;

	// 異常アドレスチェック
	if( pszPath == NULL )
		return FALSE;

	// 指定できるパス長を得る
	len = strlen(CONFIG_NAME);
	max = MAX_CONFIG_PATH - len - 1;

	// パス長チェック
	len = strlen(pszPath);
	if( len > max )
		return FALSE;

	// パスを更新
	strncpy(m_szPath, pszPath, len);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: GetConfigPath
// 内容	: コンフィグ情報のパスを取得
// 引数	: なし
// 戻値	: パスのアドレス
//-----------------------------------------------------------------------------
LPCTSTR CNetConfig::GetConfigPath(void)
{
	return m_szPath;
}

//-----------------------------------------------------------------------------
// 名前	: GetDataStr
// 内容	: パラメータ（文字列データ）読み込み
// 引数	: szSession	... セッション名
//		: szName	... キー名
//		: pszString	... 文字列格納アドレス
//		: nSize		... バッファサイズ
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::GetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	uiRet = GetPrivateProfileString(szSession, szName, "", szData, nSize, szPath);
	if( uiRet == 0 )
		return FALSE;

	strncpy(pszString, szData, nSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: GetDataDW
// 内容	: パラメータ（DWORDデータ）読み込み
//		: 注）－１は異常認識に使用しているので使用不可
// 引数	: szSession	... セッション名
//		: szName	... キー名
//		: pData		... データ格納アドレス
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::GetDataDW(const char *szSession, const char *szName, DWORD	*pData)
{
	char	szPath[MAX_CONFIG_PATH];
	UINT	uiData;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	uiData = GetPrivateProfileInt(szSession, szName, -1, szPath);
	if( uiData == -1 )
		return FALSE;

	*pData = (DWORD)(uiData);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: SetDataStr
// 内容	: パラメータ（文字列データ）読み込み
// 引数	: szSession	... セッション名
//		: szName	... キー名
//		: pszString	... 文字列格納アドレス
//		: nSize		... バッファサイズ
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	strncpy(szData, pszString, nSize);

	uiRet = WritePrivateProfileString(szSession, szName, szData, szPath);
	if( uiRet == 0 )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 名前	: SetDataDW
// 内容	: パラメータ（DWORDデータ）読み込み
//		: 注）－１は異常認識に使用しているので使用不可
// 引数	: szSession	... セッション名
//		: szName	... キー名
//		: pData		... データ格納アドレス
// 戻値	: TRUE：正常、FALSE：異常
//-----------------------------------------------------------------------------
BOOL CNetConfig::SetDataDW(const char *szSession, const char *szName, DWORD	dwData)
{
	char	szPath[MAX_CONFIG_PATH];
	char	szData[MAX_DATA_SIZE];
	UINT	uiRet;
	
	sprintf(szPath, "%s%s", m_szPath, CONFIG_NAME);
	
	sprintf(szData, "%d", dwData);

	uiRet = WritePrivateProfileString(szSession, szName, szData, szPath);
	if( uiRet == 0 )
		return FALSE;

	return TRUE;
}
