#include	"stdafx.h"
#include	"Config.h"


// read the weights of B.O. prediction neural networks.
void CConfig::ReadBONeuralNet(LPCTSTR dir, LPTSTR key, NeuralNet **netTab)
{
	int     i;
	struct	_finddata_t	fInfo;
	long	hFile;
	int		stat = 0;
	_TCHAR	buf[256];
	int		mark[NumNeuralNet];


	// make up wild card path name.
	MakeupBONNSearchPath(buf, dir, key);

	// clear the marks.
	for (i = 0; i < NumNeuralNet; i++)
		mark[i] = FALSE;

	if ((hFile = _findfirst(buf, &fInfo)) != -1L) {
		// while the trend file existing.
		for (; !stat;) {
			// get trend file entry.
			// if the entry is not sub directory, 
			if (!(fInfo.attrib & _A_SUBDIR) && *fInfo.name != _T('.')) {
				// put the find mark. */
				if ((i = GetWeightIdx(fInfo.name)) >= 0 && i < NumNeuralNet)
					mark[i] = TRUE;
			}
			// search directry entry.
			stat = _findnext(hFile, &fInfo);
		}
		_findclose(hFile);

	}
	// for each neural network,
	for (i = 0; i < NumNeuralNet; i++) {
		// if exist,
		if (mark[i]) {
			// make up full path name.
			(strcmp(key, PatNNWeightFile) == 0) ?
				_stprintf(buf, _T("%s\\%s") PatNNWeightFile, WorkRootDir, dir, i) :
				_stprintf(buf, _T("%s\\%s") CorrNNWeightFile, WorkRootDir, dir, i) ;
			// read the weight file.
			netTab[i] = loadNeuralNet(buf);
		}
	}
}

// initialize neural network table.
void CConfig::ResetBONeuralNet(NeuralNet **netTab)
{
	int    i;

	// for each pattern neural network,
	for (i = 0; i < NumNeuralNet; i++) {
		// if the neural network exists,
		if (netTab[i]) {
			// free it.
			freeNeuralNet(netTab[i]);
			netTab[i]     = NULL;
		}
	}
}

// make up wild card path name.
void CConfig::MakeupBONNSearchPath(LPTSTR path, LPCTSTR dir, LPTSTR key)
{
	_TCHAR    *p,*q;

	// make up directory name of weight file.
	_tcscat( _tcscat( _tcscpy( path, WorkRootDir ), _T("\\") ), dir );

	// make up search path.
	for (p = path + _tcslen(path), q = key; *q && *q != _T('%'); )
		*p++    = *q++;
	*p++    = _T('*');
	for ( ; *q && *q != _T('.'); q++)
		;
	for ( ; *q; )
		*p++    = *q++;
	*p      = _T('\0');
}

// get the network ID.
int	CConfig::GetWeightIdx(LPCTSTR name)
{
	for ( ; !(*name >= _T('0') && *name <= _T('9')); name++) {
		if (!*name)
			return  -1;
	}
	return	_tstoi(name);
}

// 学習結果データリストを取得する（パターン）
int CConfig::GetWeiListP(LPCTSTR lpszDir, int nNetIdx, WeiInfoList *table)
{
	WeiInfoList  *tp;
	_TCHAR		dirName[MAX_PATH];
	struct		_tfinddata_t	fInfo;
	int			n = 0;
	long		hFile;
	int			stat = 0;

	if( nNetIdx < 0 ) return 0;

	_stprintf(dirName, _T("%s\\patnn%d.bak*"),lpszDir, nNetIdx);
	if ((hFile = _tfindfirst( dirName, &fInfo )) != -1L )
	{
		// while the trend file existing.
		for (tp = table; !stat && n < WEI_BAK_CNT;) 
		{
			// make up file name.
			_stprintf(dirName, _T("%s\\%s"),lpszDir, fInfo.name);
			_tcscpy(tp->path, dirName);
			tp->updateTime = fInfo.time_write;
			n++;
			tp++;

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

// 学習結果データリストを取得する（総合）
int CConfig::GetWeiListC(LPCTSTR lpszDir, int nNetIdx, WeiInfoList *table)
{
	WeiInfoList  *tp;
	_TCHAR		dirName[MAX_PATH];
	struct		_tfinddata_t	fInfo;
	int			n = 0;
	long		hFile;
	int			stat = 0;

	if( nNetIdx < 0 ) return 0;

	_stprintf(dirName, _T("%s\\corrnn%d.bak*"),lpszDir, nNetIdx);
	if ((hFile = _tfindfirst( dirName, &fInfo )) != -1L )
	{
		// while the trend file existing.
		for (tp = table; !stat && n < WEI_BAK_CNT;) 
		{
			// make up file name.
			_stprintf(dirName, _T("%s\\%s"),lpszDir, fInfo.name);
			_tcscpy(tp->path, dirName);
			tp->updateTime = fInfo.time_write;
			n++;
			tp++;

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
int CConfig::CmpTrendEnt(WeiInfoList *p, WeiInfoList *q)
{
	if (p->updateTime < q->updateTime)
		return  1;
	if (p->updateTime > q->updateTime)
		return  -1;
	return  0;
}

