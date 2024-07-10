#include	"stdafx.h"
#include	"Config.h"


// load work directory name
BOOL CConfig::LoadWorkDirName(void)
{
	FILE	*fp;							// file handle
	_TCHAR	name[SizeConfig];				// configuration file name
	_TCHAR	dirName[MAX_PATH];
	_TCHAR	*p;

	// open the file which includes work directory name.
	_tcscat( _tcscpy( dirName, WorkRootDir ), WorkDirFileName );
	if ((fp = _tfopen( dirName, _T("r") )) == NULL) 
	{
		*name	= _T('\0');
	}
	else {
		_fgetts(name, SizeConfig, fp);
		for (p = name; *p && *p != _T('\n') && *p != _T('\r'); p++)
			;
		*p	= _T('\0');
		// after reading all lines, close the file.
		fclose(fp);
	}
	_tcsncpy(workDir, name, SizeConfig);
	_tcsncpy(oldWorkDir, name, SizeConfig);

	return	TRUE;
}

// save work directory name
BOOL CConfig::SaveWorkDirName(void)
{
	FILE	*fp;
	_TCHAR	dirName[MAX_PATH];

	_tcscat( _tcscpy( dirName, WorkRootDir ), WorkDirFileName );
	fp	= _tfopen( dirName, _T("w") );
	_ftprintf(fp, _T("%s\n"), workDir);
	fclose(fp);

	return TRUE;
}

// get work directory name
LPTSTR CConfig::GetWorkDirName(void)
{
	return workDir;
}

// set work directory name
void CConfig::SetWorkDirName(LPTSTR name)
{
	LPTSTR	p, q;

	// test configration name.
	for (p = name, q = workDir; *p; p++) {
		if (!IsCharAlphaNumeric(*p))
			continue;
		if(IsCharAlpha(*p))
			CharLowerBuff(p, sizeof(_TCHAR));		
		*q++ = *p;
	}
	*q		= _T('\0');
}

//// get old work directory name
//BOOL CConfig::LoadOldWorkDirName(void)
//{
//	FILE	*fp;							// file handle
//	_TCHAR	name[SizeConfig];				// configuration file name
//	_TCHAR	dirName[MAX_PATH];
//	_TCHAR	*p;
//
//	// open the file which includes work directory name.
//	_tcscat( _tcscpy( dirName, WorkRootDir ), OldWorkDirFileName );
//	if ((fp = _tfopen( dirName, _T("r") )) == NULL)
//	{
//		*name	= _T('\0');
//	}
//	else {
//		_fgetts(name, SizeConfig, fp);
//		for (p = name; *p && *p != _T('\n') && *p != _T('\r'); p++)
//			;
//		*p	= _T('\0');
//		// after reading all lines, close the file.
//		fclose(fp);
//	}
//	_tcsncpy(oldWorkDir, name, SizeConfig);
//
//	return	TRUE;
//}
//
//BOOL CConfig::SaveOldWorkDirName(void)
//{
//	FILE	*fp;
//	_TCHAR	dirName[MAX_PATH];
//
//	_tcscat( _tcscpy( dirName, WorkRootDir ), OldWorkDirFileName );
//	fp	= _tfopen( dirName, _T("w") );
//	_ftprintf(fp, _T("%s\n"), oldWorkDir);
//	fclose(fp);
//
//	return TRUE;
//}

// get work directory name
LPTSTR CConfig::GetOldWorkDirName(void)
{
	return oldWorkDir;
}

//// set work directory name
//void CConfig::SetOldWorkDirName(LPTSTR name)
//{
//	LPTSTR	p, q;
//
//	// test configration name.
//	for (p = name, q = oldWorkDir; *p; p++) {
//		if (!IsCharAlphaNumeric(*p))
//			continue;
//		if(IsCharAlpha(*p))
//			CharLowerBuff(p, sizeof(_TCHAR));		
//		*q++ = *p;
//	}
//	*q		= _T('\0');
//}

// copy work directory name
void CConfig::CopyWorkDirName(void)
{
	_tcsncpy( oldWorkDir, workDir, SizeConfig  );
}

