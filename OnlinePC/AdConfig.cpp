#include	"stdafx.h"
#include	"Config.h"


// initialize A/D channel assignment.
void CConfig::SetupADChannels(void)
{
	int		i;

	// for each A/D channel, 
	for (i = 0; i < NumADChannel; i++) 
	{
		// set no assignment. 
		adChannel[i].arg	= -1;
	}
}

// initialize input points structure.
void CConfig::SetupInputPoints(void)
{
	int		i;

	// clear whole entries.
	for (i = 0; i < NumInput; i++) 
	{
		inputPoint[i].gx	= inputPoint[i].gy	= -1;
		inputPoint[i].adCh	= -1;
	}
}

// read A/D assignment from configration file.
void CConfig::ReadADConfigFile(LPCTSTR dir)
{
	FILE	*fp;							// file handle
	_TCHAR	buf[1024];						// file path buffer
	int		ch, arg;						// A/D channel information
	double	min, max;						// .
	_TCHAR	unit[256];						// .
	_TCHAR	dirName[MAX_PATH];

	// make up file name. 
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir), ADConfigFileName );

	// open the configration file. 
	if ((fp = _tfopen( dirName, _T("r") )) == NULL)
		return;

	// for each line in the file, 
	for ( ; _fgetts(buf, 1024, fp); ) {
		// skip the illegal line. 
		if (_stscanf(buf, _T("%d %d %lf %lf %s"),
									&ch, &arg, &min, &max, unit) != 5)
			continue;
		ch--, arg--;

		// store the value. 
		adChannel[ch].arg	= arg;
		adChannel[ch].min	= min;
		adChannel[ch].max	= max;
		_tcscpy(adChannel[ch].unit, unit);

		// assign the A/D channel. 
		inputPoint[arg].adCh	= ch;
	}

	// after reading all lines, close the file. 
	fclose(fp);
}

// read A/D assignment from configration file.
BOOL CConfig::ReadADConfigFileBk(LPCTSTR dir)
{
	FILE	*fp;							// file handle
	_TCHAR	buf[1024];						// file path buffer
	int		ch, arg;						// A/D channel information
	double	min, max;						// .
	_TCHAR	unit[256];						// .
	_TCHAR	dirName[MAX_PATH];

	// make up file name. 
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir), ADConfigFileNameBk );

	// open the configration file. 
	if ((fp = _tfopen( dirName, _T("r") )) == NULL)
		return FALSE;

	// for each line in the file, 
	for ( ; _fgetts(buf, 1024, fp); ) {
		// skip the illegal line. 
		if (_stscanf(buf, _T("%d %d %lf %lf %s"),
									&ch, &arg, &min, &max, unit) != 5)
			continue;
		ch--, arg--;

		// store the value. 
		adChannel[ch].arg	= arg;
		adChannel[ch].min	= min;
		adChannel[ch].max	= max;
		_tcscpy(adChannel[ch].unit, unit);

		// assign the A/D channel. 
		inputPoint[arg].adCh	= ch;
	}

	// after reading all lines, close the file. 
	fclose(fp);
	return TRUE;
}

// store the configration into file.
BOOL CConfig::WriteADConfigFile(LPCTSTR dir)
{
	FILE	*fp;								// file handle
	_TCHAR	dirName[MAX_PATH];
	int	i;

	// make directories. 
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir );
	_tmkdir( dirName );

	// make up file name. 
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir), ADConfigFileName);

	// create a new file. if failed, return ERROR. 
	if ((fp = _tfopen( dirName, _T("w") )) == NULL)
		return FALSE;

	// for each A/D channel, 
	for (i = 0; i < NumADChannel; i++) {
		// if this channel is assigned, 
		if (adChannel[i].arg >= 0) {
			// store the assignment. 
			_ftprintf(fp, _T("%d %d %lg %lg %s\n"), i + 1, adChannel[i].arg + 1,
				adChannel[i].min, adChannel[i].max, adChannel[i].unit);
		}
	}

	// close the file and return OK. 
	fclose(fp);

	return TRUE;
}

