#include	"stdafx.h"
#include	"Config.h"


// initialize thermo couple combination.
void CConfig::SetupCombination(void)
{
	// clear the number of combination.
	numCombination	= 0;
}

// read combination file.
void CConfig::ReadCombination(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	buf[1024];
	_TCHAR	dirName[MAX_PATH];
	int		upper, lower, netID;

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\") ), dir ), CombiFileName );

	// open the combination file.
	if ((fp = _tfopen( dirName, "r" )) == NULL )
		return;

	// for each line in the file,
	for ( ; _fgetts(buf, 1024, fp); ) {
		// skip the illegal line.
		if (_stscanf(buf, "%d %d %d", &upper, &lower, &netID) != 3)
			continue;
		upper--,	 lower--,	netID--;

		// store the value.
		if( netID >= 0 && netID < NumNeuralNet )
		{
			tcCombination[numCombination].upper	= upper;
			tcCombination[numCombination].lower	= lower;
			tcCombination[numCombination].netID	= netID;
			numCombination++;
		}
	}

	// after reading all lines, close the file.
	fclose(fp);
}

// read combination file.
BOOL CConfig::ReadCombinationBk(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	buf[1024];
	_TCHAR	dirName[MAX_PATH];
	int		upper, lower, netID;

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\") ), dir ), CombiFileNameBk );

	// open the combination file.
	if ((fp = _tfopen( dirName, "r" )) == NULL )
		return FALSE;

	// for each line in the file,
	for ( ; _fgetts(buf, 1024, fp); ) {
		// skip the illegal line.
		if (_stscanf(buf, "%d %d %d", &upper, &lower, &netID) != 3)
			continue;
		upper--,	 lower--,	netID--;

		// store the value.
		if( netID >= 0 && netID < NumNeuralNet )
		{
			tcCombination[numCombination].upper	= upper;
			tcCombination[numCombination].lower	= lower;
			tcCombination[numCombination].netID	= netID;
			numCombination++;
		}
	}

	// after reading all lines, close the file.
	fclose(fp);
	return TRUE;
}


// store the combination into file.
BOOL CConfig::WriteCombination(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	name[SizeConfig];
	int	i;

	// make directories.
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\")), dir);
	_tmkdir(name);

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\") ), dir ), CombiFileName );

	// create a new file. if failed, return ERROR.
	if ((fp = _tfopen(name, _T("w"))) == NULL)
		return	FALSE;

	// for each combination,
	for (i = 0; i < numCombination; i++) 
	{
		// write combination.
		if( tcCombination[i].netID >= 0 && tcCombination[i].netID < NumNeuralNet )
		{
			_ftprintf(fp, _T("%d %d %d\n"),
			tcCombination[i].upper + 1, tcCombination[i].lower + 1,	tcCombination[i].netID + 1);
		}
	}

	// close the file and return OK.
	fclose(fp);

	return	TRUE;
}

// store the combination into file for backup.
BOOL CConfig::WriteCombinationBk(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	name[SizeConfig];
	int	i;

	// make directories.
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\")), dir);
	_tmkdir(name);

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\") ), dir ), CombiFileNameBk );

	// create a new file. if failed, return ERROR.
	if ((fp = _tfopen(name, _T("w"))) == NULL)
		return	FALSE;

	// for each combination,
	for (i = 0; i < numCombination; i++) 
	{
		// write combination.
		if( tcCombination[i].netID >= 0 && tcCombination[i].netID < NumNeuralNet )
		{
			_ftprintf(fp, _T("%d %d %d\n"),
			tcCombination[i].upper + 1, tcCombination[i].lower + 1,	tcCombination[i].netID + 1);
		}
	}

	// close the file and return OK.
	fclose(fp);

	return	TRUE;
}

// initialize Neural network definition.
void CConfig::SetupNNTemplate(void)
{
	int	i;
	static NeuralNetConf	nullNet;

	// for each neural network,
	for (i = 0; i < NumNeuralNet; i++) {
		// clear the entry.
		neuralnetConf[i]	= nullNet;
	}
}

