///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"

#define	INFO_ELEM_MAX		50
#define	INFO_ID_MAX			50
#define	INFO_ITEM_MAX		50
#define	INFO_INFO_MAX		50

typedef enum
{
	ID_LogicNone = 0,
	ID_LogicAnd = 1,
	ID_LogicOr	= 2
}NmInfoLogicType;

typedef struct
{
	int				nElem;		// element
	NmInfoLogicType	nmLogic;	// logic
}StInfoElem;

typedef struct
{
	int			nItemNum;
	StInfoElem	data[INFO_ELEM_MAX];
	_TCHAR		cInfo[INFO_INFO_MAX];
}StInfoItem;

typedef struct
{
	BOOL	bCondition;
}StInfoResult;

typedef struct
{
	int		nNo;
	int		nCondition;		// 0:none, 1:off, 2:on
	_TCHAR	cInfo[INFO_INFO_MAX];
}StInfoOutput;

///////////////////////////////////////////////////////////////////////////////
// function

BOOL ReadConfigInformation(void);
void ResetCastInfo(void);
StInfoItem *GetInformation(void);
StInfoResult *GetInformationResult(void);
int GetInformationNum(void);
BOOL IsInfoOutNum(void);
void CheckInformation(void);
BOOL ReadCastInfo(StInfoItem *pInfo, int *pNum);
BOOL WriteCastInfo(StInfoItem *pInfo, int num);

