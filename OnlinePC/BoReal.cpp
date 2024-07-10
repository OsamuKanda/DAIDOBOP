#include "BoReal.h"
#include	"../include/bomodule.h"
#include	"../include/bosample.h"

#ifdef _DEBUG
#include	<stdio.h>	//@ 追加機能
//@ 追加機能
double	outputP[NumCombination], outputC[NumCombination];
#endif//DEBUG

///////////////////////////////////////////////////////////////////////////////
// definition


///////////////////////////////////////////////////////////////////////////////
// realtime class function

//=============================================================================
// Name		: CBoReal
// Comment	: initialize member variable.
// JP		: メンバ変数の初期化など
//=============================================================================
CBoReal::CBoReal()
{
	m_isStrobeBO		= FALSE;
	m_nNumMaxResult		= 0;
	m_nNumSampledData	= 0;
	m_nSampledDataRIdx	= 0;
	m_nSampledDataWIdx	= 0;
	m_nNumSampledDataD	= 0;
	m_nSampledDataRIdxD	= 0;
	m_nSampledDataWIdxD	= 0;
//	m_nStartCounter		= 0;
	m_nBoAlarmTime		= 0;
	m_nNumResultCount	= 0;
	m_nBoAlarmTimeD		= 0;
	m_nNumResultCountD	= 0;
	m_nInputSize		= 0;
	m_nInputSizeD		= 0;
//	m_nNumRegister		= 0;
	m_unOutBOFetched	= 0;
	m_pResultTbl		= NULL;
	m_pResultTblD		= NULL;
	m_pSampleBuffer		= NULL;
	m_pSampleBufferD	= NULL;
	m_pSampledData		= NULL;
	m_pSampledDataD		= NULL;
	m_bCalcStart		= FALSE;
	m_bCalcStartD		= FALSE;
	m_bTdChangeBef		= FALSE;
	m_fLenBef			= 0.0f;
	memset(m_isStatusTbl, 0, sizeof(m_isStatusTbl));
	memset(&m_stCondInfo,	0, sizeof(m_stCondInfo));
	memset(&m_stBsInfo,		0, sizeof(m_stBsInfo));
	memset(&m_gp,			0, sizeof(m_gp));
	memset(&m_gpD,			0, sizeof(m_gpD));
	memset( m_pPattNN,		0, sizeof(m_pPattNN) );
	memset( m_pCorrNN,		0, sizeof(m_pCorrNN) );
	memset( m_pPattNND,		0, sizeof(m_pPattNND) );
	memset( m_pCorrNND,		0, sizeof(m_pCorrNND) );
	memset( m_dPattInputVec,0, sizeof(m_dPattInputVec) );
	memset( m_dCorrInputVec,0, sizeof(m_dCorrInputVec) );

#ifdef _DEBUG
	//@ 追加機能
	m_fpCsvOut = NULL;
	m_fpCsvOutD = NULL;
#endif//DEBUG
}

//=============================================================================
// Name		: ~CBoReal
// Comment	: uninitialize member variable.
// JP		: メンバ変数の破棄など
//=============================================================================
CBoReal::~CBoReal()
{
}

//=============================================================================
// Name		: Initialize
// Argument	: none.
// Return	: none.
// Comment	: initialize boreal class.
//=============================================================================
int CBoReal::Initialize(CConfig &cnf, CConfig &cnfD, CLogging *pLogging)
{
	m_pCnf = &cnf;
	m_pCnfD = &cnfD;
	m_pLogging = pLogging;
	return 0;
}

//=============================================================================
// Name		: ResetBOP
// Argument	: none.
// Return	: none.
// Comment	: reset B.O. prediction Information.
//=============================================================================
void CBoReal::ResetBOP(int sel)
{
	// setup bo system info.
	SetupBosysInfo(sel);

	// load configuration
	m_pCnf->LoadConfiguration(m_stBsInfo.szConfName);
	m_pCnfD->LoadConfiguration(m_stBsInfo.szConfNameD);

	// clear B.O. batch environment.
	ClearBOExeInfo();

	// read the weights of B.O. neural networks(pattern & correlation).
	m_pCnf->ReadBONeuralNet(m_stBsInfo.szConfName, PatNNWeightFile, m_pPattNN);
	m_pCnf->ReadBONeuralNet(m_stBsInfo.szConfName, CorrNNWeightFile, m_pCorrNN);
	m_pCnfD->ReadBONeuralNet(m_stBsInfo.szConfNameD, PatNNWeightFile, m_pPattNND);
	m_pCnfD->ReadBONeuralNet(m_stBsInfo.szConfNameD, CorrNNWeightFile, m_pCorrNND);

	// reset BOP info.
	ResetBopInfo();

	// reset BOP env.
	ResetBopEnv();

	// set bo line color.
//	if( m_stBsInfo.numCombiBA == 1 && m_stBsInfo.numCombiBAC == 1 )
//		m_clBoLineColor = RGB(128, 128, 0);
//	else
//		m_clBoLineColor = RGB(205, 0, 0);
}

