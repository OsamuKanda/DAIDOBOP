#pragma once
#include "afxwin.h"
#include	"../include/neuralne.h"
#include	"../include/bomodule.h"
#include "Config.h"
#include "WeiList.h"

///////////////////////////////////////////////////////////////////////////////
// definition

// declaration of teaching data buffer
typedef	struct {
	_TCHAR	path[256];
	long	fIdx;
	long	length;
	long	shift;		//@
	time_t	startTime;
	float	fFilter;
	DataFileHeader	dtHead;
	// コンビネーション番号を熱電対番号に変更
//@	int		combiID;
	int		arg1, arg2;		// 熱電対コンビ番号
} TeachDataSpec;

#define	NumPatternInputVector	256
#define	NumTrendData			200
#define SelTrendData			128
#define	RepeatTimes				10				// default B.P. repeat times.

///////////////////////////////////////////////////////////////////////////////
// CBpOpe class
class CBpOpe
{
public:
	enum{
		NORMAL			= 0,
		ERR_NO_NET,		
		ERR_PATT,		
		ERR_CORR,	
		ERR_PATT_CORR,		
		ERR_LOAD_NN,
		ERR_NO_INIT,
		ERR_NUM_DATA,
		ERR_ORD_PREP,
		ERR_ALM_PREP,
		ERR_SPEC_FILE,
		ERR_LOG_FILE
	};
	typedef enum{
		IsNone				= 0,
		IsTeachPatOrdinary	= 1,
		IsTeachPatAlarm		= 2,
		IsTeachCorOrdinary	= 4,
		IsTeachCorAlarm		= 8,
		IsMask				= 255
	}NmTeachCondition;

public:
	CBpOpe();
	virtual ~CBpOpe();

// member data.
public:
	int					m_nRepeatTimes;				// B.P. repeat times(def=10)
	int					m_nTeachCondition;			// 

	NeuralNet			*m_pPattNN;					// pattern neural network
	NeuralNet			*m_pCorrNN;					// correlation N.N.		
	int					m_nNetIdx;

	TeachDataSpec		m_stOrdDataSpec;			// ordinary teaching data spec.
	TeachDataSpec		m_stAlmDataSpec;			// alarm teaching data spec.

	SampledData			*m_stOrdData;				// ordinary teaching data buffer	
	int					m_nNumOrdData;				// the number of ordinary data buf.	
	HGLOBAL				m_hOrdData;					// handler of above buffer			

	SampledData			*m_stAlarmData;				// alarm teaching data buffer		
	int					m_nNumAlarmData;			// the number of alarm data buffer	
	HGLOBAL				m_hAlmData;					// handler of above buffer			

	double				m_dOrdinaryPatInput[NumPatternInputVector];
	double				m_dAlarmPatInput[NumPatternInputVector];
	double				m_dOrdinaryCorInput[8];
	double				m_dAlarmCorInput[8];

	BPStructure			m_stPatternBP;				// b.p. work area for pattern n.n.
	BPStructure			m_stCorrBP;					// b.p. work area for corr. n.n.

	double				m_dPattOrdTeachVal;
	double				m_dPattAlmTeachVal;
	double				m_dCorrOrdTeachVal;
	double				m_dCorrAlmTeachVal;

	CString				m_strTrendOutFilePath;

// member function.
protected:
	void	SetupTeachingDataBuffer(void);
	void	DestroyTeachingDataBuffer(void);
	int		ReadTeachDataSpec(_TCHAR *fname, TeachDataSpec *spec);
	int		LoadTeachData(TeachDataSpec *spec, SampledData *teachData, int *numTeachData, long offset);
	int		SetupBOInputVector(int nOrdArg1, int nOrdArg2, int nAlmArg1, int nAlmArg2);
	void	SetupBPBOWorkArea(void);
	void	ResetBPWorkArea(BPStructure *bp, NeuralNet *net);
	void	SaveBackupBONetwork(void);
	void	DeleteBONeuralNetBackup(void);
	void	MoveOldBackupBONetwork(void);
public:
	void	CalcNeuralNetwork(double *pdOrdPatOut, double *pdAlmPatOut,	double *pdOrdCorOut, double *pdAlmCorOut);
	int		LoadBONeuralNetwork(int nNetIdx);
	int		LoadBONeuralNetworkTmp(int nNetIdx);
	int		LoadTeachingData(int nNetIdx);
	int		LoadTeachingDataOrd(int nNetIdx);
	int		LoadTeachingDataAlm(int nNetIdx);
	void	ResetBONeuralNetwork(void);
	void	FreeBONeuralNetwork(void);
	int		DoBPBONeuralNetwork(int nOrdArg1, int nOrdArg2, int nAlmArg1, int nAlmArg2);
	void	StepBPBONeuralNetwork(void);
	void	RecoverBONeuralNetwork(void);
	void	RecoverBONeuralNetwork(LPTSTR pPathP, LPTSTR pPathC);
	int		SaveBONeuralNetwork(void);
	int		SaveBONeuralNetworkTmp(void);
	void	SetTeachVal(double dPattOrd, double dPattAlm, double dCorrOrd, double dCorrAlm);
	void	SetNeuralParam(double dEatha, double dAlpha);
	void	SetTeachCondition(int nCondition);
	int		WriteTeachDataSpec(_TCHAR *fname, TeachDataSpec *spec);
	void	ClearBpBONetwork(void);
	BOOL	IsBONeuralNetworkTmp(int nNetIdx);
	BOOL	DelBONeuralNetworkTmp(int nNetIdx);
	BOOL	CopyBONeuralNetworkTmp(int nNetIdx);
};

