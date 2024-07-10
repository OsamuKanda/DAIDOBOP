// Bolog.cpp : �����t�@�C��
//
#include "stdafx.h"
#include "Bolog.h"
#include "resource.h"


// initialize bolog.
BOOL CBolog::Initialize( void )
{
	BOOL	ret;

	// load root directry name.
	ret = DirPickUp();
	if(ret == FALSE)
		return ret;

	return TRUE;
}

BOOL CBolog::DirPickUp( void )
{
	CRegKey	reg;
	DWORD	dwSize;

	if( reg.Open(HKEY_CURRENT_USER, _T("Software\\BOP")) != ERROR_SUCCESS )
		return FALSE;

	dwSize = sizeof(LogRootDir);
	if( reg.QueryValue(&LogRootDir[0], _T("DATADIR"), &dwSize) != ERROR_SUCCESS )
		return FALSE;

	_tcscat( _tcscpy( LogRootDirBackDir, LogRootDir ), LogRootDirBack );

	return TRUE;
}

// get trend file entry in the trend file directory.
int CBolog::GetTrendDirectory(TrendDirectory *table)
{
	return GetTrendDirectoryEx(LogRootDir, table);
}

// get trend file entry in the trend file directory.
int CBolog::GetTrendDirectoryEx(LPCTSTR lpszDir, TrendDirectory *table)
{
	TrendDirectory  *tp;
	int n = 0;
	struct	_tfinddata_t	fInfo;
	long	hFile;
	_TCHAR	buf[_MAX_PATH];
	int	stat = 0;
	_TCHAR    *p, *q;
	_TCHAR	dirName[MAX_PATH];
	SYSTEMTIME	st;
	int		nSize;
	BOOL	bOld = FALSE;

	_tcscat( _tcscpy( dirName, lpszDir ), _T("\\*.*") );
	if ((hFile = _tfindfirst( dirName, &fInfo )) != -1L )
	{
		// while the trend file existing.
		for (tp = table; !stat;)
		{
			// get trend file entry.
			// if the entry is not sub directory,
			if (!(fInfo.attrib & _A_SUBDIR) && *fInfo.name != _T('.')) 
			{
				// initialize trend directory info.
				memset(tp, 0, sizeof(*tp));

				// make up file name.
				_tcscpy(tp->name, fInfo.name);

				if( (p = _tcsstr(fInfo.name, _T(".log"))) != NULL )
				{
					*p = _T('\0');
					q = fInfo.name;
					_tcsncpy(buf, q, 5);buf[4] = _T('\0');q = &fInfo.name[4];
					_stscanf(buf, _T("%04d"), &st.wYear);
					_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &fInfo.name[6];
					_stscanf(buf, _T("%02d"), &st.wMonth);
					_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &fInfo.name[8];
					_stscanf(buf, _T("%02d"), &st.wDay);
					_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &fInfo.name[10];
					_stscanf(buf, _T("%02d"), &st.wHour);
					_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &fInfo.name[12];
					_stscanf(buf, _T("%02d"), &st.wMinute);
					_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &fInfo.name[14];
					_stscanf(buf, _T("%02d"), &st.wSecond);

					CTime	ct(st);
					tp->startTime = (time_t)ct.GetTime();
					bOld = FALSE;
				}
				else
				{
					// make up trend start time.
					for (p = fInfo.name, q = buf; *p; *p++)
						if (*p != '.')
							*q++ = *p;
					*q = '\0';
					if( 1 != _stscanf(buf, _T("%ld"), &tp->startTime) )
						tp->startTime = 0;
					bOld = TRUE;
				}

				if( tp->startTime != 0 )
				{
//					_tcscpy(buf, _tctime(&tp->startTime));
					_tcscpy(buf, GetTimeStr(tp->startTime));

					for (p = buf, q = tp->date; *p && *p != _T('\r') && *p != _T('\n'); p++)
						*q++ = *p;
					*q = '\0';
					// make up the file flags.
					tp->flags = fInfo.attrib;

					// make up the number of total data in the file.
//					if( tp->startTime < 948000000 )
//					{
					// �Â��`���̃t�@�C���Ȃ�
					if( bOld )
					{
#ifdef Compatibility
						DataFileHeader header;
						FILE *fp;
						_tcscat( _tcscat( _tcscpy( dirName, lpszDir ), _T("\\")), tp->name);
						fp = _tfopen(dirName, _T("rb"));
						fread(&header, sizeof(header), 1, fp);
						fclose(fp);
						if( header.header.version <= 500 )
							nSize = sizeof(SampledDataV5);
						else
							nSize = sizeof(SampledData);
#else
						nSize = sizeof(SampledData);
#endif
						tp->totalTick =
							(fInfo.size - sizeof(DataFileHeader)) / nSize;
						tp->smplTime = header.header.samplePeriod;
					}
					// �V�����`���̃t�@�C���Ȃ�
					else
					{
						tp->totalTick = (fInfo.size - sizeof(DataFileHeader)) / sizeof(SampledData);
						tp->smplTime = SamplingPeriod;
					}
					n++;
				}
				tp++;
			}
			// search directry entry. */
			stat = _tfindnext(hFile, &fInfo);
		}
		_findclose(hFile);
	}
	// if one or more entry had been found,
	if (n) {
		// sort the entry from oldest file.
		qsort((void *)table, (size_t)n, sizeof(*table), (COMP)CmpTrendEnt);
	}
	// return the number of entry.
	return  n;
}

