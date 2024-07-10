// ChartLib.h : Chart Library
//
#pragma once

#include <afxwin.h>
#include "../include/Bodef.h"
#include "../include/Graphic.h"
#include "../include/DataBuf.h"

#define	AllTrendArea		(NumTrendChannel)

//#define	ChartWidth			900				/* trend graph X dot				*/
//#define	ChartHeight			450				/* trend graph Y dot				*/
#define	DisplayWidth		1280
#define	DisplayHeight		1024

#define	ChartXOffset		4				/* trend graph X offset				*/
#define	ChartYOffset		100				/* trend graph Y offset				*/

#define	ChartPlaneWidth		(ChartWidth)
#define	ChartPlaneHeight	(ChartHeight+60)

//#define ChartMeshXCnt		34		
//#define ChartMeshYCnt		20		
//#define	ChartMeshWidth		(ChartWidth/ChartMeshXCnt)		/* Mesh width		*/
//#define	ChartMeshHeight		(ChartHeight/ChartMeshYCnt)		/* Mesh height		*/
#define	ScrollPitch			(ChartMeshWidth*2)				/* scroll pitch		*/

#define NeuralAxisZero	0.0
#define NeuralAxisScale	0.1

// structure of the chart hit-test information
typedef struct
{
	int		idx;		// point index
	short	val;		// point value
}StChartHitInfo, *pStChartHitInfo;

// point structure (double type)
typedef struct
{
	double	x;
	double	y;
}DPOINT,*pDPOINT;

// structure definition of trend graph argument.
typedef	struct {
	int	yPos[NumADChannel];
} DisplayData;

//#define	DispDataBufSize	(ChartWidth * (DWORD)sizeof(DisplayData))
#define	DispDataBufSize	(DisplayWidth * (DWORD)sizeof(DisplayData))

// graph parameter
typedef struct
{
	int			numChannel;
	int			neuralChannel;
	int			samplePeriod;
	int			dispStartTime;
	int			nTrendChArg[AllTrendArea];
	int			nTrendChArgTm[NumADChannel];
	BOOL		isTrendIndicate[NumADChannel];
	BOOL		isNeuralCal;
	ChartAxis	timeAxis;
	ChartAxis	tempAxis;
	ChartAxis	levelAxis;
	ChartAxis	vcAxis;
	ChartAxis	snAxis;
	ChartAxis	widthAxis;
	ChartAxis	stAxisTm2[9];
	int			nCombiNo;
	double		dUnitTm[9];
}StGraphParam;

// chart axis parameter
typedef struct
{
	ChartAxis	timeAxis;
	ChartAxis	tempAxis;
	ChartAxis	levelAxis;
	ChartAxis	vcAxis;
	ChartAxis	snAxis;
//	ChartAxis	widthAxis;
}StChartAxisParam;

// chart data
typedef struct
{
	int		cntMax;
	POINT	*linePoint;
}ChartData;

// CChartLib class
class CChartLib
{
public:
	CChartLib();
	~CChartLib();

// member data
public:
	COLORREF		m_clClear;
	COLORREF		m_clClear2;
	COLORREF		m_clAxis;
	COLORREF		m_clText;
	COLORREF		m_clRed;
	COLORREF		m_clTones[AllTrendArea];
	COLORREF		m_clTonesTm[NumADChannel];
	StGraphParam	m_gp;
	CBitmap			m_bitmap;

	int				ChartWidth;
	int				ChartHeight;
	int				ChartMeshXCnt;
	int				ChartMeshYCnt;
	int				ChartMeshWidth;		
	int				ChartMeshHeight;		
	int				m_nChLineWd;
	int				m_nTmChLineWd;
	int				m_nTmCh2LineWd;
	int				m_nScooterPos;

	MemoryTable		*m_pDispBufPtr;

