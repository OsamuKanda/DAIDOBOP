#include	<windows.h>
#include	<windowsx.h>
#include	<io.h>
#include	<TCHAR.H>
#include    "trenddir.h"




typedef int (*COMP)(const void *, const void *);
	int cmpTrendEnt(TrendDirectory *p, TrendDirectory *q);

BOOL RegKeyGet( LPCTSTR lpValueName, LPTSTR lpRegKeyData )
{
	HKEY	hkResult;
	TCHAR	bData[256];
	DWORD	dwType=0, dwcbData = sizeof(bData);
	LONG	errcd;

	// まずレジストリを開く
	if ( (errcd = RegOpenKeyEx(
					HKEY_CURRENT_USER,		// 開いている親キーのハンドル
					_T("Software\\BO System"),	// 開くべきサブキーの名前
					0,						// 予約済み
					KEY_READ,				// セキュリティアクセスマスク
					&hkResult ))			// 開くことに成功したサブキーのハンドル
					== ERROR_SUCCESS )
	{
		// レジストリのデータ型を確認する(これを単独で行わないと次行のキーデータの読み込みが出来ない)
		if ( (errcd = RegQueryValueEx(
						hkResult,			// キーのハンドル
						lpValueName,		// レジストリエントリ名
						NULL,				// 予約済み(NULLでないと異常になる)
						&dwType,			// データ型が格納されるバッファ
						NULL,				// データが格納されるバッファ
						NULL ))				// データバッファのサイズ
						== ERROR_SUCCESS && dwType == REG_SZ )
		{
			// キーの内容を読み取る
			if( (errcd = RegQueryValueEx(
							hkResult,		// キーのハンドル
							lpValueName,	// レジストリエントリ名
							NULL,			// 予約済み(NULLでないと異常になる)
							&dwType,		// データ型が格納されるバッファ
							(LPBYTE)&bData[0],// データが格納されるバッファ
							&dwcbData ))	// データバッファのサイズ
							== ERROR_SUCCESS )
					{
						lstrcpy( lpRegKeyData, bData );
					}
		}
		RegCloseKey( hkResult );		// 閉じるべきキーのハンドル
	}
	if ( errcd == ERROR_SUCCESS )
		return	TRUE;					// 正常
	else
		return	FALSE;					// 異常
}

BOOL	dirPickUp( void )
{
	if ( RegKeyGet( _T("ROOTDIR"), &RootDir[0] ))
	{
		if ( RegKeyGet( _T("DATADIR"), &LogRootDir[0] ))
		{
			lstrcat( lstrcpy( WorkRootDir, RootDir ), WorkDir );
			lstrcat( lstrcpy( TrendAxisFile, WorkRootDir ), TrendAxisFileName );
			lstrcat( lstrcpy( SteelKindFile, WorkRootDir ), SteelKindFileName );
			lstrcat( lstrcpy( TrendChartPenFile, WorkRootDir ), TrendChartPenFileName );
			lstrcat( lstrcpy( LogRootDirBackDir, LogRootDir ), LogRootDirBack );
			return	TRUE;				// 正常
		}
	}
	return	FALSE;						// 異常
}

/*===================================================*/
/* get trend file entry in the trend file directory.
*/
int getTrendDirectory(TrendDirectory *table)
{
	TrendDirectory  *tp;
	int n = 0;
	struct	_tfinddata_t	fInfo;
	long	hFile;
	_TCHAR	buf[_MAX_PATH];
	int	stat = 0;
	_TCHAR    *p, *q;
	_TCHAR	dirName[MAX_PATH];

	lstrcat( lstrcpy( dirName, LogRootDir ), _T("\\*.*") );
	if ((hFile = _tfindfirst( dirName, &fInfo )) != -1L )
	{
//	if ((hFile = _findfirst(LogRootDir "\\*.*", &fInfo)) != -1L) {
		/* while the trend file existing. */
		for (tp = table; !stat;) {
			/* get trend file entry. */
			/* if the entry is not sub directory, */
			if (!(fInfo.attrib & _A_SUBDIR) && *fInfo.name != '.') {
				/* make up file name. */
				lstrcpy(tp->name, fInfo.name);
				/* make up trend start time. */
				for (p = fInfo.name, q = buf; *p; *p++)
					if (*p != '.')
						*q++ = *p;
				*q = '\0';
				_stscanf(buf, _T("%ld"), &tp->startTime);
				lstrcpy(buf, _tctime(&tp->startTime));
				for (p = buf, q = tp->date; *p && *p != '\n'; p++)
					*q++ = *p;
				*q = '\0';
				/* make up the file flags. */
				tp->flags = fInfo.attrib;
				/* make up the number of total data in the file. */
				tp->totalTick =
					(fInfo.size - sizeof(DataFileHeader)) / sizeof(SampledData);
				tp++, n++;
			}
			/* search directry entry. */
			stat = _tfindnext(hFile, &fInfo);
		}
		_findclose(hFile);
	}
	/* if one or more entry had been found, */
	if (n) {
		/* sort the entry from oldest file. */
		qsort((void *)table, (size_t)n, sizeof(*table), (COMP)cmpTrendEnt);
	}
	/* return the number of entry. */
	return  n;
}

/*===================================================*/
/* comparizon of trend file entry.
*/
int cmpTrendEnt(TrendDirectory *p, TrendDirectory *q)
{
	if (p->startTime < q->startTime)
		return  -1;
	if (p->startTime > q->startTime)
		return  1;
	return  0;
}

/*===================================================*/
_TCHAR    *makeupTrendEntString(TrendDirectory *p)
{
	_TCHAR    buf[256];
	static _TCHAR str[256];      /* static use return pointer address */
	static DataFileHeader   header;
	FILE    *fp;
	_TCHAR	dirName[MAX_PATH];

	*str    = '\0';

	/* make up trend data file name. */
	lstrcat( lstrcat( lstrcpy( dirName, LogRootDir ), _T("\\") ), p->name );
//	strcat(strcpy(buf, LogRootDir "\\"), p->name);
	/* open and read the trend file header. */
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
//	if ((fp = fopen(buf, "rb")) == NULL)
		return  str;
	fread(&header, sizeof(header), 1, fp);
	/* close the trend file. */
	fclose(fp);

	lstrcat(lstrcpy(str, p->date), _T(" "));
	_stprintf(buf, _T("[%5.1lf min] "),
		(double)p->totalTick * header.header.samplePeriod * 0.001 / 60.0);
/* 8/20 */
if (p->flags & _A_RDONLY) {
#if		defined(JAPAN)
	lstrcat(buf," 登録済み      ");
#elif	defined(ENGLISH)
	lstrcat(buf, _T(" Protect File!!"));
#endif
}
	lstrcat(str, buf);

	return  str;
}