// comparizon of trend file entry.
int CBolog::CmpTrendEnt(TrendDirectory *p, TrendDirectory *q)
{
	if (p->startTime < q->startTime)
		return  -1;
	if (p->startTime > q->startTime)
		return  1;
	return  0;
}

LPTSTR CBolog::MakeupTrendEntString(TrendDirectory *p)
{
	return  MakeupTrendEntStringEx(LogRootDir, p);
}

LPTSTR CBolog::MakeupTrendEntStringEx(LPCTSTR lpszDir, TrendDirectory *p)
{
	_TCHAR    buf[256];
//	DataFileHeader   header;
//	FILE    *fp;
//	_TCHAR	dirName[MAX_PATH];
	CString	cs;

	*m_szRetStr    = '\0';
/*
	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lpszDir ), _T("\\") ), p->name );

	// open and read the trend file header.
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
		return  m_szRetStr;
	fread(&header, sizeof(header), 1, fp);

	// close the trend file.
	fclose(fp);

	_tcscat(_tcscpy(m_szRetStr, p->date), _T(" "));
	_stprintf(buf, _T("[%5.1lf min] "),
		(double)p->totalTick * header.header.samplePeriod * 0.001 / 60.0);

	if (p->flags & _A_RDONLY) {
		cs.LoadString(IDS_BOLOG00);
		_tcscat(buf, cs);
	}
	_tcscat(m_szRetStr, buf);
*/
	_tcscat(_tcscpy(m_szRetStr, p->date), _T(" "));
	_stprintf(buf, _T("[%5.1lf min] "),
		(double)p->totalTick * p->smplTime * 0.001 / 60.0);

	if (p->flags & _A_RDONLY) {
		cs.LoadString(IDS_BOLOG00);
		_tcscat(buf, cs);
	}
	_tcscat(m_szRetStr, buf);

	return  m_szRetStr;
}

