#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"BpOpe.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;
static CBolog	&bolog = theApp.m_log;

///////////////////////////////////////////////////////////////////////////////
// CBpOpe class 
CBpOpe::CBpOpe()
{
	// initialize member
	m_nRepeatTimes		= RepeatTimes;
	m_nTeachCondition	= IsMask;
	m_pPattNN			= NULL;	
	m_pCorrNN			= NULL;	
	m_nNetIdx			= -1;
	m_stOrdData			= NULL;	
	m_nNumOrdData		= 0;	
	m_hOrdData			= NULL;		
	m_stAlarmData		= NULL;	
	m_nNumAlarmData		= 0;	
	m_hAlmData			= NULL;			
	m_dPattOrdTeachVal	= 0.0;
	m_dPattAlmTeachVal	= 1.0;
	m_dCorrOrdTeachVal	= 0.0;
	m_dCorrAlmTeachVal	= 1.0;
	m_strTrendOutFilePath	= _T("");
    memset(&m_stPatternBP,			0, sizeof(m_stPatternBP));
    memset(&m_stCorrBP,				0, sizeof(m_stCorrBP));
	memset(&m_dOrdinaryPatInput,	0, sizeof(m_dOrdinaryPatInput));
	memset(&m_dAlarmPatInput,		0, sizeof(m_dAlarmPatInput));
	memset(&m_dOrdinaryCorInput,	0, sizeof(m_dOrdinaryCorInput));
	memset(&m_dAlarmCorInput,		0, sizeof(m_dAlarmCorInput));

	// setup teaching data buffer 
	SetupTeachingDataBuffer();
}

CBpOpe::~CBpOpe()
{
	FreeBONeuralNetwork();

	// destroy buffers of teaching data.
	DestroyTeachingDataBuffer();
}

///////////////////////////////////////////////////////////////////////////////
// member function.(protected)

// set up buffer for teaching data
void CBpOpe::SetupTeachingDataBuffer(void)
{
	// allocate ordinary teaching data buffer from global heap. 
	m_hOrdData		= GlobalAlloc(GMEM_MOVEABLE,
							(DWORD)sizeof(SampledData) * NumTrendData);
	// map the memory block. 
	m_stOrdData		= (SampledData *)GlobalLock(m_hOrdData);

	// allocate alarm teaching data buffer from global heap. 
	m_hAlmData		= GlobalAlloc(GMEM_MOVEABLE,
							(DWORD)sizeof(SampledData) * NumTrendData);
	// map the memory block. 
	m_stAlarmData			= (SampledData *)GlobalLock(m_hAlmData);
}

// destroy buffers of teaching data.
void CBpOpe::DestroyTeachingDataBuffer(void)
{
	// unmap ordinary teaching data buffer. 
	GlobalUnlock(m_hOrdData);
	// release the memory block. 
	GlobalFree(m_hOrdData);

	// unmap alarm teaching data buffer. 
	GlobalUnlock(m_hAlmData);
	// release the memory block. 
	GlobalFree(m_hAlmData);
}

