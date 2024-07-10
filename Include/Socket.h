#ifndef _SOCKET_H
#define _SOCKET_H
#include <winsock.h>
#include <tchar.h>

// -----------------------------------------------------------------
// 注）このifdefは触らない事
// -----------------------------------------------------------------
#ifdef SOCKET_EXPORTS
#define SOCKET_API __declspec(dllexport)
#else
#define SOCKET_API __declspec(dllimport)
#endif
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// 履歴		：Ver1.0作成	->　2001/07 丹野
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// クラス名	：CWinSock
// 内容		：WinSockを使う為の準備クラスです。
// -----------------------------------------------------------------
class SOCKET_API CWinSock
{
	// メンバ変数
	WSADATA		m_wsaData;
	int			m_nErr;	

public:
	~CWinSock();								// デストラクタ
	BOOL	InitWinSock( BYTE, BYTE);			// WinSock初期化
	int		GetError(void);						// 異常コード取得
};



// -----------------------------------------------------------------
// クラス名	：CSock1
// 内容		：winsock1.1クラスです。（同期型）
//			：接続や送受信などの時間のかかる関数はそれ用のスレッドから
//			：呼び出すようにしましょう。
//			：メンバ関数は通常用を使う事を推奨します。
// -----------------------------------------------------------------
class SOCKET_API CSock1
{
private:												
	SOCKET	m_sock;								// socket ハンドル
	int		m_nPort;							// ポート番号
	int		m_nType;							// ソケットタイプ
	int		m_nErr;								// エラーコード
public:
	CSock1( );									// コンストラクタ
	virtual	~CSock1( );							// デストラクタ

public:
	virtual	BOOL	Create( int, int, int);			// ソケット作成（基本）
	virtual	BOOL	Create( int );					// ソケット作成（通常用　TCP or UDP）
	virtual	void	Create( SOCKET, 
						int nType = SOCK_STREAM );	// ソケット作成（ACCEPT用）
	virtual	BOOL	Shutdown( int );				// ソケット送受信禁止（通常用）
	virtual	BOOL	Close( void );					// ソケットクローズ（通常用）
	virtual	BOOL	Bind( struct sockaddr * );		// バインド（基本）
	virtual	BOOL	Bind( DWORD, int);				// バインド（通常用　主にサーバー用）
	virtual	BOOL	Connect( struct sockaddr * );	// 接続（基本）
	virtual	BOOL	Connect( DWORD, int);			// 接続（通常用）
	virtual	SOCKET	Accept( struct sockaddr * );	// 接続を受け付ける（基本）
	virtual	SOCKET	Accept( void );					// 接続を受け付ける（通常用）
	virtual	BOOL	Listen( int );					// 接続待ちを通知（基本,通常用）
	virtual	int		Recv( LPSTR, int, int);			// 受信処理（基本,TCP用）
	virtual	int		Recv( LPSTR, int, int, 
						struct sockaddr *);			// 受信処理（基本,UDP用）
	virtual	int		Recv( LPVOID, int);				// 受信処理（通常用,TCP用）
	virtual int		Recv( LPVOID, int, LPDWORD, LPINT); // 受信処理（通常用,UDP用）
	virtual	int		Send( LPSTR, int, int);			// 受信処理（基本,TCP用）
	virtual	int		Send( LPSTR, int, int, 
						struct sockaddr *);			// 受信処理（基本,UDP用）
	virtual	int		Send( LPVOID, int);				// 受信処理（通常用,TCP用）
	virtual	int		Send( LPVOID, int, DWORD, int);	// 受信処理（通常用,UDP用）
	virtual	int		Select( int, fd_set *, fd_set *, 
						fd_set *, timeval *);		// ソケット状態調べる（基本）
	virtual	BOOL	Select( DWORD, DWORD );			// ソケット状態調べる（通常用）
	virtual	SOCKET	GetSocket( void );				// ソケットハンドル取得
//	virtual	int		GetPortNo( void );				// ソケットに結びついているポート番号取得
	virtual	int		GetError( void );				// エラーコード取得
	virtual int		GetSockType();					// ソケットタイプ取得( 1..TCP  2..UDP )
	virtual void	GetErrorMsg(LPTSTR pMsg);		//	エラーメッセージ取得
	virtual BOOL	SetAsyncSock( HWND hWnd,		//	同期 -> 非同期切替え
								  UINT uMsg,
								  long nEvent );
	virtual BOOL	Get_HostInfo(LPTSTR pHostName,
								 LPTSTR pAddr);
//	2004/11/11	add
	virtual BOOL	SetSockOut(int nMode, DWORD dwTimeout);	//	タイムアウト設定 nMode=1..送信 2..受信 dwTimeout=msec
//	2004/11/11	end


};


// -----------------------------------------------------------------
// クラス名	：CSock2
// 内容		：WinSock2.0クラスです。（作るかどうかは未定）
// -----------------------------------------------------------------
//class SOCKET_API CSock2
//{
//};
#endif