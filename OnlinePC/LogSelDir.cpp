#include	"stdafx.h"
#include	"Config.h"


// load log selection file name
BOOL CConfig::LoadLogSelFileName(LPCTSTR pSaveFileName, LPTSTR pLogName)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];
	_TCHAR	sLogName[MAX_PATH];

	if( pSaveFileName == NULL )
		return FALSE;

	_tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), pSaveFileName );
	fp	= _tfopen( dirName, _T("rt") );
	if( fp == NULL )
		return FALSE;
	if( NULL != _fgetts(sLogName, MAX_PATH, fp) )
		_tcscpy(pLogName, sLogName);
	fclose(fp);


	return	TRUE;
}

// save log selection file name
BOOL CConfig::SaveLogSelFileName(LPCTSTR pSaveFileName, LPTSTR pLogName)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];
	_TCHAR	sLogName[MAX_PATH];

	if( pSaveFileName == NULL )
		return FALSE;

	_tcscpy(sLogName, pLogName);

	_tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), pSaveFileName );
	fp	= _tfopen( dirName, _T("wt") );
	if( fp == NULL )
		return FALSE;
	_fputts(sLogName, fp);
	fclose(fp);


	return TRUE;
}
