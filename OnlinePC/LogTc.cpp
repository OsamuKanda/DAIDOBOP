#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"LogTc.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf		= theApp.m_cnfBoexe;
static TcName		*tcnm	= &theApp.m_stTcNm;


// ファイルタイムの加算
FILETIME FT_Add(FILETIME ft1, FILETIME ft2);
// ？日分のデータ取得
FILETIME FT_Day(INT nCount);
// ファイルタイムの比較
int FT_Comp(FILETIME ft1, FILETIME ft2);


///////////////////////////////////////////////////////////////////////////////
// member function

CLogTC::CLogTC()
{
	m_strLogPath = _T("..\\LogTC");
	m_nDays = 30;
}

// check and create file name
void CLogTC::CheckAndCreateFileName(void)
{
	if( m_strFileName.GetLength() == 0 )
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		// create file name
		m_strFileName.Format(_T("%04d%02d%02d%02d%02d%02d.txt"), 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
}

// clear file name
void CLogTC::ClearFileName(void)
{
	m_strFileName = _T("");
}

// write tc log file
// kind = 0:Auto cutoff, 1:manual cutoff, 2:manual recover
void CLogTC::WriteLogFile(int nKind, int nTcNo1, int nTcNo2, LPCTSTR timeStr)
{
	CString	strPath, strLine;
	FILE	*streem;

	if( m_strFileName.GetLength() == 0 )
	{
		CheckAndCreateFileName();
	}

	// delete old file
	DeleteOldLogFile();

	if( nTcNo1 < 0 || nTcNo1 >= NumTCTap || nTcNo2 < 0 || nTcNo2 >= NumTCTap )
		return;

	try
	{
		strPath.Format("%s\\%s\\%s", cnf.WorkRootDir, m_strLogPath, m_strFileName);

		if(NULL == (streem = fopen(strPath, "a+t")))
		{
			return;
		}

		switch(nKind)
		{
		case 0://auto cutoff
			strLine.Format(_T("%s %s,%s 自動カットオフ\r\n"), timeStr, tcnm->name[nTcNo1], tcnm->name[nTcNo2]);
			fputs(strLine, streem);
			break;
		case 1://manual cutoff
			strLine.Format(_T("%s %s 手動カットオフ\r\n"), timeStr, tcnm->name[nTcNo1]);
			fputs(strLine, streem);
			break;
		case 2:
			strLine.Format(_T("%s %s 手動復旧\r\n"), timeStr, tcnm->name[nTcNo1]);
			fputs(strLine, streem);
			break;
		default:
			break;
		}

		fclose(streem);
		streem = NULL;
	}
	catch(...)
	{
		if(streem != NULL)
			fclose(streem);
	}
}

// delete old log file
void CLogTC::DeleteOldLogFile(void)
{
	_TCHAR	buf[MAX_PATH];
	HANDLE			fp;
	WIN32_FIND_DATA	fd; 

	try
	{
		sprintf(buf, "%s\\%s\\*.*", cnf.WorkRootDir, m_strLogPath);
		fp = FindFirstFile(buf, &fd);

		FILETIME ft;
		SYSTEMTIME st;

		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &ft);

		if(INVALID_HANDLE_VALUE != fp)
		{
			for(BOOL f = TRUE;f == TRUE;)
			{
				if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if( FT_Comp(FT_Add( fd.ftLastWriteTime, FT_Day(m_nDays)), ft) < 0)
					{
						sprintf(buf, "%s\\%s\\%s", cnf.WorkRootDir, m_strLogPath, fd.cFileName);
						DeleteFile(buf);
					}
				}
				f = FindNextFile(fp, &fd);
			}
		}
		FindClose(fp);

	}
	catch(...)
	{
	}
}

LPCTSTR CLogTC::GetTimeStr(void)
{
	time_t			t;
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	t	= time(NULL);
	tp	= localtime(&t);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