// read teaching data file specification.
int CBpOpe::ReadTeachDataSpec(_TCHAR *fname, TeachDataSpec *spec)
{
	SYSTEMTIME	st;
	FILE	*fp;
	_TCHAR	*p, *q;
	_TCHAR	buf[128];
	LPCTSTR pName;
	int		ret, arg1, arg2;

	// initialize pointer.
	*spec->path		= _T('\0');
	spec->fIdx		= -1;
	spec->length	= 0;
	spec->startTime	= 0;
//@	spec->combiID	= -1;
	spec->shift		= 0;
	spec->arg1		= -1;
	spec->arg2		= -1;

	// open the specification file. do nothing if failed.
	if ((fp = _tfopen(fname, _T("rb"))) == NULL) return ERR_SPEC_FILE;

	// get teaching ordinary data file name.
	_fgetts(spec->path, 256, fp);
	for (p = spec->path; *p && *p != _T('\n') && *p != _T('\r'); p++)
		;
	*p	= _T('\0');

	// get time
	if( _tcsstr(spec->path, _T(".log")) != NULL )
	{
		q = spec->path + strlen(spec->path) - 18;
		_tcsncpy(buf, q, 5);buf[4] = _T('\0');q = &q[4];
		_stscanf(buf, _T("%04d"), &st.wYear);
		_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &q[2];
		_stscanf(buf, _T("%02d"), &st.wMonth);
		_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &q[2];
		_stscanf(buf, _T("%02d"), &st.wDay);
		_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &q[2];
		_stscanf(buf, _T("%02d"), &st.wHour);
		_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &q[2];
		_stscanf(buf, _T("%02d"), &st.wMinute);
		_tcsncpy(buf, q, 3);buf[2] = _T('\0');q = &q[2];
		_stscanf(buf, _T("%02d"), &st.wSecond);

		CTime	ct(st);
		spec->startTime = (time_t)ct.GetTime();
	}
	else
	{
		// get file start time.
		p = spec->path + strlen(spec->path) - 1;
		for ( ; p != spec->path && *p != _T('\\'); p--)
			;
		if (*p == _T('\\'))		p++;
		for (q = buf; *p; p++)
			if (*p != _T('.'))
				*q++	= *p;
		*q	= _T('\0');
		_stscanf(buf, _T("%ld"), &spec->startTime);
	}

	// get file pointer of teaching ordinary data.
	_ftscanf(fp, _T("%ld"), &spec->fIdx);

	// get data length of teaching ordinary data.
	_ftscanf(fp, _T("%ld"), &spec->length);

	// get select combination tc
	if( 2 != (ret = _ftscanf(fp, _T("%d,%d"), &arg1, &arg2)) )
	{
		fclose(fp);
		return ERR_SPEC_FILE;
	}
	spec->arg1		= arg1;
	spec->arg2		= arg2;

	// get specified T.C. combination.
	_ftscanf(fp, _T("%f"), &spec->fFilter);

	// get specified T.C. combination.
	_ftscanf(fp, _T("%d"), &spec->shift);

	// close the file.
	fclose(fp);

	// read the header of logging data file.
	if ((fp = fopen(spec->path, _T("rb"))) == NULL)
	{
		// Tempフォルダも探す
		pName = bolog.GetTempLogFilePath(spec->path);
		if(pName == NULL) return ERR_LOG_FILE;
		if ((fp = fopen(pName, _T("rb"))) == NULL) 
			return ERR_LOG_FILE;
	}
	fread(&spec->dtHead, sizeof(spec->dtHead), 1, fp);
	fclose(fp);

	// set offset to start time.
	spec->startTime += (long)spec->dtHead.header.samplePeriod*spec->fIdx / 1000;

	return NORMAL;
}

// write teaching data file specification.
int CBpOpe::WriteTeachDataSpec(_TCHAR *fname, TeachDataSpec *spec)
{
	FILE	*fp;

	// open the specification file. do nothing if failed.
	if ((fp = _tfopen(fname, _T("w+b"))) == NULL) return ERR_SPEC_FILE;

	// put file path of teaching data
	_ftprintf(fp, _T("%s\r\n"), spec->path);

	// get file pointer of teaching data.
	_ftprintf(fp, _T("%ld\r\n"), spec->fIdx);

	// get data length of teaching data.
	_ftprintf(fp, _T("%ld\r\n"), spec->length);

	// get specified T.C. combination.
	_ftprintf(fp, _T("%d,%d\r\n"), spec->arg1, spec->arg2);

	// get specified T.C. combination.
	_ftprintf(fp, _T("%f\r\n"), spec->fFilter);

	// get specified T.C. combination.
	_ftprintf(fp, _T("%d\r\n"), spec->shift);

	// close the file.
	fclose(fp);

	return NORMAL;
}