//=============================================================================
// Name		: StartBOP
// Argument	: none.
// Return	: none.
// Comment	: enable the operation of B.O. prediction.
// JP		: ＢＯ予知開始
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::StartBOP(void)
{
	// set the execution status of B.O. prediction.
	m_isBOEnable	= TRUE;
}

//=============================================================================
// Name		: StopBOP
// Argument	: none.
// Return	: none.
// Comment	: disable the operation of B.O. prediction.
// JP		: ＢＯ予知停止
//			: 条件：BO監視中
//=============================================================================
void CBoReal::StopBOP(void)
{
	// clear the execution status of B.O. prediction.
	m_isBOEnable	= FALSE;
}

//=============================================================================
// Name		: SetupBosysInfo
// Argument	: none.
// Return	: none.
// Comment	: reset B.O. prediction Information.
//=============================================================================
int CBoReal::SetupBosysInfo(int sel)
{
	SharedMemory	*pShMem = GetSharedMemory();

	// read Sampling Period from configration file.
	if(FALSE == m_pCnf->ReadSamplingPeriod(sel, &m_nSamplingPeriod))
	{
		return ERR_OPEN_SMPL;
	}

	// read bo execute condition from configration file.
	if(FALSE == m_pCnf->ReadBosysCondition(&m_stCondInfo))
	{
		return ERR_OPEN_COND;
	}

	// initialize the Bosys Information.
	memset(&m_stBsInfo, 0, sizeof(m_stBsInfo));

	// Bosys information is read.
	if(FALSE == m_pCnf->ReadBosysInfo(sel, &m_stBsInfo))
	{
		return ERR_OPEN_BOSYS;
	}

	return NORMAL;
}

//=============================================================================
// Name		: SetupBopEnv
// Argument	: none.
// Return	: none.
// Comment	: setup B.O. prediction environment.
// JP		: ＢＯ予知環境の作成
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ResetBopInfo(void)
{
	// It computes the number of the elements of calculation result table.
	m_nNumMaxResult		= MaxWatchCount;

	// It releases if a calculation result table is being secured.
	if(m_pResultTbl != NULL) 
	{
		free(m_pResultTbl);
		m_pResultTbl = NULL;
	}

	// It secures a calculation result table.
	if( m_nNumMaxResult > 0 )
		m_pResultTbl = (ResultData *)malloc(sizeof(ResultData) * m_nNumMaxResult);
	else
		m_pResultTbl = (ResultData *)malloc(sizeof(ResultData) * 1);
	memset(m_pResultTbl, 0, sizeof(ResultData) * m_nNumMaxResult);

	// It initializes both a calculation result number and calculation result table.
	m_nNumResultCount = 0;
	m_nBoAlarmTime = 0;

	///////////////////////////////////////////////////////////////////////////
	// for dep.

	// It releases if a calculation result table is being secured.
	if(m_pResultTblD != NULL) 
	{
		free(m_pResultTblD);
		m_pResultTblD = NULL;
	}

	// It secures a calculation result table.
	if( m_nNumMaxResult > 0 )
		m_pResultTblD = (ResultData *)malloc(sizeof(ResultData) * m_nNumMaxResult);
	else
		m_pResultTblD = (ResultData *)malloc(sizeof(ResultData) * 1);
	memset(m_pResultTblD, 0, sizeof(ResultData) * m_nNumMaxResult);

	// It initializes both a calculation result number and calculation result table.
	m_nNumResultCountD = 0;
	m_nBoAlarmTimeD = 0;

	// It acquires threshold and the BO occurrence number of times.
	m_nNumCombinationsBA = m_stBsInfo.numCombiBA;
	m_nNumCombinationsBAC = m_stBsInfo.numCombiBAC;
}

