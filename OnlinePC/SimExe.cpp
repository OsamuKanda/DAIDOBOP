#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"SimExe.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;


CSimExe::CSimExe()
{
	// initialize member
	m_pPtnNN			= NULL;
	m_pCorrNN			= NULL;	
	m_inFp				= NULL;					
	m_outFp				= NULL;					
	m_pSampledData		= NULL;	
	m_nInputIdx			= 0;		
	m_nInputSize		= 0;		
	m_nInputStartIdx	= 0;	
	m_nInputReadIdx		= 0;	
	m_bFilterSwitch		= FALSE;
	m_fFilterTemp		= 1.0;
}

CSimExe::~CSimExe()
{
	ResetSimulation();
}

// get input size
int CSimExe::GetInputSize(int nNetIdx)
{
	int		ret;

	// reset simulation environment.
	ResetSimulation();

	// ニューラルネット情報（一時ファイル）を読み込む
	ret = LoadBONeuralNetworkTmp(nNetIdx);

	// 読み込み失敗なら通常の情報を読み込む
	if( ret != NORMAL )
	{
		// load specified B.O. neural networks.
		ret = LoadBONeuralNetwork(nNetIdx);
		if( ret != NORMAL || m_pPtnNN == NULL )
		{
			return -1;
		}
	}

	// set up input buffer size from pattern N.N. input size.
	return (m_pPtnNN->wMatrix[0]->numIn);
}

// execute simulation of B.O. prediction start function
int	CSimExe::SimulateBOPredictStart(LPCTSTR name, int nNetIdx, int nCombiIdx)
{
	int		ret;

	// reset simulation environment.
	ResetSimulation();

	// ニューラルネット情報（一時ファイル）を読み込む
	ret = LoadBONeuralNetworkTmp(nNetIdx);

	// 読み込み失敗なら通常の情報を読み込む
	if( ret != NORMAL )
	{
		// load specified B.O. neural networks.
		ret = LoadBONeuralNetwork(nNetIdx);
		if( ret != NORMAL )
		{
			return ret;
		}
	}

	// set up input buffer size from pattern N.N. input size.
	m_nInputSize = m_pPtnNN->wMatrix[0]->numIn - 1;

	// set up simulation environment.
	ret = SetupSimulation(name, nCombiIdx);
	if( ret != NORMAL )
	{
		return ret;
	}

	m_bFirstSim = TRUE;

	return NORMAL;
}

// execute simulation of B.O. prediction loop function
int CSimExe::SimulateBOPredictLoop(int nCombiIdx)
{
	double	patternOut, corrOut;
	BOOL	blRet;

	// if trend data are not enough for executing N.N.,
	if (m_nInputIdx < m_nInputSize) {
		// output 0 value.
		blRet = WriteSimulationOutput(0.0, 0.0);
		if( !blRet )
			return ERR_SIM_OUT;
	}
	// otherwise,
	else {
		// prepare the input vectors for the neural networks.
		prepareBOInputVector(m_pSampledData, m_nInputSize, m_nInputStartIdx,
				m_nInputSize, &m_header,
				&cnf.tcCombination[nCombiIdx],
				m_dPtnInpVec, m_dCorrInpVec);

		// execute the B.O. prediction neural networks.
		doBONeuralNet(m_pPtnNN, m_pCorrNN,
				m_dPtnInpVec, m_dCorrInpVec, &patternOut, &corrOut);

		// output the calculated value.
		blRet = WriteSimulationOutput(patternOut, corrOut);
		if( !blRet )
			return ERR_SIM_OUT;

		// increase starting point of ring buffer.
		if (++m_nInputStartIdx == m_nInputSize) m_nInputStartIdx	= 0;
	}

	return NORMAL;
}

// execute simulation of B.O. prediction loop function
void CSimExe::SimulateBOPredictEnd(void)
{
	// close I/O files.
	fclose(m_inFp);
	fclose(m_outFp);
}

// reset simulation environment.
void CSimExe::ResetSimulation(void)
{
	// if neural networks exist, free them.
	if (m_pPtnNN)	freeNeuralNet(m_pPtnNN);
	if (m_pCorrNN)	freeNeuralNet(m_pCorrNN);
	m_pPtnNN	= NULL;
	m_pCorrNN	= NULL;

	// if trend buffer exist, free it.
	if (m_pSampledData)	free(m_pSampledData);
	m_pSampledData = NULL;

	// reset trend data index.
	m_nInputIdx			= 0;

	// reset read point.
	m_nInputReadIdx		= 0;

	// reset starting point of ring buffer.
	m_nInputStartIdx	= 0;
}

// load specified B.O. neural networks.
int	CSimExe::LoadBONeuralNetwork(int netIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[256];

	// if the neural networks are not specified, return ERROR.
	if (netIdx < 0) {
		return ERR_NO_NET;
	}

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.GetWorkDirName() );
	_stprintf( buf, PatNNWeightFile, netIdx );
	_tcscat( path, buf );

	// load the weights of pattern neural network from file.
	m_pPtnNN = loadNeuralNet(path);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.GetWorkDirName() );
	_stprintf( buf, CorrNNWeightFile, netIdx );
	_tcscat( path, buf );

	// load the weights of correlation neural network from file.
	m_pCorrNN = loadNeuralNet(path);

	// return ERROR if failed.
	if (!m_pPtnNN || !m_pCorrNN) {
		return ERR_LOAD_NN;
	}

	// after above procedures, return NORMAL.
	return NORMAL;
}

