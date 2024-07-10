///////////////////////////////////////////////////////////////////////////////////
//	AJ71QE71 
//	File Name	： Aj71Qe.h
//	Contents	： Defined Transmit Data
//				：
//	Trend		：2001/04/16 Create ,K.Tanno
//				：
//				：
///////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------
// #define　定義
// -------------------------------------------------------------------------
#define	LEN_BASIC_CMD		12		// コマンド基本要求データ長
#define	LEN_BASIC_RES		2		// レスポンス基本要求データ長
#define	LEN_DEVNO			3		// デバイスＮＯバッファ数
#define	LEN_DEVCNT			2		// 点数バッファ数
#define LEN_ENDCODE			2		// 終了コード長

#define POS_DATA_CMD		21		// コマンドデータセット位置
#define POS_DATA_RES		11		// レスポンスデータ取得位置

#define	DEV_D				0xA8	// デバイスコードＤ
#define	DEV_R				0xAF	// デバイスコードＲ
#define	DEV_W				0xB4	// デバイスコードＷ

typedef struct
{
	BYTE	nSub[2],				// サブヘッダ
			nNetNo,					// ネットワークＮＯ
			nPcNo,					// ＰＣＮＯ
			nFix1[3],				// 固定値
			nDataLen[2],			// 要求データ長
			nCpuTime[2],			// ＣＰＵ監視タイマ
			nCmd[2],				// コマンド
			nSubCmd[2],				// サブコマンド
			nDevNo[LEN_DEVNO],		// 先頭デバイス
			nDevCode,				// デバイスコード
			nDevCount[LEN_DEVCNT];	// デバイス点数
}CMD_MSG,*PCMD_MSG;

typedef struct
{
	BYTE	nSub[2],				// サブヘッダ
			nNetNo,					// ネットワークＮＯ
			nPcNo,					// ＰＣＮＯ
			nFix1[3],				// 固定値
			nDataLen[2],			// 要求データ長
			nEndCode[2];			// 終了コード
}RES_MSG,*PRES_MSG;

// PCﾃﾞｰﾀ(数値)をPLCﾃﾞｰﾀ(ﾊﾞｲﾄﾃﾞｰﾀの配列)に変換する。
inline void PcToPlc(UINT uiPcData,LPBYTE lpData,int iDataLen)
{
															// ＜◆関数内部変数定義◆＞
	UINT	uiWork,											// PCﾃﾞｰﾀｺﾋﾟｰ領域
			uiShiftSize;									// ｼﾌﾄｻｲｽﾞ
	int		iCnt;											// ｲﾝﾃﾞｯｸｽ
															// ＜◆PLCﾃﾞｰﾀに変換◆＞
	uiWork = uiPcData;										// PCﾃﾞｰﾀをｺﾋﾟｰ
	uiShiftSize = sizeof(BYTE) * 8;							// ﾃﾞｰﾀをｼﾌﾄするｻｲｽﾞをｾｯﾄ
	if(iDataLen > 1)										// ﾃﾞｰﾀ長が1より大きい場合
	{
		for(iCnt = iDataLen - 1;iCnt > 0;iCnt--)			// PLCﾃﾞｰﾀ変換
		{
			lpData[iCnt] = (BYTE)(uiWork >> (uiShiftSize * iCnt));	// 上位から順番にﾃﾞｰﾀを取得する
			uiWork -= (lpData[iCnt] << (uiShiftSize * iCnt));
		}
	}
	lpData[0] = (BYTE)uiWork;										// 最下位のﾃﾞｰﾀをｾｯﾄ
}

// PLCﾃﾞｰﾀ(ﾊﾞｲﾄﾃﾞｰﾀの配列)をPCﾃﾞｰﾀ(数値)に変換する。
inline UINT PlcToPc(LPBYTE	lpData,int iDataLen)
{
															// ＜◆関数内部変数定義◆＞
	UINT	uiRet,											// 戻り値
			uiShiftSize;									// ｼﾌﾄｻｲｽﾞ
	int		iCnt;											// ｲﾝﾃﾞｯｸｽ
															// ＜◆PCﾃﾞｰﾀに変換◆＞
	uiShiftSize = sizeof(BYTE) * 8;							// ﾃﾞｰﾀをｼﾌﾄするｻｲｽﾞをｾｯﾄ
	if(iDataLen > 1)										// ﾃﾞｰﾀ長が1より大きい場合
	{
		uiRet = lpData[iDataLen - 1];						// 上位の値をｾｯﾄ
		for(iCnt = iDataLen - 1;iCnt > 0;iCnt--)			// PCﾃﾞｰﾀに変換
			uiRet = (uiRet << uiShiftSize) + lpData[iCnt - 1];
	}
	else													// ﾃﾞｰﾀ長が1以下の場合
		uiRet = *lpData;									// 1ﾃﾞｰﾀｾｯﾄ

	return uiRet;											// 変換したﾃﾞｰﾀを返す
}

