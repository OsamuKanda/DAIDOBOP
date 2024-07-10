#ifndef		bodef_h
# define	bodef_h

/*------------------------------------------------------------------------------------------------*/
// <JP>:BOシステムバージョン
// B.O. System special Version
/*------------------------------------------------------------------------------------------------*/
// commonly defined items for Break out prediction system all apprications must use this file.
//#define	BOSystemVer		600		// B.O. prediction system version 6.00
//#define	BOSystemVer		620		// B.O. prediction system version 6.20 (Gsteel2008 Ver)
//#define	BOSystemVer		700		// B.O. prediction system version 7.00 (Topy2010 Ver)
//#define	BOSystemVer		800		// B.O. prediction system version 8.00 (Daido2011 Ver)
#define	BOSystemVer		801		// B.O. prediction system version 8.01 (Daido201104 Ver)
// 下位互換フラグ
#define	Compatibility


/*------------------------------------------------------------------------------------------------*/
// <JP>:動作モード
/*------------------------------------------------------------------------------------------------*/
#ifndef _DEBUG
//#define		OFFLINE					// Offlineモードの時は生かす（ADなどを無効にする）
#else
#define		OFFLINE					// デバッグモード時は強制OFFLINE
#endif
#define		BATCH_OUT				// バッチシムの計算結果出力機能を有効にするときは生かす
#ifndef OFFLINE
#define		USE_PLC
#else
#define		USE_PLC
#endif
/*------------------------------------------------------------------------------------------------*/
// <JP>:言語タイプ
// The linguistic type
/*------------------------------------------------------------------------------------------------*/
#define		ENGLISH


/*------------------------------------------------------------------------------------------------*/
// <JP>:テスト定義
// The test definition
/*------------------------------------------------------------------------------------------------*/
// debug real tiemr
#ifdef _DEBUG
#define	USE_DEBUG_TIMER
#endif

/*------------------------------------------------------------------------------------------------*/
// <JP>:ヘッダファイル定義
// The Hedda file definition
/*------------------------------------------------------------------------------------------------*/
#include	<windows.h>
#include	<direct.h>
#include	<TCHAR.H>
#include	<io.h>
#include	<sys/stat.h>


/*------------------------------------------------------------------------------------------------*/
// <JP>:ファイルパスの定義.
// The file path definition
/*------------------------------------------------------------------------------------------------*/
#define	WorkDir				_T("bowork")			/* root working directory	*/

#define	WorkDirFileName		_T("\\work.dir")		/* configrated work name	*/
//#define	OldWorkDirFileName	_T("\\oldwork.dir")		/* configrated oldwork name.*/
#define	ADConfigFileName	_T("\\adassign.cnf")	/* A/D channel assignment	*/
#define	CombiFileName		_T("\\combi.def")		/* definition of combination*/
#define	NNTemplateFileName	_T("\\template.def")	/* Nueral network structure	*/
#define	NNTemplateFileName2	_T("\\template2.def")	/* Nueral network structure2*/
#define	PatNNWeightFile		_T("\\patnn%d.wei")		/* weights of patten N.N.	*/
#define	PatNNWeightBackup	_T("\\patnn%d.bak")		/* backup of pattern N.N.	*/
#define	PatNNWeightFileTmp	_T("\\patnn%d.tmp")		/* weights of patten N.N. for tempo	*/
#define	CorrNNWeightFile	_T("\\corrnn%d.wei")	/* weights of correlationNN.*/
#define	CorrNNWeightBackup	_T("\\corrnn%d.bak")	/* backup of correlation NN.*/
#define	CorrNNWeightFileTmp	_T("\\corrnn%d.tmp")	/* weights of correlationNN. for tempo	*/
#define	TrendChartPenFName	_T("\\trendpen.def")	/* trend chart pens			*/
#define	TrendAxisFileName	_T("\\trndaxis.def")	/* trend chart axis			*/
#define	BPOrdinaryDataFile	_T("\\ordin%d.dat")		/* ordinary data for B.P.	*/
#define	BPAlarmDataFile		_T("\\alarm%d.dat")		/* alarm data for B.P.		*/
#define	BOSimulateFile		_T("\\simu.out")		/* simulate output of N.N.	*/
#define	BOExecConfFile		_T("\\boexec.def")		/* boexec config 			*/
#define	SteelKindFileName	_T("\\steel.def")		/* steel kind assignment	*/
#define	MoldWidthFile		_T("\\mold.def")		/* Mold width				*/
#define	SamplingFile		_T("\\sample.def")		/* sampling period			*/
#define	BoSystemFile		_T("\\bosys%d.def")		/* Bo system data			*/
#define	BoSysCondFile		_T("\\bosysc.def")		/* Bo system condition		*/
#define	BoSysNetFile		_T("\\bonet.def")		/* Bo network condition		*/
#define	BoSysStFile			_T("\\strand.def")		/* Bo strand condition		*/
#define	TpPosFile			_T("\\TpPos.dat")		/* Tp Pos data				*/
#define	LogRootDirBack		_T("\\bolog.bak")		/* root logged data backup dir.	*/
#define	LogBackUpNameWork	_T("\\backname")		/* logged data backup file name	*/
#define	TCExceptFileName	_T("\\tcexp.dat")		/* TC exception info.		*/
#define	TCExceptSetFileName	_T("\\tcexp.txt")		/* TC exception setting info.*/
#define	TCAxis2FileName		_T("\\tcaxis2.cnf")		/* TC graph axis info.		*/
//#define	CastCondFile		_T("\\castcond.dat")	/* casting condition		*/
//#define	CastInfoFile		_T("\\castinfo.dat")	/* casting information		*/
//#define	Chart3DName			_T("\\Chart3D.exe")		/* chart 3D name			*/
#define	BOViewConfFile		_T("\\boview.def")		/* boview config 			*/
#define	BOLogTempFolder		_T("\\..\\Temp")		/* boview config 			*/
#define	ADConfigFileNameBk	_T("\\adassign.bak")	/* A/D channel assignment for backup	*/
#define	CombiFileNameBk		_T("\\combi.bak")		/* definition of combination for backup */
#define	STFLogoFileName		_T("\\STF_logo.jpg")	/* stf logo file			*/	
#define	TCNameFileName		_T("\\tcname.dat")		/* tc name					*/

