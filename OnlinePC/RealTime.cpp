#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"RealTime.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf = theApp.m_cnfBoexe;
static CConfig		&cnfD = theApp.m_cnfBoexeD;
static CRealTime	*pR = NULL;


///////////////////////////////////////////////////////////////////////////////
// realtime class function

//=============================================================================
// Name		: CRealTime
// Comment	: initialize member variable.
// JP		: メンバ変数の初期化など
//=============================================================================
CRealTime::CRealTime()
{
	pR				= this;
	m_pShMem		= GetSharedMemory();
	m_pLog			= GetLoggingClass();
}

//=============================================================================
// Name		: ~CRealTime
// Comment	: uninitialize member variable.
// JP		: メンバ変数の破棄など
//=============================================================================
CRealTime::~CRealTime()
{
}

//=============================================================================
// Name		: TimeCallBack
// Comment	: interrupt service routine for real time B.O. prediction.
// JP		: リアルタイム処理（メイン）
//=============================================================================
void CALLBACK CRealTime::TimeCallBack(UINT timerID, UINT msg, DWORD usrParam, DWORD dw1, DWORD dw2)
{
	static int  isBusy = FALSE;
	static int  nRunCnt = 0;

	// if another contexts are busy, do nothing.
	if(isBusy)	return;

	// 起動待ちカウンタ
	if( nRunCnt < 6 )
	{	
		nRunCnt++;
		return;
	}

	// if time call back routine is disabled, do nothing.
	if(pR == NULL || !pR->IsBOEnable())	return;

	// set this contexts be in execution B.O. prediction.
	isBusy = TRUE;

	//MessageBeep(MB_ICONEXCLAMATION);

	// realtime routine
	pR->InputBoData();
	pR->RealtimeBoPrediction();
	pR->OutputBoData();

	// set this context be free.
	isBusy = FALSE;
}

// initialize realtime.
int	CRealTime::Initialize(int sel)
{
	CWnd	*wnd = AfxGetMainWnd();
	int		ret;

	if( 0 != (ret = m_com.Initialize(wnd->GetSafeHwnd())))
		return ret;

	m_real.Initialize(cnf, cnfD, m_pLog);
	m_real.ResetBOP(sel);
	m_real.StartBOP();
	return 0;
}

// reset BOP execute.
int	CRealTime::ResetBopExe(int sel)
{
	m_real.ResetBOP(sel);
	return 0;
}

// initialize realtime.
int	CRealTime::Uninitialize(void)
{
	m_real.StopBOP();
	m_real.DiscardBopEnv();
	m_com.Uninitialize();
	return 0;
}

// input BO data.
int	CRealTime::InputBoData(void)
{
	StComInParam	prm;
	StComInResult	rst;

	// input param
	prm.isEnabLevelCtl		= m_pShMem->isEnabLevelCtl;		
	prm.isEnabSimSignal		= m_pShMem->isEnabSimSignal;		
	prm.simVcOn				= m_pShMem->simVcOn;		
	prm.simChargeChange		= m_pShMem->simChargeChange;		
	prm.simCastStart		= m_pShMem->simCastStart;		
	prm.simL1Err			= m_pShMem->simL1Err;		
	prm.simAutoLVCtl		= m_pShMem->simAutoLVCtl;		
	prm.inAlarmReset		= m_pShMem->inAlarmReset;		

	// read input data.
	m_com.ReadInputData(&prm, &rst);

	// input result
	m_pShMem->tstVcOn			= rst.tstVcOn;			
	m_pShMem->tstChargeChange	= rst.tstChargeChange;	
	m_pShMem->tstCastStart		= rst.tstCastStart;	
	m_pShMem->tstL1Err			= rst.tstL1Err;	
	m_pShMem->tstAutoLVCtl		= rst.tstAutoLVCtl;		
	m_pShMem->isVcOn			= rst.isVcOn;				
	m_pShMem->isChargeChange	= rst.isChargeChange;		
	m_pShMem->isCastStart		= rst.isCastStart;		
	m_pShMem->isL1Err			= rst.isL1Err;		
	m_pShMem->isAutoLVCtl		= rst.isAutoLVCtl;		
	m_pShMem->outAlarmReset		= rst.outAlarmReset;		
	m_pShMem->inPlcErr1			= rst.inPlcErr1;		
	m_pShMem->inPlcErr2			= rst.inPlcErr2;		

	// set strobe state.
	m_real.SetBoStrobe(m_pShMem->isStrobeBO);

	// alarm reset
	if( m_pShMem->outAlarmReset == 1 )
	{
		m_real.ResetBOSignal();
		m_pShMem->outAlarmReset = 0;
	}

	return 0;
}

