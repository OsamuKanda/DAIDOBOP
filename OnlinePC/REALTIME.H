#pragma once
#include    "../include/bodef.h"
#include	"BoReal.h"
#include	"Logging.h"
#include	"ComDrv.h"

// CRealTimeƒNƒ‰ƒX
class CRealTime
{
public:
	CRealTime(); 
	~CRealTime();

// member data
public:
	SharedMemory	*m_pShMem;
	CBoReal			m_real;
	CLogging		*m_pLog;
	CComDrv			m_com;

// member function
public:
	int	Initialize(int sel);				// initialize realtime.
	int	ResetBopExe(int sel);				// reset BOP execute.
	int	Uninitialize(void);					// uninitialize realtime.
	int	InputBoData(void);					// input bop data.
	int	RealtimeBoPrediction(void);			// run realtime bop prediction.
	int	OutputBoData(void);					// output bop data.

	BOOL IsBOEnable(void);					// 
	void StartBOP(void);
	void StopBOP(void);
	
	// realtime callback routine
	static void CALLBACK  TimeCallBack(UINT timerID, UINT msg, DWORD usrParam, DWORD dw1, DWORD dw2);
};
