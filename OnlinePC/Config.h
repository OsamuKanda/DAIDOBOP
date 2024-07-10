// ConfigIf.h : Configuration Interface
//
#pragma once

// include
#include "../include/Bodef.h"
#include "../include/Neuralne.h"

// CConfigIf class
class CConfig
{
public:
	CConfig(){};
	~CConfig(){};

// member data
public:
	_TCHAR	RootDir[MAX_PATH];
	_TCHAR	WorkRootDir[MAX_PATH];
	_TCHAR	TrendAxisFile[MAX_PATH];
	_TCHAR	SteelKindFile[MAX_PATH];
	_TCHAR	TrendChartPenFile[MAX_PATH];

	// The A/D Channel definition.
	InputPoint	inputPoint[NumInput];				// input arguments
	ADChannel	adChannel[NumADChannel];			// A/D channels
	BOOL		bTcExp[NumInput];					// exception TC

	// tc position data
	int	nTpPosData[(NumTCTap / TapRaw)];			// number of tc taps

	// The TC Combination definition.
	TCCombination	tcCombination[NumCombination];	// T.C. combinations
	int				numCombination;					// the number of combinations

	// The neural network definition.
	NeuralNetConf	neuralnetConf[NumNeuralNet];	// neural network set

	// The steel kinds definition.
//	SteelKind		steel[NumSteelKind];			// Steel Kinds

	// The configuration file relation.
	_TCHAR	workDir[SizeConfig];					// The active configuration file name.
	_TCHAR	oldWorkDir[SizeConfig];					// The old configuration file name.

	// Axis data
	ChartAxis	m_stTimeAxis;	
	ChartAxis	m_stTempAxis;	
	ChartAxis	m_stLevelAxis;
	ChartAxis	m_stVcAxis;	
	ChartAxis	m_stSNAxis;
//	ChartAxis	m_stWidthAxis;

	// trend
	int			m_nTrendArg[NumTrendChannel];
	int			m_isEnable[NumTrendChannel];

	// tc except setting
	StTcExpSet	m_stTcExpSet;

	// translation from A/D input value to the phisical value.
	ADTranslation	m_adTranslation[NumADChannel];

	// The neural network definition.
	NeuralNetConf	m_neuralnetConf[NumNeuralNet];		// neural network set
	NeuralNet		*m_patternNeuralNet[NumNeuralNet];	// pattern neural network
	NeuralNet		*m_corrNeuralNet[NumNeuralNet];		// correlation N.N.

	DataFileHeader  m_logDtFileHeader;					// file header of logging file.

// member function
public:
	BOOL Initialize(void);
	BOOL Uninitialize(void);
	BOOL DirPickUp(void);
	BOOL LoadConfiguration(LPCTSTR dir);
	BOOL SaveConfiguration(void);
	int FileCopy(LPTSTR d, LPTSTR s);
	void cpFileCopy(void);
	void FileMove(void);
	int	GetSubDirList(LPTSTR home, _TCHAR table[][SizeConfig], int max);
	BOOL DeleteConfigFile(LPSTR name);
	BOOL DeleteCheck(LPSTR szbuf);
//	int	GetParamNameList(LPTSTR home, _TCHAR table[][SizeConfig], int max);
	int GetConfigDir(LPTSTR pszDir, _TCHAR table[][SizeConfig], int max);
	DWORD CopyFile2(LPCTSTR pDestFilePath, LPCTSTR pSrcFilePath);
	DWORD DeleteFile2(LPCTSTR pFilePath);
	BOOL BackupADAndCombi(void);

	// workdir.cpp------------------
	BOOL LoadWorkDirName(void);
	BOOL SaveWorkDirName(void);
	LPTSTR GetWorkDirName(void);
	void SetWorkDirName(LPTSTR name);
	BOOL LoadOldWorkDirName(void);
	BOOL SaveOldWorkDirName(void);
	LPTSTR GetOldWorkDirName(void);
	void SetOldWorkDirName(LPTSTR name);
	void CopyWorkDirName(void);

