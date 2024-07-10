#include "resource.h"

#define CH_ENGLISHPREFIX    0x1E    // 英語モードのアクセスキープレフィクス
#define CH_KANJIPREFIX      0x1F    // カタカナモードのアクセスキープレフィクス
#define KMM_ENGLISH         2       // 英語モード
#define KMM_KANJI           3       // カタカナモード

//
// DDLのエントリ関数
//
#if	defined(WIN32)
__declspec( dllexport ) int CALLBACK InitHooksDll(HWND);
__declspec( dllexport ) int CALLBACK InstallFilter(int);
#else
int __export CALLBACK InitHooksDll(HWND);
int __export CALLBACK InstallFilter(int);
#endif
