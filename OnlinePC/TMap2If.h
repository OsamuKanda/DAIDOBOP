#pragma once
#include <afxwin.h>
#include "BoBatch.h"

#define	WM_MAPDATA		(WM_APP + 0)

typedef struct
{
	CBoBatch	*pBatch;
	time_t		nStartTime;
	int			nOffsetTime;
}StLogInfo;

CDialog *GetTMapDlg(CDialog *parent, BOOL isRec);
void SetLogInfo(CDialog *pThis, StLogInfo *pInfo);
BOOL GetBatchStopReq(CDialog *pThis);
void SetBatchStopReq(CDialog *pThis, BOOL bSet);

