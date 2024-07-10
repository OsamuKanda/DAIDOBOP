#include	"stdafx.h"
#include	"OnlinePC.h"
#include	"Logging.h"


///////////////////////////////////////////////////////////////////////////////
// reference data
static CBolog	&lg	= theApp.m_log;


///////////////////////////////////////////////////////////////////////////////
// definition
static CLogging		*pL = NULL;				// own pointer.(for realtime)
CLogging *GetLoggingClass(void)				// get logger class instance.
{
	return pL;
}

///////////////////////////////////////////////////////////////////////////////
// realtime class function

//=============================================================================
// Name		: CLogging
// Comment	: initialize member variable.
// JP		: メンバ変数の初期化など
//=============================================================================
CLogging::CLogging()
{
	pL					= this;
	m_isLogEnable		= FALSE;
	m_isNewLogRequest	= FALSE;
	m_nNumLogData		= 0;    
	m_nLogDataRIdx		= 0;   
	m_nLogDataWIdx		= 0;   
	memset(m_stSampleBuf,	0, sizeof(m_stSampleBuf));
	memset(m_szLogFileName, 0, sizeof(m_szLogFileName));
	memset(m_stTrendDir,	0, sizeof(m_stTrendDir));
}

//=============================================================================
// Name		: ~CLogging
// Comment	: uninitialize member variable.
// JP		: メンバ変数の破棄など
//=============================================================================
CLogging::~CLogging()
{
}

//=============================================================================
// Name		: SetupLogBuffer
// Argument	: none.
// Return	: none.
// Comment	: set up the logging data buffer area.
// JP		: 共有バッファ（リング）のクリア
//			: 条件：LogStop状態
//=============================================================================
void CLogging::SetupLogBuffer(void)
{
	// initialize the FIFO mechanism.
	m_nNumLogData	= 0;
	m_nLogDataRIdx	= 0;
	m_nLogDataWIdx	= 0;
	memcpy(m_stSampleBuf, 0, sizeof(m_stSampleBuf));
}

//=============================================================================
// Name		: LogStart
// Argument	: none.
// Return	: none.
// Comment	: enable the logging operation.
// JP		: ログの開始。条件：なし
//=============================================================================
void CLogging::LogStart(void)
{
	// let logging operation enable.
	if(!m_isLogEnable) m_isLogEnable = TRUE;
}

//=============================================================================
// Name		: LogStop
// Argument	: none.
// Return	: none.
// Comment	: disable the logging operation.
// JP		: ログの停止。条件：なし
//=============================================================================
void CLogging::LogStop(void)
{
	// let logging operation disable.
	m_isLogEnable = FALSE;
}

//=============================================================================
// Name		: AppendData
// Argument	: data		... sample data address.
//			: result	... bop result data address.
//			: allRes	... bop all result data address.
//			: isNewLog	... new log file flag.
// Return	: none.
// Comment	: append sampled data to the tail of logging data buffer.
//			: this function is called by interrupt service routine.
// JP		: 共有バッファ（リング）にデータ追加。
//			: 条件：リアルタイム処理から呼び出す
//=============================================================================
void CLogging::AppendData(StSampledData *data, BOOL *result, BOOL *resultD, int allRes, int isNewLog)
{
	// if the logging operation is disabled, do nothing.
	if (!m_isLogEnable)					return;

	// set the request of a new logging data file.
	if (isNewLog)			m_isNewLogRequest	= TRUE;

	// if the logging data buffer is full, do nothing.
	if (m_nNumLogData >= LogBufferSize)	return;

	// store the sampled data.
	m_stSampleBuf[m_nLogDataWIdx] = *data;

	// store the result data.
	memcpy(m_stSampleBuf[m_nLogDataWIdx].isResultDat, result, sizeof(m_stSampleBuf[m_nLogDataWIdx].isResultDat));
	memcpy(m_stSampleBuf[m_nLogDataWIdx].isResultDatD, resultD, sizeof(m_stSampleBuf[m_nLogDataWIdx].isResultDatD));

	// store the all result data
	m_stSampleBuf[m_nLogDataWIdx].isAllResult = allRes;

	// increase the number of sampled data in the logging buffer.
	m_nNumLogData++;

	// increase the write index of the logging data buffer.
	if (++m_nLogDataWIdx == LogBufferSize)		
		m_nLogDataWIdx	= 0;
}

