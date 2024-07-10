#include	"stdafx.h"
#include	"Config.h"

// read TC exception file.
void CConfig::ReadTcExpFile(int *pbTcExp)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];

	// make up file name. 
	_tcscat( _tcscpy( dirName, WorkRootDir ), TCExceptFileName );

	// open the configration file. 
	if((fp = _tfopen( dirName, _T("rb") )) == NULL)
	{
		if((fp = _tfopen( dirName, _T("wb+") )) == NULL)
			return;
		fwrite( pbTcExp, sizeof(int) * NumADChannel, 1, fp );
		fclose(fp);
		return;
	}

	// read TC exception info.
	fread( pbTcExp, sizeof(int) * NumADChannel, 1, fp );

	// close the file. 
	fclose(fp);
}

// write TC exception file.
void CConfig::WriteTcExpFile(int *pbTcExp)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];

	// make up file name. 
	_tcscat( _tcscpy( dirName, WorkRootDir ), TCExceptFileName );

	// open the configration file. 
	if((fp = _tfopen( dirName, _T("wb+") )) == NULL)
		return;

	// write TC exception info.
	fwrite( pbTcExp, sizeof(int) * NumADChannel, 1, fp );

	//  close the file. 
	fclose(fp);
}


// read tc except setting file.
BOOL CConfig::ReadTcExpSetFile(StTcExpSet *pInfo)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];
	_TCHAR	buf[1000];
	StTcExpSet dummy = {0};

	// make up file name. 
	_tcscat( _tcscpy( dirName, WorkRootDir ), TCExceptSetFileName );

	// open the trend graph axis definition file. if not found,
	if ((fp = _tfopen( dirName, _T("r") )) == NULL) 
	{
		pInfo->dTempMin		= 0.0;
		pInfo->dTempMax		= 500.0;
		pInfo->nWatchTime	= 5;
		pInfo->dDeltaT		= 10.0;
		pInfo->nCheckTime	= 1;
		return FALSE;
	}
	// otherwise, read the definition.
	else 
	{
		fgets(buf, sizeof(buf), fp);
		fscanf(fp, "%lf,%lf,%d,%d,%lf", 
			&dummy.dTempMin, &dummy.dTempMax, &dummy.nWatchTime,
			&dummy.nCheckTime, &dummy.dDeltaT);
		fclose(fp);

		if( dummy.nCheckTime > (TCEXP_CHK_TIME_MAX-1) )
			dummy.nCheckTime = (TCEXP_CHK_TIME_MAX-1);

		memcpy(pInfo, &dummy, sizeof(dummy));
	}

	return TRUE;
}

// write tc except setting file.
BOOL CConfig::WriteTcExpSetFile(StTcExpSet *pInfo)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];

	// make up file name. 
	_tcscat( _tcscpy( dirName, WorkRootDir ), TCExceptSetFileName );

	// open the trend graph axis definition file. if not found,
	if ((fp = _tfopen( dirName, _T("wb+") )) == NULL) 
		return FALSE;

	//TempMin,TempMax,WatchTime(sec),CheckTime(sec:Max20sec),deltaTemp
	// write data
	if(_ftprintf(fp, _T("TempMin,TempMax,WatchTime(sec),CheckTime(sec:Max20sec),deltaTemp\n")) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// write data
	if(_ftprintf(fp, _T("%lf,%lf,%d,%d,%lf\n"), 
		pInfo->dTempMin, pInfo->dTempMax, pInfo->nWatchTime,
		pInfo->nCheckTime, pInfo->dDeltaT) < 0)
	{
		fclose(fp);
		return FALSE;
	}
	fclose(fp);

	return TRUE;
}