/*------------------------------------------------------------------------------------------------*/
// <JP>:ファイル検索の定義.
// The file find definition
/*------------------------------------------------------------------------------------------------*/
#define	FindBoSystemFile	_T("\\bosys")			/* Bo system data(for find)	*/
#define	FindSteelKindFile	_T("\\steel.def")		/* steel kind assignment(for find)	*/
#define	FindTpPosFile		_T("\\TpPos.dat")		/* Tp Pos data(for find)	*/


/*------------------------------------------------------------------------------------------------*/
// <JP>:過去データ保存定義.（鋳込開始時に最大２分さかのぼってデータ保存する機能）
// old data save definition.(old sample save function while two minutes)
/*------------------------------------------------------------------------------------------------*/
#define	OLD_SMPL_NUM	(240)	// 2 minutes data(2*60*2)


/*------------------------------------------------------------------------------------------------*/
// <JP>:コンフィグの基本定義.
// The Configuration basic definition
/*------------------------------------------------------------------------------------------------*/
#define	SizeConfig	256							// The size of the configuration file name.
#define	NumConfig	256							// The number of the maximum configuration files.


/*------------------------------------------------------------------------------------------------*/
// <JP>:A/Dチャンネルの基本定義.
// The A/D channel basic definition
/*------------------------------------------------------------------------------------------------*/
#define	NumADChannelV5	 64				/* the max. number of input channels */
#define	NumADChannel	 120			/* the max. number of input channels */
#define	NumADChannelDisp 64				/* the max. number of input channels for display */
#define	ADMaximumInput	(32768UL)		/* the max. value of A/D converter	*/

typedef	struct {
	int	arg;					/* connected argument						*/
	double	min, max;			/* range of phisical signal					*/
	_TCHAR	unit[32];			/* unit of phisical signal					*/
} ADChannel;


/*------------------------------------------------------------------------------------------------*/
// <JP>:A/D変換の定義.
// The A/D chagne definition
/*------------------------------------------------------------------------------------------------*/
/* translation table from A/D input value to phisical value.
	The translation formula is given as follows.

	phisical value = ((A/D input value) * scale + offset) / ADTransRange
*/
#define	ADTransRange		(65536L)	/* coefficient of phisical division	*/
#define AdTemp_Max			(13500)
#define AdTemp_Min			(-16500)
#define	AdTemp_Scale		(AdTemp_Max-AdTemp_Min)
#define	AdTemp_Offset		AdTemp_Min