//=============================================================================
// Name		: GetLogData
// Argument	: data		... sample data address.
//			: result	... bop result data address.
//			: allRes	... bop all result data address.
// Return	: getting result.
// Comment	: get sampled data from the head of the logging data buffer.
// JP		: 共有バッファ（リング）からデータ取り出し。
//			: 条件：アプリケーション処理から呼び出す
//=============================================================================
int CLogging::GetLogData(StSampledData *data, BOOL *result, BOOL *resultD, int *allRes)
{
	// if the logging data buffer is empty, return LogDataAbsent.
	if (!m_nNumLogData)	return	LogDataAbsent;

	// if the request of a new logging data file asserted,
	if (m_isNewLogRequest) {
		// clear the request. and return ChangeLog.
		m_isNewLogRequest	= FALSE;
		return	ChangeLog;
	}

	// get the sampled data.
	*data = m_stSampleBuf[m_nLogDataRIdx];

	// get the result data.
	memcpy(result, data->isResultDat, sizeof(data->isResultDat));
	memcpy(resultD, data->isResultDatD, sizeof(data->isResultDatD));

	*allRes = data->isAllResult;

	// decrease the number of sampled data in the logging buffer.
	m_nNumLogData--;

	// increase the read index of the logging data buffer.
	if (++m_nLogDataRIdx == LogBufferSize)		
		m_nLogDataRIdx	= 0;

	// after above procedure, return LogDataPresent.
	return	LogDataPresent;
}

//=============================================================================
// Name		: SetupDataHeader
// Argument	: pHeader ... log header info address.
// Return	: none.
// Comment	: setup the logging data file header.
// JP		: ログヘッダ情報を設定する。
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
void CLogging::SetupDataHeader(DataFileHeader *pHeader)
{
	// set data file header info.
	m_stFileHead = *pHeader;
}

//=============================================================================
// Name		: CreateLogFile
// Argument	: none.
// Return	: none.
// Comment	: create a new logging data file.
// JP		: ログファイルの生成。（鋳込開始やチャージ変更など）
//			: 条件：アプリケーション処理から呼び出す
//=============================================================================
void CLogging::CreateLogFile(void)
{
	time_t	currentTime;
	int	fd;

	// get current time.
	currentTime	= time(NULL);

	// allocate disk space for a new logging file and if failed,
	if (AllocateLogFileEntry() == FALSE) {
		// clear the name of logging data file.
		m_szLogFileName[0]	= _T('\0');
		// do not do following procedures.
		return;
	}

	// generate logging file name and create the file. if failed, 
	if ((fd = _lcreat(GetLogFileNameXp(currentTime), 0)) < 0) {
		// clear the name of logging data file. 
		m_szLogFileName[0]	= _T('\0');
		// do not do following procedures. 
		return;
	}

	// write the header of the logging file. 
	_lwrite(fd, (LPCSTR)&m_stFileHead, sizeof(m_stFileHead));

	// close the file. 
	_lclose(fd);
}

// create a new logging data file and save old data.
void CLogging::CreateLogFile(SampledData *pOld, int num, int idx)
{
	time_t	currentTime;
	FILE	*fd;
	int		i, index;

	// get current time.
	currentTime	= time(NULL);
	currentTime -= (time_t)((double)num * (m_stFileHead.header.samplePeriod / 1000.0));

	// allocate disk space for a new logging file and if failed,
	if (AllocateLogFileEntry() == FALSE) {
		// clear the name of logging data file.
		m_szLogFileName[0]	= _T('\0');
		// do not do following procedures.
		return;
	}

	// generate logging file name and create the file. if failed, 
	if ((fd = _tfopen(GetLogFileNameXp(currentTime), _T("w+b"))) == NULL) {
		// clear the name of logging data file. 
		m_szLogFileName[0]	= _T('\0');
		// do not do following procedures. 
		return;
	}

	// write the header of the logging file. 
	fwrite(&m_stFileHead, sizeof(m_stFileHead), 1, fd);

	// close the file. 
	fclose(fd);

	// calc start index
	idx -= num;
	if( idx < 0 ) idx += OLD_SMPL_NUM;

	// write old data
	for( i = 0; i < num; i++ )
	{
		index = (idx + i) % OLD_SMPL_NUM;
		WriteData(&pOld[index]);
	}
}

//=============================================================================
// Name		: WriteData
// Argument	: data ... log data address.
// Return	: none.
// Comment	: write the sampled data at the tail of the logging file.
// JP		: データをログに書き込む
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
void CLogging::WriteData(SampledData *data)
{
	int	fd;

	// test name of the logging data file. do nothing if invalid.
	if (!*m_szLogFileName) return;

	// open the logging data file. do nothing if failed.
	if ((fd = _lopen(m_szLogFileName, OF_READWRITE)) < 0) return;

	// seek to the tail of the file.
	_llseek(fd, 0L, 2);

	// write the logging data into the logging data file.
	_lwrite(fd, (LPCSTR)data, sizeof(*data));

	// close the logging data file.
	_lclose(fd);
}