// run realtime bo prediction.
int	CRealTime::RealtimeBoPrediction(void)
{
	static StSampledParam	prm;
	static StSampledResult	rst;
	static StSampledData	data;
	static int			preIsVcOn2;
	static int			preIsChargeChange2;
	SYSTEMTIME			st;
	BOOL				isNewLog = FALSE;
	int					isAllRes = 0;

	// test the edge of 'InVcOn'. if raising edge,
	if(m_pShMem->isVcOn && !preIsVcOn2) 
	{
		// clear B.O. sampled data buffer.
		m_real.ClearSampledData();
		// assert request of a new logging data file.
		isNewLog = TRUE;
		// clear the BO occurrence status table.
		m_real.ClearResultTbl();
		// reset casting length offset
		//m_real.ResetOffsetLength();
	}

	// test the edge of 'InChargeChange'. if raising edge,
	if(m_pShMem->isVcOn && m_pShMem->isChargeChange && !preIsChargeChange2) 
	{
		// assert request of a new logging data file.
		isNewLog = TRUE;
	}

	// set param
	prm.filterSwitch	= m_pShMem->filterSwitch;
	prm.filterFacter	= m_pShMem->filterFacter;
	prm.filterFacterSn	= m_pShMem->filterFacterSn;
	prm.filterFacterPr	= m_pShMem->filterFacterPr;
	prm.filterFacterLv	= m_pShMem->filterFacterLv;
//	prm.filterFacterWd	= m_pShMem->filterFacterWd;
//	prm.waterFlowRate	= m_pShMem->waterFlowRate;
//	memcpy(prm.bTcExp, cnf.bTcExp, sizeof(prm.bTcExp));

	// get the sampled data.
	m_com.GetSampledData(&prm, &rst, &data);

	// get result value.
	//if( m_pShMem->isEnabSimSignal == 0 )
	//{
	//	m_pShMem->moldSize	= rst.moldSize;
	//}
	//else
	//{
	//	rst.moldSizeL = m_pShMem->moldSize / 2;
	//	rst.moldSizeR = m_pShMem->moldSize / 2;
	//}
//	m_pShMem->moldSizeL			= rst.moldSizeL;
//	m_pShMem->moldSizeR			= rst.moldSizeR;
//	m_pShMem->MoldLevelNotUp	= rst.MoldLevelNotUp;
//	m_pShMem->VcSpeedNotUp		= rst.VcSpeedNotUp;
//	m_pShMem->SNLevelUp			= rst.SNLevelUp;

	// recalc group.
	m_real.RecalcGroup();

	// test the B.O. prediction mode, and if enabled,
	if (m_pShMem->isVcOn && (!m_pShMem->isEnabLevelCtl || m_pShMem->isAutoLVCtl)) 
	{
		// execute B.O. prediction. if B.O. detected,
		isAllRes = m_real.PredictBO(&data.sd, m_pShMem->isEnabSimSignal);
		if ( isAllRes != 0 ) 
		{
			// set B.O. detected status.
			m_pShMem->isBOFetched = TRUE;
		}
		// execute B.O. prediction. if B.O. detected,
		isAllRes |= m_real.PredictBOD(&data.sd, m_pShMem->isEnabSimSignal);
		if ( isAllRes != 0 ) 
		{
			// set B.O. detected status.
			m_pShMem->isBOFetched = TRUE;
		}
		// set bo condition
		m_pShMem->BopCondition = m_real.GetBopCondition(m_pShMem->isEnabSimSignal);
	}
	// otherwise
	else {
		// clear the status table.
		m_real.ClearStatusTbl();
		// un-occurrence is registered.
		m_real.ClearOccureResult();
		// clear B.O. sampled data buffer.
		m_real.ClearSampledData();
		// set bo condition
		m_pShMem->BopCondition = FALSE;
	}

	// get current file time
	GetLocalTime(&st);
	SystemTimeToFileTime(&st, &data.ft);

	// set current config
	data.numCombination = cnf.numCombination;
	memcpy( data.tcCombination, cnf.tcCombination, sizeof(data.tcCombination) );
	memcpy( data.inputPoint, cnf.inputPoint, sizeof(data.inputPoint) );
	memcpy( data.adChannel, cnf.adChannel, sizeof(data.adChannel) );
//	memcpy( data.nTpPosData, cnf.nTpPosData, sizeof(data.nTpPosData) );
//	memcpy( data.bTcExp, cnf.bTcExp, sizeof(data.bTcExp) );
	data.isChgHeat = isNewLog;

	// append the sampled data at the tail of the logging data buffer.
	m_pLog = GetLoggingClass();
	if( m_pLog != NULL )
		m_pLog->AppendData(&data, m_real.m_isStatusTbl, m_real.m_isStatusTblD, isAllRes, isNewLog);

	// keep the inputs for detecting the input edges.
	preIsVcOn2         = m_pShMem->isVcOn;
	preIsChargeChange2 = m_pShMem->isChargeChange;

	return 0;
}

// output BO data.
int	CRealTime::OutputBoData(void)
{
	StComOutParam	prm;
	StComOutResult	rst;

	// input param
	prm.pulseOutBOFetched	= m_real.GetBOSignal();			
	prm.isEnabSimSignal		= m_pShMem->isEnabSimSignal;
	prm.simBocOut			= m_pShMem->simBocOut;		
	prm.simBoOut			= m_pShMem->simBoOut;	

	// read input data.
	m_com.WriteOutputData(&prm, &rst);

	// input result
	m_pShMem->outBOFetched	= rst.OutBOFetched;

	return 0;
}

// output BO data.
BOOL CRealTime::IsBOEnable(void)
{
	return m_real.m_isBOEnable;
}

// start bop.
void CRealTime::StartBOP(void)
{
	m_real.StartBOP();
}

// stop bop.
void CRealTime::StopBOP(void)
{
	m_real.StopBOP();
}

