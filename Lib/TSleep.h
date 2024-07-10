// TSleep.h: CTSleep クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLEEP_H_)
#define AFX_TSLEEP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

class CTSleep  
{
public:
	CTSleep();
	virtual ~CTSleep();

public:
	void Sleep(DWORD dwMsec);
	void Sleep(DWORD dwMsec, LPDWORD pdwStop, DWORD dwTick = 1);	// 停止フラグ付

private:
	void StartSleep(void);
	void WatchSleep(LPDWORD pdwStop, DWORD dwTick);
};

#endif // !defined(AFX_TSLEEP_H_)