//=============================================================================
// Name		: ResetBopEnv
// Argument	: none.
// Return	: none.
// Comment	: reset B.O. prediction environment.
// JP		: ＢＯ予知環境のリセット
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ResetBopEnv(void)
{
	int     i, numIn, maxIn;

	// if the sampled data buffer is present,
	if (m_pSampledData) {
		// free it.
		unlockDataBuf(m_pSampleBuffer);
		freeDataBuf(m_pSampleBuffer);
		m_pSampledData     = NULL;
	}
	// get the maximum number of inputs of pattern neural networks.
	for (i = 0, maxIn = 16; i < NumNeuralNet; i++) {
		if (m_pPattNN[i]) {
			if ((numIn = m_pPattNN[i]->wMatrix[0]->numIn) > maxIn)
				maxIn   = numIn;
		}
	}
	// store the maximum input size.
	m_nInputSize		= maxIn - 1;

	// allocate the sample data buffer.
	m_pSampleBuffer	= allocDataBuf((long)sizeof(SampledData) * m_nInputSize,
									GMEM_FIXED | GMEM_ZEROINIT);
	// map the area onto main memory.
	lockDataBuf(m_pSampleBuffer);

	// store the pointer of the first element of the sampled data.
	m_pSampledData	= PtrInMTab(SampledData, m_pSampleBuffer, 0);

	// set up the ring buffer access pointers.
	m_nNumSampledData  = 0;
	m_nSampledDataRIdx = 0;
	m_nSampledDataWIdx = 0;

	///////////////////////////////////////////////////////////////////////////
	// for dep.

	// if the sampled data buffer is present,
	if (m_pSampledDataD) {
		// free it.
		unlockDataBuf(m_pSampleBufferD);
		freeDataBuf(m_pSampleBufferD);
		m_pSampledDataD     = NULL;
	}
	// get the maximum number of inputs of pattern neural networks. for dep.
	for (i = 0, maxIn = 16; i < NumNeuralNet; i++) {
		if (m_pPattNND[i]) {
			if ((numIn = m_pPattNND[i]->wMatrix[0]->numIn) > maxIn)
				maxIn   = numIn;
		}
	}
	// store the maximum input size.
	m_nInputSizeD		= maxIn - 1;

	// allocate the sample data buffer.
	m_pSampleBufferD	= allocDataBuf((long)sizeof(SampledData) * m_nInputSizeD,
									GMEM_FIXED | GMEM_ZEROINIT);
	// map the area onto main memory.
	lockDataBuf(m_pSampleBufferD);

	// store the pointer of the first element of the sampled data.
	m_pSampledDataD	= PtrInMTab(SampledData, m_pSampleBufferD, 0);

	// set up the ring buffer access pointers.
	m_nNumSampledDataD  = 0;
	m_nSampledDataRIdxD = 0;
	m_nSampledDataWIdxD = 0;

	// BO検出開始OFF
	m_bCalcStart = FALSE;
}

//=============================================================================
// Name		: DiscardBopEnv
// Argument	: none.
// Return	: none.
// Comment	: discard B.O. prediction environment.
// JP		: ＢＯ予知環境の破棄
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::DiscardBopEnv(void)
{
	// clear BOP execute info.
	ClearBOExeInfo();
	
	// if the input ring buffer for the neural networks, free it.
	if(m_pSampledData) 
	{
		unlockDataBuf(m_pSampleBuffer);
		freeDataBuf(m_pSampleBuffer);
	}
	// clear the field.
	m_pSampleBuffer		= NULL;
	m_pSampledData		= NULL;

	// It releases if a calculation result table is being secured.
	if(m_pResultTbl != NULL) 
	{
		free(m_pResultTbl);
		m_pResultTbl = NULL;
	}
	// clear the field.
	m_pResultTbl = NULL;

	///////////////////////////////////////////////////////////////////////////
	// for dep.

	// if the input ring buffer for the neural networks, free it.
	if(m_pSampledDataD) 
	{
		unlockDataBuf(m_pSampleBufferD);
		freeDataBuf(m_pSampleBufferD);
	}
	// clear the field.
	m_pSampleBufferD		= NULL;
	m_pSampledDataD			= NULL;

	// It releases if a calculation result table is being secured.
	if(m_pResultTblD != NULL) 
	{
		free(m_pResultTblD);
		m_pResultTblD = NULL;
	}
	// clear the field.
	m_pResultTblD = NULL;

}


/////////////////////////////////////////////////////////////////////////////////////////
// real function definition

//=============================================================================
// Name		: ClearSampledData(for real)
// Argument	: none.
// Return	: none.
// Comment	: clear B.O. sampled data buffer.
// JP		: 入力バッファのクリア
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ClearSampledData(void)
{
	// set up the ring buffer access pointers.
	m_nNumSampledData  = 0;
	m_nSampledDataRIdx = 0;
	m_nSampledDataWIdx = 0;

	// set up the ring buffer access pointers. for dep.
	m_nNumSampledDataD  = 0;
	m_nSampledDataRIdxD = 0;
	m_nSampledDataWIdxD = 0;
}

//=============================================================================
// Name		: ClearStatusTbl(for real)
// Argument	: none.
// Return	: none.
// Comment	: clear B.O. status table buffer.
// JP		: ステータステーブルのクリア
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ClearStatusTbl(void)
{
	memset(m_isStatusTbl, 0, sizeof(m_isStatusTbl));
	memset(m_isStatusTblD, 0, sizeof(m_isStatusTblD));
}