// read neural network template file.
void CConfig::ReadNNTemplate(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	buf[1024];
	_TCHAR	token[256];
	NNTemplate	pat, cor;
	_TCHAR	*p, *q;
	int		id;

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( buf, WorkRootDir ), _T("\\") ), dir ), NNTemplateFileName );

	// open the combination file.
	if ((fp = _tfopen(buf, _T("r"))) == NULL)
		return;

	// for each line in the file,
	for ( ; ; ) {
		// read identifier line. if failed, end.
		if (!_fgetts(buf, 1024, fp) || _stscanf(buf, _T("%d"), &id) != 1)
			break;
		id--;

		// read pattern neural network template.
		if (!_fgetts(buf, 1024, fp))
			break;
		memset(&pat, 0, sizeof(pat));
		for (p = buf; *p; ) {
			for (q = token; *p && *p != _T(',') && *p != _T('\n') && *p != _T('\r'); p++)
				*q++	= *p;
			*q	= _T('\0');
			_stscanf(token, _T("%d"), &pat.numElm[pat.numLayer]);
			pat.numLayer++;
			if (*p)
				p++;
		}

		// read correlation neural network template.
		if (!_fgetts(buf, 1024, fp))
			break;
		memset(&cor, 0, sizeof(cor));
		for (p = buf; *p; ) {
			for (q = token; *p && *p != _T(',') && *p != _T('\n') && *p != _T('\r'); p++)
				*q++	= *p;
			*q	= '\0';
			_stscanf(token, _T("%d"), &cor.numElm[cor.numLayer]);
			cor.numLayer++;
			if (*p)
				p++;
		}

		// read neural network remark.
		if (!_fgetts(buf, 1024, fp))
			break;
		for (p = buf; *p && *p != _T('\n') && *p != _T('\r'); p++)
			;
		*p	= '\0';

		// store the value.
		neuralnetConf[id].patternNN	= pat;
		neuralnetConf[id].corrNN	= cor;
		strcpy(neuralnetConf[id].remark, buf);
	}

	// after reading all lines, close the file.
	fclose(fp);
}

// read neural network template file.
void CConfig::ReadNNTemplate2(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	buf[1024];
	int			boWatchTime;				// B.O watch time
	int			numRegister;				// Number of Registration
	float		boAlarm[NumBoOccur];		// Threshold
	int			boTimes[NumBoOccur];		// Number of Times
//	int			numCombiBA;					// Number of combinations for BO Alarm.
//	int			numCombiBAC;				// Number of combinations for BO Alarm Control.
	int			i, id;

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( buf, WorkRootDir ), _T("\\") ), dir ), NNTemplateFileName2 );

	// open the combination file.
	if ((fp = _tfopen(buf, _T("r"))) == NULL)
		return;

	// for each line in the file,
	for ( ; ; ) {
		// read identifier line. if failed, end.
		if (!_fgetts(buf, 1024, fp) || _stscanf(buf, _T("%d"), &id) != 1)
			break;
		id--;

		// read data
		if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
		{
			break;
		}

		// if value is normal,
		if (_stscanf(buf, _T("%d %d"), &boWatchTime, &numRegister) == 2)
		{
			// if value is abnormal,
			if(numRegister > NumBoOccur)
			{
				break;
			}

			// while  the registration count is no end,
			for(i = 0;i < numRegister;i++)
			{
				// read data
				if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
				{
					break;
				}
				// if value is abnormal,
				if (_stscanf(buf, _T("%f %d"), &boAlarm[i],&boTimes[i]) != 2)
				{
					break;
				}
			}

			//// read data
			//if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
			//{
			//	numCombiBA = 1;			// <- The code for the compatibility
			//}
			//else if (_stscanf(buf, _T("%d"), &numCombiBA) != 1)
			//{
			//	numCombiBA = 1;			// <- The code for the compatibility
			//}
			//if(_fgetts(buf, 256, fp) == NULL && ferror(fp))
			//{
			//	numCombiBAC = 1;			// <- The code for the compatibility
			//}
			//else if (_stscanf(buf, _T("%d"), &numCombiBAC) != 1)
			//{
			//	numCombiBAC = 1;			// <- The code for the compatibility
			//}
		}
		// if value is abnormal,
		else
		{
			break;
		}

		// store the value.
		neuralnetConf[id].boWatchTime	= boWatchTime;
		neuralnetConf[id].numRegister	= numRegister;
		for(i = 0; i < numRegister; i++ )
			neuralnetConf[id].boAlarm[i]	= boAlarm[i];
		for(i = 0; i < numRegister; i++ )
			neuralnetConf[id].boTimes[i]	= boTimes[i];
		//neuralnetConf[id].numCombiBA	= numCombiBA;
		//neuralnetConf[id].numCombiBAC	= numCombiBAC;
	}

	// after reading all lines, close the file.
	fclose(fp);
}

