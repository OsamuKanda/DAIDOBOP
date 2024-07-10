#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"BoBatch.h"

#ifdef BATCH_OUT
#include	<stdio.h>
double	outputPb[NumCombination], outputCb[NumCombination];
#endif//BATCH_OUT

///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnfBatch;
static CConfig	&cnfD = theApp.m_cnfBatchD;
static NeuralNetConf	*neuralnetConf = theApp.m_cnfBatch.neuralnetConf;
static NeuralNetConf	*neuralnetConfD = theApp.m_cnfBatchD.neuralnetConf;
static CBolog	&lg = theApp.m_log;
static TcName	*tcnm	= &theApp.m_stTcNm;

#define MAXTEMP		3000
#define MINTEMP		0


///////////////////////////////////////////////////////////////////////////////
// CBoBatch class 
CBoBatch::CBoBatch()
{
	// initialize member
	m_inFp				= NULL;
	m_pSampledData		= NULL;	
	m_pSampledDataD		= NULL;	
	m_nInputSize		= 0;
	m_nNumSampledData	= 0;		
	m_nSampledDataRIdx	= 0;		
	m_nSampledDataWIdx	= 0;
	m_nInputSizeD		= 0;
	m_nNumSampledDataD	= 0;		
	m_nSampledDataRIdxD	= 0;		
	m_nSampledDataWIdxD	= 0;
	m_pResultTbl		= NULL;
	m_pResultTblD		= NULL;
	m_nNumMaxResult		= 0;
	m_nNumResultCount	= 0;
	m_nBoAlarmTime		= 0;
	m_nSamplingPeriod	= 500;
	m_clBoLineColor		= 0;
	m_nFileSize			= 0;
	m_nRecCur			= 0;
	m_nRecCnt			= 0;
	m_bCalcStart		= FALSE;
	m_bFirstSim			= TRUE;
	m_bFilterSwitch		= FALSE;
	m_fFilterTemp		= 1.0f;
	m_fFilterSn			= 1.0f;
	m_fFilterLv			= 1.0f;
	m_fFilterPr			= 1.0f;
	m_fFilterLen		= 1.0f;
	memset(&m_header,		0, sizeof(m_header));
	memset(&m_blStatusTbl,	0, sizeof(m_blStatusTbl));
	memset(&m_stCondInfo,	0, sizeof(m_stCondInfo));
	memset(&m_stBsInfo,		0, sizeof(m_stBsInfo));
	memset(&m_gr,			0, sizeof(m_gr));
	memset(&m_grD,			0, sizeof(m_grD));
	memset( m_pPattNN,		0, sizeof(m_pPattNN) );
	memset( m_pCorrNN,		0, sizeof(m_pCorrNN) );
	memset( m_pPattNND,		0, sizeof(m_pPattNND) );
	memset( m_pCorrNND,		0, sizeof(m_pCorrNND) );
	memset( m_dPattInputVec,0, sizeof(m_dPattInputVec) );
	memset( m_dCorrInputVec,0, sizeof(m_dCorrInputVec) );

#ifdef BATCH_OUT
	m_bEnaCsvOut	= FALSE;
	m_fpCsvOut		= NULL;
	m_fpCsvOutD		= NULL;
	m_strLogName	= _T("");
#endif//BATCH_OUT
}

CBoBatch::~CBoBatch()
{
	// destroy buffers of teaching data.
	ClearBOBatchInfo();
}


///////////////////////////////////////////////////////////////////////////////
// member function.(protected)