	// tpconfig.cpp------------------
	//void SetupTPPos(void);
	//BOOL ReadTPPosFile(void);
	//BOOL WriteTPPosFile(void);

	// adconfig.cpp------------------
	void SetupADChannels(void);
	void SetupInputPoints(void);
	void ReadADConfigFile(LPCTSTR dir);
	BOOL ReadADConfigFileBk(LPCTSTR dir);
	BOOL WriteADConfigFile(LPCTSTR dir);
	BOOL WriteADConfigFileBk(LPCTSTR dir);
	void SetupADTranslation(void);

	// nnconfig.cpp------------------
	void SetupCombination(void);
	void ReadCombination(LPCTSTR dir);
	BOOL ReadCombinationBk(LPCTSTR dir);
	BOOL WriteCombination(LPCTSTR dir);
	BOOL WriteCombinationBk(LPCTSTR dir);
	void SetupNNTemplate(void);
	void ReadNNTemplate(LPCTSTR dir);
	void ReadNNTemplate2(LPCTSTR dir);
	BOOL WriteNNTemplate(LPCTSTR dir);
	BOOL WriteNNTemplate2(LPCTSTR dir);

	// skconfig.cpp------------------
	//void SetupSteelKind(void);
	//void ReadSteelConfigFile(void);
	//BOOL WriteSteelConfigFile(void);

	// axisconfig.cpp------------------
	void ReadAxisFile(void);
	BOOL WriteAxisFile(void);

	// penconfig.cpp------------------
	void ReadTrendGraphPen(void);
	BOOL WriteTrendGraphPen(void);

	// bsconfig.cpp------------------
	BOOL ReadBosysInfo(int sel, BoSysInfo *pInfo);
	BOOL WriteBosysInfo(int sel, BoSysInfo *pInfo);
	BOOL ReadBosysCondition(BoCondInfo *pInfo);
	BOOL WriteBosysCondition(BoCondInfo *pInfo);
	BOOL ReadBosysNetwork(BoNetInfo *pInfo);
	BOOL WriteBosysNetwork(BoNetInfo *pInfo);

	// smplconf.cpp------------------
	BOOL ReadSamplingPeriod(int sel, int *pSamplePeriod);
	int	WriteSamplingPeriod(int sel, int *pSamplePeriod);

	// weiconf.cpp--------------------
	void ReadBONeuralNet(LPCTSTR dir, LPTSTR key, NeuralNet **netTab);
	void ResetBONeuralNet(NeuralNet **netTab);
	void MakeupBONNSearchPath(LPTSTR path, LPCTSTR dir, LPTSTR key);
	int	 GetWeightIdx(LPCTSTR name);
	int	 GetWeiListP(LPCTSTR lpszDir, int nNetIdx, WeiInfoList *table);
	int	 GetWeiListC(LPCTSTR lpszDir, int nNetIdx, WeiInfoList *table);
	static int CmpTrendEnt(WeiInfoList *p, WeiInfoList *q);

	// tcexpcnf.cpp--------------------
//	void SetupTcExp(void);
	void ReadTcExpFile(int *pbTcExp);
	void WriteTcExpFile(int *pbTcExp);
	BOOL ReadTcExpSetFile(StTcExpSet *pInfo);
	BOOL WriteTcExpSetFile(StTcExpSet *pInfo);

	// axis2conf.cpp------------------
	void ReadAxis2File(ChartAxis *pAxis);
	BOOL WriteAxis2File(ChartAxis *pAxis);

	// LogSelDir.cpp------------------
	BOOL LoadLogSelFileName(LPCTSTR pSaveFileName, LPTSTR pLogName);
	BOOL SaveLogSelFileName(LPCTSTR pSaveFileName, LPTSTR pLogName);
};

