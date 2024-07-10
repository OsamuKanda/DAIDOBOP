#pragma once
#include    "../include/bodef.h"


// CLogging class
class CLogging
{
public:
	CLogging(); 
	~CLogging();

// member data
public:
	StSampledData	m_stSampleBuf[LogBufferSize];
	TrendDirectory  m_stTrendDir[NumTrendFile*2];
	DataFileHeader	m_stFileHead;
	_TCHAR			m_szLogFileName[256];
	BOOL			m_isLogEnable;
	BOOL			m_isNewLogRequest;
	int				m_nNumLogData;    
	int				m_nLogDataRIdx;   
	int				m_nLogDataWIdx;   

private:

// member function
public:
	void	SetupLogBuffer(void);
	void	LogStart(void);
	void	LogStop(void);
	void	AppendData(StSampledData *data, BOOL *result, BOOL *resultD, int allRes, int isNewLog);
	int		GetLogData(StSampledData *data, BOOL *result, BOOL *resultD, int *allRes);
	void	SetupDataHeader(DataFileHeader *header);
	void	CreateLogFile(void);
	void	CreateLogFile(SampledData *pOld, int num, int idx);
	void	WriteData(SampledData *data);
	BOOL	AllocateLogFileEntry(void);
	LPTSTR	GetLogFileName(time_t time);
	LPTSTR	GetLogFileNameXp(time_t time);
	void	SaveHeatInfo(int nPtnNo);
	void	SaveInputInfo(DataFileHeader *pHeader);

private:
};

CLogging *GetLoggingClass(void);