// load teaching data.
int CBpOpe::LoadTeachData(TeachDataSpec *spec, SampledData *teachData, int *numTeachData, long offset)
{
	DataFileHeader	header;
	SampledData		buf;
	SampledData		bufBk;
	FILE			*fd;
	FILE			*fdBk;
	LPCTSTR			pName;
	float			fFilter;
	int				n, i, j;
	long			idx;

	// initialize the numbers of data in buffer.
	*numTeachData	= 0;

	// open logging data file. return if failed.
	if ((fd = _tfopen(spec->path, _T("rb"))) == NULL)
	{
		// Tempフォルダも探す
		pName = bolog.GetTempLogFilePath(spec->path);
		if(pName == NULL) return ERR_LOG_FILE;
		if ((fd = fopen(pName, _T("rb"))) == NULL) 
			return ERR_SPEC_FILE;
	}

	// read header data.
	fread(&header, sizeof(header), 1, fd);

	// calculate reading point.
	idx	= spec->fIdx + spec->length - SelTrendData + offset;
	if (idx < 0) idx	= 0;

	/////////////////////////////////////////////////////////////////////////

	// filter calc for ver6.20
	if( spec->dtHead.header.version >= 620 && m_strTrendOutFilePath.GetLength() > 0 )
	{
		// open the trend data file. return 0 if failed.
		if ((fdBk = _tfopen( m_strTrendOutFilePath, _T("wb+") )) != NULL)
		{
			fwrite(&header, sizeof(header), 1, fdBk);
			// read data
			if( fread(&bufBk, sizeof(SampledData), 1, fd) == 1)
			{
				fwrite(&bufBk, sizeof(SampledData), 1, fdBk);
				for( i = 0; i < (idx + NumTrendData); i++ )
				{
					if( fread(&buf, sizeof(SampledData), 1, fd) != 1)
						break;

					// for each A/D channel,
					for(j = 0; j < NumADChannel; j++)
					{
						switch(header.header.arg[j])
						{
						case -1:
						case SNID:		// SN Channel No.
						case LevelID:	// Level Channel No.
						case VcID: 		// P/R Speed Channel No.
						case LengthID:	// Length Channel No.
							fFilter = 1.0;	break;
						default:		// Temp Channel No.
							fFilter = spec->fFilter;break;
						}
						if( header.header.arg[j] >= LastID )
							fFilter = 1.0;
						bufBk.val[j] =	(short)((float)bufBk.val[j] * (1.0f - fFilter) +
												(float)buf.val[j] * fFilter);
					}
					fwrite(&bufBk, sizeof(SampledData), 1, fdBk);
				}
			}
			fclose(fd);
			fd = fdBk;
		}
	}

	/////////////////////////////////////////////////////////////////////////

#ifdef Compatibility
	if( header.header.version <= 500 )
	{
		// seek to the head of data.
		fseek(fd, idx * sizeof(SampledDataV5) + sizeof(DataFileHeader), SEEK_SET);

		// for each sampled data,
		for (n = 0; n < NumTrendData; n++, teachData++) {
			// if no more data, break.
			if (fread(teachData, sizeof(SampledDataV5), 1, fd) != 1)
				break;
			for( i = NumADChannelV5; i < NumADChannel; i++ )
				teachData->val[i] = 0;
		}
	}
	else
	{
		// seek to the head of data.
		fseek(fd, idx * sizeof(SampledData) + sizeof(DataFileHeader), SEEK_SET);

		// for each sampled data,
		for (n = 0; n < NumTrendData; n++, teachData++) {
			// if no more data, break.
			if (fread(teachData, sizeof(*teachData), 1, fd) != 1)
				break;
		}
	}
#else
	// seek to the head of data.
	fseek(fd, idx * sizeof(SampledData) + sizeof(DataFileHeader), SEEK_SET);

	// for each sampled data,
	for (n = 0; n < NumTrendData; n++, teachData++) {
		// if no more data, break.
		if (fread(teachData, sizeof(*teachData), 1, fd) != 1)
			break;
	}
#endif

	// close the file.
	fclose(fd);

	// store the number of read data.
	*numTeachData	= n;

	return NORMAL;
}

// set up input vector.
int	CBpOpe::SetupBOInputVector(int nOrdArg1, int nOrdArg2, int nAlmArg1, int nAlmArg2)
{
	TCCombination	tc;
	int				numTrend;

	// get the number of trend data.
	numTrend = m_pPattNN->wMatrix[0]->numIn - 1;

	// test the number of teaching data buffer.
	if (m_nNumOrdData < numTrend || m_nNumAlarmData < numTrend) {
		// return error if short of data.
		return	ERR_NUM_DATA;
	}

	// prepare the ordinary input vectors for the neural networks.
	tc.upper	= nOrdArg1;
	tc.lower	= nOrdArg2;
	tc.netID	= 0;	// 未使用なので仮に0
	if (prepareBOInputVector(m_stOrdData, NumTrendData,
//			m_nNumOrdData - numTrend, numTrend,
			SelTrendData - numTrend + m_stOrdDataSpec.shift, numTrend,
			&m_stOrdDataSpec.dtHead,
			&tc,
			m_dOrdinaryPatInput, m_dOrdinaryCorInput) == ERROR) {
		// return ERROR, if failed.
		return	ERR_ORD_PREP;
	}

	// prepare the alarm input vectors for the neural networks.
	tc.upper	= nAlmArg1;
	tc.lower	= nAlmArg2;
	tc.netID	= 0;	// 未使用なので仮に0
	if (prepareBOInputVector(m_stAlarmData, NumTrendData,
//			m_nNumAlarmData - numTrend, numTrend,
			SelTrendData - numTrend + m_stAlmDataSpec.shift, numTrend,
			&m_stAlmDataSpec.dtHead,
			&tc,
			m_dAlarmPatInput, m_dAlarmCorInput) == ERROR) {
		// return ERROR, if failed.
		return	ERR_ALM_PREP;
	}

	// after above procedures, return NORMAL.
	return	NORMAL;
}