#define	PhisicalTempUnit	(0.1)		/* phisical unit of temperature log	*/
#define	PhisicalLevelUnit	(0.1)		/* phisical unit of level log		*/
#define	PhisicalVcUnit		(0.01)		/* phisical unit of Vc log			*/
#define	PhisicalSNUnit		(1.0)		/* phisical unit of SN log			*/
#define	PhisicalWidthUnit	(1.0)		/* phisical unit of mold width log	*/
#define	PhisicalLengthUnit	(0.01)		/* phisical unit of Cast Length log	*/

typedef struct {
	short		isValid;				/* is assigned for input argument.			*/
	long		scale;					/* scalling coefficiency.					*/
	long		offset;					/* offset									*/
	int			arg;
} ADTranslation;

/*------------------------------------------------------------------------------------------------*/
// <JP>:タップの基本定義.
// The input arguments basic definition
/*------------------------------------------------------------------------------------------------*/
#define	NumInput     	128	 				// the max. number of input arguments
#define	NumTCTap		28					// the number of thermo-couple taps
//#define	NumTCTap		16					// the number of thermo-couple taps
#define TapRaw			3
#define	NumLine			6
//#define	NumFixTc		6
//#define	NumLooseTc		4
//#define	NumWestTc		3
//#define	NumEastTc		3
//#define	NumOtherTap		5
#define	NumFixTc		8
#define	NumLooseTc		8
#define	NumWestTc		6
#define	NumEastTc		6
#define	NumOtherTap		9

#define	TCTapID			0					// ID. of the first thermo-couple tap
#define	LevelID			TCTapID + NumTCTap	// ID. of meniscus level
#define	VcID			LevelID + 1			// ID. of casting speed
#define	SNID			VcID + 1			// ID. of SN
#define	LengthID		SNID + 1			// ID. of Casting Length
#define	LastID			LengthID + 1		// ID. of Last
// 追加項目(保存のみ)
#define	TdWeiID			LastID				// ID. of Td Weight
#define	MdWtAID			TdWeiID + 1			// ID. of M/D water A Side
#define	MdWtCID			MdWtAID + 1			// ID. of M/D water C Side
#define	MdWtDID			MdWtCID + 1			// ID. of M/D water D Side
#define	MdTmpDID		MdWtDID + 1			// ID. of M/D water temp. D Side

#define	NonSide			0
#define	FixSide			1					// mold side attribute (fix-side)
#define	LooseSide		2					// mold side attribute (loose-side)
#define NarrowWest		3					// mold side attribute (Narrow-West)
#define	NarrowEast		4					// mold side attribute (Narrow-East)

typedef struct {
	int	side;								// mold side					
//	int	x, y;								// geometry on the mold surface
	int	adCh;								// ID. of assigned A/D channel
	int	gx, gy;								// displaying position
//	int	no;									// assign data no.
} InputPoint;


/*------------------------------------------------------------------------------------------------*/
// <JP>:ネットワークカップルの基本定義.
// The thermo-couple basic definition
/*------------------------------------------------------------------------------------------------*/
#define	NumCombination	256		// the maximum number of combination.

typedef	struct {
	int	upper;					// TC ID. of upper position.
	int	lower;					// TC ID. of lower position.
	int	netID;					// neural network ID.
} TCCombination;


/*------------------------------------------------------------------------------------------------*/
// <JP>:ニューラルネットワークの基本定義.
// The neural network basic definition
/*------------------------------------------------------------------------------------------------*/
#define	NumLayer		8			// the maximum number of network layers
#define	NumElement		90			// the maximum number of network element of each layers
#define NumBoOccur		11			// The BO occurrence recognition number of the maximums

typedef	struct {
	int	numLayer;					// the number of layers	
	int	numElm[NumLayer];			// the number of elements in each layer
} NNTemplate;

#define	NumNeuralNet	16			// the muximum number of network kinds

typedef	struct {
	_TCHAR	remark[80];				// remark for neural network
	NNTemplate	patternNN;			// template for wave pattern neural network
	NNTemplate	corrNN;				// template for correlation N.N.
	//@ 
	int		boWatchTime;				// B.O watch time
	int		numRegister;				// Number of Registration
	float	boAlarm[NumBoOccur];		// Threshold
	int		boTimes[NumBoOccur];		// Number of Times
//	int		numCombiBA;					// Number of combinations for BO Alarm.
//	int		numCombiBAC;				// Number of combinations for BO Alarm Control.
} NeuralNetConf;

typedef struct {
	float	val[NumCombination];	//result data of neural network execution
} ResultData;