	int				m_nDataCount;
	int				m_nStartDataNo;
	int				m_nLoadFileRecordCount;
	int				m_nReadDataCnt;
	int				m_nScrollVal;
	int				m_nBufHeadIdx;
	int				m_nBufTailIdx;
	int				m_nNumDispData;
	int				m_nDispStartTimeIni;
	int				m_nFileStartIdx;
	int				m_nSamplePeriod;
	int				m_nDispStartTime;
	int				m_nDisplayPitch;
	int				m_nTimeCounter;
	int				m_nScrollCount;
	int				m_nNumTrendChannel;
	int				m_nNumSimChannel;
	int				m_nTrendChArg[AllTrendArea];
	int				m_nTrendChArgTm[NumADChannel];
	int				m_isTrendIndicate[NumADChannel];
	ChartAxis		m_stTimeAxis;
	ChartAxis		m_stTempAxis;
	ChartAxis		m_stLevelAxis;
	ChartAxis		m_stVcAxis;
	ChartAxis		m_stSNAxis;
	ChartAxis		m_stWidthAxis;
	ChartAxis		m_stAxisTm2[NumADChannel];
	CString			m_strSelFileName;
	CString			m_strSimFileName;
	CString			m_strTrendOutFileName;
	BOOL			m_isLineUp;
	BOOL			m_isLineDown;
	BOOL			m_isCutAreaSelect;
	BOOL			m_isSelecting;
	BOOL			m_bTrack;
	BOOL			m_isNeuralCal;
	BOOL			m_bChScaleDisp;
	BOOL			m_isScooter;
	int				m_LeftX;
	int				m_RightX;
	int				m_LeftX_Bk;
	int				m_RightX_Bk;
	int				m_testOffset;
	double			m_dUnitTm[NumADChannel];

	DisplayData		m_stDispPos, m_stPreDispPos;

	BOOL			m_bFilterSwitch;
	float			m_fFilterTemp;
	float			m_fFilterSn;
	float			m_fFilterLv;
	float			m_fFilterPr;
	float			m_fFilterLen;
	int				m_nCutAreaLen;

// member function
public:
	void GraphPaint(CWnd *wnd, CDC &dc);
	void GraphIdmTrendLoad(CWnd *wnd, CDC &dc, LPCTSTR name, StGraphParam *pGPrm, LPCTSTR simName=NULL);
	void GraphHScroll(CWnd *wnd, CDC &dc, UINT nSBCode, int nPos);
	void GraphSbLineUp(CWnd *wnd);
	void GraphSbLineDown(CWnd *wnd);
	void DrawTrendWave(CDC &dc, int sx, int ex, int nChannel);
	void DrawChartAxis(CDC &dc, int sIdx, int eIdx);
	void DrawChart(CDC &dc, int pen, int sIdx, int eIdx);
	DWORD FileSizePkup(LPCTSTR name);
	DWORD LoadTrendFile(MemoryTable *mem, LPCTSTR name, DWORD fileIdx,
						int memIdx, DWORD numData, int pitch);
	int	ScaleY(int arg, int val);
	void GraphSelectAreaClear(CWnd *wnd);
	void SelectStart(void);
	void GraphLButtonDown(CWnd *wnd, CDC &dc, CPoint pt);
	void GraphMouseMove(CWnd *wnd, CDC &dc, CPoint pt);
	void GraphLButtonUp(CDC &dc);
	DWORD LoadSimuFile(MemoryTable *mem, LPCTSTR name, DWORD fileIdx, int memIdx, DWORD numData, int pitch);
	int ScaleSimY(double val);
	void DrawNeuralTrendWave(CDC &dc, int sx, int ex);
	void DrawYAxis(CDC &dc, LPCTSTR pName, LPCTSTR pUnit, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height);

	void InitTimeGraph(CWnd *wnd, CDC &dc, StGraphParam *pGPrm);
	void DrawTimeChart(CDC &dc, SampledData *data);
	void DrawFetchLine(CDC &dc, COLORREF col);

	void InitTimeGraph2(CWnd *wnd, CDC &dc, CDC &mdc, StGraphParam *pGPrm);
	void DrawTimeChart2(CDC &dc, SampledData *data);
	int	ScaleY2(int arg, int val);
	void DrawYAxis2(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height);
	void InitTimeGraph3(CWnd *wnd, CDC &dc, CDC &mdc, StGraphParam *pGPrm);
	void DrawTimeChart3(CDC &dc, SampledData *data);
	void DrawYAxis3(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height);
	void DrawYAxis4(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height);
//@test
	BOOL CalcNearPoint(POINT p1, POINT p2, POINT p3, pDPOINT cp);
	BOOL GetChartData(int nIdx, SampledData *data);
	BOOL CChartLib::GetChartTime(int nIdx, time_t *data);
	int	ChartHitTest(POINT point, StChartHitInfo *chi);
	BOOL LoadTrendData(LPCTSTR name, DWORD fileIdx, SampledData	*data);
};

