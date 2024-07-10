// DrvChk.cpp: CDrvChk クラスのインプリメンテーション
//
#include "DrvChk.h"

// enumeration of the effective drive.
int CDrvChk::EnumDrive(StDrvInfo* pDrvInfo, int nSize)
{
	_TCHAR		szDrvBuf[128];
	int			i, nCnt, nDrvNum;

	// clear drive info.
	memset( pDrvInfo, 0, nSize );

	// get drive strings
	nCnt = ::GetLogicalDriveStrings( sizeof(szDrvBuf), szDrvBuf );

	// copy drive info buffer.
	for( i = 0, nDrvNum = 0; i < nCnt; )
	{
		_tcscpy( pDrvInfo->szDrv, &szDrvBuf[i] );
		pDrvInfo->nmType = (NmDrvType)::GetDriveType( pDrvInfo->szDrv );
		i += _tcslen( pDrvInfo->szDrv ) + sizeof(_TCHAR);
		pDrvInfo++;
		nDrvNum++;
	}

	return nDrvNum;
}

// copy file with the check drive.
DWORD CDrvChk::CopyFile(HWND hWnd, LPCTSTR pDestFilePath, LPCTSTR pSrcFilePath)
{
	struct _stat stDst; 

	// copy file.
	SHFILEOPSTRUCT st;
	st.hwnd		= hWnd;
	st.wFunc	= FO_COPY; 
	st.pFrom	= pSrcFilePath;
	st.pTo		= pDestFilePath;
	st.fFlags	= FOF_MULTIDESTFILES;
	if( 0 != SHFileOperation(&st) )
	{
		if( st.fAnyOperationsAborted == TRUE )
			return ERROR_CANCELLED;
		else
			return GetLastError();
	}

	// get file size.
	if( -1 == _tstat( pDestFilePath, &stDst ) )
		return ERROR_CANNOT_COPY;

	return 0L;
}