int CBoBatch::SetupBosysInfo(int sel)
{
	// read Sampling Period from configration file.
	if(FALSE == cnf.ReadSamplingPeriod(sel, &m_nSamplingPeriod))
	{
		return ERR_OPEN_SMPL;
	}

	// read bo execute condition from configration file.
	if(FALSE == cnf.ReadBosysCondition(&m_stCondInfo))
	{
		return ERR_OPEN_COND;
	}

	// initialize the Bosys Information.
	memset(&m_stBsInfo, 0, sizeof(m_stBsInfo));

	// Bosys information is read.
	if(FALSE == cnf.ReadBosysInfo(sel, &m_stBsInfo))
	{
		return ERR_OPEN_BOSYS;
	}

	return NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
// member function.(public)

// reset B.O. prediction Information
void CBoBatch::ResetBOBatchInfo(int sel, int nMoldSize)
{
	int     i, numIn, maxIn;

	// setup bo system info.
	SetupBosysInfo(sel);

	// load configuration
	cnf.LoadConfiguration(m_stBsInfo.szConfName);
	cnfD.LoadConfiguration(m_stBsInfo.szConfNameD);

	// create group
	m_gr.CreateGroup(cnf);
	m_grD.CreateGroup(cnfD);

	// clear B.O. batch environment.
	ClearBOBatchInfo();

	// read the weights of B.O. neural networks(pattern & correlation).
	cnf.ReadBONeuralNet(m_stBsInfo.szConfName, PatNNWeightFile, m_pPattNN);
	cnf.ReadBONeuralNet(m_stBsInfo.szConfName, CorrNNWeightFile, m_pCorrNN);
	cnfD.ReadBONeuralNet(m_stBsInfo.szConfNameD, PatNNWeightFile, m_pPattNND);
	cnfD.ReadBONeuralNet(m_stBsInfo.szConfNameD, CorrNNWeightFile, m_pCorrNND);

	// get the maximum number of inputs of pattern neural networks.
	for (i = 0, maxIn = 16; i < NumNeuralNet; i++) {
		if (m_pPattNN[i]) {
			if ((numIn = m_pPattNN[i]->wMatrix[0]->numIn) > maxIn)
				maxIn   = numIn;
		}
	}
	// store the maximum input size.
	m_nInputSize = maxIn - 1;

	// get the maximum number of inputs of pattern neural networks.(for dep)
	for (i = 0, maxIn = 16; i < NumNeuralNet; i++) {
		if (m_pPattNND[i]) {
			if ((numIn = m_pPattNND[i]->wMatrix[0]->numIn) > maxIn)
				maxIn   = numIn;
		}
	}
	// store the maximum input size.(for dep)
	m_nInputSizeD = maxIn - 1;

	// allocate the sample data buffer.
	m_pSampledData = (SampledData *)calloc(m_nInputSize, sizeof(SampledData));
	m_nNumSampledData  = 0;
	m_nSampledDataRIdx = 0;
	m_nSampledDataWIdx = 0;

	// allocate the sample data buffer.(for dep)
	m_pSampledDataD = (SampledData *)calloc(m_nInputSizeD, sizeof(SampledData));
	m_nNumSampledDataD  = 0;
	m_nSampledDataRIdxD = 0;
	m_nSampledDataWIdxD = 0;

	// It computes the number of the elements of xcalculation result table.
	m_nNumMaxResult		= MaxWatchCount;
//	m_nNumMaxResult		= m_stBsInfo.boWatchTime / m_nSamplingPeriod;
	m_nNumResultCount	= 0;
	m_nNumResultCountD	= 0;

	// It secures a calculation result table.
	m_pResultTbl		= (ResultData *)calloc(m_nNumMaxResult, sizeof(ResultData));
	m_nBoAlarmTime		= 0;

	// It secures a calculation result table.(for dep)
	m_pResultTblD		= (ResultData *)calloc(m_nNumMaxResult, sizeof(ResultData));
	m_nBoAlarmTimeD		= 0;

	// clear status table
	memset(m_blStatusTbl, 0, sizeof(m_blStatusTbl));
	memset(m_blStatusTblD, 0, sizeof(m_blStatusTblD));

	// initialize calc start flag
	m_bCalcStart = FALSE;
	m_bCalcStartD = FALSE;

#ifdef BATCH_OUT
	CString	strFile;
	int		ch, arg, upper, lower;

	//@ 追加機能
	if( m_bEnaCsvOut )
	{
		strFile = _T("BatchOut_") + m_strLogName + _T(".csv");
		m_fpCsvOut = fopen(strFile, _T("wt+"));
		if( m_fpCsvOut != NULL )
		{
			// タイトル書込み
			fprintf(m_fpCsvOut, "time,");
			for( i = 0; i < NumInput; i++ )
			{
				if( (ch = cnf.inputPoint[i].adCh) != -1 )
				{
					arg = cnf.adChannel[ch].arg;
					switch(arg)
					{
					case LevelID:
						fprintf(m_fpCsvOut, "レベル,");
						break;
					case VcID:
						fprintf(m_fpCsvOut, "鋳込速度,");
						break;
					case SNID:
						fprintf(m_fpCsvOut, "SN開度,");
						break;
					case LengthID:
						fprintf(m_fpCsvOut, "鋳込長,");
						break;
					case TdWeiID:
						fprintf(m_fpCsvOut, "TD重量,");
						break;
					case MdWtAID:
						fprintf(m_fpCsvOut, "MD冷却水流量A面内,");
						break;
					case MdWtCID:
						fprintf(m_fpCsvOut, "MD冷却水流量C面外,");
						break;
					case MdWtDID:
						fprintf(m_fpCsvOut, "MD冷却水流量D面右,");
						break;
					case MdTmpDID:
						fprintf(m_fpCsvOut, "MD冷却水温度D面右,");
						break;
					default:
						if( arg < NumTCTap )
							fprintf(m_fpCsvOut, "%s,", tcnm->name[arg]);
						break;
					}
				}
			}
			// タイトル２書込み
			for( i = 0; i < cnf.numCombination; i++ )
			{
				upper = cnf.tcCombination[i].upper;
				lower = cnf.tcCombination[i].lower;
				if( upper >= 0 && upper < NumTCTap && lower >= 0 && lower < NumTCTap )
				{
					fprintf(m_fpCsvOut, "(%s:%s)P,(%s:%s)C,", 
						tcnm->name[upper], tcnm->name[lower],
						tcnm->name[upper], tcnm->name[lower]);
				}
//				for(j = 0; j < m_nInputSize+1; j++)
//					fprintf(m_fpCsvOut, "Y1_%d,", j+1); 
//				fprintf(m_fpCsvOut, "Y2,"); 
			}

			fprintf(m_fpCsvOut, "\n");
		}

		// for dep.
		strFile = _T("BatchOutD_") + m_strLogName + _T(".csv");
		m_fpCsvOutD = fopen(strFile, _T("wt+"));
		if( m_fpCsvOutD != NULL )
		{
			// タイトル書込み
			fprintf(m_fpCsvOutD, "time,");
			for( i = 0; i < NumInput; i++ )
			{
				if( (ch = cnfD.inputPoint[i].adCh) != -1 )
				{
					arg = cnfD.adChannel[ch].arg;
					switch(arg)
					{
					case LevelID:
						fprintf(m_fpCsvOutD, "レベル,");
						break;
					case VcID:
						fprintf(m_fpCsvOutD, "鋳込速度,");
						break;
					case SNID:
						fprintf(m_fpCsvOutD, "SN開度,");
						break;
					case LengthID:
						fprintf(m_fpCsvOutD, "鋳込長,");
						break;
					case TdWeiID:
						fprintf(m_fpCsvOutD, "TD重量,");
						break;
					case MdWtAID:
						fprintf(m_fpCsvOutD, "MD冷却水流量A面内,");
						break;
					case MdWtCID:
						fprintf(m_fpCsvOutD, "MD冷却水流量C面外,");
						break;
					case MdWtDID:
						fprintf(m_fpCsvOutD, "MD冷却水流量D面右,");
						break;
					case MdTmpDID:
						fprintf(m_fpCsvOutD, "MD冷却水温度D面右,");
						break;
					default:
						if( arg < NumTCTap )
							fprintf(m_fpCsvOutD, "%s,", tcnm->name[arg]);
						break;
					}
				}
			}
			// タイトル２書込み
			for( i = 0; i < cnfD.numCombination; i++ )
			{
				upper = cnfD.tcCombination[i].upper;
				lower = cnfD.tcCombination[i].lower;
				if( upper >= 0 && upper < NumTCTap && lower >= 0 && lower < NumTCTap )
				{
					fprintf(m_fpCsvOutD, "(%s:%s)P,(%s:%s)C,", 
						tcnm->name[upper], tcnm->name[lower],
						tcnm->name[upper], tcnm->name[lower]);
				}
//				for(j = 0; j < m_nInputSize+1; j++)
//					fprintf(m_fpCsvOut, "Y1_%d,", j+1); 
//				fprintf(m_fpCsvOut, "Y2,"); 
			}

			fprintf(m_fpCsvOutD, "\n");
		}
	}
#endif//BATCH_OUT

	// set bo line color.
//	if( m_stBsInfo.numCombiBA == 1 && m_stBsInfo.numCombiBAC == 1 )
//		m_clBoLineColor = RGB(128, 128, 0);
//	else
//		m_clBoLineColor = RGB(205, 0, 0);
}

// clear B.O. prediction Information
void CBoBatch::ClearBOBatchInfo(void)
{
	// reset bo neural net area.
	cnf.ResetBONeuralNet(m_pPattNN);
	cnf.ResetBONeuralNet(m_pCorrNN);
	cnfD.ResetBONeuralNet(m_pPattNND);
	cnfD.ResetBONeuralNet(m_pCorrNND);

	// if the input ring buffer for the neural networks, free it.
	if(m_pSampledData != NULL)
	{
		free(m_pSampledData);
		m_pSampledData = NULL;
	}

	// It releases if a calculation result table is being secured.
	if(m_pResultTbl != NULL) 
	{
		free(m_pResultTbl);
		m_pResultTbl = NULL;
	}

#ifdef BATCH_OUT
	//@ 追加機能
	if( m_bEnaCsvOut )
	{
		if( m_fpCsvOut != NULL )
			fclose(m_fpCsvOut);
		m_fpCsvOut = NULL;
		if( m_fpCsvOutD != NULL )
			fclose(m_fpCsvOutD);
		m_fpCsvOutD = NULL;
	}
#endif//BATCH_OUT
}

// bo prediction
int CBoBatch::DoBOBatch(SampledData *data, ResultData *result)
{
	int				i,k,j,n,boCount;
	TCCombination	*cp;
	int				netID, numIn;
	NeuralNet		*patNet, *corrNet;
	double			output;
	int				status;
	int				nCombs, nResCount;
	int				ch;
	float			fVc, fLen;

	// get vc
	ch = cnf.inputPoint[VcID].adCh;
	if( ch != -1 )
		fVc = (float)(data->val[ch] * PhisicalVcUnit);
	else
		fVc = 99.9f;

	// get length
	ch = cnf.inputPoint[LengthID].adCh;
	if( ch != -1 )
		fLen = (float)(data->val[ch] * PhisicalLengthUnit);
	else
		fLen = 999999.9f;

	// append the sampled data. 
	m_pSampledData[m_nSampledDataWIdx++]  = *data;
	if (m_nSampledDataWIdx == m_nInputSize) m_nSampledDataWIdx = 0;
	if (++m_nNumSampledData > m_nInputSize) m_nNumSampledData  = m_nInputSize;

	//  start Wait time check.条件成立で検出開始
	if( fLen > m_stCondInfo.fCondLen && fVc > m_stCondInfo.fCondVc )
		m_bCalcStart = TRUE;
//	else if( fLen <= m_stCondInfo.fCondLen ) 鋳込長がChargeChangeで0リセットされる為OFF
//		m_bCalcStart = FALSE;

	// for each thermo couple combination, 
	status  = 0;
	for (i = 0, cp = cnf.tcCombination; i < cnf.numCombination; i++, cp++)
	{
		// clear the output. 
		output	= 0.0;
#ifdef BATCH_OUT
		//@ 追加機能
		outputPb[i] = 0.0;
		outputCb[i] = 0.0;
#endif//BATCH_OUT

		// if the neural networks are configured
		//	and if the number of sampled data in the buffer is enough to
		//		execute pattern neural network
		//	and if successed preparation of the input vectors
		//		for the neural networks,
		netID	= cp->netID;
//		if (netID < 0 && netID >= NumNeuralNet)		goto next;
		if (netID < 0 || netID >= NumNeuralNet)		goto next;
		patNet	= m_pPattNN[netID];
		corrNet	= m_pCorrNN[netID];
		if (!patNet || !corrNet)					goto next;
		numIn	= patNet->wMatrix[0]->numIn;
		if (m_nNumSampledData < numIn - 1)			goto next;
		if (prepareBOInputVector(m_pSampledData, m_nInputSize,
					(m_nSampledDataRIdx - (numIn - 1) + 1 + m_nInputSize) % m_nInputSize,
					numIn - 1, &m_header, cp,
					m_dPattInputVec, m_dCorrInputVec) == ERROR)
			goto next;

		if( m_bCalcStart )
		{
			// execute the B.O. prediction neural networks
#ifndef BATCH_OUT
			doBONeuralNet(patNet, corrNet, m_dPattInputVec, m_dCorrInputVec, NULL, &output);
#else
			//@ 追加機能
			doBONeuralNet(patNet, corrNet, m_dPattInputVec, m_dCorrInputVec, &outputPb[i], &outputCb[i]);	//@
			output = outputCb[i];	//@
#endif//BATCH_OUT
		}

next:
		// store the neural network output.
		result->val[i]	= (float)output;

// ------------------------------------------------------------------------
		// It sets the latest calculation result to the calculation result table.
		CopyMemory(&m_pResultTbl[m_nBoAlarmTime],result,sizeof(*result));
		// While not being the number of the greatest calculation results, 
		// it counts the number of the calculation results.
		if(m_nNumResultCount < m_nNumMaxResult)m_nNumResultCount++;

		// if group is nomal,
		if(	//m_gr.IsGroup(cp->upper, cp->lower) &&
			cnf.inputPoint[cp->upper].adCh != -1 &&
			cnf.inputPoint[cp->lower].adCh != -1 )
		{
			// It executes only the number of the BO occurrence recognition registration.
//			for(k = 0;k < m_stBsInfo.numRegister;k++)
			for(k = 0;k < neuralnetConf[cp->netID].numRegister;k++)
			{
				boCount = 0;
				j = m_nBoAlarmTime;
				// It executes only the number of the calculation results.
//				for(n = 0;n < m_nNumResultCount;n++)
				nResCount = (neuralnetConf[cp->netID].boWatchTime / m_nSamplingPeriod);
				if( m_nNumResultCount < nResCount )
					nResCount = m_nNumResultCount;
				for(n = 0;n < nResCount;n++)
				{
					// It does BO occurrence recognition.
//					if(m_pResultTbl[j--].val[i] >= m_stBsInfo.boAlarm[k])
					if(m_pResultTbl[j--].val[i] >= neuralnetConf[cp->netID].boAlarm[k])  
					{
//						if(++boCount == m_stBsInfo.boTimes[k])
						if(++boCount == neuralnetConf[cp->netID].boTimes[k])  
						{
							status = 1;
							m_blStatusTbl[i] = TRUE;
							break;
						}
					}
					if(j < 0) j =  m_nNumMaxResult - 1;
				}
			}
		}
// --------------------------------------------------------------------------
	}

#ifdef BATCH_OUT
	int			arg;

	//@ 追加機能
	if( m_bEnaCsvOut && m_fpCsvOut != NULL )
	{
		SYSTEMTIME		st;
		FILETIME		lft;
		FileTimeToLocalFileTime(&m_ftTime, &lft);
		FileTimeToSystemTime(&lft, &st);

		// 時刻書込み
		fprintf(m_fpCsvOut, "\'%02d:%02d:%02d:%03d,",
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		// データ書込み
		for( i = 0; i < NumInput; i++ )
		{
			if( (ch = cnf.inputPoint[i].adCh) != -1 )
			{
				arg = cnf.adChannel[ch].arg;
				switch(arg)
				{
				case LevelID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * PhisicalLevelUnit);
					break;
				case VcID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * PhisicalVcUnit);
					break;
				case SNID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * PhisicalSNUnit);
					break;
				case LengthID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * PhisicalLengthUnit);
					break;
				case TdWeiID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * 0.01);
					break;
				case MdWtAID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * 1.0);
					break;
				case MdWtCID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * 1.0);
					break;
				case MdWtDID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * 1.0);
					break;
				case MdTmpDID:
					fprintf(m_fpCsvOut, "%lf,", data->val[ch] * 1.0);
					break;
				default:
					if( arg < NumTCTap )
						fprintf(m_fpCsvOut, "%lf,", data->val[ch] * PhisicalTempUnit);
					break;
				}
			}
		}
		// 出力値書込み
		for( i = 0; i < cnf.numCombination; i++ )
		{
			fprintf(m_fpCsvOut, "%lf,%lf,", outputPb[i], outputCb[i]);
//			for( j = 0; j < m_nInputSize+1; j++ )
//			{
//				fprintf(m_fpCsvOut, "%lf,", m_dPattInputVecS[i][j]);
//			}
//			fprintf(m_fpCsvOut, "%lf,", m_dCorrInputVecS[i][1]);
		}

		fprintf(m_fpCsvOut, "\n");
	}
