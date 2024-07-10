#include	"stdafx.h"
#include	"Config.h"


// read axis file.
void CConfig::ReadAxisFile(void)
{
	FILE	*fp;

	// open the trend graph axis definition file. if not found,
	if ((fp = _tfopen( TrendAxisFile, _T("r") )) == NULL) {
		// initialize the axis.
		m_stTimeAxis.scale	= 0.25;	m_stTimeAxis.zero	= 0.0;
		m_stTempAxis.scale	= 20.0;	m_stTempAxis.zero	= 0.0;
		m_stLevelAxis.scale	= 50.0;	m_stLevelAxis.zero	= 0.0;
		m_stVcAxis.scale	=  0.5;	m_stVcAxis.zero		= 0.0;
		m_stSNAxis.scale	= 10.0;	m_stSNAxis.zero		= 0.0;
//		m_stWidthAxis.scale	= 15.0;	m_stWidthAxis.zero	= 0.0;
	}
	// otherwise, read the definition.
	else {
		fscanf(fp, "%lf %lf", &m_stTimeAxis.scale,	&m_stTimeAxis.zero);
		fscanf(fp, "%lf %lf", &m_stTempAxis.scale,	&m_stTempAxis.zero);
		fscanf(fp, "%lf %lf", &m_stLevelAxis.scale, &m_stLevelAxis.zero);
		fscanf(fp, "%lf %lf", &m_stVcAxis.scale,	&m_stVcAxis.zero);
		fscanf(fp, "%lf %lf", &m_stSNAxis.scale,	&m_stSNAxis.zero);
//		fscanf(fp, "%lf %lf", &m_stWidthAxis.scale, &m_stWidthAxis.zero);
		fclose(fp);
	}
}

// write axis file.
BOOL CConfig::WriteAxisFile(void)
{
	FILE	*fp;

	// create the trend graph axis definition file. if failed, do nothing.
	if ((fp = _tfopen( TrendAxisFile, _T("w") )) == NULL)
		return FALSE;

	// write the configration.
	_ftprintf(fp, _T("%lf %lf\n"), m_stTimeAxis.scale,	m_stTimeAxis.zero);
	_ftprintf(fp, _T("%lf %lf\n"), m_stTempAxis.scale,	m_stTempAxis.zero);
	_ftprintf(fp, _T("%lf %lf\n"), m_stLevelAxis.scale, m_stLevelAxis.zero);
	_ftprintf(fp, _T("%lf %lf\n"), m_stVcAxis.scale,	m_stVcAxis.zero);
	_ftprintf(fp, _T("%lf %lf\n"), m_stSNAxis.scale,	m_stSNAxis.zero);
//	_ftprintf(fp, _T("%lf %lf\n"), m_stWidthAxis.scale, m_stWidthAxis.zero);

	// close the file.
	fclose(fp);

	return TRUE;
}

