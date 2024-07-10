// Bolog.h : Configuration Interface
//
#pragma once

// include
#include "../include/Bodef.h"

// CConfig class
class CBolog
{
public:
	CBolog(){};
	~CBolog(){};

// member data
public:
	_TCHAR	LogRootDir[MAX_PATH];
	_TCHAR	LogRootDirBackDir[MAX_PATH];
	_TCHAR	m_szRetStr[256];
	_TCHAR	TempLogFilePath[MAX_PATH];

// member function
public:
	BOOL Initialize( void );
	BOOL DirPickUp( void );
	int GetTrendDirectory(TrendDirectory *table);
	int GetTrendDirectoryEx(LPCTSTR lpszDir, TrendDirectory *table);
	LPTSTR MakeupTrendEntString(TrendDirectory *p);
	LPTSTR MakeupTrendEntStringEx(LPCTSTR lpszDir, TrendDirectory *p);
	static int CmpTrendEnt(TrendDirectory *p, TrendDirectory *q);
	LPCTSTR GetTimeStr(time_t tm);
	BOOL SaveLogFileToTemp(LPCTSTR pSaveFileName);
	LPCTSTR GetTempLogFilePath(LPCTSTR pSaveFileName);
	void ChkTempLogFile(void);
	void DeleteTempFile(LPCTSTR lpszDir);
	DWORD DeleteFile2(LPCTSTR pFilePath);
};

