///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"

#define	COND_BLOCK_MAX		20
#define	COND_ITEM_MAX		50
#define	COND_REMARK_MAX		50

typedef enum
{
	ID_TypeA	= 1,
	ID_TypeB	= 2,
	ID_TypeC	= 3,
	ID_TypeD	= 4,
	ID_TypeE	= 5,
	ID_TypeF	= 6,
	ID_TypeAdd	= 33,
	ID_TypeSub	= 34,
	ID_TypeMul	= 35,
	ID_TypeDiv	= 36,
	ID_TypeLO	= 43,
	ID_TypeRO	= 44,
	ID_TypeLOE	= 45,
	ID_TypeROE	= 46,
	ID_TypeInp	= 53
}NmCondElemType;

typedef enum
{
	ID_BType1 = 1,				// "A > B" type
	ID_BType2 = 2,				// "A" type
	ID_BType3 = 3,				// "10" type
}NmCondBlockType;

typedef enum
{
	ID_LTypeNone = 0,
	ID_LTypeAnd = 1,
	ID_LTypeOr	= 2
}NmCondLogicType;

typedef struct
{
	NmCondBlockType	nmBlock;	// block type
	NmCondElemType	nmElem[7];	// element type (Index = 0:left, 1:center 2:right)
	double			dVal;		// value for ID_TypeInp
}StCondElem;

typedef struct
{
	StCondElem		stElemL;	// left element
	NmCondElemType	nmElemC;	// center element
	StCondElem		stElemR;	// right element
	NmCondLogicType	nmLogic;	// center element
}StCondBlock;

typedef struct
{
	int			nID;
	int			nItemNum;
	StCondBlock	data[COND_BLOCK_MAX];
	int			nTime;
	_TCHAR		cRemark[COND_REMARK_MAX];
}StCondItem;

typedef struct
{
	double		dTempAve[6];		// Temp average ABCDEF
}StCondInput;

typedef struct
{
	int			nTime;
	BOOL		bCondition;
}StCondResult;

///////////////////////////////////////////////////////////////////////////////
// function

BOOL ReadConfigCondition(void);
void ResetCastCond(void);
StCondItem *GetCondition(void);
StCondResult *GetConditionResult(void);
int GetConditionNum(void);
void SetRestartRequest(void);
BOOL GetRestartRequest(void);
void CheckCondition(StCondInput *pIn);
BOOL ReadCastCond(StCondItem *pCond, int *pNum);
BOOL WriteCastCond(StCondItem *pCond, int num);
