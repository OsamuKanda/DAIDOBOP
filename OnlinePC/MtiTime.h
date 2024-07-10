/*=============================================================================
  ファイル名    : CMultiTimer.h
  説明			: マルチメディアタイマクラス(継承を考慮してません)
===============================================================================
 history |   date     |     name     |              comment
---------+------------+--------------+-----------------------------------------
		 | 2002.03.XX |  K.Tanno     | Create.
=============================================================================*/
#ifndef	_INCCMULTITIMER_H
#define _INCCMULTITIMER_H


#include <mmsystem.h>


// -----------------
// クラス宣言
// -----------------
class CMultiTimer
{
public:	
	CMultiTimer();									// コンストラクタ
	~CMultiTimer();									// デストラクタ

private:
	INT		m_ID;									// タイマーID
	INT		m_uiTime;								// タイマー周期
	
public:
	BOOL SetTimer(UINT, LPTIMECALLBACK, LPVOID);	// タイマー起動
	BOOL KillTimer();								// タイマー終了
};

#endif // _INCCMULTITIMER_H

