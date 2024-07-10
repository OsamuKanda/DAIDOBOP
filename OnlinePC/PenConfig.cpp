#include	"stdafx.h"
#include	"Config.h"


// read trend graph pen file.
void CConfig::ReadTrendGraphPen(void)
{
	FILE	*fp;
	int		i;
	int		arg, isEnab;

	// open the trend graph pen definition file. if not found,
	if ((fp = _tfopen( TrendChartPenFile, _T("r"))) == NULL) {
		// initialize the pens in no use.
		for (i = 0; i < NumTrendChannel; i++)
			m_nTrendArg[i]	= -1;
	}
	// otherwise, read the definition.
	else {
		for (i = 0; i < NumTrendChannel; i++) {
			_ftscanf(fp, _T("%d %d"), &arg, &isEnab);
			if (!isEnab)	arg	= -1;
			m_nTrendArg[i]	= arg;
			m_isEnable[i]	= isEnab;
		}
		// close the file.
		fclose(fp);
	}
}

// write trend graph pen file.
BOOL CConfig::WriteTrendGraphPen(void)
{
	FILE	*fp;
	int		i;

	// create the trend graph pen definition file. if failed,
	if ((fp = fopen( TrendChartPenFile, _T("w"))) == NULL)
		return FALSE;

	// save current setting.
	for( i = 0; i < NumTrendChannel; i++ )
		fprintf(fp, "%d %d\n", m_nTrendArg[i], m_isEnable[i]);

	// close the definition file.
	fclose(fp);

	return TRUE;
}

