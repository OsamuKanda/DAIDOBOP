// DrvChk.h: CTSleep クラスのインターフェイス
//
#if !defined(AFX_DRVCHK_H_)
#define AFX_DRVCHK_H_

#include <afxwin.h>
#include <sys/stat.h>


// constant value definition.
#define	DRV_NUM_MAX		26


// enumeration of drive type.
typedef enum
{
	idTypeUnknown	= DRIVE_UNKNOWN,
	idTypeNoRootDIr	= DRIVE_NO_ROOT_DIR,
	idTypeRemovable	= DRIVE_REMOVABLE,
	idTypeFixed		= DRIVE_FIXED,
	idTypeRemote	= DRIVE_REMOTE,
	idTypeCdRom		= DRIVE_CDROM,
	idTypeRamDisk	= DRIVE_RAMDISK
}NmDrvType;

// definition of drive information.
typedef struct
{
	_TCHAR		szDrv[MAX_PATH];	// drive name
	NmDrvType	nmType;				// drive type
}StDrvInfo;

// drive check class
class CDrvChk  
{
public:
	static int		EnumDrive(StDrvInfo* pDrvInfo, int nSize);
	static DWORD	CopyFile(HWND hWnd, LPCTSTR pDestFilePath, LPCTSTR pSrcFilePath);
};

#endif // !defined(AFX_DRVCHK_H_)
