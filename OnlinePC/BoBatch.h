#pragma once
#include "afxwin.h"
#include "Config.h"
#include "BoGroup.h"
#include "../include/neuralne.h"
#include "../include/bomodule.h"


///////////////////////////////////////////////////////////////////////////////
// definition


///////////////////////////////////////////////////////////////////////////////
// CBoBatch class
class CBoBatch
{
public:
	enum{
		NORMAL			= 0,
		ERR_OPEN_SMPL,
		ERR_OPEN_COND,
		ERR_OPEN_BOSYS
	};

public:
	CBoBatch();
	virtual ~CBoBatch();

//---------------
// member data.
//---------------
public:
	// log file...
	FILE			*m_inFp;
	DataFileHeader	m_header;

	// sampling...
	SampledData		*m_pSampledData;
	SampledData		*m_pSampledDataD;
	int				m_nInputSize;
	int				m_nInputSizeD;
	int				m_nNumSampledData;			
	int				m_nNumSampledDataD;			
	int				m_nSampledDataRIdx;			
	int				m_nSampledDataRIdxD;			
	int				m_nSampledDataWIdx;		
	int				m_nSampledDataWIdxD;		

	// parameter...
	int				m_nSamplingPeriod;
	BoCondInfo		m_stCondInfo;
	BoSysInfo		m_stBsInfo;
	CBoGroup		m_gr;
	CBoGroup		m_grD;

	// bo batch result...
	ResultData		*m_pResultTbl;
	ResultData		*m_pResultTblD;
	int				m_nNumMaxResult;
	int				m_nNumResultCount;
	int				m_nNumResultCountD;
	int				m_nBoAlarmTime;
	int				m_nBoAlarmTimeD;
	BOOL			m_blStatusTbl[NumCombination];
	BOOL			m_blStatusTblD[NumCombination];
	BOOL			m_bCalcStart;
	BOOL			m_bCalcStartD;

	// neural network...
	NeuralNet		*m_pPattNN[NumNeuralNet];
	NeuralNet		*m_pCorrNN[NumNeuralNet];
	NeuralNet		*m_pPattNND[NumNeuralNet];
	NeuralNet		*m_pCorrNND[NumNeuralNet];
	double			m_dPattInputVec[256];
	double			m_dCorrInputVec[2];		

	// file info
	int				m_nFileSize;
	int				m_nRecCur;
	int				m_nRecCnt;

	// filter
	SampledData		m_dataBk;
	BOOL			m_bFirstSim;
	BOOL			m_bFilterSwitch;
	float			m_fFilterTemp;
	float			m_fFilterSn;
	float			m_fFilterLv;
	float			m_fFilterPr;
	float			m_fFilterLen;

	// etc.
	COLORREF		m_clBoLineColor;

#ifdef BATCH_OUT
	//@ ’Ç‰Á‹@”\
	BOOL			m_bEnaCsvOut;	//@
	FILE			*m_fpCsvOut;
	FILE			*m_fpCsvOutD;
	FILETIME		m_ftTime;
	CString			m_strLogName;
#endif

//------------------
// member function.
//------------------
protected:
	int		SetupBosysInfo(int sel);							// setup	BO system info.
public:
	void	ResetBOBatchInfo(int sel, int nMoldSize);			// reset	BO batch info.
	void	ClearBOBatchInfo(void);								// clear	BO batch info.
	int		DoBOBatch(SampledData *data, ResultData *result);	// execute	BO batch.
	int		DoBOBatchD(SampledData *data, ResultData *result);	// execute	BO batch for dep.
	BOOL	SetupLogFile(LPCTSTR name, int nIndex);				// setup	log data.
	BOOL	GetSampledData(SampledData *data);					// get		log data.
	void	DiscardLogFile(void);								// discard	log data.
	int		GetRecordCount(void);
	int		GetCurrentRecord(void);
	int		SetLogPointer(int nPoint);
	void	SetAverageData(SampledData *data, int *pData);
	void	LimitTempData(int *pDat);
	void	RecalcGroup(void);
	void	ClearSampledData(void);
	void	ClearStatusTbl(void);
	void	ClearResultTbl(void);
	void	ClearOccureResult(void);
	void	SetLogFilter(BOOL bSwitch, float fTemp, float fSn, float fLv, float fPr);
	void	SetCsvOut(BOOL bSet);		//@
	void	SetLogName(LPCTSTR pName);	//@
};