// �w�肳�ꂽ���Ԃ̕�����𐶐�
LPCTSTR CBolog::GetTimeStr(time_t tm)
{
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	tp	= localtime(&tm);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

// ���O�f�[�^��Temp�t�H���_�ɕۑ�����
BOOL CBolog::SaveLogFileToTemp(LPCTSTR pSaveFileName)
{
	CString	strPath, strSrc, strDst, strFile;
	int		ret, len, pos, mpos;

	// �t�H���_����
	strPath.Format(_T("%s%s"), LogRootDir, BOLogTempFolder);
	ret = _tmkdir(strPath);

	// �t�@�C�����R�s�[
	if( pSaveFileName != NULL )
	{
		// �t�@�C�������擾
		strFile = pSaveFileName;
		len = strFile.GetLength();
		pos = 0;
		while( (pos = strFile.Find(_T('\\'), pos)) != -1 )mpos = pos++;
		if( mpos != -1 && (len - mpos) > 0 )
		{
			strFile = strFile.Right(len - mpos);
		}

		// Temp�t�H���_�Ƀ��O�t�@�C�����R�s�[
		strSrc = pSaveFileName;
		strDst.Format(_T("%s%s"), strPath, strFile);
		CopyFile(strSrc, strDst, FALSE);

		// ���O�f�[�^�iTemp�j�̍폜�`�F�b�N
		ChkTempLogFile();
	}

	return	TRUE;
}

// ���O�f�[�^��Temp�t�H���_�ɕۑ�����
LPCTSTR CBolog::GetTempLogFilePath(LPCTSTR pSaveFileName)
{
	LPCTSTR pRet = NULL;
	CString	strPath, strSrc, strDst, strFile;
	int		len, pos, mpos;

	// �t�@�C�����R�s�[
	if( pSaveFileName != NULL )
	{
		// �t�@�C�������擾
		strFile = pSaveFileName;
		len = strFile.GetLength();
		pos = 0;
		while( (pos = strFile.Find(_T('\\'), pos)) != -1 )mpos = pos++;
		if( mpos != -1 && (len - mpos) > 0 )
		{
			strFile = strFile.Right(len - mpos);
		}

		// Temp�t�@�C���p�X�𐶐�
		strPath.Format(_T("%s%s"), LogRootDir, BOLogTempFolder);
		strDst.Format(_T("%s%s"), strPath, strFile);
		_tcscpy(TempLogFilePath, strDst);
		pRet = (LPCTSTR)TempLogFilePath;
	}

	return pRet;
}

// Temp�t�H���_�̃��O�t�@�C�����폜���邩�̃`�F�b�N
void CBolog::ChkTempLogFile(void)
{
    ULARGE_INTEGER i64UseBytes;   // �Ăяo���������p�ł���o�C�g��
    ULARGE_INTEGER i64TotalBytes; // �f�B�X�N�S�̂̃o�C�g��
    ULARGE_INTEGER i64FreeBytes;  // �f�B�X�N�S�̂̋󂫃o�C�g��
	CString	strPath;
	int		mb;					  // �󂫗e��(MB)

	// �󂫗e�ʂ𒲂ׂ�
    if(GetDiskFreeSpaceEx(LogRootDir, &i64UseBytes, &i64TotalBytes, &i64FreeBytes) ) 
	{
		// �󂫗e�ʂ��ݒ�l�����Ȃ�ł��Â��A�N�Z�X���Ԃ̃t�@�C�����폜
		mb	= (int)(i64FreeBytes.QuadPart / (1024*1024));
		if( mb < CHK_FREE_SPACE )
		{
			strPath.Format(_T("%s%s"), LogRootDir, BOLogTempFolder);
			DeleteTempFile(strPath);
		}
	}
}

// �ł��Â��A�N�Z�X���Ԃ̃t�@�C�����폜����
void CBolog::DeleteTempFile(LPCTSTR lpszDir)
{
	struct	_tfinddata_t	fInfo;
	_TCHAR		dirName[MAX_PATH];
	_TCHAR		oldName[MAX_PATH];
	__time32_t	old_t = 0;
	long		hFile;
	int			stat = 0;
	int			ret = 0;

	_tcscat( _tcscpy( dirName, lpszDir ), _T("\\*.*") );
	if ((hFile = _tfindfirst( dirName, &fInfo )) != -1L )
	{
		// while the trend file existing.
		for (; !stat;)
		{
			// get trend file entry.
			// if the entry is not sub directory,
			if (!(fInfo.attrib & _A_SUBDIR) && *fInfo.name != _T('.')) 
			{
				// ����͂��̂܂ܐݒ�
				if( old_t == 0 )
				{
					_tcscpy(oldName, fInfo.name);
					old_t = fInfo.time_access;
				}
				// �Q��ڈȍ~��
				else
				{
					// �ł��Â�������T��
					if( old_t > fInfo.time_access )
					{
						// ���O�Ǝ������L�^
						_tcscpy(oldName, fInfo.name);
						old_t = fInfo.time_access;
					}
				}
			}

			// ���̃G���g��
			stat = _tfindnext(hFile, &fInfo);
		}

		// �I��
		_findclose(hFile);

		// ��ԌÂ��t�@�C��������������폜
		if( old_t != 0 )
		{
			// �폜�p�X�쐬�i�Ō��NULL���Q�K�v�j
			_stprintf( dirName, _T("%s\\%s%c"), lpszDir, oldName, 0 );

			// �폜
			DeleteFile2(dirName);
		}
	}
}

// ���C���h�J�[�h�\�ȃt�@�C���폜�i�G���[�\���Ȃ��j
// �p�X�̏I�[��\0���Q�K�v
DWORD CBolog::DeleteFile2(LPCTSTR pFilePath)
{
	// copy file.
	SHFILEOPSTRUCT st;
	st.hwnd		= NULL;
	st.wFunc	= FO_DELETE; 
	st.pFrom	= pFilePath;
	st.pTo		= NULL;
	st.fFlags	= FOF_NOCONFIRMATION|FOF_NOERRORUI;
	if( 0 != SHFileOperation(&st) )
	{
		if( st.fAnyOperationsAborted == TRUE )
			return ERROR_CANCELLED;
		else
			return GetLastError();
	}

	return 0L;
}
