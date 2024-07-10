#include	"stdafx.h"
#include	"Config.h"


// setup TC position.
void CConfig::SetupTPPos(void)
{
	memset(nTpPosData, 0, sizeof(nTpPosData));
}

// read TC position file.
BOOL CConfig::ReadTPPosFile(void)
{
	FILE	*fp;					// file handle
	_TCHAR	buf[32];
	_TCHAR	dirName[MAX_PATH];
	int	i;

	_tcscat( _tcscpy( dirName, WorkRootDir ), TpPosFile );

	// open the file which includes work directory name.
	if ((fp = _tfopen( dirName, _T("r+b"))) == NULL)
	{
		if ((fp = _tfopen( dirName, _T("w+b"))) == NULL)
		{
			for ( i = 0; i < (NumTCTap / TapRaw); i++ )
			{
				nTpPosData[i] = 0;
			}
			return FALSE;
		}
	}
	else
	{
		for ( i = 0; i < (NumTCTap / TapRaw); i++ )
		{
			if( NULL == _fgetts( buf, 10, fp ) )
				break;
			nTpPosData[i] = _tstoi(buf);
		}
		for ( ; i < (NumTCTap / TapRaw); i++ )
			nTpPosData[i] = 0;
	}
	// after reading all lines, close the file.
	fclose(fp);

	return	TRUE;
}

// write TC position file.
BOOL CConfig::WriteTPPosFile(void)
{
	FILE	*fp;
	TCHAR	cTPPos[16];
	TCHAR	dirName[MAX_PATH];
	int	i;

	_tcscat( _tcscpy( dirName, WorkRootDir ), TpPosFile );

	// open the file which includes work directory name.
	if ((fp = _tfopen( dirName, _T("w+b"))) == NULL)
		return FALSE;							

	for ( i = 0; i < (NumTCTap / TapRaw); i++ )
	{
		_stprintf(cTPPos, _T("%d\n"), nTpPosData[i]);
		_fputts( cTPPos, fp );
	}

	// after reading all lines, close the file.
	fclose(fp);

	return	TRUE;
}