//=============================================================================
// Name		: AllocateLogFileEntry(private)
// Argument	: none.
// Return	: true or false.
// Comment	: allocate disk space for a new logging file.
// JP		: 所定フォルダのログファイルリストを生成。
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
BOOL CLogging::AllocateLogFileEntry(void)
{
	static _TCHAR	path[256];
	TrendDirectory	*tp;
	int				numEntry;
	int				i, cnt;

	// get the entries of the existing logging data files.
	numEntry = lg.GetTrendDirectory(m_stTrendDir);

	// if the number of the entries does not exceed the maximum,
	if (numEntry < NumTrendFile) {
		return TRUE;
	}

	cnt = numEntry - NumTrendFile + 1;
	for(i = 0; cnt > 0; cnt--)
	{

		// otherwise, search the oldest entry which does not have a remark.
		for (tp = &m_stTrendDir[i]; i < numEntry; i++, tp++) {
			if (!(tp->flags & _A_RDONLY))
				break;
		}
		// return ERROR if not found.
		if (i == numEntry) return FALSE;
		i++;

		// remove the oldest entry.
		_tcscat( _tcscat( _tcscpy( path, lg.LogRootDir ), _T("\\") ), tp->name );
		_tremove(path);

	}

	// after above procedure, return OK.
	return TRUE;
}

//=============================================================================
// Name		: GetLogFileName(private)
// Argument	: time ... current time.
// Return	: log file name address.
// Comment	: generate logging file name.
// JP		: 現在の時間からファイル名を生成する。
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
LPTSTR CLogging::GetLogFileName(time_t time)
{
	static _TCHAR	buf[256];
	_TCHAR	*p, *q;
	int	i;

	// generate a decimal character string of the time.
	_stprintf(buf, _T("%011ld"), time);

	// make the directory path.
	p		= _tcscpy(m_szLogFileName, lg.LogRootDir);
	p		+= _tcslen(p);
	*p++	= '\\';

	// append the file name.
	for (i = 0, q = buf; i < 8; i++)
		*p++	= *q++;
	*p++	= '.';
	for (i = 0; i < 3; i++)
		*p++	= *q++;
	*p	= '\0';

	// return the head of the generated path.
	return	m_szLogFileName;
}

//=============================================================================
// Name		: GetLogFileName(private)
// Argument	: time ... current time.
// Return	: log file name address.
// Comment	: generate logging file name.
// JP		: 現在の時間からファイル名を生成する。
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
LPTSTR CLogging::GetLogFileNameXp(time_t time)
{
	CString		strName;
	struct	tm	*lctm;

	// current time message
	lctm = localtime(&time);
	strName.Format(_T("\\%04d%02d%02d%02d%02d%02d.log"),
		lctm->tm_year+1900, lctm->tm_mon+1, lctm->tm_mday,
		lctm->tm_hour, lctm->tm_min, lctm->tm_sec);

	// make path
	_tcscat( _tcscpy(m_szLogFileName, lg.LogRootDir), strName );

	// return the head of the generated path.
	return m_szLogFileName;
}

//=============================================================================
// Name		: SaveHeatInfo
// Argument	: nPtnNo ... パターンNo
// Return	: none.
// Comment	: ログファイルにヒート情報を書き込む
//=============================================================================
void CLogging::SaveHeatInfo(int nPtnNo)
{
	int	fd;

	// test name of the logging data file. do nothing if invalid.
	if (!*m_szLogFileName) return;

	// open the logging data file. do nothing if failed.
	if ((fd = _lopen(m_szLogFileName, OF_READWRITE)) < 0) return;

	// seek to the tail of the file.
	_llseek(fd, 0L, 0);

	// write the header of the logging file. 
	m_stFileHead.header.nPtnNo = nPtnNo;
	_lwrite(fd, (LPCSTR)&m_stFileHead, sizeof(m_stFileHead));

	// close the logging data file.
	_lclose(fd);
}

//=============================================================================
// Name		: SetupDataHeader
// Argument	: pHeader ... log header info address.
// Return	: none.
// Comment	: setup the logging data file header.
// JP		: ログヘッダ情報を設定する。（入力情報のみ）
//			: 条件：アプリケーション処理から呼び出す。
//=============================================================================
void CLogging::SaveInputInfo(DataFileHeader *pHeader)
{
	int	fd;

	// test name of the logging data file. do nothing if invalid.
	if (!*m_szLogFileName) return;

	// open the logging data file. do nothing if failed.
	if ((fd = _lopen(m_szLogFileName, OF_READWRITE)) < 0) return;

	// seek to the tail of the file.
	_llseek(fd, 0L, 0);

	// write the header of the logging file. 
	memcpy(m_stFileHead.header.arg, pHeader->header.arg, sizeof(m_stFileHead.header.arg));
	memcpy(m_stFileHead.header.adCh, pHeader->header.adCh, sizeof(m_stFileHead.header.adCh));
	_lwrite(fd, (LPCSTR)&m_stFileHead, sizeof(m_stFileHead));

	// close the logging data file.
	_lclose(fd);
}

