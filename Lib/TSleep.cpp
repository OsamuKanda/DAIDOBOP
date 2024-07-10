// TSleep.cpp: CTSleep クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "TSleep.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CTSleep::CTSleep(){}
CTSleep::~CTSleep(){}

void CTSleep::Sleep(DWORD dwMsec)
{
	::Sleep(dwMsec);
}

void CTSleep::Sleep(DWORD dwMsec, LPDWORD pdwStop, DWORD dwTick)
{
	DWORD	dwStart, dwNow, dwBase;

	dwBase = 0;
	dwStart = GetTickCount();

	while( *pdwStop == 0 )
	{
		dwNow = GetTickCount();
		if(dwNow < dwStart)
		{
			dwBase = 0xffffffff - dwStart + 1;
			dwStart = 0;
		}

		if( (dwNow + dwBase - dwStart) > dwMsec )
			break;

		::Sleep(dwTick);
	}
}

