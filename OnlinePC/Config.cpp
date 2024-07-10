// ConfigIf.cpp : �����t�@�C��
//
#include "stdafx.h"
#include "Config.h"

// initialize configuration.
BOOL CConfig::Initialize( void )
{
	BOOL	ret;

	// load root directry name.
	ret = DirPickUp();
	if(ret == FALSE)
		return ret;

	// setup data.
	SetupADChannels();
	SetupInputPoints();
	//SetupTPPos();
	SetupCombination();
	SetupNNTemplate();
//	SetupTcExp();

	// read Tc Exeption info.
//	ReadTcExpFile();
	ReadTcExpSetFile(&m_stTcExpSet);

	// read axis
	ReadAxisFile();
	ReadTrendGraphPen();

	// load work directry name.
	LoadWorkDirName();
//	LoadOldWorkDirName();

	// load configuration.
	LoadConfiguration(workDir);

	return TRUE;
}

BOOL CConfig::Uninitialize(void)
{
	// save work directry name.
	SaveWorkDirName();
//	SaveOldWorkDirName();

	return TRUE;
}

BOOL CConfig::DirPickUp( void )
{
	CRegKey	reg;
	DWORD	dwSize;

	if( reg.Open(HKEY_CURRENT_USER, _T("Software\\BOP")) != ERROR_SUCCESS )
		return FALSE;

	dwSize = sizeof(RootDir);
	if( reg.QueryValue(&RootDir[0], _T("ROOTDIR"),&dwSize) != ERROR_SUCCESS )
		return FALSE;

	_tcscat( _tcscpy( WorkRootDir, RootDir ), WorkDir );
	_tcscat( _tcscpy( TrendAxisFile, WorkRootDir ), TrendAxisFileName );
	_tcscat( _tcscpy( SteelKindFile, WorkRootDir ), SteelKindFileName );
	_tcscat( _tcscpy( TrendChartPenFile, WorkRootDir ), TrendChartPenFName );

	return TRUE;
}

// load configuration.
BOOL CConfig::LoadConfiguration(LPCTSTR dir)
{
	// read A/D config file.
	SetupADChannels();
	SetupInputPoints();
	ReadADConfigFile(dir);
	SetupADTranslation();

	// read combination
	SetupCombination();
	ReadCombination(dir);

	// read network templete.
	SetupNNTemplate();
	ReadNNTemplate(dir);
	ReadNNTemplate2(dir);

	// read steel kind data.
//	SetupSteelKind();
//	ReadSteelConfigFile();

	return TRUE;
}

BOOL CConfig::SaveConfiguration(void)
{
	// write file
	if (WriteADConfigFile(workDir) == FALSE ||
		WriteCombination(workDir)  == FALSE ||
		WriteNNTemplate(workDir)   == FALSE ||
		WriteNNTemplate2(workDir)   == FALSE ) 
//		WriteTPPosFile() == FALSE ||
//		WriteSteelConfigFile() == FALSE) 
	{
		return FALSE;
	}

	//  file remove & copy.
	cpFileCopy();

	return TRUE;
}

//  file copy sub.
//	para	dist 	file name
//			source 	file name
//	ret 0: normal end
//		1:s file open error
//		2:d file open error
//		4:write error
//		5:s file close error
//		6:d file close error
int CConfig::FileCopy(LPTSTR d, LPTSTR s)
{
	_TCHAR data[1024];
	FILE	*sFp, *dFp;
	size_t	sSize;

	if (NULL == (sFp = fopen(s, "rb"))) {
		return 1;
	}

	if (NULL == (dFp = fopen(d, "wb"))) {
		fclose(sFp);
		return 2;
	}

	while(0 != (sSize = fread(data, 1, sizeof(data), sFp))) {
		if (0 == fwrite(data, sSize, 1, dFp)) {
			fclose(sFp);
			fclose(dFp);
			return 4;
		}
	}

	if (0 != fclose(sFp)) {
		fclose(dFp);
		return 5;
	}

	if (0 != fclose(dFp)) {
		return 6;
	}
	return 0;
}