#endif//BATCH_OUT

	// increase the ring buffer index for read operation.
	if (++m_nSampledDataRIdx == m_nInputSize) m_nSampledDataRIdx = 0;
	if(++m_nBoAlarmTime >= m_nNumMaxResult)	m_nBoAlarmTime = 0;

	// bo occur?
	if(status)
	{
		status = 0;
		// check BO group.
		for(i = 0;i < m_gr.m_nGrMax;i++)
		{
			for(j = 0, nCombs = 0;j < m_gr.m_stGr[i].cnt;j++)
			{
				if( TRUE == m_blStatusTbl[m_gr.m_stGr[i].no[j]] )
				{
					nCombs++;
				}
			}
			// if The number of the combinations >= setting value. (common) 
			if( (nCombs >= m_stBsInfo.numCombiBA && fVc >= m_stCondInfo.fCondVc) )
			{
				status |= 1;
			}
		}
	}	

	// return the system status.
	return  status;
}

// bo prediction
int CBoBatch::DoBOBatchD(SampledData *data, ResultData *result)
{
	int				i,k,j,n,boCount;
	TCCombination	*cp;
	int				netID, numIn;
	NeuralNet		*patNet, *corrNet;
	double			output;
	int				status;
	int				nCombs, nResCount;
	int				ch;
	float			fVc, fLen;

	// get vc
	ch = cnfD.inputPoint[VcID].adCh;
	if( ch != -1 )
		fVc = (float)(data->val[ch] * PhisicalVcUnit);
	else
		fVc = 99.9f;

	// get length
	ch = cnfD.inputPoint[LengthID].adCh;
	if( ch != -1 )
		fLen = (float)(data->val[ch] * PhisicalLengthUnit);
	else
		fLen = 999999.9f;

	// append the sampled data. 
	m_pSampledDataD[m_nSampledDataWIdxD++]  = *data;
	if (m_nSampledDataWIdxD == m_nInputSizeD) m_nSampledDataWIdxD = 0;
	if (++m_nNumSampledDataD > m_nInputSizeD) m_nNumSampledDataD  = m_nInputSizeD;

	//  start Wait time check.条件成立で検出開始
	if( fLen > m_stCondInfo.fCondLen && fVc > m_stCondInfo.fCondVc )
		m_bCalcStartD = TRUE;
//	else if( fLen <= m_stCondInfo.fCondLen ) 鋳込長がChargeChangeで0リセットされる為OFF
//		m_bCalcStartD = FALSE;

	// for each thermo couple combination, 
	status  = 0;
	for (i = 0, cp = cnfD.tcCombination; i < cnfD.numCombination; i++, cp++)
	{
		// clear the output. 
		output	= 0.0;
#ifdef BATCH_OUT
		//@ 追加機能
		outputPb[i] = 0.0;
		outputCb[i] = 0.0;
#endif//BATCH_OUT

		// if the neural networks are configured
		//	and if the number of sampled data in the buffer is enough to
		//		execute pattern neural network
		//	and if successed preparation of the input vectors
		//		for the neural networks,
		netID	= cp->netID;
//		if (netID < 0 && netID >= NumNeuralNet)		goto next;
		if (netID < 0 || netID >= NumNeuralNet)		goto next;
		patNet	= m_pPattNND[netID];
		corrNet	= m_pCorrNND[netID];
		if (!patNet || !corrNet)					goto next;
		numIn	= patNet->wMatrix[0]->numIn;
		if (m_nNumSampledDataD < numIn - 1)			goto next;
		if (prepareBOInputVector(m_pSampledDataD, m_nInputSizeD,
					(m_nSampledDataRIdxD - (numIn - 1) + 1 + m_nInputSizeD) % m_nInputSizeD,
					numIn - 1, &m_header, cp,
					m_dPattInputVec, m_dCorrInputVec) == ERROR)
			goto next;

		if( m_bCalcStartD )
		{
			// execute the B.O. prediction neural networks
#ifndef BATCH_OUT
			doBONeuralNet(patNet, corrNet, m_dPattInputVec, m_dCorrInputVec, NULL, &output);
#else
			//@ 追加機能
			doBONeuralNet(patNet, corrNet, m_dPattInputVec, m_dCorrInputVec, &outputPb[i], &outputCb[i]);	//@
			output = outputCb[i];	//@
#endif//BATCH_OUT
		}

next:
		// store the neural network output.
		result->val[i]	= (float)output;

// ------------------------------------------------------------------------
		// It sets the latest calculation result to the calculation result table.
		CopyMemory(&m_pResultTblD[m_nBoAlarmTimeD],result,sizeof(*result));
		// While not being the number of the greatest calculation results, 
		// it counts the number of the calculation results.
		if(m_nNumResultCountD < m_nNumMaxResult)m_nNumResultCountD++;

		// if group is nomal,
		if(	//m_gr.IsGroup(cp->upper, cp->lower) &&
			cnfD.inputPoint[cp->upper].adCh != -1 &&
			cnfD.inputPoint[cp->lower].adCh != -1 )
		{
			// It executes only the number of the BO occurrence recognition registration.
//			for(k = 0;k < m_stBsInfo.numRegister;k++)
			for(k = 0;k < neuralnetConfD[cp->netID].numRegister;k++)
			{
				boCount = 0;
				j = m_nBoAlarmTimeD;
				// It executes only the number of the calculation results.
//				for(n = 0;n < m_nNumResultCount;n++)
				nResCount = (neuralnetConfD[cp->netID].boWatchTime / m_nSamplingPeriod);
				if( m_nNumResultCountD < nResCount )
					nResCount = m_nNumResultCountD;
				for(n = 0;n < nResCount;n++)
				{
					// It does BO occurrence recognition.
//					if(m_pResultTbl[j--].val[i] >= m_stBsInfo.boAlarm[k])
					if(m_pResultTblD[j--].val[i] >= neuralnetConfD[cp->netID].boAlarm[k])  
					{
//						if(++boCount == m_stBsInfo.boTimes[k])
						if(++boCount == neuralnetConfD[cp->netID].boTimes[k])  
						{
							status = 1;
							m_blStatusTblD[i] = TRUE;
							break;
						}
					}
					if(j < 0) j =  m_nNumMaxResult - 1;
				}
			}
		}
// --------------------------------------------------------------------------
	}

#ifdef BATCH_OUT
	int			arg;

	//@ 追加機能
	if( m_bEnaCsvOut && m_fpCsvOutD != NULL )
	{
		SYSTEMTIME		st;
		FILETIME		lft;
		FileTimeToLocalFileTime(&m_ftTime, &lft);
		FileTimeToSystemTime(&lft, &st);

		// 時刻書込み
		fprintf(m_fpCsvOutD, "\'%02d:%02d:%02d:%03d,",
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		// データ書込み
		for( i = 0; i < NumInput; i++ )
		{
			if( (ch = cnfD.inputPoint[i].adCh) != -1 )
			{
				arg = cnfD.adChannel[ch].arg;
				switch(arg)
				{
				case LevelID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * PhisicalLevelUnit);
					break;
				case VcID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * PhisicalVcUnit);
					break;
				case SNID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * PhisicalSNUnit);
					break;
				case LengthID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * PhisicalLengthUnit);
					break;
				case TdWeiID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * 0.01);
					break;
				case MdWtAID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * 1.0);
					break;
				case MdWtCID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * 1.0);
					break;
				case MdWtDID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * 1.0);
					break;
				case MdTmpDID:
					fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * 1.0);
					break;
				default:
					if( arg < NumTCTap )
						fprintf(m_fpCsvOutD, "%lf,", data->val[ch] * PhisicalTempUnit);
					break;
				}
			}
		}
		// 出力値書込み
		for( i = 0; i < cnfD.numCombination; i++ )
		{
			fprintf(m_fpCsvOutD, "%lf,%lf,", outputPb[i], outputCb[i]);
//			for( j = 0; j < m_nInputSize+1; j++ )
//			{
//				fprintf(m_fpCsvOut, "%lf,", m_dPattInputVecS[i][j]);
//			}
//			fprintf(m_fpCsvOut, "%lf,", m_dCorrInputVecS[i][1]);
		}

		fprintf(m_fpCsvOutD, "\n");
	}
