#pragma once
#include    "../include/bodef.h"

// CLogging class
class CLogTC
{
public:
	CLogTC(); 

// member data
public:
	CString		m_strFileName;
	CString		m_strLogPath;
	int			m_nDays;

// member function
public:
	void CheckAndCreateFileName(void);
	void ClearFileName(void);
	void WriteLogFile(int nKind, int nTcNo1, int nTcNo2, LPCTSTR timeStr);
	void DeleteOldLogFile(void);
	LPCTSTR GetTimeStr(void);
private:
};
