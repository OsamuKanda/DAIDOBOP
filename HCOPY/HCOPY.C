//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

#define		STRICT
#include	<windows.h>
#include	"hcopy.h"

#define		TimerID	1
//---------------------------------------------------------------------------
// 関数宣言
//---------------------------------------------------------------------------
int PASCAL WinMain( HINSTANCE, HINSTANCE, LPSTR, int );
#if	defined( WIN32 )
LRESULT CALLBACK HcopyWndProc( HWND, UINT, WPARAM, LPARAM );
#else
LONG __export CALLBACK HcopyWndProc( HWND, UINT, WPARAM, LPARAM );
#endif

//---------------------------------------------------------------------------
// グロ－バル変数
//---------------------------------------------------------------------------
HINSTANCE hInstance;		// アプリケーションのグローバルインスタンスハンドル
HWND	hwndMain;			// メインウィンドウのハンドル(コールバックで必要)

int nTimer;					/* Timer unit 						*/


//---------------------------------------------------------------------------
// WinMain
//---------------------------------------------------------------------------
int PASCAL WinMain( HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpstrCmdLine, int cmdShow )
{
	MSG        msgMain;
	WNDCLASS   wc;

	// グローバルインスタンス変数を設定
	hInstance = hInst;

	// このインスタンスが最初のインスタンスならば、ウィンドウクラスを登録
	if ( hInstPrev == NULL )
	{
		wc.lpszMenuName     = "HCOPY";
		wc.lpszClassName    = "HCOPY";
		wc.hInstance        = hInst;
		wc.hIcon	    	= LoadIcon( hInst, "HCOPY" );
		wc.hCursor          = LoadCursor( NULL, IDC_ARROW );
		wc.hbrBackground    = (HBRUSH)COLOR_WINDOW + 1;
		wc.style            = 0;
		wc.lpfnWndProc	    = HcopyWndProc;
		wc.cbClsExtra       = 0;
		wc.cbWndExtra       = 0;

		if ( !RegisterClass( &wc ) )
			return( 0 );
	}

	// メインウィンドウを作成
	if ((hwndMain = CreateWindow( "HCOPY",
									"HCOPY",
									WS_OVERLAPPED | WS_CAPTION |
									WS_SYSMENU | WS_MINIMIZEBOX,
//									WS_OVERLAPPEDWINDOW,
									0, 0,
									1, 1,
									NULL, NULL, hInst, NULL ) ) == NULL )
		return( 0 );

	// ウィンドウを表示し、必ず更新されるようにする
	ShowWindow( hwndMain, cmdShow );
	UpdateWindow( hwndMain );

	// ウィンドウを更新できるように、hwndMainをDLLに渡す
	InitHooksDll( hwndMain );

	ShowWindow( hwndMain, SW_SHOWMINIMIZED );

	// メインメッセージの取得とディスパッチ
	while( GetMessage( (LPMSG) &msgMain, NULL, 0, 0 ) )
	{
		TranslateMessage( (LPMSG) &msgMain );
		DispatchMessage( (LPMSG) &msgMain );
	}
	return( 0 );
}


//---------------------------------------------------------------------------
// HookSampleWndProc
//
// ウィンドウのためのウィンドウプロシージャ
//
//---------------------------------------------------------------------------
#if	defined( WIN32 )
LRESULT CALLBACK HcopyWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
#else
LONG __export CALLBACK HcopyWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
#endif
{
	switch( msg )
	{
		case WM_CREATE:
			nTimer = (int)SetTimer( hwnd, TimerID, 10, NULL );
			break;
		case WM_TIMER:
			if ( InstallFilter( TRUE ) != -1 )
			{
				KillTimer( hwnd, nTimer );
			}
			break;

		case WM_DESTROY:
			InstallFilter( FALSE );
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	}
	return( 0 );
}