#endif//BATCH_OUT

	// increase the ring buffer index for read operation.
	if (++m_nSampledDataRIdxD == m_nInputSizeD) m_nSampledDataRIdxD = 0;
	if(++m_nBoAlarmTimeD >= m_nNumMaxResult)	m_nBoAlarmTimeD = 0;

	// bo occur?
	if(status)
	{
		status = 0;
		// check BO group.
		for(i = 0;i < m_grD.m_nGrMax;i++)
		{
			for(j = 0, nCombs = 0;j < m_grD.m_stGr[i].cnt;j++)
			{
				if( TRUE == m_blStatusTblD[m_grD.m_stGr[i].no[j]] )
				{
					nCombs++;
				}
			}
			// if The number of the combinations >= setting value. (common) 
			if( (nCombs >= m_stBsInfo.numCombiBAC && fVc >= m_stCondInfo.fCondVc) )
			{
				status |= 2;
			}
		}
	}	

	// return the system status.
	return  status;
}

// set log filter.
void CBoBatch::SetLogFilter(BOOL bSwitch, float fTemp, float fSn, float fLv, float fPr)
{
	m_bFilterSwitch = bSwitch;
	m_fFilterTemp	= fTemp;
	m_fFilterSn		= fSn;
	m_fFilterLv		= fLv;
	m_fFilterPr		= fPr;
}