// ワード一括書込みコマンドデータセット(戻り値はデータセット位置)
inline int SetWriteCmdW( LPBYTE pbyData, BYTE nDev, UINT uiDevNo, WORD wLen)
{
	PCMD_MSG	pcm = (PCMD_MSG)pbyData;
	UINT	uiDataLen;

	pcm->nSub[0]		= 0x50;					// サブコマンド
	pcm->nSub[1]		= 0x00;					// ↓
	pcm->nNetNo			= 0x00;					// ネットＮＯ
	pcm->nPcNo			= 0xFF;					// ＰＣＮＯ
	pcm->nFix1[0]		= 0xFF;					// 固定値
	pcm->nFix1[1]		= 0x03;					// ↓
	pcm->nFix1[2]		= 0x00;					// ↓
	uiDataLen =  LEN_BASIC_CMD + wLen * 2;		// 要求データ長
	PcToPlc( uiDataLen, pcm->nDataLen, 2);		// ↓
	pcm->nCpuTime[0]	= 0x10;					// ＣＰＵ監視タイマ
	pcm->nCpuTime[1]	= 0x00;					// ↓
	pcm->nCmd[0]		= 0x01;					// コマンド
	pcm->nCmd[1]		= 0x14;					// ↓
	pcm->nSubCmd[0]		= 0x00;					// サブコマンド
	pcm->nSubCmd[1]		= 0x00;					// ↓
	PcToPlc( uiDevNo, pcm->nDevNo, LEN_DEVNO);	// デバイスＮＯ
	pcm->nDevCode		= nDev;					// デバイスコード
	PcToPlc( wLen, pcm->nDevCount, LEN_DEVCNT);	// 点数

	return (int)(POS_DATA_CMD + wLen * 2);
}

// ワード一括読込みコマンドデータセット
inline int SetReadCmdW( LPBYTE pbyData, BYTE nDev, UINT uiDevNo, WORD wLen)
{
	PCMD_MSG	pcm = (PCMD_MSG)pbyData;
	UINT		uiDataLen;

	pcm->nSub[0]		= 0x50;					// サブコマンド
	pcm->nSub[1]		= 0x00;					// ↓
	pcm->nNetNo			= 0x00;					// ネットＮＯ
	pcm->nPcNo			= 0xFF;					// ＰＣＮＯ
	pcm->nFix1[0]		= 0xFF;					// 固定値
	pcm->nFix1[1]		= 0x03;					// ↓
	pcm->nFix1[2]		= 0x00;					// ↓
	uiDataLen =  LEN_BASIC_CMD;					// 要求データ長
	PcToPlc( uiDataLen, pcm->nDataLen, 2);		// ↓
	pcm->nCpuTime[0]	= 0x10;					// ＣＰＵ監視タイマ
	pcm->nCpuTime[1]	= 0x00;					// ↓
	pcm->nCmd[0]		= 0x01;					// コマンド
	pcm->nCmd[1]		= 0x04;					// ↓
	pcm->nSubCmd[0]		= 0x00;					// サブコマンド
	pcm->nSubCmd[1]		= 0x00;					// ↓
	PcToPlc( uiDevNo, pcm->nDevNo, LEN_DEVNO);	// デバイスＮＯ
	pcm->nDevCode		= nDev;					// デバイスコード
	PcToPlc( wLen, pcm->nDevCount, LEN_DEVCNT);	// 点数

	return (int)(POS_DATA_CMD);
}

// ワード一括書込みレスポンスデータ取得(チェック有り)
inline int GetWriteResW( LPBYTE pbyData )
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;
	UINT		uiData;

	// サブヘッダチェック
	uiData = PlcToPc( pcm->nSub, 2);
	if(	0xD0 != uiData)
		return -1;

	// ネットＮＯチェック
	if( 0x00 != pcm->nNetNo )
		return -2;

	// ＰＣＮＯチェック
	if( 0xFF != pcm->nPcNo )
		return -2;

	// 要求データ長チェック
	uiData = PlcToPc( pcm->nDataLen, 2);
	if(	UINT(LEN_BASIC_RES) != uiData)
		return -3;

	// 終了コードチェック
	uiData = PlcToPc( pcm->nEndCode, LEN_ENDCODE);
	if(	UINT(0x00) != uiData)
		return -4;

	return (int)(POS_DATA_RES);
}

// ワード一括読込みレスポンスデータ取得(チェック有り)
inline int GetReadResW( LPBYTE pbyData, WORD wLen)
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;
	UINT		uiData;

	// サブヘッダチェック
	uiData = PlcToPc( pcm->nSub, 2);
	if(	0xD0 != uiData)
		return -1;

	// ネットＮＯチェック
	if( 0x00 != pcm->nNetNo )
		return -2;

	// ＰＣＮＯチェック
	if( 0xFF != pcm->nPcNo )
		return -2;

	// 要求データ長チェック
	uiData = PlcToPc( pcm->nDataLen, 2);
	if(	UINT(LEN_BASIC_RES + wLen) != uiData)
		return -3;

	// 終了コードチェック
	uiData = PlcToPc( pcm->nEndCode, LEN_ENDCODE);
	if(	UINT(0x00) != uiData)
		return -4;

	return (int)(POS_DATA_RES);
}

// 終了コード取得
inline int GetEndCode( LPBYTE pbyData )
{
	PRES_MSG	pcm = (PRES_MSG)pbyData;

	return (int)(PlcToPc( pcm->nEndCode, LEN_ENDCODE));
}


