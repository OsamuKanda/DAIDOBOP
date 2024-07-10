#pragma once
#include "BoexeGraph.h"
#include "BoexeAxis.h"
#include "TrendSel.h"
//#include "TrendView.h"
#include "PtnSel.h"
#include "Logging.h"
#include "../include/MsgWnd.h"
#include "../lib/ColorStatic.h"
#include "../lib/ColorButton.h"
#include "../lib/ColorEdit.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "Realtime.h"
#include "MtiTime.h"
#include "TcVwDlg.h"
#include "TestDlg.h"
#include "LogTC.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	TIMER_FIRST_CYCLE	200					// [ms]
#define	TIMER_SLOW_CYCLE	1000				// [ms]
#define	AREAGROUP			6

#define	WM_RESTART_BOP		(WM_APP + 0)
#define	WM_MANU_TC_CUT		(WM_APP + 1)
#define	WM_MESSAGE_OUT		(WM_APP + 2)


// CPredicDlg class
class CPredicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPredicDlg)

public:
	CPredicDlg(CWnd* pParent = NULL);
	virtual ~CPredicDlg();

// member data
public:
	SharedMemory	*m_pShMem;					// shared memory
	CMsgWnd			m_msg;						// message window class
	StActiveInfo	m_info;
	CLogTC			m_logtc;

// member data
private:
	// sub class
	CBoexeGraph		m_graph;					// graph window class
	CLogging		m_logging;					// log access class
	CRealTime		m_rc;						// realtime class
	CMultiTimer		m_rt;						// multi media timer class
	CImageList		m_image;					// 高さを調整する為のイメージリスト

	// for bo calc.
	BoSysInfo		m_stBsInfo;						// bo system info.
	BOOL			m_blResultData[NumCombination];	// bo calc result table.
	BOOL			m_blResultDataD[NumCombination];// bo calc result table for dep.
	int				m_nAllResult;					// bo calculation result of all
	BOOL			*m_pResultMap[2];				// result table map

	// for error procedure
	int				m_nAveArea[AREAGROUP][TapRaw];	// temp average area
	int				m_nAveAreaA[AREAGROUP];			// temp average areaA
	int				m_aveAreac[AREAGROUP];
	int				m_nErrBf[20];					// error count buffer.
	int				m_nErrFlk[20];					// error flicker count.
	COLORREF		m_clAlarm;					// alarm color

	// etc..
	int				m_nPtnNo;					// pattern number
	int				m_nMoldSize;				// mold size
	int				m_nRecvSteel;				// receive pattern no.
	int				m_nRunWaitCnt;				// run wait count
	//int				m_nCastLength;				// casting length for display
	//int				m_nMdCWTmp;					// mold cool water temp. for display
	//int				m_nMdCWFRFix;				// mold cool water fix for display
	//int				m_nMdCWFRLoose;				// mold cool water loose for display
	//int				m_nMdCWFREast;				// mold cool water east for display
	//int				m_nMdCWFRWest;				// mold cool water west for display
	int				m_nActSendCnt;
	UINT			m_nTimerID;					// first timer ID
	UINT			m_nTimerID2;				// slow timer ID
	UINT			m_nTimerID3;				// fastest timer ID for debug
	BOOL			m_blManuSteel;				// manual steel flag
	BOOL			m_blPreVcOn;				// pre vc on flag
	BOOL			m_blPreChargeChange;		// pre charge change flag
	BOOL			m_blPreCastStart;			// pre Cast Start flag
	BOOL			m_blPreL1Err;				// pre L1 Err flag
	BOOL			m_blPreAutoLVCtl;			// pre auto level control flag
	BOOL			m_blPulseBOFetched;			// pre bo fetch flag
	BOOL			m_blPulseBopCondition;		// pre bop condition
	BOOL			m_blReqBoPosClear;			// BO位置表示クリア要求
	BOOL			m_bRunWait;					// run wait flag
	BOOL			m_bRunOnce;					// run once flag
	BOOL			m_blOnSteel;				// on steel
	TrendSelect		m_stTdSel[NumInput];		// trend channel select info.
	TrendSelect		m_stTdView[NumInput];		// trend view select info.
	CRect			m_rcMold;					// mold rectangle size
	CString			m_timeStr;					// time string
	BOOL			m_isFinish;
	int				m_nCutOffTimer[NumADChannel];// cut off TC timer.
	int				m_nHealthy;
	BOOL			m_isGCMode;					// 鋼種切り替えモード
	BOOL			m_isManMode;				// 強制手動モード

	// for test (stand alone mode)
	CTestDlg		*m_pDlgTest;				// test screen class
	CTcVwDlg		*m_pDlgTcView;				// TC View screen class