// store the configration into file for backup.
BOOL CConfig::WriteADConfigFileBk(LPCTSTR dir)
{
	FILE	*fp;								// file handle
	_TCHAR	dirName[MAX_PATH];
	int	i;

	// make directories. 
	_tmkdir(WorkRootDir);
	_tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir );
	_tmkdir( dirName );

	// make up file name. 
	_tcscat( _tcscat( _tcscat( _tcscpy( dirName, WorkRootDir ), _T("\\")), dir), ADConfigFileNameBk);

	// create a new file. if failed, return ERROR. 
	if ((fp = _tfopen( dirName, _T("w") )) == NULL)
		return FALSE;

	// for each A/D channel, 
	for (i = 0; i < NumADChannel; i++) {
		// if this channel is assigned, 
		if (adChannel[i].arg >= 0) {
			// store the assignment. 
			_ftprintf(fp, _T("%d %d %lg %lg %s\n"), i + 1, adChannel[i].arg + 1,
				adChannel[i].min, adChannel[i].max, adChannel[i].unit);
		}
	}

	// close the file and return OK. 
	fclose(fp);

	return TRUE;
}

// generate the A/D translation table.
void CConfig::SetupADTranslation(void)
{
	int	i;
	ADChannel		*ap;
	ADTranslation	*tp;
//	double	pScale, pOffset;

	/* for each A/D channel, */
	ap	= adChannel,	tp	= m_adTranslation;
	for (i = 0; i < NumADChannel; i++, ap++, tp++) {
		/* if no input argument is configured, do nothing. */
		if (ap->arg < 0)			continue;

		// set channel data.
		tp->scale	= 0;
		tp->offset	= 0;
		tp->arg		= ap->arg;
/*
		// calculate the phisical value.
		pScale	= (ap->max - ap->min) / ADMaximumInput;
		pOffset	= ap->min;

		// if the signal is temperature,
		if (!lstrcmp(ap->unit, DefaultTempUnit)) {
			// scale in the unit of temperature.
			tp->scale	= (long)(pScale	* ADTransRange / PhisicalTempUnit);
			tp->offset	= (long)(pOffset	* ADTransRange / PhisicalTempUnit);
			tp->arg		= ap->arg;
		}
		// if the signal is casting speed,
		else if (!lstrcmp(ap->unit, DefaultVcUnit)) {
			// scale in the unit of casting speed.
			tp->scale	= (long)(pScale	* ADTransRange / PhisicalVcUnit);
			tp->offset	= (long)(pOffset	* ADTransRange / PhisicalVcUnit);
			tp->arg		= ap->arg;
		}
		// if the signal is meniscus level,
		else if (!lstrcmp(ap->unit, DefaultLevelUnit)) {
			// scale in the unit of meniscus level.
			tp->scale	= (long)(pScale	* ADTransRange / PhisicalLevelUnit);
			tp->offset	= (long)(pOffset	* ADTransRange / PhisicalLevelUnit);
			tp->arg		= ap->arg;
		}
		// if the signal is SN.
		else if (!lstrcmp(ap->unit, DefaultSNUnit)) {
			// scale in the unit of meniscus level.
			tp->scale	= (long)(pScale	* ADTransRange / PhisicalSNUnit);
			tp->offset	= (long)(pOffset	* ADTransRange / PhisicalSNUnit);
			tp->arg		= ap->arg;
		}
		// if the signal is Width.
		else if (!lstrcmp(ap->unit, DefaultWidthUnit)) {
			// scale in the unit of meniscus level.
			tp->scale	= (long)(pScale	* ADTransRange / PhisicalWidthUnit);
			tp->offset	= (long)(pOffset	* ADTransRange / PhisicalWidthUnit);
			tp->arg		= ap->arg;
		}
		// otherwise, do nothing.
		else	continue;
*/
		// after above procedures, set this element valid.
		tp->isValid	= TRUE;
	}
}

