// SysConfig.h: CNetConfig クラスのインターフェイス
//
#if !defined(AFX_SYSCONFIG_H_INCLUDED_)
#define AFX_SYSCONFIG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//===================================================================
// 定数
//===================================================================
#define	DEF_CONFIG_PATH		"../Config/"
#define	CONFIG_NAME			"Log.ini"	
#define	LOG_SECTION_NAME	"Log"
#define	MAX_DATA_SIZE		256
#define	MAX_LOG_PATH		256
#define	MAX_CONFIG_PATH		MAX_PATH

//===================================================================
// 構造体定義
//===================================================================
// ログパラメータ
typedef struct
{
	char	szSysLogPath[MAX_LOG_PATH];			// システムログパス
	DWORD	dwSysLogDays;						// システムログ保存日数
	DWORD	dwSysLogSw;							// システムログ保存有無
}StLogPrm;

//===================================================================
// クラス情報
//===================================================================
class CNetConfig  // 継承しない
{
// 構築＆破棄
public:
	CNetConfig();						// 構築
	~CNetConfig();						// 破棄

/////////////////////////////
// メンバ変数（外部公開）
public:
	StLogPrm	m_stLog;				// ログパラメータ

/////////////////////////////
// メンバ変数（外部非公開）
private:
	char	m_szPath[MAX_CONFIG_PATH];	// コンフィグ情報のパス

/////////////////////////////
// メソッド（外部公開）
public:
	BOOL LoadConfig(void);				// コンフィグ情報を読み込む
	BOOL SaveConfig(void);				// コンフィグ情報を書き込む
	BOOL SetConfigPath(LPCTSTR);		// コンフィグ情報のパスを設定
	LPCTSTR GetConfigPath(void);		// コンフィグ情報のパスを取得

/////////////////////////////
// メソッド（外部非公開）
private:
	BOOL GetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize);
										// パラメータ（文字列データ）読み込み
	BOOL GetDataDW(LPCTSTR szSession, LPCTSTR szName, DWORD	*pData);
										// パラメータ（DWORDデータ）読み込み
	BOOL SetDataStr(LPCTSTR szSession, LPCTSTR szName, LPTSTR pszString, int nSize);
										// パラメータ（文字列データ）書き込み
	BOOL SetDataDW(const char *szSession, const char *szName, DWORD	dwData);
										// パラメータ（DWORDデータ）書き込み
};

#endif // !defined(AFX_SYSCONFIG_H_INCLUDED_)