// set up the B.P. working area of BO.
void CBpOpe::SetupBPBOWorkArea(void)
{
	// reset B.P. working area of pattern neural network.
	ResetBPWorkArea(&m_stPatternBP, m_pPattNN);

	// reset B.P. working area of correlation neural network.
	ResetBPWorkArea(&m_stCorrBP, m_pCorrNN);
}

// reset B.P. working area of neural network.
void CBpOpe::ResetBPWorkArea(BPStructure *bp, NeuralNet *net)
{
	int	i;

	// reset deltaNet field of B.P. working area.
	if (bp->deltaNet)			freeNeuralNet(bp->deltaNet);
	bp->deltaNet	= NULL;
	// reset outVec and dEdx field of pattern n.n.
	for (i = 0; i < NumNetLayer; i++) {
		if (bp->outVec[i])			free(bp->outVec[i]);
		bp->outVec[i]	= NULL;
		if (bp->dEdx[i])			free(bp->dEdx[i]);
		bp->dEdx[i]		= NULL;
	}

	// store the neural network into net field of B.P.working area.
	bp->net	= net;
}

void CBpOpe::SaveBackupBONetwork(void)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];

	// make up pattern neural network backup file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// save weights of pattern neural network.
	saveNeuralNet(path, "", m_pPattNN);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// save weights of correlation neural network.
	saveNeuralNet(path, "", m_pCorrNN);
}

// delete backup files.
void CBpOpe::DeleteBONeuralNetBackup(void)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];

	// make up pattern neural network backup file name.
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// delete the file.
	_tremove(path);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// delete the file.
	_tremove(path);
}


///////////////////////////////////////////////////////////////////////////////
// member function.(public)

void CBpOpe::CalcNeuralNetwork(
		double *pdOrdPatOut, double *pdAlmPatOut, 
		 double *pdOrdCorOut, double *pdAlmCorOut)
{
	doBONeuralNet(
		m_stPatternBP.net, m_stCorrBP.net,
		m_dOrdinaryPatInput, m_dOrdinaryCorInput,
		pdOrdPatOut, pdOrdCorOut);
	doBONeuralNet(
		m_stPatternBP.net, m_stCorrBP.net,
		m_dAlarmPatInput, m_dAlarmCorInput,
		pdAlmPatOut, pdAlmCorOut);
}

// load specified BO neural networks.
int CBpOpe::LoadBONeuralNetwork(int nNetIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];
	int		ret = NORMAL;
	NNTemplate	*p;
	int	isFirst = TRUE;

	// if the neural networks are not specified, do nothing.
	if (nNetIdx < 0) {
		return ERR_NO_NET;
	}
	m_nNetIdx = nNetIdx;

	// free B.O. neural networks.
	FreeBONeuralNetwork();

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFile, nNetIdx);
	_tcscat(path, buf);

	// load the weights of pattern neural network from file.
	m_pPattNN	= loadNeuralNet(path);
	p	= &cnf.neuralnetConf[nNetIdx].patternNN;
	// if failed, create a new network and set random weights.
	if (!m_pPattNN) {
		isFirst	= FALSE;
		m_pPattNN	= newNeuralNet(p->numLayer, p->numElm);
		setRandomWeight(m_pPattNN);
		ret = ERR_PATT;
	}

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFile, nNetIdx);
	_tcscat(path, buf);

	// load the weights of correlation neural network from file.
	m_pCorrNN	= loadNeuralNet(path);
	p	= &cnf.neuralnetConf[nNetIdx].corrNN;
	// if faild, create a new network and set random weights.
	if (!m_pCorrNN) {
		if (!isFirst) 
			ret = ERR_PATT_CORR;
		else
			ret = ERR_CORR;
		isFirst	= FALSE;
		m_pCorrNN	= newNeuralNet(p->numLayer, p->numElm);
		setRandomWeight(m_pCorrNN);
	}

	return ret;
}