/*------------------------------------------------------------------------------------------------*/
// <JP>:パスワードの基本定義.
// The password basic definition
/*------------------------------------------------------------------------------------------------*/
#define	PASS_VALID_SEC	300


/*------------------------------------------------------------------------------------------------*/
// <JP>:収集データファイル構造の定義.
// The definition of logged data file structure
/*------------------------------------------------------------------------------------------------
	Logged data files are named in MSDOS "xxxxxxxx.yyy" format.
	The decimal value "xxxxxxxxyyy" is the time from Jan. 1, 1970 0:00:00
	to start time in sec.

	A logged data file consists of two part, data header and data body.
	The size of data header is round by 1024 bytes.
	Each data per a sample in data body is round by 128 bytes.

	Total time of logged data in the file is calculated by following formula.
		(fileSize - 1024) / 128 * SamplingPeriod

		1.data header
	1)The entry `remark' is used for marking by user.
	2)The entry `adCh' is used for translation from input argument to
	  A/D channel number.
	  Each element in the table is A/D channel number (0 to NumADChannel-1)
	  or negative if the argument is not specified.
	3)The entry `arg' is used for translation from A/D channel number to
	  input argument.
	  Each element in the table is identifier of input argument
	  or negative value if the channel is not connected.

		2.data body
	each datum in data body is scaled to phisical values.
	1)temperature			unit value is 0.1Celcius
	2)meniscus level		unit value is 0.1mm
	3)casting speed			unit value is 0.01m/min

*/

typedef	struct {
	_TCHAR	remark[128];		/* remark of data							*/
	short		version;			/* version of system used in sampling		*/
	short		samplePeriod;		/* sample period in milli-second			*/
	short		adCh[NumInput];		/* translation from input arg to AD channel	*/
	short		arg[NumADChannel];	/* translation from AD channel to input arg	*/
	short		nPtnNo;				/* B.O. Pattern No. */
//	short		moldWidth;			/* mold width */
} DataHeader;

typedef	union {
	DataHeader	header;			/* data header								*/
	_TCHAR	align[1024];		/* reserved space for later update			*/
} DataFileHeader;

typedef	struct {
	short		val[NumADChannelV5];/* scaled phisical values					*/
} SampledDataV5;

typedef	struct {
	short		val[NumADChannel];	/* scaled phisical values					*/
} SampledData;

typedef struct {
	short	nPtnNo;					// BOP Pattern No.
	BYTE	cGradeCode[21];			// 溶解鋼種名
	BYTE	cHeatNo[9];				// 溶番
	BYTE	cStandardNo[7];			// 標準No
	short	nMdLevel;				// Mold Level
	short	nCastSpeed;				// Casting Speed.
	short	nSnOpen;				// SN Opening.
	//short	nMdWidth;				// Mold Width.
	//short	nMdThick;				// Mold Thickness
	int		nCastLengthS;			// Casting Length for series.
	short	nCastLengthC;			// Casting Length for heat
	short	nTdWeidht;				// Tundish Weight
	short	nMdCoolWtA;				// Mold Cooling Water Flow A
	short	nMdCoolWtC;				// Mold Cooling Water Flow C
	short	nMdCoolWtD;				// Mold Cooling Water Flow D
	short	nMdCoolWtTempD;			// Mold Cooling Water inlet Temp D.
	short	isPrOn;					// P/R on signal.
	short	isCharge;				// Charge Change signal.
	short	isAutoLvCtrl;			// Auto Level Control signal.
	short	isHeatDataChg;			// 鋳造開始.
	short	isBoRst;				// B.O. reset signal.
	short	isLdOn;					// LD開.
	short	isL1Err;				// 上位通信異常.
	short	nTemp[NumTCTap];		// 熱電対温度
	short	nPlcErr1;				// PLC異常(1).
	short	nPlcErr2;				// PLC異常(2).
} ViewData;
ViewData *GetSharedMemory2(void);
void ViewLock(void);
void ViewUnlock(void);

typedef struct {
	FILETIME		ft;
	SampledData		sd;
	SampledData		sd_raw;
	ViewData		vd;	
	BOOL			isResultDat[NumCombination];
	BOOL			isResultDatD[NumCombination];
	int				isAllResult;
	int				numCombination;					// the number of combinations
	TCCombination	tcCombination[NumCombination];	// T.C. combinations
	InputPoint		inputPoint[NumInput];			// input point
	ADChannel		adChannel[NumADChannel];		// ad channel
	BOOL			isChgHeat;						// change heat
} StSampledData;