// store the neural network template into file.
BOOL CConfig::WriteNNTemplate(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	name[SizeConfig];
	int	i, j;

	// make directories.
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( name, WorkRootDir ),_T("\\") ), dir );
	_tmkdir(name);

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\") ), dir ), NNTemplateFileName );

	// create a new file. if failed, return ERROR.
	if ((fp = _tfopen(name, _T("w"))) == NULL)
		return	FALSE;

	// for each neural network,
	for (i = 0; i < NumNeuralNet; i++) {
		// skip null definition.
		if (neuralnetConf[i].patternNN.numLayer == 0 ||
			neuralnetConf[i].corrNN.numLayer == 0)
			continue;

		// write neural network ID. 
		fprintf(fp, _T("%d\n"), i + 1);
		// write pattern neural network structure. */
		for (j = 0; j < neuralnetConf[i].patternNN.numLayer; j++) {
			_ftprintf(fp, _T("%d"), neuralnetConf[i].patternNN.numElm[j]);
			if (j == NumLayer - 1 ||
				j == neuralnetConf[i].patternNN.numLayer - 1)
				_ftprintf(fp, _T("\n"));
			else
				_ftprintf(fp, _T(","));
		}
		// write correlation neural network structure.
		for (j = 0; j < neuralnetConf[i].corrNN.numLayer; j++) {
			_ftprintf(fp, _T("%d"), neuralnetConf[i].corrNN.numElm[j]);
			if (j == NumLayer - 1 ||
				j == neuralnetConf[i].corrNN.numLayer - 1)
				_ftprintf(fp, _T("\n"));
			else
				_ftprintf(fp, _T(","));
		}
		// write neural network remark.
		_ftprintf(fp, _T("%s\n"), neuralnetConf[i].remark);
	}

	// close the file and return OK.
	fclose(fp);

	return	TRUE;
}

// store the neural network template into file.
BOOL CConfig::WriteNNTemplate2(LPCTSTR dir)
{
	FILE	*fp;
	_TCHAR	name[SizeConfig];
	int		i, j;

	// make directories.
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( name, WorkRootDir ),_T("\\") ), dir );
	_tmkdir(name);

	// make up file name.
	_tcscat( _tcscat( _tcscat( _tcscpy( name, WorkRootDir ), _T("\\") ), dir ), NNTemplateFileName2 );

	// create a new file. if failed, return ERROR.
	if ((fp = _tfopen(name, _T("w"))) == NULL)
		return	FALSE;

	// for each neural network,
	for (i = 0; i < NumNeuralNet; i++) 
	{
		// skip null definition.
		if (neuralnetConf[i].patternNN.numLayer == 0 ||
			neuralnetConf[i].corrNN.numLayer == 0)
			continue;

		// write neural network ID. 
		fprintf(fp, _T("%d\n"), i + 1);

		// write data
		if(_ftprintf(fp, _T("%d %d\n"), neuralnetConf[i].boWatchTime, neuralnetConf[i].numRegister) < 0)
		{
			break;
		}

		// while  the registration count is no end,
		for( j = 0; j < neuralnetConf[i].numRegister; j++ )
		{
			// write data
			if(_ftprintf(fp, _T("%.2f %d\n"), neuralnetConf[i].boAlarm[j], neuralnetConf[i].boTimes[j]) < 0)
			{
				break;
			}
		}

		//if(_ftprintf(fp, _T("%d\n"), neuralnetConf[i].numCombiBA) < 0)
		//{
		//	break;
		//}
		//if(_ftprintf(fp, _T("%d\n"), neuralnetConf[i].numCombiBAC) < 0)
		//{
		//	break;
		//}
	}

	// close the file and return OK.
	fclose(fp);

	return	TRUE;
}