// load specified BO neural networks for tempo.
int CBpOpe::LoadBONeuralNetworkTmp(int nNetIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];
	int		ret = NORMAL;
	NNTemplate	*p;
	int	isFirst = TRUE;

	// if the neural networks are not specified, do nothing.
	if (nNetIdx < 0) {
		return ERR_NO_NET;
	}
	m_nNetIdx = nNetIdx;

	// free B.O. neural networks.
	FreeBONeuralNetwork();

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// load the weights of pattern neural network from file.
	m_pPattNN	= loadNeuralNet(path);
	p	= &cnf.neuralnetConf[nNetIdx].patternNN;
	// if failed, create a new network and set random weights.
	if (!m_pPattNN) {
		isFirst	= FALSE;
		//m_pPattNN	= newNeuralNet(p->numLayer, p->numElm);
		//setRandomWeight(m_pPattNN);
		ret = ERR_PATT;
	}

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// load the weights of correlation neural network from file.
	m_pCorrNN	= loadNeuralNet(path);
	p	= &cnf.neuralnetConf[nNetIdx].corrNN;
	// if faild, create a new network and set random weights.
	if (!m_pCorrNN) {
		if (!isFirst) 
			ret = ERR_PATT_CORR;
		else
			ret = ERR_CORR;
		isFirst	= FALSE;
		//m_pCorrNN	= newNeuralNet(p->numLayer, p->numElm);
		//setRandomWeight(m_pCorrNN);
	}

	return ret;
}

// load teaching data from log file initially.
int CBpOpe::LoadTeachingData(int nNetIdx)
{
	_TCHAR	name[256];
	int		ret, ret2;

	// initialize the numbers of data in buffer. 
	m_nNumOrdData	= 0;
	m_nNumAlarmData	= 0;
	ret2 = NORMAL;

	// read ordinary teaching data file specification.
	_stprintf(name, _T("%s\\%s") BPOrdinaryDataFile, cnf.WorkRootDir, cnf.workDir, nNetIdx);
	ret2 |= ReadTeachDataSpec(name, &m_stOrdDataSpec);

	// read alarm teaching data file specification.
	_stprintf(name, _T("%s\\%s") BPAlarmDataFile, cnf.WorkRootDir, cnf.workDir, nNetIdx);
	ret2 |= ReadTeachDataSpec(name, &m_stAlmDataSpec);

	if( ret2 != NORMAL )
		return ret2;

	// load ordinary teaching data. 
	ret = LoadTeachData(&m_stOrdDataSpec, m_stOrdData, &m_nNumOrdData, 0L);
	if( ret != NORMAL )
		return ret;

	// load alarm teaching data.
	ret = LoadTeachData(&m_stAlmDataSpec, m_stAlarmData, &m_nNumAlarmData, 0L);
	if( ret != NORMAL )
		return ret;

	return NORMAL;
}

// load teaching ordinary data from log file initially.
int CBpOpe::LoadTeachingDataOrd(int nNetIdx)
{
	_TCHAR	name[256];
	int		ret;

	// initialize the numbers of data in buffer. 
	m_nNumOrdData	= 0;

	// read ordinary teaching data file specification.
	_stprintf(name, _T("%s\\%s") BPOrdinaryDataFile, cnf.WorkRootDir, cnf.workDir, nNetIdx);
	ret = ReadTeachDataSpec(name, &m_stOrdDataSpec);
	if( ret != NORMAL )
		return ret;

	// load ordinary teaching data. 
	ret = LoadTeachData(&m_stOrdDataSpec, m_stOrdData, &m_nNumOrdData, 0L);
	if( ret != NORMAL )
		return ret;

	return NORMAL;
}

// load teaching alarm data from log file initially.
int CBpOpe::LoadTeachingDataAlm(int nNetIdx)
{
	_TCHAR	name[256];
	int		ret;

	// initialize the numbers of data in buffer. 
	m_nNumAlarmData	= 0;

	// read alarm teaching data file specification.
	_stprintf(name, _T("%s\\%s") BPAlarmDataFile, cnf.WorkRootDir, cnf.workDir, nNetIdx);
	ret = ReadTeachDataSpec(name, &m_stAlmDataSpec);
	if( ret != NORMAL )
		return ret;

	// load alarm teaching data.
	ret = LoadTeachData(&m_stAlmDataSpec, m_stAlarmData, &m_nNumAlarmData, 0L);
	if( ret != NORMAL )
		return ret;

	return NORMAL;
}