// set up the logging file.
BOOL CBoBatch::SetupLogFile(LPCTSTR name, int nIndex)
{
	_TCHAR	buf[256];

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( buf, lg.LogRootDir ), _T("\\") ), name );

	// open log file.
	if (NULL == (m_inFp = _tfopen(buf, _T("rb"))))
		return FALSE;

	// get file size
	WIN32_FIND_DATA	stfd;
	HANDLE hFind = FindFirstFile(buf, &stfd);
	m_nFileSize = stfd.nFileSizeLow;
	FindClose(hFind);

	// calc file info
	m_nRecCur = 0;
#ifdef Compatibility
	if(m_header.header.version <= 500)
		m_nRecCnt = (m_nFileSize - sizeof(DataFileHeader)) / sizeof(SampledDataV5)-1;
	else
		m_nRecCnt = (m_nFileSize - sizeof(DataFileHeader)) / sizeof(SampledData)-1;
#else
	m_nRecCnt = (m_nFileSize - sizeof(DataFileHeader)) / sizeof(SampledData)-1;
#endif

	// read sample data.
	fread(&m_header, sizeof(DataFileHeader), 1, m_inFp);
#ifdef Compatibility
	if(m_header.header.version <= 500)
		fseek(m_inFp, nIndex * sizeof(SampledDataV5), 1);
	else
		fseek(m_inFp, nIndex * sizeof(SampledData), 1);