//=============================================================================
// Name		: ClearResultTbl(for real)
// Argument	: none.
// Return	: none.
// Comment	: clear B.O. result table buffer.
// JP		: 結果テーブルのクリア
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ClearResultTbl(void)
{
	if(!m_isBOEnable)	return;

	memset(m_pResultTbl, 0, sizeof(ResultData) * m_nNumMaxResult);
	memset(m_pResultTblD, 0, sizeof(ResultData) * m_nNumMaxResult);
}

//=============================================================================
// Name		: ClearOccureResult(for real)
// Argument	: none.
// Return	: none.
// Comment	: clear B.O. occur result table buffer.
// JP		: 発生中の結果テーブルのクリア
//			: 条件：BO監視中以外
//=============================================================================
void CBoReal::ClearOccureResult(void)
{
	if(!m_isBOEnable)	return;

	memset(&m_pResultTbl[m_nBoAlarmTime], 0, sizeof(ResultData));
	memset(&m_pResultTblD[m_nBoAlarmTimeD], 0, sizeof(ResultData));
}

//=============================================================================
// Name		: PredictBO(for real)
// Argument	: pData ... sample data address.
// Return	: none.
// Comment	: execute B.O. prediction.
// JP		: ＢＯ予知
//=============================================================================
int CBoReal::PredictBO(SampledData *pData, BOOL bSimMode)
{
	ResultData		result;
	int				i,k,j,n,boCount;
	TCCombination	*cp;
	int				netID, numIn;
	NeuralNet		*patNet, *corrNet;
	double			output;
	int				status;
	int				nCombs, nResCount;
	int				ch;
	float			fVc, fLen;

	// if the operation of B.O. prediction is disabled, return FALSE.
	if(!m_isBOEnable)	return FALSE;

	// get vc
	ch = m_pCnf->inputPoint[VcID].adCh;
	if( ch != -1 )
		fVc = (float)(pData->val[ch] * PhisicalVcUnit);
	else
		fVc = 99.9f;

	// get length
	ch = m_pCnf->inputPoint[LengthID].adCh;
	if( ch != -1 )
		fLen = (float)(pData->val[ch] * PhisicalLengthUnit);
	else
	{
		fLen = 999999.9f;
		//m_fLenBef = 0.0;
	}

	//// if T/D change is off down,
	//if( m_bTdChangeBef && !bTdChange )
	//{
	//	// save current casting length for offset
	//	m_fLenBef = fLen;
	//}
	//// subtracts casting length offset
	//fLen -= m_fLenBef;

	// append the sampled data.
	m_pSampledData[m_nSampledDataWIdx++]  = *pData;
	if (m_nSampledDataWIdx == m_nInputSize)	m_nSampledDataWIdx = 0;
	if (++m_nNumSampledData > m_nInputSize)	m_nNumSampledData  = m_nInputSize;

	//  start Wait time check.
	if( fLen > m_stCondInfo.fCondLen && fVc > m_stCondInfo.fCondVc )
		m_bCalcStart = TRUE;
//	else if( fLen <= m_stCondInfo.fCondLen )鋳込長がChargeChangeで0リセットされる為OFF
//		m_bCalcStart = FALSE;

	// for each thermo couple combination,
	status  = 0;
	memset(m_isStatusTbl, 0, sizeof(m_isStatusTbl));
	for (i = 0, cp = m_pCnf->tcCombination; i < m_pCnf->numCombination; i++, cp++) 
	{
		// clear the output.
		output	= 0.0;
#ifdef _DEBUG
		//@ 追加機能
		outputP[i] = 0.0;
		outputC[i] = 0.0;
#endif//DEBUG

		// if the neural networks are configured
		//	and if the number of sampled data in the buffer is enough to
		//		execute pattern neural network
		//	and if successed preparation of the input vectors
		//		for the neural networks,
		netID	= cp->netID;
//		if (netID < 0 && netID >= NumNeuralNet)			goto next;
		if (netID < 0 || netID >= NumNeuralNet)			goto next;
		patNet	= m_pPattNN[netID];
		corrNet	= m_pCorrNN[netID];
		if (!patNet || !corrNet)						goto next;
		numIn	= patNet->wMatrix[0]->numIn;
		if (m_nNumSampledData < numIn - 1)					goto next;
		if (prepareBOInputVector(m_pSampledData, m_nInputSize,
					(m_nSampledDataRIdx - (numIn - 1) + 1 + m_nInputSize) % m_nInputSize,
					numIn - 1, &m_pLogging->m_stFileHead, cp,
					m_patternInputVector, m_corrInputVector) == ERROR)
			goto next;

//		if( (m_bCalcStart && !bTdChange) || bSimMode )
		if( m_bCalcStart || bSimMode )
		{
			// execute the B.O. prediction neural networks
			//	and if the result value exceed the threshold,
#ifndef _DEBUG
			doBONeuralNet(patNet, corrNet, m_patternInputVector, m_corrInputVector, NULL, &output);
#else
			doBONeuralNet(patNet, corrNet, m_patternInputVector, m_corrInputVector, &outputP[i], &outputC[i]);	//@
			output = outputC[i];	//@
#endif
		}

next:
		// store the neural network output.
		result.val[i]	= (float)output;

// ------------------------------------------------------------------------
		// It sets the latest calculation result to the calculation result table.
		CopyMemory(&m_pResultTbl[m_nBoAlarmTime],&result,sizeof(result));
		// While not being the number of the greatest calculation results, 
		// it counts the number of the calculation results.
		if(m_nNumResultCount < m_nNumMaxResult)	m_nNumResultCount++;

		// It executes only the number of the BO occurrence recognition registration.
		if(	//m_gp.IsGroup(cp->upper, cp->lower) &&
			m_pCnf->inputPoint[cp->upper].adCh != -1 &&
			m_pCnf->inputPoint[cp->lower].adCh != -1 )
		{

//			for(k = 0;k < m_nNumRegister;k++)
			for(k = 0;k < m_pCnf->neuralnetConf[netID].numRegister;k++)
			{
				boCount = 0;
				j = m_nBoAlarmTime;
				// It executes only the number of the calculation results.
//				for(n = 0;n < m_nNumResultCount;n++)
				nResCount = (m_pCnf->neuralnetConf[netID].boWatchTime / m_nSamplingPeriod);
				if( m_nNumResultCount < nResCount )
					nResCount = m_nNumResultCount;
				for(n = 0;n < nResCount;n++)
				{
					// It does BO occurrence recognition.
//					if(m_pResultTbl[j--].val[i] >= boAlarm[k])
					if(m_pResultTbl[j--].val[i] >= m_pCnf->neuralnetConf[netID].boAlarm[k])  
					{
//						if(++boCount == boTimes[k])
						if(++boCount == m_pCnf->neuralnetConf[netID].boTimes[k])  
						{
							status = 1;
							m_isStatusTbl[i] = TRUE;
							break;
						}
					}

					if(j < 0) j =  m_nNumMaxResult - 1;
				}
			}
		}
// --------------------------------------------------------------------------
	}

#ifdef _DEBUG
	int			arg;
	FILETIME	m_ftTime;

	//@ 追加機能
	if( bSimMode )
	{
		if( m_fpCsvOut == NULL )
		{
			m_fpCsvOut = fopen(_T("RealOut.csv"), _T("wt+"));
			if( m_fpCsvOut != NULL )
			{
				// タイトル書込み
				fprintf(m_fpCsvOut, "time,");
				for( i = 0; i < 18; i++ )
				{
					if( (ch = m_pCnf->inputPoint[i].adCh) != -1 )
					{
						arg = m_pCnf->adChannel[ch].arg;
						switch(arg)
						{
						case 16:
							fprintf(m_fpCsvOut, "Level,");
							break;
						case 17:
							fprintf(m_fpCsvOut, "Vc,");
							break;
						default:
							fprintf(m_fpCsvOut, "#%d,", arg+1);
							break;
						}
					}
				}
				// タイトル２書込み
				for( i = 0; i < m_pCnf->numCombination; i++ )
				{
					fprintf(m_fpCsvOut, "(#%d:#%d)P,(#%d:#%d)C,", 
						m_pCnf->tcCombination[i].upper+1, m_pCnf->tcCombination[i].lower+1,
						m_pCnf->tcCombination[i].upper+1, m_pCnf->tcCombination[i].lower+1);
	//				for(j = 0; j < m_nInputSize+1; j++)
	//					fprintf(m_fpCsvOut, "Y1_%d,", j+1); 
	//				fprintf(m_fpCsvOut, "Y2,"); 
				}

				fprintf(m_fpCsvOut, "\n");
			}
		}

		SYSTEMTIME		st;
		FILETIME		lft;
		FileTimeToLocalFileTime(&m_ftTime, &lft);
		FileTimeToSystemTime(&lft, &st);

		// 時刻書込み
		fprintf(m_fpCsvOut, "\'%02d:%02d:%02d:%03d,",
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		// データ書込み
		for( i = 0; i < 18; i++ )
		{
			if( (ch = m_pCnf->inputPoint[i].adCh) != -1 )
			{
				arg = m_pCnf->adChannel[ch].arg;
				switch(arg)
				{
				case 16:
					fprintf(m_fpCsvOut, "%lf,", pData->val[ch] * PhisicalLevelUnit);
					break;
				case 17:
					fprintf(m_fpCsvOut, "%lf,", pData->val[ch] * PhisicalVcUnit);
					break;
				default:
					fprintf(m_fpCsvOut, "%lf,", pData->val[ch] * PhisicalTempUnit);
					break;
				}
			}
		}
		// 出力値書込み
		for( i = 0; i < m_pCnf->numCombination; i++ )
		{
			fprintf(m_fpCsvOut, "%lf,%lf,", outputP[i], outputC[i]);
//			for( j = 0; j < m_nInputSize+1; j++ )
//			{
//				fprintf(m_fpCsvOut, "%lf,", m_dPattInputVecS[i][j]);
//			}
//			fprintf(m_fpCsvOut, "%lf,", m_dCorrInputVecS[i][1]);
		}

		fprintf(m_fpCsvOut, "\n");
	}
	else
	{
		if( m_fpCsvOut != NULL )
		{
			fclose(m_fpCsvOut);
			m_fpCsvOut = NULL;
		}
	}
#endif//DEBUG


	// increase the ring buffer index for read operation.
	if (++m_nSampledDataRIdx == m_nInputSize) m_nSampledDataRIdx = 0;

	if(++m_nBoAlarmTime >= m_nNumMaxResult)	m_nBoAlarmTime = 0;

	
	if(status)
	{
		status = 0;
		for(i = 0;i < m_gp.m_nGrMax;i++)
		{
			for(j = 0, nCombs = 0;j < m_gp.m_stGr[i].cnt;j++)
			{
				if( TRUE == m_isStatusTbl[m_gp.m_stGr[i].no[j]] )
				{
					nCombs++;
				}
			}
			// if The number of the combinations >= setting value. (common)
			if( (nCombs >= m_nNumCombinationsBA && m_isStrobeBO) && (fVc >= m_stCondInfo.fCondVc || bSimMode) )
			{
				m_unOutBOFetched  |= 0x01;
				status |= 1;
			}
		}
	}
	// auto reset.
	if( fVc < m_stCondInfo.fCondVc && !bSimMode )
	{
		ResetBOSignal();
	}

//	m_bTdChangeBef	= bTdChange;

	// return the system status.
	return  status;
}

//=============================================================================
// Name		: PredictBOD(for real)
// Argument	: pData ... sample data address.
// Return	: none.
// Comment	: execute B.O. prediction for dep.
// JP		: ＢＯ予知（デプレッション側）
//=============================================================================
int CBoReal::PredictBOD(SampledData *pData, BOOL bSimMode)
{
	ResultData		result;
	int				i,k,j,n,boCount;
	TCCombination	*cp;
	int				netID, numIn;
	NeuralNet		*patNet, *corrNet;
	double			output;
	int				status;
	int				nCombs, nResCount;
	int				ch;
	float			fVc, fLen;

	// if the operation of B.O. prediction is disabled, return FALSE.
	if(!m_isBOEnable)	return FALSE;

	// get vc
	ch = m_pCnfD->inputPoint[VcID].adCh;
	if( ch != -1 )
		fVc = (float)(pData->val[ch] * PhisicalVcUnit);
	else
		fVc = 99.9f;

	// get length
	ch = m_pCnfD->inputPoint[LengthID].adCh;
	if( ch != -1 )
		fLen = (float)(pData->val[ch] * PhisicalLengthUnit);
	else
	{
		fLen = 999999.9f;
		//m_fLenBef = 0.0;
	}

	//// if T/D change is off down,
	//if( m_bTdChangeBef && !bTdChange )
	//{
	//	// save current casting length for offset
	//	m_fLenBef = fLen;
	//}
	//// subtracts casting length offset
	//fLen -= m_fLenBef;

	// append the sampled data.
	m_pSampledDataD[m_nSampledDataWIdxD++]  = *pData;
	if (m_nSampledDataWIdxD == m_nInputSizeD)	m_nSampledDataWIdxD = 0;
	if (++m_nNumSampledDataD > m_nInputSizeD)	m_nNumSampledDataD  = m_nInputSizeD;

	//  start Wait time check.
	if( fLen > m_stCondInfo.fCondLen && fVc > m_stCondInfo.fCondVc )
		m_bCalcStartD = TRUE;
//	else if( fLen <= m_stCondInfo.fCondLen )鋳込長がChargeChangeで0リセットされる為OFF
//		m_bCalcStart = FALSE;

	// for each thermo couple combination,
	status  = 0;
	memset(m_isStatusTblD, 0, sizeof(m_isStatusTblD));
	for (i = 0, cp = m_pCnfD->tcCombination; i < m_pCnfD->numCombination; i++, cp++) 
	{
		// clear the output.
		output	= 0.0;
#ifdef _DEBUG
		//@ 追加機能
		outputP[i] = 0.0;
		outputC[i] = 0.0;
#endif//DEBUG

		// if the neural networks are configured
		//	and if the number of sampled data in the buffer is enough to
		//		execute pattern neural network
		//	and if successed preparation of the input vectors
		//		for the neural networks,
		netID	= cp->netID;
//		if (netID < 0 && netID >= NumNeuralNet)			goto next;
		if (netID < 0 || netID >= NumNeuralNet)			goto next;
		patNet	= m_pPattNND[netID];
		corrNet	= m_pCorrNND[netID];
		if (!patNet || !corrNet)						goto next;
		numIn	= patNet->wMatrix[0]->numIn;
		if (m_nNumSampledDataD < numIn - 1)					goto next;
		if (prepareBOInputVector(m_pSampledDataD, m_nInputSizeD,
					(m_nSampledDataRIdxD - (numIn - 1) + 1 + m_nInputSizeD) % m_nInputSizeD,
					numIn - 1, &m_pLogging->m_stFileHead, cp,
					m_patternInputVector, m_corrInputVector) == ERROR)
			goto next;

//		if( (m_bCalcStart && !bTdChange) || bSimMode )
		if( m_bCalcStartD || bSimMode )
		{
			// execute the B.O. prediction neural networks
			//	and if the result value exceed the threshold,
#ifndef _DEBUG
			doBONeuralNet(patNet, corrNet, m_patternInputVector, m_corrInputVector, NULL, &output);
#else
			doBONeuralNet(patNet, corrNet, m_patternInputVector, m_corrInputVector, &outputP[i], &outputC[i]);	//@
			output = outputC[i];	//@
#endif
		}

next:
		// store the neural network output.
		result.val[i]	= (float)output;

// ------------------------------------------------------------------------
		// It sets the latest calculation result to the calculation result table.
		CopyMemory(&m_pResultTblD[m_nBoAlarmTimeD],&result,sizeof(result));
		// While not being the number of the greatest calculation results, 
		// it counts the number of the calculation results.
		if(m_nNumResultCountD < m_nNumMaxResult)	m_nNumResultCountD++;

		// It executes only the number of the BO occurrence recognition registration.
		if(	//m_gp.IsGroup(cp->upper, cp->lower) &&
			m_pCnfD->inputPoint[cp->upper].adCh != -1 &&
			m_pCnfD->inputPoint[cp->lower].adCh != -1 )
		{

//			for(k = 0;k < m_nNumRegister;k++)
			for(k = 0;k < m_pCnfD->neuralnetConf[netID].numRegister;k++)
			{
				boCount = 0;
				j = m_nBoAlarmTimeD;
				// It executes only the number of the calculation results.
//				for(n = 0;n < m_nNumResultCount;n++)
				nResCount = (m_pCnfD->neuralnetConf[netID].boWatchTime / m_nSamplingPeriod);
				if( m_nNumResultCountD < nResCount )
					nResCount = m_nNumResultCountD;
				for(n = 0;n < nResCount;n++)
				{
					// It does BO occurrence recognition.
//					if(m_pResultTbl[j--].val[i] >= boAlarm[k])
					if(m_pResultTblD[j--].val[i] >= m_pCnfD->neuralnetConf[netID].boAlarm[k])  
					{
//						if(++boCount == boTimes[k])
						if(++boCount == m_pCnfD->neuralnetConf[netID].boTimes[k])  
						{
							status = 1;
							m_isStatusTblD[i] = TRUE;
							break;
						}
					}

					if(j < 0) j =  m_nNumMaxResult - 1;
				}
			}
		}
// --------------------------------------------------------------------------
	}

#ifdef _DEBUG
	int			arg;
	FILETIME	m_ftTime;

	//@ 追加機能
	if( bSimMode )
	{
		if( m_fpCsvOutD == NULL )
		{
			m_fpCsvOutD = fopen(_T("RealOutD.csv"), _T("wt+"));
			if( m_fpCsvOutD != NULL )
			{
				// タイトル書込み
				fprintf(m_fpCsvOutD, "time,");
				for( i = 0; i < 18; i++ )
				{
					if( (ch = m_pCnfD->inputPoint[i].adCh) != -1 )
					{
						arg = m_pCnfD->adChannel[ch].arg;
						switch(arg)
						{
						case 16:
							fprintf(m_fpCsvOutD, "Level,");
							break;
						case 17:
							fprintf(m_fpCsvOutD, "Vc,");
							break;
						default:
							fprintf(m_fpCsvOutD, "#%d,", arg+1);
							break;
						}
					}
				}
				// タイトル２書込み
				for( i = 0; i < m_pCnfD->numCombination; i++ )
				{
					fprintf(m_fpCsvOutD, "(#%d:#%d)P,(#%d:#%d)C,", 
						m_pCnfD->tcCombination[i].upper+1, m_pCnfD->tcCombination[i].lower+1,
						m_pCnfD->tcCombination[i].upper+1, m_pCnfD->tcCombination[i].lower+1);
	//				for(j = 0; j < m_nInputSize+1; j++)
	//					fprintf(m_fpCsvOut, "Y1_%d,", j+1); 
	//				fprintf(m_fpCsvOut, "Y2,"); 
				}

				fprintf(m_fpCsvOutD, "\n");
			}
		}

		SYSTEMTIME		st;
		FILETIME		lft;
		FileTimeToLocalFileTime(&m_ftTime, &lft);
		FileTimeToSystemTime(&lft, &st);

		// 時刻書込み
		fprintf(m_fpCsvOutD, "\'%02d:%02d:%02d:%03d,",
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		// データ書込み
		for( i = 0; i < 18; i++ )
		{
			if( (ch = m_pCnfD->inputPoint[i].adCh) != -1 )
			{
				arg = m_pCnfD->adChannel[ch].arg;
				switch(arg)
				{
				case 16:
					fprintf(m_fpCsvOutD, "%lf,", pData->val[ch] * PhisicalLevelUnit);
					break;
				case 17:
					fprintf(m_fpCsvOutD, "%lf,", pData->val[ch] * PhisicalVcUnit);
					break;
				default:
					fprintf(m_fpCsvOutD, "%lf,", pData->val[ch] * PhisicalTempUnit);
					break;
				}
			}
		}
		// 出力値書込み
		for( i = 0; i < m_pCnfD->numCombination; i++ )
		{
			fprintf(m_fpCsvOutD, "%lf,%lf,", outputP[i], outputC[i]);
//			for( j = 0; j < m_nInputSize+1; j++ )
//			{
//				fprintf(m_fpCsvOut, "%lf,", m_dPattInputVecS[i][j]);
//			}
//			fprintf(m_fpCsvOut, "%lf,", m_dCorrInputVecS[i][1]);
		}

		fprintf(m_fpCsvOutD, "\n");
	}
	else
	{
		if( m_fpCsvOutD != NULL )
		{
			fclose(m_fpCsvOutD);
			m_fpCsvOutD = NULL;
		}
	}
#endif//DEBUG


	// increase the ring buffer index for read operation.
	if (++m_nSampledDataRIdxD == m_nInputSizeD) m_nSampledDataRIdxD = 0;

	if(++m_nBoAlarmTimeD >= m_nNumMaxResult)	m_nBoAlarmTimeD = 0;

	
	if(status)
	{
		status = 0;
		for(i = 0;i < m_gpD.m_nGrMax;i++)
		{
			for(j = 0, nCombs = 0;j < m_gpD.m_stGr[i].cnt;j++)
			{
				if( TRUE == m_isStatusTblD[m_gpD.m_stGr[i].no[j]] )
				{
					nCombs++;
				}
			}
			// if The number of the combinations >= setting value. (common)
			if( (nCombs >= m_nNumCombinationsBAC && m_isStrobeBO) && (fVc >= m_stCondInfo.fCondVc || bSimMode) )
			{
				m_unOutBOFetched  |= 0x02;
				status |= 2;
			}
		}
	}
	// auto reset.
	if( fVc < m_stCondInfo.fCondVc && !bSimMode )
	{
		ResetBOSignal();
	}

//	m_bTdChangeBef	= bTdChange;

	// return the system status.
	return  status;
}

//=============================================================================
// Name		: ResetBOSignal(for real)
// Argument	: none.
// Return	: none.
// Comment	: reset a B.O. signal.
// JP		: ＢＯ警報のリセット
//=============================================================================
void CBoReal::ResetBOSignal(void)
{
	// reset the pulse width of BO Fetched signal.
	m_unOutBOFetched   = 0;
}

UINT CBoReal::GetBOSignal(void)
{
	return m_unOutBOFetched;
}

// clear B.O. prediction Information
void CBoReal::ClearBOExeInfo(void)
{
	// reset bo neural net area.
	m_pCnf->ResetBONeuralNet(m_pPattNN);
	m_pCnf->ResetBONeuralNet(m_pCorrNN);

	// reset bo neural net area.(for dep.)
	m_pCnfD->ResetBONeuralNet(m_pPattNND);
	m_pCnfD->ResetBONeuralNet(m_pCorrNND);
}

void CBoReal::SetBoStrobe(BOOL bSet)
{
	m_isStrobeBO = bSet;
}

void CBoReal::RecalcGroup(void)
{
	// create group
	m_gp.CreateGroup(*m_pCnf);
	// create group
	m_gpD.CreateGroup(*m_pCnfD);
}

//
void CBoReal::ResetOffsetLength(void)
{
	// reset casting length offset
	m_fLenBef = 0.0f;
}

BOOL CBoReal::GetBopCondition(BOOL bSimMode)
{
	if(!m_isBOEnable)
		return FALSE;

	if( bSimMode )
		return TRUE;

	return (m_bCalcStart || m_bCalcStartD);
}