// reset the weights of BO neural networks.
void CBpOpe::ResetBONeuralNetwork(void)
{
	NNTemplate	*p;

	// if the neural networks is not specified, do nothing.
	if (m_nNetIdx < 0)	return;

	// free B.O. neural network.
	FreeBONeuralNetwork();

	// create a new neural network.
	p	= &cnf.neuralnetConf[m_nNetIdx].patternNN;
	m_pPattNN	= newNeuralNet(p->numLayer, p->numElm);

	// set random weights.
	setRandomWeight(m_pPattNN);

	// create a new neural network.
	p	= &cnf.neuralnetConf[m_nNetIdx].corrNN;
	m_pCorrNN		= newNeuralNet(p->numLayer, p->numElm);

	// set random weights.
	setRandomWeight(m_pCorrNN);
}

// free B.O. neural networks.
void CBpOpe::FreeBONeuralNetwork(void)
{
	// if pattern neural network is in memory, free the network.
	if (m_pPattNN)		freeNeuralNet(m_pPattNN);
	m_pPattNN	= NULL;

	// if correlation neural network is in memory, free the network.
	if (m_pCorrNN)			freeNeuralNet(m_pCorrNN);
	m_pCorrNN		= NULL;
}


// set up back propagation BO neural networks.
int CBpOpe::DoBPBONeuralNetwork(int nOrdArg1, int nOrdArg2, int nAlmArg1, int nAlmArg2 )
{
	int		ret;

	// if the neural networks are not specified, return error.
	if (m_nNetIdx < 0) {
		return	ERR_NO_NET;
	}

	// if the neural network is not generated, return error.
	if (!m_pPattNN || !m_pCorrNN) {
		return	ERR_LOAD_NN;
	}

	// set up input vector. return error if failed.
	if ((ret = SetupBOInputVector(nOrdArg1, nOrdArg2, nAlmArg1, nAlmArg2)) != NORMAL) {
		return ret;
	}

	// set up the B.P. working area of BO.
	SetupBPBOWorkArea();

	// BAKファイルを１つ前にシフト
	MoveOldBackupBONetwork();

	// save backup for recovery.
	SaveBackupBONetwork();

	// step execution of back propagation.
	StepBPBONeuralNetwork();

	// 学習回数カウントアップ
	m_pPattNN->times++;
	m_pCorrNN->times++;

	// after all procedures, return OK.
	return	NORMAL;
}

// step execution of back propagation.
void CBpOpe::StepBPBONeuralNetwork(void)
{
	int	times;
	double	ordPatOut, almPatOut;

	// repeat specified times following procedure.
	for (times = 0; times < m_nRepeatTimes; times++) {
		// execute backpropagation of pattern neural network.
		if (m_nTeachCondition & IsTeachPatOrdinary)
			doBPNeuralNet(&m_stPatternBP,m_dOrdinaryPatInput,&m_dPattOrdTeachVal);
		if (m_nTeachCondition & IsTeachPatAlarm)
			doBPNeuralNet(&m_stPatternBP, m_dAlarmPatInput, &m_dPattAlmTeachVal);

		// calculate output of pattern neural networks.
		doNeuralNet(m_stPatternBP.net, m_dOrdinaryPatInput, &ordPatOut);
		doNeuralNet(m_stPatternBP.net, m_dAlarmPatInput, &almPatOut);
		// store the outputs into correlation neural network inputs.
		m_dOrdinaryCorInput[0]		= ordPatOut;
		m_dAlarmCorInput[0]		= almPatOut;

		// execute backpropagation of pattern neural network.
		if (m_nTeachCondition & IsTeachCorOrdinary)
			doBPNeuralNet(&m_stCorrBP, m_dOrdinaryCorInput, &m_dCorrOrdTeachVal);
		if (m_nTeachCondition & IsTeachCorAlarm)
			doBPNeuralNet(&m_stCorrBP, m_dAlarmCorInput, &m_dCorrAlmTeachVal);
	}
}