//  file remove & copy
void CConfig::FileMove(void)
{
	int		cnt;
	_TCHAR	oldDir[SizeConfig], nowDir[SizeConfig], nowName[SizeConfig],oldName[SizeConfig],
			name[32], no[32];

	_tcscpy(nowName, workDir);
	_tcscpy(oldName, oldWorkDir);

	/* now directry name NE old dirctry name */
	if (_tcscmp(nowName, oldName) != 0) {
		/* now diectroy name set */
		_tcscat( _tcscat( _tcscpy( nowDir, WorkRootDir ), _T("\\")), nowName );
		/* copy diectroy name set */
		_tcscat( _tcscat( _tcscpy( oldDir, WorkRootDir ), _T("\\")), oldName);

		for(cnt = 0; cnt < 16; cnt++) {
			/* patnn wei file name set */
			_stprintf(no, _T("%d"), cnt);
			_tcscat(_tcscpy(name, _T("\\patnn")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".wei"));
			/* remove now patnn wei file */
			_tremove(nowName);

			/* copy patnn wei file name set */
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".wei"));
			/* copy patnn wei file */
			FileCopy(nowName, oldName);

			/* patnn bak file name set */
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".bak"));
			/* remove now patnn bak file */
			_tremove(nowName);

			/* corrnn wei file name set */
			_tcscat(_tcscpy(name, _T("\\corrnn")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".wei"));
			/* remove now corrnn wei file */
			_tremove(nowName);

			/* copy corrnn wei file name set */
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".wei"));
			/* copy corrnn wei file */
			FileCopy(nowName, oldName);

			/* corrnn bak file name set */
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".bak"));
			/* remove now corrnn bak file */
			_tremove(nowName);

			/* ararm area file name set */
			_tcscat(_tcscpy(name, _T("\\alarm")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".dat"));
			/* remove ararm area file */
			_tremove(nowName);

			/* copy ararm area file name set */
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".dat"));
			/* copy ararm area file */
			FileCopy(nowName, oldName);

			/* ordin area file name set */
			_tcscat(_tcscpy(name, _T("\\ordin")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".dat"));
			/* remove ordin area file */
			_tremove(nowName);

			/* copy ordin area file name set */
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".dat"));
			/* copy ordin area file */
			FileCopy(nowName, oldName);
		}
	}
}

void CConfig::cpFileCopy(void)
{
	WeiInfoList		list[WEI_BAK_CNT];
	int				cnt, n, i;
	_TCHAR	oldDir[SizeConfig], nowDir[SizeConfig], nowName[SizeConfig],oldName[SizeConfig],
			name[32], no[32];

	_tcscpy(nowName, workDir);
	_tcscpy(oldName, oldWorkDir);

	// now directry name NE old dirctry name
	if (_tcscmp(nowName, oldName) != 0) {
		// now diectroy name set
		_tcscat( _tcscat( _tcscpy( nowDir, WorkRootDir ), _T("\\")), nowName );
		// copy diectroy name set
		_tcscat( _tcscat( _tcscpy( oldDir, WorkRootDir ), _T("\\")), oldName);

		for(cnt = 0; cnt < NumNeuralNet; cnt++) {
			// patnn wei file name set
			_stprintf(no, "%d", cnt);
			_tcscat(_tcscpy(name, _T("\\patnn")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".wei"));
			// remove now patnn wei file
			_tremove(nowName);

			// copy patnn wei file name set
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".wei"));
			// copy patnn wei file
			FileCopy(nowName, oldName);

			// patnn bak file name set
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".bak"));
			// remove now patnn bak file
			_tremove(nowName);

			// corrnn wei file name set
			_tcscat(_tcscpy(name, _T("\\corrnn")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".wei"));
			// remove now corrnn wei file
			_tremove(nowName);

			// copy corrnn wei file name set
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".wei"));
			// copy corrnn wei file
			FileCopy(nowName, oldName);

			// corrnn bak file name set
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".bak"));
			// remove now corrnn bak file
			_tremove(nowName);

			// ararm area file name set
			_tcscat(_tcscpy(name, _T("\\alarm")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".dat"));
			// remove ararm area file
			_tremove(nowName);

			// copy ararm area file name set
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".dat"));
			// copy ararm area file
			FileCopy(nowName, oldName);

			// ordin area file name set
			_tcscat(_tcscpy(name, _T("\\ordin")), no);
			_tcscat(_tcscat(_tcscpy(nowName, nowDir), name), _T(".dat"));
			// remove ordin area file
			_tremove(nowName);

			// copy ordin area file name set
			_tcscat(_tcscat(_tcscpy(oldName, oldDir), name), _T(".dat"));
			// copy ordin area file
			FileCopy(nowName, oldName);
		}

		// copy sample def file name set
		_tcscat(_tcscpy(nowName, nowDir), SamplingFile);
		_tcscat(_tcscpy(oldName, oldDir), SamplingFile);
		// copy ordin area file
		FileCopy(nowName, oldName);

		// �S�Ẵl�b�g���[�N�ɂ���
		for(cnt = 0; cnt < NumNeuralNet; cnt++) 
		{
			// �w�K���ʁi�����j��S�č폜
			n = GetWeiListP(nowDir, cnt, list);
			for( i = 0; i < n; i++ )
				_tremove(list[i].path);
			n = GetWeiListC(nowDir, cnt, list);
			for( i = 0; i < n; i++ )
				_tremove(list[i].path);
		}

		// �S�Ă̊w�K���ʁi�����j���R�s�[
		_stprintf(oldName, _T("%s\\*.bak*%c"), oldDir, 0);
		_stprintf(nowName, _T("%s\\%c"), nowDir, 0);
		CopyFile2(nowName, oldName);
	}
}

