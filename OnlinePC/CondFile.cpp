///////////////////////////////////////////////////////////////////////////////
// file
#include "stdafx.h"
#include "OnlinePC.h"
#include "CondFile.h"
#include "Config.h"

static CConfig		&cnf = theApp.m_cnf;

// condition data
static StCondItem	g_cond[COND_ITEM_MAX] = {0};	
static StCondResult	g_condrslt[COND_ITEM_MAX] = {0};	
static int			g_numCond = 0;
static double		g_lfTempAve[6];
static BOOL			g_bRestart = FALSE;

///////////////////////////////////////////////////////////////////////////////
// function definition

BOOL CheckBlock(StCondBlock &block);
double GetTempAveValue(NmCondElemType nmElem);


///////////////////////////////////////////////////////////////////////////////
// function

// read config condition.
BOOL ReadConfigCondition(void)
{
	return ReadCastCond(g_cond, &g_numCond);
}

// reset config condition.
void ResetCastCond(void)
{
	// read config condition.
	ReadConfigCondition();

	// reset memory
	memset(g_condrslt, 0, sizeof(g_condrslt));
	g_bRestart = FALSE;
}

// get condition setting;
StCondItem *GetCondition(void)
{
	return (StCondItem *)g_cond;
}

// get condition setting;
StCondResult *GetConditionResult(void)
{
	return (StCondResult *)g_condrslt;
}

// get condition setting number.
int GetConditionNum(void)
{
	return g_numCond;
}

// set restart request
void SetRestartRequest(void)
{
	g_bRestart = TRUE;
}

// set restart request
BOOL GetRestartRequest(void)
{
	return g_bRestart;
}

// check condition
void CheckCondition(StCondInput *pIn)
{
	NmCondLogicType	nmLogic;
	int				i, j, num;
	BOOL			bChk;

	// input data
	g_lfTempAve[0] = pIn->dTempAve[0];
	g_lfTempAve[1] = pIn->dTempAve[1];
	g_lfTempAve[2] = pIn->dTempAve[2];
	g_lfTempAve[3] = pIn->dTempAve[3];
	g_lfTempAve[4] = pIn->dTempAve[4];
	g_lfTempAve[5] = pIn->dTempAve[5];
	
	// check condition
	for( i = 0; i < g_numCond; i++ )
	{
		num = g_cond[i].nItemNum;
		bChk = FALSE;
		for( j = 0; j < num; j++ )
		{
			if( j == 0 )
			{
				bChk = CheckBlock(g_cond[i].data[j]);
				nmLogic = g_cond[i].data[j].nmLogic;
			}
			else
			{
				switch(nmLogic)
				{
				case ID_LTypeAnd: bChk &= CheckBlock(g_cond[i].data[j]); break;
				case ID_LTypeOr:  bChk |= CheckBlock(g_cond[i].data[j]); break;
				}
				nmLogic = g_cond[i].data[j].nmLogic;
				if( !bChk || nmLogic == ID_LTypeNone )
					break;
			}
		}
		if( bChk )
		{
			if( g_condrslt[i].nTime >= g_cond[i].nTime * 2 )
				g_condrslt[i].bCondition = TRUE;
			else
				g_condrslt[i].nTime++;
		}
		else
		{
			g_condrslt[i].nTime = 0;
			g_condrslt[i].bCondition = FALSE;
		}
	}
}

// check block condition.
BOOL CheckBlock(StCondBlock &block)
{
	double	dSideL, dSideR, dValL, dValR;
	BOOL	bCondition;

	///////////////
	// Left Side
	dSideL = 0.0;
	switch(block.stElemL.nmBlock)
	{
	case ID_BType1:
		dValL = GetTempAveValue(block.stElemL.nmElem[0]);
		dValR = GetTempAveValue(block.stElemL.nmElem[2]);
		switch(block.stElemL.nmElem[1])
		{
		case ID_TypeAdd: dSideL = dValL + dValR; break;
		case ID_TypeSub: dSideL = dValL - dValR; break;
		case ID_TypeMul: dSideL = dValL * dValR; break;
		case ID_TypeDiv: if( dValR != 0.0 ) dSideL = dValL / dValR; break;
		}
		break;
	case ID_BType2:
		dSideL = GetTempAveValue(block.stElemL.nmElem[0]);
		break;
	default:
		break;
	}

	///////////////
	// Right Side
	dSideR = 0.0;
	switch(block.stElemR.nmBlock)
	{
	case ID_BType1:
		dValL = GetTempAveValue(block.stElemR.nmElem[0]);
		dValR = GetTempAveValue(block.stElemR.nmElem[2]);
		switch(block.stElemL.nmElem[1])
		{
		case ID_TypeAdd: dSideR = dValL + dValR; break;
		case ID_TypeSub: dSideR = dValL - dValR; break;
		case ID_TypeMul: dSideR = dValL * dValR; break;
		case ID_TypeDiv: if( dValR != 0.0 ) dSideL = dValL / dValR; break;
		}
		break;
	case ID_BType2:
		dSideR = GetTempAveValue(block.stElemR.nmElem[0]);
		break;
	case ID_BType3:
		dSideR = block.stElemR.dVal;
		break;
	default:
		break;
	}

	///////////////
	// check
	bCondition = FALSE;
	switch(block.nmElemC)
	{
	case ID_TypeLO: bCondition = (dSideL > dSideR) ? TRUE: FALSE; break;
	case ID_TypeRO: bCondition = (dSideL < dSideR) ? TRUE: FALSE; break;
	case ID_TypeLOE:bCondition = (dSideL >= dSideR) ? TRUE: FALSE; break;
	case ID_TypeROE:bCondition = (dSideL <= dSideR) ? TRUE: FALSE; break;
	}

	// return condition.
	return bCondition;
}

// get temperature average
double GetTempAveValue(NmCondElemType nmElem)
{
	double	dVal;

	switch(nmElem)
	{
	case ID_TypeA: dVal = g_lfTempAve[0]; break;
	case ID_TypeB: dVal = g_lfTempAve[1]; break;
	case ID_TypeC: dVal = g_lfTempAve[2]; break;
	case ID_TypeD: dVal = g_lfTempAve[3]; break;
	case ID_TypeE: dVal = g_lfTempAve[4]; break;
	case ID_TypeF: dVal = g_lfTempAve[5]; break;
	default:break;
	}

	return dVal;
}


// read information file.
BOOL ReadCastCond(StCondItem *pCond, int *pNum)
{
	FILE		*fp;
	_TCHAR		buf[256];
	int			num;
	StCondItem	dummy[COND_ITEM_MAX] = {0};

	// make up file name.
	_stprintf(buf, _T("%s") CastCondFile, cnf.WorkRootDir);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("rb"))) == NULL)
		return FALSE;

	// read data
	if(fread(&num, sizeof(num), 1, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// read data
	if(fread(dummy, sizeof(StCondItem), COND_ITEM_MAX, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// after reading all lines, close the file.
	fclose(fp);

	// Read Data is adopted.
	memcpy( pCond, &dummy, sizeof(dummy));
	*pNum = num;

	return TRUE;
}

// save information.
BOOL WriteCastCond(StCondItem *pCond, int num)
{
	FILE		*fp;
	_TCHAR		buf[512];

	// make up file name.
	_stprintf(buf, _T("%s") CastCondFile, cnf.WorkRootDir);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("wb"))) == NULL)
		return FALSE;

	fwrite(&num, sizeof(num), 1, fp);
	fwrite(pCond, sizeof(StCondItem), COND_ITEM_MAX, fp);

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}