// recover BO neural networks.
void CBpOpe::RecoverBONeuralNetwork(void)
{
	_TCHAR		path[256];
	_TCHAR		buf[32];
	NeuralNet	*p;

	// if the neural networks are not specified, do nothing.
	if (m_nNetIdx < 0)	return;

	// make up pattern neural network backup file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// if the backup of pattern neural network exists,
	p	= m_pPattNN;
	if (p && (m_pPattNN = loadNeuralNet(path)) != NULL) {
		// free pattern neural network.
		freeNeuralNet(p);
	}
	// otherwise, keep now network.
	else {
		m_pPattNN	= p;
	}

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// if the backup of correlation neural network exists,
	p	= m_pCorrNN;
	if (p && (m_pCorrNN = loadNeuralNet(path)) != NULL) {
		// free correlation neural network.
		freeNeuralNet(p);
	}
	// otherwise, keep now network.
	else {
		m_pCorrNN	= p;
	}

	// if neural networks are valid,
	if (m_pPattNN && m_pCorrNN) {
		// set up the B.P. working area of BO.
		SetupBPBOWorkArea();
	}
}

// recover BO neural networks.
void CBpOpe::RecoverBONeuralNetwork(LPTSTR pPathP, LPTSTR pPathC)
{
	NeuralNet	*p;

	// if the neural networks are not specified, do nothing.
	if (m_nNetIdx < 0 || pPathP == NULL || pPathC == NULL)	return;

	// if the backup of pattern neural network exists,
	p	= m_pPattNN;
	if (p && (m_pPattNN = loadNeuralNet(pPathP)) != NULL) {
		// free pattern neural network.
		freeNeuralNet(p);
	}
	// otherwise, keep now network.
	else {
		m_pPattNN	= p;
	}

	// if the backup of correlation neural network exists,
	p	= m_pCorrNN;
	if (p && (m_pCorrNN = loadNeuralNet(pPathC)) != NULL) {
		// free correlation neural network.
		freeNeuralNet(p);
	}
	// otherwise, keep now network.
	else {
		m_pCorrNN	= p;
	}

	// if neural networks are valid,
	if (m_pPattNN && m_pCorrNN) {
		// set up the B.P. working area of BO.
		SetupBPBOWorkArea();
	}
}

// save these BO neural networks.
int CBpOpe::SaveBONeuralNetwork(void)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];

	// if the neural networks are not specified, do nothing.
	if (m_nNetIdx < 0) {
		return ERR_NO_NET;
	}

	// if the neural networks are not loaded nor initialized, do noting.
	if (!m_pPattNN || !m_pCorrNN) {
		return ERR_NO_INIT;
	}

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightFile, m_nNetIdx);
	strcat(path, buf);

	// save weights of pattern neural network.
	saveNeuralNet(path, "", m_pPattNN);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFile, m_nNetIdx);
	strcat(path, buf);

	// save weights of correlation neural network.
	saveNeuralNet(path, "", m_pCorrNN);

	return NORMAL;
}

// save these BO neural networks for tempo.
int CBpOpe::SaveBONeuralNetworkTmp(void)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];

	// if the neural networks are not specified, do nothing.
	if (m_nNetIdx < 0) {
		return ERR_NO_NET;
	}

	// if the neural networks are not loaded nor initialized, do noting.
	if (!m_pPattNN || !m_pCorrNN) {
		return ERR_NO_INIT;
	}

	// make up pattern neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightFileTmp, m_nNetIdx);
	strcat(path, buf);

	// save weights of pattern neural network.
	saveNeuralNet(path, "", m_pPattNN);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFileTmp, m_nNetIdx);
	strcat(path, buf);

	// save weights of correlation neural network.
	saveNeuralNet(path, "", m_pCorrNN);

	return NORMAL;
}

// save backup for recovery.
void CBpOpe::SetTeachVal(double dPattOrd, double dPattAlm, double dCorrOrd, double dCorrAlm)
{
	m_dPattOrdTeachVal = dPattOrd;
	m_dPattAlmTeachVal = dPattAlm;
	m_dCorrOrdTeachVal = dCorrOrd;
	m_dCorrAlmTeachVal = dCorrAlm;
}

// set neural network parameter.
void CBpOpe::SetNeuralParam(double dEatha, double dAlpha)
{
	eatha = dEatha;
	alpha = dAlpha;
}

// set teach condition.
void CBpOpe::SetTeachCondition(int nCondition)
{
	m_nTeachCondition = (nCondition & IsMask);
}