// check deletion.
BOOL CConfig::DeleteCheck(LPSTR szbuf) 
{
	BoSysInfo	stBsInfo;
	int			i;

	// It checks whether or not a configuration file name isn't used.
	if(!_tcscmp(szbuf, workDir) || !_tcscmp(szbuf, oldWorkDir) )
		return FALSE;

	for(i = 0;i < NumSteelKind;i++)
	{
		ReadBosysInfo(i, &stBsInfo);
		if(!lstrcmp(szbuf, stBsInfo.szConfName))
			return FALSE;
	}
	
	return TRUE;
}

// get subdirectory name list.
int	CConfig::GetSubDirList(LPTSTR home, _TCHAR table[][SizeConfig], int max)
{
	_TCHAR	buf[MAX_PATH];
	struct _finddata_t	info;
	intptr_t hFile;
	_TCHAR	*p, *q;
	int	n = 0,stat = 0;

	// make up search key string.
	_tcscat(_tcscpy(buf, home), _T("\\*.*"));

	if ((hFile = _findfirst(buf, &info)) != -1L) {
		// while finding subdirectory,
		for (; n < max && !stat;) {
			// store into the table entry with tolowerring character.
			if ((info.attrib & _A_SUBDIR) && *info.name != _T('.')) {
				for (p = info.name, q = table[n++]; *p; p++)
					*q++	= (*p >= _T('A') && *p <= _T('Z') ) ?
								*p - _T('A') + _T('a') : *p;
				*q	= _T('\0');
			}
			// search directry entry.
			stat = _findnext(hFile, &info);
		}
		_findclose(hFile);
		// if found,
		if (n) {
			// sort directory name.
			qsort(table, n, SizeConfig, (COMP)strcmp);
		}
	}
	return	n;
}
// delete config file
BOOL CConfig::DeleteConfigFile(LPTSTR name)
{
	_TCHAR	Path[SizeConfig];
	//int		i;

	//_stprintf( Path, _T("%s\\%s") ADConfigFileName, WorkRootDir, name );
	//DeleteFile(Path);
	//_stprintf( Path, _T("%s\\%s") CombiFileName, WorkRootDir, name );
	//DeleteFile(Path);
	//_stprintf( Path, _T("%s\\%s") NNTemplateFileName, WorkRootDir, name );
	//DeleteFile(Path);
	//_stprintf( Path, _T("%s\\%s") NNTemplateFileName2, WorkRootDir, name );  
	//DeleteFile(Path);
	//_stprintf( Path, _T("%s\\%s") SamplingFile, WorkRootDir, name );
	//DeleteFile(Path);
	//for(i = 0;i < NumNeuralNet;i++)
	//{
	//	_stprintf( Path, _T("%s\\%s") PatNNWeightFile, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//	_stprintf( Path, _T("%s\\%s") PatNNWeightBackup, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//	_stprintf( Path, _T("%s\\%s") CorrNNWeightFile, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//	_stprintf( Path, _T("%s\\%s") CorrNNWeightBackup, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//	_stprintf( Path, _T("%s\\%s") BPOrdinaryDataFile, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//	_stprintf( Path, _T("%s\\%s") BPAlarmDataFile, WorkRootDir, name, i );
	//	DeleteFile(Path);
	//}
	//_stprintf( Path, _T("%s\\%s"), WorkRootDir, name );
	//RemoveDirectory(Path);

	// �폜����R���t�B�O�����w�肳��Ă����
	if( name != NULL && _tcslen(name) > 0 )
	{
		// �폜�p�X�쐬�i�Ō��NULL���Q�K�v�j
		_stprintf( Path, _T("%s\\%s%c"), WorkRootDir, name, 0 );

		// ���[�N�t�H���_�������Ȃ��ׂ̔���
		if( _tcscmp(WorkRootDir, Path) != 0 )
			// �R���t�B�O�t�H���_���폜�i���̃t�@�C�����S���j
			DeleteFile2(Path);
	}

	return TRUE;
}

