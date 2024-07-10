#pragma once
#include "afxwin.h"
#include	"../include/neuralne.h"
#include	"../include/bomodule.h"
#include "Config.h"

#define	InputVectorSize		256

// CSimExe
class CSimExe
{
public:
	enum{
		NORMAL			= 0,
		ERR_NO_NET,		
		ERR_LOAD_NN,	
		ERR_MEM_ALLOC,	
		ERR_NOT_TC,		
		ERR_NO_DATA,	
		ERR_OPEN_DATA,	
		ERR_NO_UPCH,	
		ERR_NO_LOCH,	
		ERR_OPEN_OUT,
		ERR_SIM_OUT
	};

public:
	CSimExe();
	virtual ~CSimExe();

// member data.
public:
	// neural network structure
	NeuralNet		*m_pPtnNN;					// pattern neural network
	NeuralNet		*m_pCorrNN;					// correlation N.N.		

	// trend data for N.N. execution
	FILE			*m_inFp;					// input trend file descripter		
	FILE			*m_outFp;					// output trend file descripter
	DataFileHeader	m_header;					// trend data header			
	SampledData		*m_pSampledData;			// ring buffer of trend data	
	int				m_nInputIdx;				// trend data index				
	int				m_nInputSize;				// ring buffer size				
	int				m_nInputStartIdx;			// starting point of ring buffer
	int				m_nInputReadIdx;			// read point of ring buffer	

	// input vectors for neural network
	double	m_dPtnInpVec[InputVectorSize];		// input for pattern N.N.		
	double	m_dCorrInpVec[2];					// input for correlation NN		
	double	upperTrend[InputVectorSize];		// work area for correlation	
	double	lowerTrend[InputVectorSize];		// work area for correlation	

	// filter
	SampledData		m_dataBk;
	BOOL			m_bFirstSim;
	BOOL			m_bFilterSwitch;
	float			m_fFilterTemp;

// member function.
public:
	int		GetInputSize(int nNetIdx);
	int		SimulateBOPredictStart(LPCTSTR name, int nNetIdx, int nCombiIdx);
	int		SimulateBOPredictLoop(int nCombiIdx);
	void	SimulateBOPredictEnd(void);
	void	ResetSimulation(void);
	int		LoadBONeuralNetwork(int netIdx);
	int		LoadBONeuralNetworkTmp(int netIdx);
	int		SetupSimulation(LPCTSTR name, int nNetIdx);
	BOOL	WriteSimulationOutput(double patternOut, double corrOut);
	BOOL	GetSampledData(void);
	void	SetLogFilter(BOOL bSwitch, float fTemp);
};

