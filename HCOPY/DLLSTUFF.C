//---------------------------------------------------------------------------
//  DLLSTUFF - Windows DLLサポート関数
//
//  このファイルには、標準DLL関数のためのソースコードが含まれています。
//
//  Copyright(C) Microsoft Corp.
//
//---------------------------------------------------------------------------

#include "windows.h"

//---------------------------------------------------------------------------
// 関数宣言
//---------------------------------------------------------------------------
int   FAR PASCAL LibMain(HANDLE hModule, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine);
int   FAR PASCAL WEP (int bSystemExit);

//---------------------------------------------------------------------------
// グロ−バル変数
//---------------------------------------------------------------------------
HANDLE	hInstance;		// DLLのグローバルインスタンスハンドル

//---------------------------------------------------------------------------
// LibMain
//---------------------------------------------------------------------------
int FAR PASCAL LibMain(HANDLE hModule, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
{
    hInstance = hModule;
    return 1;
}

#pragma alloc_text(FIXEDSEG, WEP)

//---------------------------------------------------------------------------
// WEP
//---------------------------------------------------------------------------
int FAR PASCAL WEP (int bSystemExit)
{
    return(1);
}
