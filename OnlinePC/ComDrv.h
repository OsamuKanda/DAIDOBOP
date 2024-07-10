#pragma once
#include "../include/bodef.h"

#ifdef SMART_CASTING
#include "SharedMemSet.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// definition

typedef struct {
	int		isEnabLevelCtl;			/* [in]does detect while auto level ctl		*/
	int		isEnabSimSignal;		/* [in]is enable simulation signal or not	*/
	int		simVcOn;				/* [in]simulation status of 'Vc On'			*/
	int		simChargeChange;		/* [in]simulation status of 'Charge Change'	*/
	int		simAutoLVCtl;			/* [in]simu. status of 'Auto Level Control'	*/
	int		simCastStart;			/* [in]simu. status of 'CastStart'			*/
	int		simL1Err;				/* [in]simu. status of 'L1 Err'				*/
	int		inAlarmReset;			/* [in]input status of 'Alarm Reset'		*/
} StComInParam;

typedef struct {
	int		tstVcOn;				/* [out]test status of 'Vc On'				*/
	int		tstChargeChange;		/* [out]test status of 'Charge Change'		*/
	int		tstAutoLVCtl;			/* [out]test status of 'Auto Level Control'	*/
	int		tstCastStart;			/* [out]test status of 'CastStart'			*/
	int		tstL1Err;				/* [out]test status of 'L1 Err'				*/
	int		isVcOn;					/* [out]signal status of 'Vc On'			*/
	int		isChargeChange;			/* [out]signal status of 'Charge Change'	*/
	int		isAutoLVCtl;			/* [out]signal status of 'Auto Level Ctl'	*/
	int		isCastStart;			/* [out]signal status of 'CastStart'		*/
	int		isL1Err;				/* [out]signal status of 'L1 Err'			*/
	int		outAlarmReset;			/* [out]output status of 'Alarm Reset'		*/
	int		inPlcErr1;				/* [in]input status of 'Plc Error(1)'		*/
	int		inPlcErr2;				/* [in]input status of 'Plc Error(2)'		*/
} StComInResult;

typedef struct {
	UINT   	filterSwitch;			/* [in]A/D Bord filter switch				*/
	float	filterFacter;			/* [in]A/D Bord filter facter				*/
	float	filterFacterSn;			/* [in]A/D Bord filter facter S/N			*/
	float	filterFacterLv;			/* [in]A/D Bord filter facter Level			*/
	float	filterFacterPr;			/* [in]A/D Bord filter facter P/R  Speed	*/
//	float	filterFacterWd;			/* [in]A/D Bord filter facter Width			*/
//	float	waterFlowRate;			/* [in]Water Flow Rate						*/
//	BOOL	bTcExp[NumInput];		/* [in]TC exception info.					*/
} StSampledParam;

typedef struct {
	//int		moldSize;				/* [out]Mold Size Total(include filter)		*/
	//int		moldSizeL;				/* [out]Mold Size Left						*/
	//int		moldSizeR;				/* [out]Mold Size Right						*/
	BOOL	MoldLevelNotUp;			/* [out]MOLD LEVEL åpë±è„è∏íÜÇ≈ñ≥Ç¢			*/
	BOOL	VcSpeedNotUp;			/* [out]Vc åpë±è„è∏íÜÇ≈ñ≥Ç¢					*/
	BOOL	SNLevelUp;				/* [out]SN LEVEL åpë±è„è∏íÜ					*/
} StSampledResult;

typedef struct {
	int		isEnabSimSignal;		/* [in]is enable simulation signal or not	*/
	int		simBocOut;				/* [out]simulation status of 'Boc Out'		*/
	int		simBoOut;				/* [out]simulation status of 'Bo Out'		*/
	int		pulseOutBOFetched;
} StComOutParam;

typedef struct {
	int		OutBOFetched;
} StComOutResult;

typedef struct {
	int		nInVc;
	int		nInChChg;
	int		nInAtLvC;
	int		nInAlmRst;
	int		nInTdChg;
} StInSignal;

// CComDrv class
class CComDrv
{
public:
	CComDrv(); 
	~CComDrv();

// member data
public:
	StSampledData	m_stData;
	StSampledData	m_stOldData;
	BOOL			m_isFirst;
	int				m_nDevCntR;
	int				m_nDevCntW;
	int				m_nVcOffCnt;

	// ç|éÌèÓïÒFIXä÷åW
	BOOL			m_bPreVcOn;
	BOOL			m_bPreChargeChg;
	BOOL			m_bPreCastStart;
	BOOL			m_bSteelInfoFix;
	int				m_nFixPtnNo;
	BYTE			m_cFixGradeCode[21];		// Grade Code
	BYTE			m_cFixHeatNo[9];			// Heat Number
	BYTE			m_cFixStandardNo[7];		// Standard Number
	// A/D

	short			m_nID;

#ifdef SMART_CASTING
	SharedMemSet	m_shm;
#endif

private:

// member function
public:
	// initialize Siemens PLC Communication
	int Initialize(HWND hWnd);
	int ReadInputData(StComInParam *pPrm, StComInResult *pRst);
	int WriteOutputData(StComOutParam *pPrm, StComOutResult *pRst);
	// uninitialize Siemens PLC Communication
	int Uninitialize(void);
	void GetSampledData(StSampledParam *pPrm, StSampledResult *pRst, StSampledData *pData);

private:
	void SetSampleData(VARIANT *pVals, StInSignal *pSig);
	#ifdef USE_SAMPLE_DUMMY
	void SetSampleDummy(void);
    #endif
};