/*------------------------------------------------------------------------------------------------*/
// <JP>:収集データファイルディレクトリの定義.
// The definition of logged data file directory
/*------------------------------------------------------------------------------------------------*/
#define	NumTrendFile	4096			/* the maximum number of file entries	*/
#define	CHK_FREE_SPACE	5120			/* Tempファイル削除判定用空き容量(MB)	*/

typedef	struct {
	_TCHAR	name[24];					/* file name							*/
	_TCHAR	date[24];					/* start date							*/
	time_t	startTime;					/* start time in sec from Jan.1,'70 0:0	*/
	UINT	flags;						/* file flag read only, sub dir, etc	*/
	DWORD	totalTick;					/* total tick in data body				*/
	DWORD	smplTime;					/* sampling time						*/
} TrendDirectory;


/*------------------------------------------------------------------------------------------------*/
// The definition of directory list
/*------------------------------------------------------------------------------------------------*/
typedef int (*COMP)(const void *, const void *);


/*------------------------------------------------------------------------------------------------*/
// <JP>：トレンドの基本定義.
// The Chart Axis basic definition
/*------------------------------------------------------------------------------------------------*/
// trend channel number.
#define NumTrendChannel		16

// structure definition of trend graph axis.
typedef struct {
	double	scale;
	double	zero;
} ChartAxis;


/*------------------------------------------------------------------------------------------------*/
// <JP>:鋼種の基本定義.
// The steel kinds basic definition
/*------------------------------------------------------------------------------------------------*/
#define	NumSteelKind	99				/* the max. number of Steel Kind */

//typedef	struct {
//	_TCHAR	confName[256];				/* Configuration file name			*/
//} SteelKind;


/*------------------------------------------------------------------------------------------------*/
// <JP>:BO予知の基本定義.
// The BO prediction basic definition
/*------------------------------------------------------------------------------------------------*/
#define	MaxWatchCount	200  
//#define NumPowder		20				// The powder recognition number of the maximums
//#define	NumPowderChar	20				// The powder character number of the maximums

typedef	struct 
{
	_TCHAR	szComment[32];				// displayed comment
	int		numCombiBA;					// Number of combinations for BO Alarm.
	int		numCombiBAC;				// Number of combinations for BO Alarm Control.
	//int		numRegPow;					// Number of Registration of the Powder
	//_TCHAR	szPowder[NumPowder][32];	// displayed the Powder
	_TCHAR	szConfName[SizeConfig];		// The config name to relate to the steel group for BO.
	_TCHAR	szConfNameD[SizeConfig];	// The config name to relate to the steel group for DEP.
} BoSysInfo;

// BO condition parameter
typedef	struct 
{
	float	fCondLen;					// cast length of BOP execute condition.
	float	fCondVc;					// cast speed of BOP execute condition.
//	float	fFlowFacter;				// flow rate facter
//	int		nAlarmTimer;				// alarm watch timer
} BoCondInfo;

// BO network parameter
typedef	struct 
{
	DWORD	dwST;						// strand no.
	DWORD	dwIP;						// plc IP address.
	DWORD	dwPortC;					// plc port for CMD.
	DWORD	dwPortD;					// plc port for DATA.
	DWORD	dwAddrR;					// plc regno for Read.
	DWORD	dwAddrW;					// plc regno for Write.
} BoNetInfo;

/*------------------------------------------------------------------------------------------------*/
// <JP>:トレンド波形の表示/非表示機能の基本定義.
// The trend wave display / non-display basic definition
/*------------------------------------------------------------------------------------------------*/
typedef	struct {
	int		arg;						/* connected argument				*/
	BOOL	fIndication;				/* indication flag					*/
} TrendSelect;