// load specified B.O. neural networks for tempo.
int	CSimExe::LoadBONeuralNetworkTmp(int netIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[256];

	// if the neural networks are not specified, return ERROR.
	if (netIdx < 0) {
		return ERR_NO_NET;
	}

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.GetWorkDirName() );
	_stprintf( buf, PatNNWeightFileTmp, netIdx );
	_tcscat( path, buf );

	// load the weights of pattern neural network from file.
	m_pPtnNN = loadNeuralNet(path);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.GetWorkDirName() );
	_stprintf( buf, CorrNNWeightFileTmp, netIdx );
	_tcscat( path, buf );

	// load the weights of correlation neural network from file.
	m_pCorrNN = loadNeuralNet(path);

	// return ERROR if failed.
	if (!m_pPtnNN || !m_pCorrNN) {
		return ERR_LOAD_NN;
	}

	// after above procedures, return NORMAL.
	return NORMAL;
}

// set log filter.
void CSimExe::SetLogFilter(BOOL bSwitch, float fTemp)
{
	m_bFilterSwitch = bSwitch;
	m_fFilterTemp	= fTemp;
}

// set up simulation environment.
int CSimExe::SetupSimulation(LPCTSTR name, int nCombiIdx)
{
	_TCHAR dirName[MAX_PATH];
	int	upperCh, lowerCh;

	// allocate trend data buffer.
	m_pSampledData = (SampledData *)calloc(m_nInputSize, sizeof(SampledData));
	if (!m_pSampledData) {
		return ERR_MEM_ALLOC;
	}

	// if T.C. combination is not specified, return ERROR.
	if (nCombiIdx < 0) {
		return ERR_NOT_TC;
	}

	// if trend data are not specified, return ERROR.
	if (!name) {
		return ERR_NO_DATA;
	}

	// open the trend file.
	if ((m_inFp = _tfopen(name, _T("rb"))) < 0) {
		// return ERROR if failed.
		return ERR_OPEN_DATA;
	}

	// load data file header.
	fread(&m_header, sizeof(m_header), 1, m_inFp);

	// set up data position of temperature.
	upperCh	= m_header.header.adCh[cnf.tcCombination[nCombiIdx].upper];
	lowerCh	= m_header.header.adCh[cnf.tcCombination[nCombiIdx].lower];

	// if upper thermo couple is not assigned for A/D channel, return ERROR.
	if (upperCh < 0) {
		fclose(m_inFp);
		return ERR_NO_UPCH;
	}

	// if lower thermo couple is not assigned for A/D channel, return ERROR.
	if (lowerCh < 0) {
		fclose(m_inFp);
		return ERR_NO_LOCH;
	}

	// create the output file.
	_tcscat( _tcscpy( dirName, cnf.WorkRootDir ), BOSimulateFile );
	if ((m_outFp = _tfopen( dirName, _T("w+b") )) == NULL ) {
		fclose(m_inFp);
		return ERR_OPEN_OUT;
	}

	// after above procedures, return NORMAL.
	return NORMAL;
}

// output the calculated value.
BOOL CSimExe::WriteSimulationOutput(double patternOut, double corrOut)
{
	int	len;

	// write the values into simulation output file.
	len	= fwrite((LPVOID)&patternOut, sizeof(patternOut), 1, m_outFp);
	len	+= fwrite((LPVOID)&corrOut, sizeof(corrOut), 1, m_outFp);

	// return the status.
	return	(len == 2) ? TRUE: FALSE;
}

// get sampled data from trend file
BOOL CSimExe::GetSampledData(void)
{
	SampledData	data;
	float		fFilter;
	int			j, nReadSize;
#ifdef Compatibility
	if( m_header.header.version <= 500 ){
		nReadSize = sizeof(SampledDataV5);
		memset(&data, 0, sizeof(SampledData));
	}else
		nReadSize = sizeof(SampledData);
#else
	nReadSize = sizeof(SampledData);
#endif
	// read trend data from file.
	if ( fread(&data, nReadSize, 1, m_inFp) != 1 ) {
		// if fetch eof, return FALSE.
		return	FALSE;
	}

	// filter?
	if( m_header.header.version >= 620 && m_bFilterSwitch == TRUE )
	{
		if( m_bFirstSim )
		{
			m_dataBk = data;
			m_bFirstSim = FALSE;
		}

		// for each A/D channel,
		for(j = 0; j < NumADChannel; j++)
		{
			switch(m_header.header.arg[j])
			{
			case -1:
			case SNID:		// SN Channel No.
			case LevelID:	// Level Channel No.
			case VcID: 		// P/R Speed Channel No.
			case LengthID:	// Length Channel No.
				fFilter = 1.0;	break;
			default:		// Temp Channel No.
				fFilter = m_fFilterTemp;break;
			}
			if( m_header.header.arg[j] >= LastID )
				fFilter = 1.0;
			m_dataBk.val[j] =	(short)((float)m_dataBk.val[j] * (1.0f - fFilter) +
										(float)data.val[j] * fFilter);
		}
		data = m_dataBk;
	}

	m_pSampledData[m_nInputReadIdx++] = data;

	// round the read point.
	if (m_nInputReadIdx == m_nInputSize) 
		m_nInputReadIdx	= 0;

	// increase input index.
	m_nInputIdx++;
	return	TRUE;
}