#else
	fseek(m_inFp, nIndex * sizeof(SampledData), 1);
#endif

	m_bFirstSim = TRUE;

	return TRUE;
}

// get the sampled data from the logging file.
BOOL CBoBatch::GetSampledData(SampledData *data)
{
	float	fFilter;
	int		nSize, j;

	// if the input file descriptor is not valid, return FALSE.
	if (m_inFp == NULL) return	FALSE;
	
	// read the sampled data from the file. if successed, return TRUE.
#ifdef Compatibility
	if( m_header.header.version <= 500){
		nSize = sizeof(SampledDataV5);
		memset(data, 0, sizeof(SampledData));
	}
	else
		nSize = sizeof(SampledData);
#else
	nSize = sizeof(SampledData);
#endif
	if (fread(data, nSize, 1, m_inFp) == 1)
	{
		// filter?
		if( m_header.header.version >= 620 && m_bFilterSwitch == TRUE )
		{
			if( m_bFirstSim )
			{
				m_dataBk = *data;
				m_bFirstSim = FALSE;
			}

			// for each A/D channel,
			for(j = 0; j < NumADChannel; j++)
			{
				switch(cnf.adChannel[j].arg)
				{
				case -1:
					fFilter = 1.0; break;
				case SNID:		// SN Channel No.
					fFilter = m_fFilterSn;	break;
				case LevelID:	// Level Channel No.
					fFilter = m_fFilterLv;	break;
				case VcID: 		// P/R Speed Channel No.
					fFilter = m_fFilterPr;	break;
				case LengthID:	// Length Channel No.
					fFilter = m_fFilterLen;	break;
				default:		// Temp Channel No.
					fFilter = m_fFilterTemp;break;
				}
				if( cnf.adChannel[j].arg >= LastID )
					fFilter = 1.0;
				m_dataBk.val[j] =	(short)((float)m_dataBk.val[j] * (1.0f - fFilter) +
											(float)(*data).val[j]  * fFilter);
			}
			*data = m_dataBk;
		}

		m_nRecCur++;
		return	TRUE;
	}

	// discard the logging file.
	DiscardLogFile();

	return	FALSE;
}