// member function
public:
	void EndRequest(void);
	BOOL IsFinish(void);
	BOOL IsRun(void);

// member function
private:
	void ResetBoexeGraph(void);
	void DrawYScale(CDC &dc);
	void SetCurrentTime(void);
	void SetupTdViewListItem(void);
	void SetTdViewListData(SampledData *pDat);
	void DrawFixMold(CDC &dc);
	void DispActData(void);
	void SetAverageData(SampledData *data, int *pData);
	void BoConditionManage(WPARAM wParam, int cmd);
	void TestBOStatus(BOOL *status, BOOL *statusD, int allRes, FILETIME *pFt = NULL);
	BOOL SetupTickTimer(void);
	LPCTSTR GetTimeStr(void);
	LPCTSTR GetTimeStr(time_t tm);
	void ErrorProc(void);
	void SetMoldSize(void);
	void ButtomOnOff(void);
	void SetupBOCondition(void);
	BOOL ConfDataSetUp(void);
	BOOL ConfDataUpDate(void);
	void CheckAlarm(void);
	void CheckAutoRecoverTC(SampledData *pDat);
	void CheckAutoCutOffTC(SampledData *pDat);
	void LimitSampleData(SampledData *pDat);
	void LimitTempData(int *pDat);
	void SetPtnName(LPCTSTR pszName);
	BOOL SaveExtTC(int nPtnNo);
	void DeleteCombination(int upper, int lower);
	void DeleteCombinationD(int upper, int lower);
	void ChangeChArgTm(void);

// dialog data
public:
	enum { IDD = IDD_PREDIC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnAxisset();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnTrendchg();
//	afx_msg void OnTrendview();
	afx_msg void OnSelsteelkind();
	afx_msg void OnClickedFilterSw();
	afx_msg void OnClickedResetAlarm();
	afx_msg void OnMove(int x, int y);

public:
	CStatic m_textCurTime;
	CStatic m_textYScale;
	CStatic m_textMdSize;
	CListCtrl m_listData;
	CStatic m_textPtnNo;
	CButton m_chkFilter;
	CButton m_rdoResetAlm;
	CStatic m_textFixCwFlow;
	CStatic m_textFixCwFlow2;
	CStatic m_textFixCwFlow3;
	CStatic m_textLooseCwFlow;
	CStatic m_textEastCwFlow;
	CStatic m_textWestCwFlow;
	CStatic m_textCastLen;
	CStatic m_textMdCwTemp;
	CStatic m_textCastVc;
	CStatic m_textActLevel;
	CStatic m_textTdWeight;
	CStatic m_textEMBr;

//	CEdit m_editFlowFacter;
	CEdit m_editTempFilter;
	CEdit m_editSnFilter;
	CEdit m_editLevelFilter;
	CEdit m_editVcFilter;
	CEdit m_editWidthFilter;
	CStatic m_mold;
	CButton m_rdoVcOnIn;
	CButton m_rdoChChgIn;
	CButton m_rdoMdLvcIn;
	CButton	m_rdoTdChgIn;
	CButton m_rdoBoAlmOut;
	CButton m_rdoBoAlmcOut;
	CButton m_chkStrobe;
	CButton m_chkLvCtrl;
	CButton	m_chkAutoCutTC;
	afx_msg void OnIndividual();
	CColorStatic m_mode;
	CColorStatic m_automode;
	afx_msg void OnUpdateSelsteelkind(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CColorEdit m_textBoMsgFirst;
	CColorEdit m_textBoMsgFirstD;
	afx_msg void OnTcposition();
	afx_msg LRESULT OnRestartBOP(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnManualCutoffTC(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageOut(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton2();
	CColorButton m_btnAutoChg;
	CColorButton m_btnManChg;
};
