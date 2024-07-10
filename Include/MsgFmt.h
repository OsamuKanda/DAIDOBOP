// MsgFmt.h: ���b�Z�[�W�t�H�[�}�b�g
//
//////////////////////////////////////////////////////////////////////

#if !defined(INC_MAILFMT_H_)
#define INC_MAILFMT_H_

#define	MFT_SZ			16384		// �S�̃T�C�Y
#define	MFT_SZ_HEAD		64			// �w�b�_�T�C�Y
#define	MFT_SZ_DATA		(MFT_SZ-MFT_SZ_HEAD)	
									// �f�[�^�T�C�Y

///////////////////////////////////////////////////////////////////////////////
// basic message definition.

// mail header of structure definition.
typedef union
{
	struct{
	int				nSize;			// �f�[�^�T�C�Y�i�w�b�_���݁j
	FILETIME		ft;				// ����(�t�@�C���^�C���j
	};
	BYTE			buf[64];		// �S�̃T�C�Y
}StMsgHeader;

// mail data of structure definition.
typedef union
{
	struct{
	StMsgHeader		stHead;
	BYTE			byData[MFT_SZ_DATA];
	};
	BYTE			buf[MFT_SZ];	// �S�̃T�C�Y
}StMsgData;


///////////////////////////////////////////////////////////////////////////////
// command definition.

// command type of definition.
typedef enum
{
	idCmdHealthy,		// healthy command.
	idCmdAlmRst,		// alarm reset command.
	idCmdGetPrmList,	// get param list.
	idCmdGetConfFile,	// get config file.
	idCmdGetSysFile,	// get system file.
	idCmdSetConfFile,	// set config file.
	idCmdSetSysFile,	// set system file.
	idCmdLast			// command number.
}NmCmdId;

// basic command definition.
typedef struct
{
	int			nSize;
	NmCmdId		nCmd;
	BYTE		byData[1];
}StCmd;

// config file info.
typedef struct
{
	int			nSize;
	int			nRet;
	_TCHAR		szName[MAX_PATH];
}StConfInfo;


///////////////////////////////////////////////////////////////////////////////
// command function.

inline void MakeSendCommand(StMsgData *p, NmCmdId nmCmd, BYTE *pBuf, int nSize)
{
	StCmd	*pCmd;

	memset(p->buf, 0, sizeof(StMsgData));
	p->stHead.nSize = sizeof(StMsgHeader) + sizeof(StCmd) - sizeof(BYTE) + nSize;
	pCmd = (StCmd *)p->byData;
	pCmd->nCmd = nmCmd;
	if( pBuf != NULL && nSize != 0 )
		memcpy(pCmd->byData, pBuf, nSize);
}


#endif // !defined(INC_MAILFMT_H_)