// discard the logging file.
void CBoBatch::DiscardLogFile(void)
{
	// if log file is open,
	if (m_inFp != NULL) 
	{
		// close log file.
		fclose(m_inFp);
		m_inFp	= NULL;
	}
}

int CBoBatch::GetRecordCount(void)
{
	return m_nRecCnt;
}

int CBoBatch::GetCurrentRecord(void)
{
	return m_nRecCur;
}

int CBoBatch::SetLogPointer(int nPoint)
{
	int	nOffset;

	if( m_inFp == NULL )
		return -1;

#ifdef Compatibility
	if( m_header.header.version <= 500)
		nOffset = sizeof(DataFileHeader) + sizeof(SampledDataV5) * nPoint;
	else
		nOffset = sizeof(DataFileHeader) + sizeof(SampledData) * nPoint;
#else
	nOffset = sizeof(DataFileHeader) + sizeof(SampledData) * nPoint;
#endif

	fseek( m_inFp, nOffset, SEEK_SET );
	m_nRecCur = nPoint;

	return 0;
}

// setAverageData.
void CBoBatch::SetAverageData(SampledData *data, int *pData)
{
	static int	left[NumTCTap][11] = 
	{
		{26,24,21,19,17,15,12,10,7,5,3},//#1
		{27,25,22,20,18,16,13,11,8,6,4},//#2
		{1,26,24,21,19,17,15,12,10,7,5},//#3
		{2,27,25,22,20,18,16,13,11,8,6},//#4
		{3,1,26,24,21,19,17,15,12,10,7},//#5
		{4,2,27,25,22,20,18,16,13,11,8},//#6
		{5,3,1,26,24,21,19,17,15,12,10},//#7
		{6,4,2,27,25,22,20,18,16,13,11},//#8
		{28,23,14,-1,-1,-1,-1,-1,-1,-1,-1},//#9
		{7,5,3,1,26,24,21,19,17,15,12},//#10
		{8,6,4,2,27,25,22,20,18,16,13},//#11
		{10,7,5,3,1,26,24,21,19,17,15},//#12
		{11,8,6,4,2,27,25,22,20,18,16},//#13
		{9,28,23,-1,-1,-1,-1,-1,-1,-1,-1},//#14
		{12,10,7,5,3,1,26,24,21,19,17},//#15
		{13,11,8,6,4,2,27,25,22,20,18},//#16
		{15,12,10,7,5,3,1,26,24,21,19},//#17
		{16,13,11,8,6,4,2,27,25,22,20},//#18
		{19,15,12,10,7,5,3,1,26,24,21},//#19
		{20,16,13,11,8,6,4,2,27,25,22},//#20
		{21,19,15,12,10,7,5,3,1,26,24},//#21
		{22,20,16,13,11,8,6,4,2,27,25},//#22
		{14,9,28,-1,-1,-1,-1,-1,-1,-1,-1},//#23
		{24,21,19,15,12,10,7,5,3,1,26},//#24
		{25,22,20,16,13,11,8,6,4,2,27},//#25
		{26,24,21,19,15,12,10,7,5,3,1},//#26
		{27,25,22,20,16,13,11,8,6,4,2},//#27
		{23,14,9,-1,-1,-1,-1,-1,-1,-1,-1}//#28
	};
	static int	right[28][11] = 
	{
		{3,5,7,10,12,15,17,19,21,24,26},//#1
		{4,6,8,11,13,16,18,20,22,25,27},//#2
		{5,7,10,12,15,17,19,21,24,26,1},//#3
		{6,8,11,13,16,18,20,22,25,27,2},//#4
		{7,10,12,15,17,19,21,24,26,1,3},//#5
		{8,11,13,16,18,20,22,25,27,2,4},//#6
		{10,12,15,17,19,21,24,26,1,3,5},//#7
		{11,13,16,18,20,22,25,27,2,4,6},//#8
		{14,23,28,-1,-1,-1,-1,-1,-1,-1,-1},//#9
		{12,15,17,19,21,24,26,1,3,5,7},//#10
		{13,16,18,20,22,25,27,2,4,6,8},//#11
		{15,17,19,21,24,26,1,3,5,7,10},//#12
		{16,18,20,22,25,27,2,4,6,8,11},//#13
		{23,28,9,-1,-1,-1,-1,-1,-1,-1,-1},//#14
		{17,19,21,24,26,1,3,5,7,10,12},//#15
		{18,20,22,25,27,2,4,6,8,11,13},//#16
		{19,21,24,26,1,3,5,7,10,12,15},//#17
		{20,22,25,27,2,4,6,8,11,13,16},//#18
		{21,24,26,1,3,5,7,10,12,15,17},//#19
		{22,25,27,2,4,6,8,11,13,16,18},//#20
		{24,26,1,3,5,7,10,12,15,17,19},//#21
		{25,27,2,4,6,8,11,13,16,18,20},//#22
		{28,9,14,-1,-1,-1,-1,-1,-1,-1,-1},//#23
		{26,1,3,5,7,10,12,15,17,19,21},//#24
		{27,2,4,6,8,11,13,16,18,20,22},//#25
		{1,3,5,7,10,12,15,17,19,21,24},//#26
		{2,4,6,8,11,13,16,18,20,22,25},//#27
		{9,14,23,-1,-1,-1,-1,-1,-1,-1,-1}//#28
	};
	int		i, j, iL, iR;

	//配列変数の初期化
	memset( pData, 0, sizeof( pData ) );

	// 基本ロジック
	// 熱電対データが無い場合（又は切断）、左右の有効な熱電対温度から
	// 計算して求める

	// 全熱電対温度について熱電対順位並べ替え＆無いとこを計算
	for( i = 0; i < NumTCTap; i++ )
	{
		// データが無いなら計算する
		if( cnf.inputPoint[i].adCh == -1 )
		{
			pData[i] = 0;
			iL = -1;
			iR = -1;

			// 左方向の有効な熱電対Indexを探す
			for( j = 0; j < 11; j++ )
			{
				if( left[i][j] != -1 )
				{
					if( cnf.inputPoint[left[i][j]-1].adCh != -1 )
					{
						iL = j;
						break;
					}
				}
			}

			// 右方向の有効な熱電対Indexを探す
			for( j = 0; j < 11; j++ )
			{
				if( right[i][j] != -1 )
				{
					if( cnf.inputPoint[right[i][j]-1].adCh != -1 )
					{
						iR = j;
						break;
					}
				}
			}

			// 左右に有効な熱電対が見つかったら
			if( iL != -1 && iR != -1 )
			{
				// 左右のIndexが同じなら左側のデータをセット
				if( left[i][iL] == right[i][iR] )
				{
					pData[i] = data->val[cnf.inputPoint[left[i][j]-1].adCh];
				}
				// 違うなら割合計算
				else
				{
					int	div, oft;
					div = data->val[cnf.inputPoint[right[i][iR]-1].adCh] - 
						  data->val[cnf.inputPoint[left[i][iL]-1].adCh];
					oft = data->val[cnf.inputPoint[left[i][iL]-1].adCh];
					pData[i] = div * (iL+1) / (iL+iR+2) + oft;
				}
			}
		}
		// データがあるならそのままセット
		else
		{
			pData[i] = data->val[cnf.inputPoint[i].adCh];
		}
	}

	// 温度以外はそのままセット
	for( i = NumTCTap; i < NumADChannel; i++ )
		pData[i] = data->val[i];
}

