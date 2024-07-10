#include	"stdafx.h"
#include	"Config.h"


// read axis file.
void CConfig::ReadAxis2File(ChartAxis *pAxis)
{
	_TCHAR	szPath[MAX_PATH];
	FILE	*fp;

	_stprintf( szPath, _T("%s\\%s"), WorkRootDir, TCAxis2FileName );

	// open the tc graph axis definition file. if not found,
	if ((fp = _tfopen( szPath, _T("rt") )) == NULL) {
		// initialize the axis.
		pAxis->scale	= 50.0;	pAxis->zero	= 0.0;
	}
	// otherwise, read the definition.
	else {
		fscanf(fp, "%lf %lf", &pAxis->scale,	&pAxis->zero);
		fclose(fp);
	}
}

// write axis file.
BOOL CConfig::WriteAxis2File(ChartAxis *pAxis)
{
	_TCHAR	szPath[MAX_PATH];
	FILE	*fp;

	_stprintf( szPath, _T("%s\\%s"), WorkRootDir, TCAxis2FileName );

	// create the tc graph axis definition file. if failed, do nothing.
	if ((fp = _tfopen( szPath, _T("w+t") )) == NULL)
		return FALSE;

	// write the configration.
	_ftprintf(fp, _T("%lf %lf\n"), pAxis->scale,	pAxis->zero);

	// close the file.
	fclose(fp);

	return TRUE;
}

