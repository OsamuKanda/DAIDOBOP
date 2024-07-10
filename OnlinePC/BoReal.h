#pragma once
#include    "../include/bodef.h"
#include    "../include/databuf.h"
#include	"Config.h"
#include	"Logging.h"
#include	"BoGroup.h"

// CBoReal class
class CBoReal
{
public:
	enum{
		NORMAL			= 0,
		ERR_OPEN_SMPL,
		ERR_OPEN_COND,
		ERR_OPEN_BOSYS
	};

public:
	CBoReal(); 
	~CBoReal();

// member data
public:
	BOOL			m_isBOEnable;
	BOOL			m_isStrobeBO;
	BOOL			m_isStatusTbl[NumCombination];
	BOOL			m_isStatusTblD[NumCombination];
	BOOL			m_bCalcStart;
	BOOL			m_bCalcStartD;

private:
	CConfig			*m_pCnf;
	CConfig			*m_pCnfD;
	CLogging		*m_pLogging;
	int				m_nNumMaxResult;
	int				m_nNumSampledData;
	int				m_nSampledDataRIdx;
	int				m_nSampledDataWIdx;
	int				m_nNumSampledDataD;
	int				m_nSampledDataRIdxD;
	int				m_nSampledDataWIdxD;
//	int				m_nStartCounter;
	int				m_nBoAlarmTime;
	int				m_nNumResultCount;
	int				m_nBoAlarmTimeD;
	int				m_nNumResultCountD;
	int				m_nInputSize;
	int				m_nInputSizeD;
	int				m_nNumRegister;
	int				m_nNumCombinationsBA;
	int				m_nNumCombinationsBAC;
	UINT			m_unOutBOFetched;
	double			m_patternInputVector[256];	
	double			m_corrInputVector[2];		
	float			boAlarm[NumBoOccur];		// The threshold buffer.
	int				boTimes[NumBoOccur];		// The number of times buffer.
	ResultData		*m_pResultTbl;
	ResultData		*m_pResultTblD;
	MemoryTable		*m_pSampleBuffer;
	MemoryTable		*m_pSampleBufferD;
	SampledData		*m_pSampledData;
	SampledData		*m_pSampledDataD;
	CBoGroup		m_gp;
	CBoGroup		m_gpD;
	BoCondInfo		m_stCondInfo;
	BoSysInfo		m_stBsInfo;
	int				m_nSamplingPeriod;
	BOOL			m_bTdChangeBef;
	float			m_fLenBef;

	// neural network...
	NeuralNet		*m_pPattNN[NumNeuralNet];
	NeuralNet		*m_pCorrNN[NumNeuralNet];
	NeuralNet		*m_pPattNND[NumNeuralNet];
	NeuralNet		*m_pCorrNND[NumNeuralNet];
	double			m_dPattInputVec[256];
	double			m_dCorrInputVec[2];		

	// etc.
	COLORREF		m_clBoLineColor;

#ifdef _DEBUG
	//@ ’Ç‰Á‹@”\
	FILE			*m_fpCsvOut;
	FILE			*m_fpCsvOutD;
#endif

// member function
public:
	int  Initialize(CConfig &cnf, CConfig &cnfD, CLogging *pLogging);
	void ResetBOP(int sel);
	void StartBOP(void);
	void StopBOP(void);
	int  SetupBosysInfo(int sel);
	void ResetBopInfo(void);
	void ResetBopEnv(void);
	void DiscardBopEnv(void);
	void ResetOffsetLength(void);
	BOOL GetBopCondition(BOOL bSimMode);

	// real
	void ClearSampledData(void);
	void ClearStatusTbl(void);
	void ClearResultTbl(void);
	void ClearOccureResult(void);
	void ResetBOSignal(void);
	int PredictBO(SampledData *pData, BOOL bSimMode);
	int PredictBOD(SampledData *pData, BOOL bSimMode);
	void ClearBOExeInfo(void);
	UINT GetBOSignal(void);
	void SetBoStrobe(BOOL bSet);
	void RecalcGroup(void);

private:
};
