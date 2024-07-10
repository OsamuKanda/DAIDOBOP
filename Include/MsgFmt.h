// MsgFmt.h: メッセージフォーマット
//
//////////////////////////////////////////////////////////////////////

#if !defined(INC_MAILFMT_H_)
#define INC_MAILFMT_H_

#define	MFT_SZ			16384		// 全体サイズ
#define	MFT_SZ_HEAD		64			// ヘッダサイズ
#define	MFT_SZ_DATA		(MFT_SZ-MFT_SZ_HEAD)	
									// データサイズ

///////////////////////////////////////////////////////////////////////////////
// basic message definition.

// mail header of structure definition.
typedef union
{
	struct{
	int				nSize;			// データサイズ（ヘッダ込み）
	FILETIME		ft;				// 日時(ファイルタイム）
	};
	BYTE			buf[64];		// 全体サイズ
}StMsgHeader;

// mail data of structure definition.
typedef union
{
	struct{
	StMsgHeader		stHead;
	BYTE			byData[MFT_SZ_DATA];
	};
	BYTE			buf[MFT_SZ];	// 全体サイズ
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
