///////////////////////////////////////////////////////////////////////////////
// file
#include "stdafx.h"
#include "OnlinePC.h"
#include "InfoFile.h"
#include "CondFile.h"
#include "Config.h"

static CConfig		&cnf = theApp.m_cnf;

// information data
static StInfoItem	g_info[INFO_ITEM_MAX] = {0};	
static StInfoResult	g_inforslt[INFO_ITEM_MAX] = {0};
static int			g_numInfo = 0;

///////////////////////////////////////////////////////////////////////////////
// function definition

BOOL CheckElement(StInfoElem &elem);


///////////////////////////////////////////////////////////////////////////////
// function

// read config information.
BOOL ReadConfigInformation(void)
{
	return ReadCastInfo(g_info, &g_numInfo);
}

// reset config condition.
void ResetCastInfo(void)
{
	// read config condition.
	ReadConfigInformation();

	// reset memory
	memset(g_inforslt, 0, sizeof(g_inforslt));
}

// get condition setting;
StInfoItem *GetInformation(void)
{
	return (StInfoItem *)g_info;
}

// get condition setting;
StInfoResult *GetInformationResult(void)
{
	return (StInfoResult *)g_inforslt;
}

// get condition setting number.
int GetInformationNum(void)
{
	return g_numInfo;
}

BOOL IsInfoOutNum(void)
{
	int		i;

	for( i = 0; i < g_numInfo; i++ )
	{
		if( g_inforslt[i].bCondition ) 
			return TRUE;
	}

	return FALSE;
}

// check information
void CheckInformation(void)
{
	NmInfoLogicType	nmLogic;
	int				i, j, num;
	BOOL			bChk;


	for( i = 0; i < g_numInfo; i++ )
	{
		num = g_info[i].nItemNum;
		bChk = FALSE;
		for( j = 0; j < num; j++ )
		{
			if( j == 0 )
			{
				bChk = CheckElement(g_info[i].data[j]);
				nmLogic = g_info[i].data[j].nmLogic;
			}
			else
			{
				switch(nmLogic)
				{
				case ID_LogicAnd: bChk &= CheckElement(g_info[i].data[j]); break;
				case ID_LogicOr:  bChk |= CheckElement(g_info[i].data[j]); break;
				}
				nmLogic = g_info[i].data[j].nmLogic;
				if( !bChk || nmLogic == ID_LogicNone )
					break;
			}
		}
//		if( bChk || g_inforslt[i].bCondition )
		if( bChk )
		{
			g_inforslt[i].bCondition = TRUE;
		}
		else
		{
			g_inforslt[i].bCondition = FALSE;
		}
	}
}

// check element condition.
BOOL CheckElement(StInfoElem &elem)
{
	StCondItem		*pCond;
	StCondResult	*pResult;
	BOOL			bCondition;
	int				i, nCondNum;

	// get condition setting
	pCond = GetCondition();
	pResult = GetConditionResult();
	nCondNum = GetConditionNum();

	bCondition = FALSE;
	for( i = 0; i < nCondNum; i++ )
	{
		if( elem.nElem == pCond[i].nID )
		{
			bCondition = pResult[i].bCondition;
			break;
		}
	}

	// return condition.
	return bCondition;
}

// read information file.
BOOL ReadCastInfo(StInfoItem *pInfo, int *pNum)
{
	FILE		*fp;
	_TCHAR		buf[256];
	int			num;
	StInfoItem	dummy[INFO_ITEM_MAX] = {0};

	// make up file name.
	_stprintf(buf, _T("%s") CastInfoFile, cnf.WorkRootDir);

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
	if(fread(dummy, sizeof(StInfoItem), INFO_ITEM_MAX, fp) == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// after reading all lines, close the file.
	fclose(fp);

	// Read Data is adopted.
	memcpy( pInfo, &dummy, sizeof(dummy));
	*pNum = num;

	return TRUE;
}

// save information.
BOOL WriteCastInfo(StInfoItem *pInfo, int num)
{
	FILE		*fp;
	_TCHAR		buf[512];

	// make up file name.
	_stprintf(buf, _T("%s") CastInfoFile, cnf.WorkRootDir);

	// open the configration file.
	if ((fp = _tfopen(buf, _T("wb"))) == NULL)
		return FALSE;

	fwrite(&num, sizeof(num), 1, fp);
	fwrite(pInfo, sizeof(StInfoItem), INFO_ITEM_MAX, fp);

	// after reading all lines, close the file.
	fclose(fp);

	return TRUE;
}