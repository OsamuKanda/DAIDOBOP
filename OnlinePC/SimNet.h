#ifndef _SIMNET_H
#define _SIMNET_H

#include <windows.h>

// -----------------------------------------------------------------
// íçÅjÇ±ÇÃifdefÇÕêGÇÁÇ»Ç¢éñ
// -----------------------------------------------------------------
#ifdef SIMNET_EXPORTS
#define SIMNET_API __declspec(dllexport)
#else
#define SIMNET_API __declspec(dllimport)
#endif

extern "C"{
SIMNET_API int		InitSimNet(HWND hWnd, LPWSTR *pszItemIDR, int nItemCntR, LPWSTR *pszItemIDW, int nItemCntW);
SIMNET_API VARIANT	*GetDevData(void);
SIMNET_API int		SetDevData(VARIANT *pValues);
SIMNET_API int		UninitSimNet(void);
}

#endif//_SIMNET_H