void CBoBatch::LimitTempData(int *pDat)
{
	int			i;

	for(i = 0; i < NumTCTap; i++)
	{
		if( (double)pDat[i] > MAXTEMP )
			pDat[i] = MAXTEMP;
		if( (double)pDat[i] < MINTEMP )
			pDat[i] = MINTEMP;
	}
}

// recalculation of grouping.
void CBoBatch::RecalcGroup(void)
{
	// create group
	m_gr.CreateGroup(cnf);
	m_grD.CreateGroup(cnfD);
}

// clear B.O. sampled data buffer.
void CBoBatch::ClearSampledData(void)
{
	m_nNumSampledData  = 0;
	m_nSampledDataRIdx = 0;
	m_nSampledDataWIdx = 0;

	// for dep.
	m_nNumSampledDataD  = 0;
	m_nSampledDataRIdxD = 0;
	m_nSampledDataWIdxD = 0;
}

// clear status table.
void CBoBatch::ClearStatusTbl(void)
{
	memset(m_blStatusTbl, 0, sizeof(m_blStatusTbl));
	memset(m_blStatusTblD, 0, sizeof(m_blStatusTbl));	// for dep.
}

// clear result table.
void CBoBatch::ClearResultTbl(void)
{
	memset(m_pResultTbl, 0, sizeof(ResultData) * m_nNumMaxResult);
	memset(m_pResultTblD, 0, sizeof(ResultData) * m_nNumMaxResult);	// for dep.
}

// clear B.O. occur result table buffer.
void CBoBatch::ClearOccureResult(void)
{
	memset(&m_pResultTbl[m_nBoAlarmTime], 0, sizeof(ResultData));
	memset(&m_pResultTblD[m_nBoAlarmTimeD], 0, sizeof(ResultData));	// for dep.
}

// BO結果出力有効/無効
void CBoBatch::SetCsvOut(BOOL bSet)	//@
{
	m_bEnaCsvOut = bSet;
}

// BO結果出力用ログファイル名
void CBoBatch::SetLogName(LPCTSTR pName)	//@
{
	m_strLogName = pName;
}