// get config file in the config directory.
int CConfig::GetConfigDir(LPTSTR pszDir, _TCHAR table[][SizeConfig], int max)
{
	_TCHAR	buf[MAX_PATH];
	struct _finddata_t	info;
	intptr_t hFile;
	_TCHAR	*p, *q;
	int	n = 0,stat = 0;

	// make up search key string.
	_tcscpy(buf, pszDir);
	_tcscat(buf, _T("\\*.*"));

	if ((hFile = _findfirst(buf, &info)) != -1L) {
		// while finding subdirectory,
		for (; n < max && !stat;) {
			// store into the table entry with tolowerring character.
			if (!(info.attrib & _A_SUBDIR)) {
				for (p = info.name, q = table[n++]; *p; p++)
					*q++	= (*p >= _T('A') && *p <= _T('Z') ) ?
								*p - _T('A') + _T('a') : *p;
				*q	= _T('\0');
			}
			// search directry entry.
			stat = _findnext(hFile, &info);
		}
		_findclose(hFile);
	}
	return	n;
}

// ���C���h�J�[�h�\�ȃt�@�C���R�s�[�i�G���[�\���Ȃ��j
// �p�X�̏I�[��\0���Q�K�v
DWORD CConfig::CopyFile2(LPCTSTR pDestFilePath, LPCTSTR pSrcFilePath)
{
	struct _stat stDst; 

	// copy file.
	SHFILEOPSTRUCT st;
	st.hwnd		= NULL;
	st.wFunc	= FO_COPY; 
	st.pFrom	= pSrcFilePath;
	st.pTo		= pDestFilePath;
	st.fFlags	= FOF_MULTIDESTFILES|FOF_NOERRORUI;
	if( 0 != SHFileOperation(&st) )
	{
		if( st.fAnyOperationsAborted == TRUE )
			return ERROR_CANCELLED;
		else
			return GetLastError();
	}

	// get file size.
	if( -1 == _tstat( pDestFilePath, &stDst ) )
		return ERROR_CANNOT_COPY;

	return 0L;
}

// ���C���h�J�[�h�\�ȃt�@�C���폜�i�G���[�\���Ȃ��j
// �p�X�̏I�[��\0���Q�K�v
DWORD CConfig::DeleteFile2(LPCTSTR pFilePath)
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

// �f�[�^����t���ƃR���r�l�[�V���������o�b�N�A�b�v����
BOOL CConfig::BackupADAndCombi(void)
{
	// write file
	if (WriteADConfigFileBk(workDir) == FALSE ||
		WriteCombinationBk(workDir)  == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}

