#include	"stdafx.h"
#include	"Config.h"


// read Sampling Period from configration file. 
BOOL CConfig::ReadSamplingPeriod(int sel, int *pSamplePeriod)
{
	FILE	*fp;
	_TCHAR	buf[MAX_PATH];
	int		numSample;

	BoSysInfo	stBsInfo;
	ReadBosysInfo(sel, &stBsInfo);
	
	// make up file name.
	_tcscat(_tcscat(_tcscat(_tcscpy( buf, WorkRootDir ), _T("\\") ), stBsInfo.szConfName ), SamplingFile );

	// open the configration file.
	if ((fp = _tfopen(buf, _T("rb"))) == NULL)
	{
		*pSamplePeriod = 500;
		return TRUE;	// ファイル無しなら固定値
	}

	// read data,
	if(_fgetts(buf, 256, fp) != NULL || !ferror(fp)) 
	{
		// if value is normal,
		if (_stscanf(buf, _T("%d"), &numSample) == 1)
		{
			*pSamplePeriod = numSample;
			fclose(fp);
			return TRUE; 
		}
	}

	// after reading all lines, close the file.
	fclose(fp);

	*pSamplePeriod = 500;
	return FALSE;
}

// save Sampling Period to configration file. 
int	CConfig::WriteSamplingPeriod(int sel, int *pSamplePeriod)
{
	FILE	*fp;
	_TCHAR	buf[MAX_PATH];

	BoSysInfo	stBsInfo;
	if( !ReadBosysInfo(sel, &stBsInfo) )
	{
		return FALSE;
	}

	// make directories.
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( buf, WorkRootDir ), _T("\\") ), stBsInfo.szConfName );
	_tmkdir(buf);

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( buf, WorkRootDir ), _T("\\") ), stBsInfo.szConfName ), SamplingFile );

	// open the configration file.
	if ((fp = _tfopen(buf, _T("w"))) == NULL)
		return FALSE;

	// write data
	if(_ftprintf(fp, _T("%d\n"), *pSamplePeriod) < 0)
	{
		fclose(fp);
		return FALSE;
	}

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}