/*------------------------------------------------------------------------------------------------*/
// <JP>:共有メモリ定義.
// The shared memory definition
/*------------------------------------------------------------------------------------------------*/
typedef struct {
	int		isStrobeBO;				/* [in]does strobe B.O. signal or not			*/
	int		isEnabLevelCtl;			/* [in]does detect while auto level ctl			*/
	int		isEnabSimSignal;		/* [in]is enable simulation signal or not		*/
	int		simVcOn;				/* [in]simulation status of 'Vc On'				*/
	int		simChargeChange;		/* [in]simulation status of 'Charge Change'		*/
	int		simAutoLVCtl;			/* [in]simu. status of 'Auto Level Control'		*/
	int		simCastStart;			/* [in]simu. status of 'CastStart'				*/
	int		simL1Err;				/* [in]simu. status of 'L1Error'				*/
	int		tstVcOn;				/* [out]test status of 'Vc On'					*/
	int		tstChargeChange;		/* [out]test status of 'Charge Change'			*/
	int		tstAutoLVCtl;			/* [out]test status of 'Auto Level Control'		*/
	int		tstCastStart;			/* [out]test status of 'Cast Start'				*/
	int		tstL1Err;				/* [out]test status of 'L1 Err'					*/
	int		isVcOn;					/* [out]signal status of 'Vc On'				*/
	int		isChargeChange;			/* [out]signal status of 'Charge Change'		*/
	int		isAutoLVCtl;			/* [out]signal status of 'Auto Level Ctl'		*/
	int		isCastStart;			/* [out]signal status of 'CastStart'			*/
	int		isL1Err;				/* [out]signal status of 'L1Error'				*/
	int		isBOFetched;			/* [out]B.O. detection status					*/
	int		outBOFetched;			/* [in]signal status of 'BO Fetched'			*/
	int		inAlarmReset;			/* [in]input status of 'Alarm Reset'			*/
	int		outAlarmReset;			/* [in]output status of 'Alarm Reset'			*/
	UINT   	filterSwitch;			/* [in]A/D Bord filter switch					*/
	float	filterFacter;			/* [in]A/D Bord filter facter					*/
	float	filterFacterSn;			/* [in]A/D Bord filter facter S/N				*/
	float	filterFacterLv;			/* [in]A/D Bord filter facter Level				*/
	float	filterFacterPr;			/* [in]A/D Bord filter facter P/R  Speed		*/
	int		moldSize;				/* [out]Mold Size								*/
	BOOL	BopCondition;			/* [out]BOP Condition							*/
	int		simBocOut;				/* [out]警報出力の疑似信号						*/
	int		simBoOut;				/* [out]注意報出力の疑似信号					*/
	int		simBoRun;				/* [out]BO稼働中の疑似信号						*/
	int		outPlcErrReset;			/* [out]output status of 'Plc Error Reset'		*/
	int		inPlcErr1;				/* [in]signal status of 'Plc Error(1)'			*/
	int		inPlcErr2;				/* [in]signal status of 'Plc Error(2)'			*/
} SharedMemory;
SharedMemory *GetSharedMemory(void);

/*------------------------------------------------------------------------------------------------*/
// <JP>:ログ関連
// The log relation.
/*------------------------------------------------------------------------------------------------*/
#define		LogBufferSize   512             // the size of logging data buffer
#define		LogDataPresent  1				// status: the logging data present
#define		LogDataAbsent   2				// status: the logging data absent
#define		ChangeLog       3				// status: the file changed
#define		ChargeChange    4				// status: the charge changed

/*------------------------------------------------------------------------------------------------*/
// <JP>:サンプリング周期定義
// The sampling period definition
/*------------------------------------------------------------------------------------------------*/
#define		SamplingPeriod	500				// The sampling period.

/*------------------------------------------------------------------------------------------------*/
// <JP>:TC除外設定定義
// The sampling period definition
/*------------------------------------------------------------------------------------------------*/
typedef struct
{
	double	dTempMin;
	double	dTempMax;
	int		nWatchTime;
	double	dDeltaT;
	int		nCheckTime;
}StTcExpSet;
#define TCEXP_CHK_TIME_MAX	21

/*------------------------------------------------------------------------------------------------*/
// <JP>:TC除外設定定義
// The sampling period definition
/*------------------------------------------------------------------------------------------------*/
// 学習結果リスト情報
typedef	struct {
	_TCHAR	path[260];				/* file name							*/
	time_t	updateTime;				/* update time in sec from Jan.1,'70 0:0*/
} WeiInfoList;
#define	WEI_BAK_CNT		10


/*------------------------------------------------------------------------------------------------*/
// <JP>:グラフ背景色
// The graph back color definition
/*------------------------------------------------------------------------------------------------*/
#define	COL_GRAPH_BACK	RGB(230,230,230)


/*------------------------------------------------------------------------------------------------*/
// <JP>:TC名定義
// The tc name definition
/*------------------------------------------------------------------------------------------------*/
// tc name information
typedef	struct {
	_TCHAR	name[NumTCTap][10];				/* tc name							*/
} TcName;
#define	TCNM_MAX_LEN	4					/* tc name max length				*/

#endif	/* bodef_h	*/