// Backファイルを１つシフト
void CBpOpe::MoveOldBackupBONetwork(void)
{
	CString		str;
	WeiInfoList	listP[WEI_BAK_CNT];
	WeiInfoList	listC[WEI_BAK_CNT];
	int			nCntP, nCntC;
	_TCHAR		path[256];
	_TCHAR		buf[32];


	// make up pattern neural network backup file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );

	// 学習結果データリストを取得
	nCntP = cnf.GetWeiListP(path, m_nNetIdx, listP);
	nCntC = cnf.GetWeiListC(path, m_nNetIdx, listC);

	// 最大数なら一番古いデータを削除
	if( nCntP == WEI_BAK_CNT )
	{
		// 最古のBackファイルを削除する（パターン）
		_tremove(listP[WEI_BAK_CNT-1].path);
		_tremove(listC[WEI_BAK_CNT-1].path);
	}

	// make up pattern neural network backup file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, PatNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// 現状のBackファイルを名前変更する（パターン）
	str.Format(_T("%s%ld"), path, listP[0].updateTime);
	_trename(path, str);

	// make up correlation neural network file name.
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightBackup, m_nNetIdx);
	_tcscat(path, buf);

	// 現状のBackファイルを名前変更する（総合）
	str.Format(_T("%s%ld"), path, listC[0].updateTime);
	_trename(path, str);
}

// BPエリアをクリア
void CBpOpe::ClearBpBONetwork(void)
{
	SetupBPBOWorkArea();
}

// ニューラルネット情報（一時ファイル）が存在するかチェック
BOOL CBpOpe::IsBONeuralNetworkTmp(int nNetIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];
	int		ret = NORMAL;
	int	isFirst = TRUE;

	// ネット番号が不正なら存在しないを返す
	if (nNetIdx < 0) {
		return FALSE;
	}

	// パターンニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// パターンニューラルネットファイルの存在チェック
	if( !::PathFileExists( path ) )
	{
		// 存在しない
		return FALSE;
	}

	// 総合ニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// パターンニューラルネットファイルの存在チェック
	if( !::PathFileExists( path ) )
	{
		// 存在しない
		return FALSE;
	}

	// 存在する
	return TRUE;
}

// ニューラルネット情報（一時ファイル）を削除する
BOOL CBpOpe::DelBONeuralNetworkTmp(int nNetIdx)
{
	_TCHAR	path[256];
	_TCHAR	buf[32];
	int		ret = NORMAL;
	int	isFirst = TRUE;

	// ネット番号が不正なら存在しないを返す
	if (nNetIdx < 0) {
		return FALSE;
	}

	// パターンニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// 一時ファイル削除
	_tremove(path);

	// 総合ニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// 一時ファイル削除
	_tremove(path);

	// 存在する
	return TRUE;
}

// ニューラルネット情報（一時ファイル）を正式ファイルにコピーする
BOOL CBpOpe::CopyBONeuralNetworkTmp(int nNetIdx)
{
	_TCHAR	path[256];
	_TCHAR	path2[256];
	_TCHAR	buf[32];
	int		ret = NORMAL;
	int	isFirst = TRUE;

	// ネット番号が不正なら存在しないを返す
	if (nNetIdx < 0) {
		return FALSE;
	}

	// パターンニューラルネット（一時ファイル）のファイルパスを生成
	_tcscat( _tcscat( _tcscpy(path, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// パターンニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy(path2, cnf.WorkRootDir ), _T("\\")), cnf.workDir );
	_stprintf(buf, PatNNWeightFile, nNetIdx);
	_tcscat(path2, buf);

	// 一時ファイルを正式にコピー
	cnf.FileCopy(path2, path);

	// 総合ニューラルネット（一時ファイル）のファイルパスを生成
	_tcscat( _tcscat( _tcscpy( path, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFileTmp, nNetIdx);
	_tcscat(path, buf);

	// 総合ニューラルネットのファイルパスを生成
	_tcscat( _tcscat( _tcscpy( path2, cnf.WorkRootDir ), _T("\\") ), cnf.workDir );
	_stprintf(buf, CorrNNWeightFile, nNetIdx);
	_tcscat(path2, buf);

	// 一時ファイルを正式にコピー
	cnf.FileCopy(path2, path);

	// 存在する
	return TRUE;
}

