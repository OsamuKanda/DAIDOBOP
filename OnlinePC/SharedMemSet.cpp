#include "StdAfx.h"
#include "SharedMemSet.h"
#include "TMSParameter.h"

//---------------------------------------
SCOWriteDataStruct		*scoWriteData = NULL;		//	SCO Write data struct
TMSWriteDataStruct		*tmsWriteData = NULL;		//  TMS Write data struct

HANDLE hSCOWriteMutex;		// SCO Write Mutex.
HANDLE hTMSWriteMutex;		// TMS Write Mutex.

SharedMemSet::SharedMemSet(void)
{
}

SharedMemSet::~SharedMemSet(void)
{
}

BOOL SharedMemSet::createSharedMem(void)
{
	DWORD	dwMemsize;
	int rt;
	BOOL isExisted;

	// Create mutex.
	hSCOWriteMutex = CreateMutex(FALSE, 0, _T("SCOWriteMutex"));
    if(::GetLastError() == ERROR_ALREADY_EXISTS)
    {
#if _DEBUG
     //   AfxMessageBox( "SCO WriteData Mutex Exit", MB_OK);
#endif
    }

	// Create mutex.
	hTMSWriteMutex = CreateMutex(FALSE, 0, _T("TMSWriteMutex"));
    if(::GetLastError() == ERROR_ALREADY_EXISTS)
    {
#if _DEBUG
     //   AfxMessageBox( "TMS WriteData Mutex Exit", MB_OK);
#endif
    }

	// Shared memory create. (Smart casting observer HMI write data.)
	isExisted = FALSE;
	dwMemsize = sizeof(SCOWriteDataStruct);
	rt = m_atlFileMapSCOWrite.MapSharedMem(dwMemsize,"SCOWriteData",&isExisted,NULL,PAGE_READWRITE,FILE_MAP_ALL_ACCESS);

	if(rt != S_OK){
		AfxMessageBox( "Shared memory HMS writeData create Fail!", MB_OK);
		return( FALSE);
	}

	// Shared memory create. (TMS HMI write data.)
	isExisted = FALSE;
	dwMemsize = sizeof( TMSWriteDataStruct);
	rt = m_atlFileMapBOPWrite.MapSharedMem(dwMemsize,"TMSWriteData",&isExisted,NULL,PAGE_READWRITE,FILE_MAP_ALL_ACCESS);
	
	if(rt != S_OK){
		AfxMessageBox( "Shared memory TMS writeData create Fail!", MB_OK);
		return( FALSE);
	}

	// Shared memory mapping.
	scoWriteData = (SCOWriteDataStruct*)m_atlFileMapSCOWrite.GetData();
	tmsWriteData = (TMSWriteDataStruct*)m_atlFileMapBOPWrite.GetData();

	// Reset
	if(!isExisted)
	{
		memset(scoWriteData, 0, sizeof(SCOWriteDataStruct));
		memset(tmsWriteData, 0, sizeof(TMSWriteDataStruct));
	}

	return(TRUE);
}

void SharedMemSet::closeSharedMem(void)
{
   // Mutex release and close handle.
    ReleaseMutex(hSCOWriteMutex);
    CloseHandle(hSCOWriteMutex);

   // Mutex release and close handle.
    ReleaseMutex(hTMSWriteMutex);
    CloseHandle(hTMSWriteMutex);

   // Shared memory unmap.
	m_atlFileMapSCOWrite.Unmap();
	m_atlFileMapBOPWrite.Unmap();
}


void SharedMemSet::writeSharedData(short *vt){

	if(WaitForSingleObject(hTMSWriteMutex, 100L) != WAIT_TIMEOUT)
	{
		tmsWriteData->val[0] = vt[0];
		tmsWriteData->val[1] = vt[1];
		ReleaseMutex(hTMSWriteMutex);
	}
}

void SharedMemSet::readSharedData(StSampledData *stData, void *sg){
	static int preIndex = 0, isFirst = TRUE;
	SCOStatusStruct st;

	if(0)
	{
		memset(&stData->sd,			0, sizeof(SampledData));
		memset(&stData->vd,			0, sizeof(ViewData));
		memset((StInSignal *)sg,	0, sizeof(StInSignal));
	}

	if(WaitForSingleObject(hSCOWriteMutex, 100L) != WAIT_TIMEOUT)
	{
		memcpy(&st, &scoWriteData->st, sizeof(SCOStatusStruct));

		if(preIndex == st.index){
			//ReleaseMutex(hSCOWriteMutex);
			//return;
		}
		preIndex = st.index;

		memcpy(&stData->sd,			&scoWriteData->sd, sizeof(SampledData));
		memcpy(&stData->vd,			&scoWriteData->vd, sizeof(ViewData));
		memcpy((StInSignal *)sg,	&scoWriteData->sg, sizeof(StInSignal));

		ReleaseMutex(hSCOWriteMutex);
	}
	isFirst = FALSE;
}

