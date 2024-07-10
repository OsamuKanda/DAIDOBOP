#include	"stdafx.h"
#include	"Config.h"


// initialize Steel Kind assignment.
void CConfig::SetupSteelKind(void)
{
	memset(steel, 0, sizeof(steel));
}

// read Steel Kind assignment from configration file.
void CConfig::ReadSteelConfigFile(void)
{
	FILE	*fp;
	_TCHAR	buf[1024];
	_TCHAR	szConf[256];
	int		numKind;

	// open the configration file.
	if ((fp = _tfopen(SteelKindFile, _T("r"))) == NULL)
		return;

	// for each line in the file,
	for ( ; _fgetts(buf, 1024, fp); ) 
	{
		// skip the illegal line.
		if (_stscanf(buf, _T("%d %s"), &numKind, szConf) != 2)
			continue;

		// Steel Kind check
		if(numKind < 0 || numKind >= NumSteelKind)
			continue;

		// store the value.
		strcpy(steel[numKind].confName,szConf);
	}

	// after reading all lines, close the file.
	fclose(fp);
}

// write Steel Kind assignment to configration file.
BOOL CConfig::WriteSteelConfigFile(void)
{
	FILE	*fp;
	int		i;

	// open the configration file.
	if ((fp = _tfopen( SteelKindFile, _T("w") )) == NULL)
		return FALSE;

	// for each line in the file,
	for (i = 0;i < NumSteelKind;i++) 
		_ftprintf(fp, _T("%d %s\n"), i, steel[i].confName);

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}
