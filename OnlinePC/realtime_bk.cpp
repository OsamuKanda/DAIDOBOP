#include	"stdafx.h"
#include	"realtime.h"

void	realtimeBOPrediction(void);
BOOL	AdSampleStart( void );
int		isSampleDataEnab(void);
void	clearSampledData(void);
void	readInputData(void);
void	writeOutputData(void);
//void	getSampleData(SampledData far *);
extern int isBOEnable;							// B.O. prediction enable

//for test
BOOL		StatusTbl[NumCombination];
void	appendLogData(SampledData far *data, BOOL far *result, BOOL allRes,int isNewLog);

// interrupt service routine for real time B.O. prediction.
void CALLBACK  timeCallBack(UINT timerID, UINT msg, DWORD usrParam, DWORD dw1, DWORD dw2)
{
	static int  isBusy = FALSE;

	// if another contexts are busy, do nothing.
	if(isBusy)		return;


	// if time call back routine is disabled, do nothing.
//	if(!isBOEnable)	return;

	// set this contexts be in execution B.O. prediction.
	isBusy = TRUE;

	// read intpu data.
	readInputData();

	// execute a real time B.O. prediction procedure.
	realtimeBOPrediction();

	// write output data.
	writeOutputData();

	// set this context be free.
	isBusy = FALSE;

}

void readInputData(void)
{
	
}

void writeOutputData(void)
{
	
}

// execute a real time B.O. prediction procedure.
void realtimeBOPrediction(void)
{
	SharedMemory	*pSharedMemory = GetSharedMemory();
	static SampledData  data;
	static ResultData	result;
	static int		preIsVcOn2;
	static int		preIsChargeChange2;
	int		isNewLog = FALSE;
	BOOL	isAllRes = FALSE;

	// test the input status of 'InVcOn'. do nothing if the signal negated.
	if(!pSharedMemory->isVcOn)	return;

	// test the edge of 'InVcOn'. if raising edge,
	if(!preIsVcOn2) 
	{
		// clear the execution count of prediction.
		pSharedMemory->executeCount  = 0;
		// clear B.O. sampled data buffer.
		clearSampledData();
		// assert request of a new logging data file.
		isNewLog = TRUE;
		// clear the BO occurrence status table.
		ZeroMemory(resultTbl,sizeof(ResultData) * numMaxResult);
	}

	// test the edge of 'InChargeChange'. if raising edge,
	if(pSharedMemory->isChargeChange && !preIsChargeChange2) 
	{
		// assert request of a new logging data file.
		isNewLog = TRUE;
	}

#if 0
	// get the sampled data.
	getSampledData((SampledData *)&data);

	// clear the result data of B.O. prediction.
	ZeroMemory(&result, sizeof(result));

	// test the B.O. prediction mode, and if enabled,
	if (!sharedMemory.isEnabLevelCtl || sharedMemory.isAutoLVCtl) 
	{
		// BOó\ímÇé¿çsÇµÅABOî≠ê∂Ç»ÇÁ,
		// execute B.O. prediction. if B.O. detected,
		isAllRes = boNewPrediction(&data, &result);
		if( isAllRes )
		{
			// set B.O. detected status.
			sharedMemory.isBOFetched = TRUE;
		}
		// increase the execution count of prediction.
		sharedMemory.executeCount++;
	}
	// otherwise
	else 
	{
		// clear the status table.
		ZeroMemory(StatusTbl,sizeof(StatusTbl));
		// un-occurrence is registered.
		ZeroMemory(&resultTbl[boAlarmTime],sizeof(ResultData));
		// clear B.O. sampled data buffer.
		clearSampledData();
	}
#endif
	// test
	int i;
	for( i = 0; i < 80; i++ )
	{
		data.val[i] = rand()%10 + 600 + i*10;
	}
	// append the sampled data at the tail of the logging data buffer.
	appendLogData(&data, StatusTbl, isAllRes, isNewLog);

	// keep the inputs for detecting the input edges.
	preIsVcOn2         = pSharedMemory->isVcOn;
	preIsChargeChange2 = pSharedMemory->isChargeChange;
}

#if 0
/*====================================================*/
/* assert a B.O. signal.
*/
void assertBOSignal(void)
{
	/* set the pulse width of BO Fetched signal. */
/* 2000/07 tanno update '=6' -> '|= 0x01' */
	pulseOutBOFetched2   |= 0x01;
}

/* 2000/07 tanno add */
/*====================================================*/
/* assert a B.O control signal.
*/
void assertBOSignal2(void)
{
	/* set the pulse width of BO control signal. */
	pulseOutBOFetched2   |= 0x02;
}

/*====================================================*/
/* reset a B.O. signal.
*/
void negateBOSignal(void)
{
	/* reset the pulse width of BO Fetched signal. */
	pulseOutBOFetched2   = 0;
}
#endif
