//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

#define		STRICT
#include	<windows.h>

#include	"../hcopy/hcopy.h"
#include	"print.h"
#include	"capture.h"

#define		PrintScreen 44

//---------------------------------------------------------------------------
// 関数宣言
//---------------------------------------------------------------------------
#if	defined(WIN32)
LRESULT CALLBACK KeyboardFunc( int, WPARAM, LPARAM );
#else
int   __export CALLBACK LibMain( HINSTANCE hModule, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine );
int   __export CALLBACK _WEP( int bSystemExit );
int   __export CALLBACK InitHooksDll( HWND hwndMainWindow );
int   __export CALLBACK InstallFilter( int nCode );
int   __export CALLBACK KeyboardFunc( int nCode, WORD wParam, DWORD lParam );
#endif
//---------------------------------------------------------------------------
// グロ－バル変数
//---------------------------------------------------------------------------
HANDLE	hInstance;				// DLLのグローバルインスタンスハンドル
HWND	hwndMain;				// メインウィンドウのハンドル
								// このハンドルはアプリケーションから取得する
int	InitCalled = 0; 			// 初期化関数が呼び出されたか?


// フック状態
int HookStates = 0;

// フックハンドル
HHOOK hhookHooks;

// アクセスキー情報
char szKanjiMenu[] = "KanjiMenu";
char szEnglishMode[] = "r";
char szKanjiMode[]   = "k";
int  KanjiMenuMode = KMM_ENGLISH;

#if	defined(WIN32)

//---------------------------------------------------------------------------
// DllMain
//---------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
			hInstance = hModule;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
    }
	return TRUE;
}

#else

//---------------------------------------------------------------------------
// LibMain
//---------------------------------------------------------------------------
int __export CALLBACK LibMain( HINSTANCE hModule, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine )
{
	hInstance = hModule;
	return 1;
}

#pragma alloc_text( FIXEDSEG, _WEP )


//---------------------------------------------------------------------------
// WEP
//---------------------------------------------------------------------------
int __export CALLBACK _WEP( int bSystemExit )
{
	return( 1 );
}

#endif

//---------------------------------------------------------------------------
// InitHooksDll
//---------------------------------------------------------------------------
#if	defined( WIN32 )
__declspec( dllexport ) int CALLBACK InitHooksDll( HWND hwndMainWindow )
#else
int __export CALLBACK InitHooksDll( HWND hwndMainWindow )
#endif
{
	char     rgch[2];

	hwndMain = hwndMainWindow;

	// 現在ののアクセスキーの設定を取得する
	GetProfileString( (LPSTR)"Windows", (LPSTR)szKanjiMenu,
						  (LPSTR)szEnglishMode, (LPSTR)rgch, 2 );
	rgch[0] = (char)(DWORD)AnsiLower( (LPSTR)(LONG)rgch[0] );
	if ( rgch[0] == *( (LPSTR)szEnglishMode ) )
		KanjiMenuMode = KMM_ENGLISH;
	else if ( rgch[0] == *( (LPSTR)szKanjiMode ) )
		KanjiMenuMode = KMM_KANJI;

	InitCalled = 1;
	return( 0 );
}


//---------------------------------------------------------------------------
// InstallSysMsgFilter
//
// WH_SYSMSGFILTERのためのフィルタ関数をインストール
// またはインストール解除する関数です。
//
//---------------------------------------------------------------------------
#if	defined( WIN32 )
__declspec( dllexport ) int CALLBACK InstallFilter( int nCode )
#else
int __export CALLBACK InstallFilter( int nCode )
#endif
{

	if ( ! InitCalled )
	{
		return (-1);
	}
	if ( nCode )
	{
		hhookHooks =
			SetWindowsHookEx( WH_KEYBOARD,
						  (HOOKPROC)KeyboardFunc, (HINSTANCE)hInstance, 0 );
		HookStates = TRUE;
	}
	else
	{
		UnhookWindowsHookEx( hhookHooks );
		HookStates = FALSE;
		InvalidateRect( hwndMain, NULL, TRUE );
		UpdateWindow( hwndMain );
	}
	return 0;
}


//---------------------------------------------------------------------------
// KeyboardFunc
//
// WH_KEYBOARDのためのフィルタ関数です。
//
//---------------------------------------------------------------------------
#if	defined(WIN32)
LRESULT CALLBACK KeyboardFunc( int nCode, WPARAM wParam, LPARAM lParam )
#else
int __export CALLBACK KeyboardFunc( int nCode, WORD wParam, DWORD lParam )
#endif
{
	HANDLE	hDIB;
	RECT	rect;
	static	int nRun = 0;	//tanno'070614

	if ( nCode >= 0 )
	{
		if ( wParam == PrintScreen && nRun == 0 )	//tanno'070614
		{
			nRun = 1;	//tanno'070614
			hDIB = CaptureFullScreen( hwndMain );
			if ( hDIB )
			{
				DIBPrint(hDIB,
						&rect,
						UNITS_STRETCHTOPAGE,
						SRCPAINT,
						TRUE,
						"HCOPY" );
				DestroyDIB( hDIB );		// kawai
			}
			nRun = 0;	//tanno'070614
		}
	}

   // メッセージを確認して処理を行ったが、確認しただけなので、
   // CallNextHookExにすべてのメッセージを渡す
#if	defined(WIN32)
	return CallNextHookEx( hhookHooks, nCode, wParam, lParam );
#else
	return( (int)CallNextHookEx( hhookHooks, nCode, wParam, lParam ) );
#endif
}

