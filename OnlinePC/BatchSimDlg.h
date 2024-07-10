#pragma once
#include "afxwin.h"
#include "BatchGraph.h"
//#include "TMapDlgEx.h"
//#include "Chart3dDlg.h"
#include "TMap2If.h"
#include "Chart3D2If.h"
#include "BoBatch.h"
#include "PassDlg.h"
#include "TrendSel.h"
#include "TcVwDlg.h"
#include "PtnSel99.h"
#include "../include/MsgWnd.h"
#include "../Lib/ColorButton.h"
#include "../Lib/ColorEdit.h"


// CBatchSimDlg ダイアログ

class CBatchSimDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchSimDlg)

public:
	CBatchSimDlg(CWnd* pParent = NULL);
	virtual ~CBatchSimDlg();

// member data
public:
	CDialog			*m_pDlgTMapForBatch;
	CDialog			*m_pDlgChart3dForBatch;
	CTcVwDlg		*m_pDlgTcView;				// TC View screen class
	CBoBatch		m_batch;
	CMsgWnd			m_msg;
	CBatchGraph		m_graph;
	TrendDirectory	trendDir[NumTrendFile*2];
	TrendSelect		m_stTdSel[NumInput];	// trend channel select info.
	DataFileHeader	m_header;
	BOOL			m_blStopFlg;
	BOOL			m_blOutBOFetched;
	BOOL			m_blOutBOFetchedD;
	BOOL			m_blPulseOutBOFetched;
	BOOL			m_blPulseOutBOFetchedD;
	int				m_isEnabLevelCtrl;
	int				m_simAutoLvCtrl;
	int				m_isAutoMoldWidth;
	int				m_nSelectInfo;
	int				m_nSelFile;
	time_t			m_nStartTime;
	int				m_nOffsetTime;
	int				m_nTickCount;
	int				m_nMdSize;
	int				m_nTrendNum;
	UINT			m_nTimer;
	UINT_PTR		m_nTimerID;
	BOOL			m_bPass;
	BOOL			m_bFirst;
	BOOL			m_bFirstBO;
	BOOL			m_bFirstBOD;
	BOOL			m_blPause;

	BOOL			m_bFilterSwitch;
	float			m_fFilterFacter;
	float			m_fFilterFacterSn;
	float			m_fFilterFacterLv;
	float			m_fFilterFacterPr;

// member function
public:
	void ResetBatchGraph(void);
	void SetupBatchsimDlg(void);
	void SetupLogList(void);
	void DrawYScale(CDC &dc);
	void SetSelectedTrendEntry(int sel);
	void StartBatchSimulate(void);
	void StepBatchSimulate(void);
	void StopBatchSimulate(void);
	void EnableControl(BOOL blEnable);
	void SetSimulationTime(void);
	void TestBOStatus(void);
	void SetAverageData(SampledData *data, int *pData);
	void StartTimer(DWORD dwTime);
	void StopTimer(void);
	LPCTSTR GetTimeStr(time_t tm);
	void BoConditionManage(WPARAM wParam, int cmd);

// dialog data
	enum { IDD = IDD_BATCHSIM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnSelchangeCombo1();
	CStatic m_textYScale;
	//CComboBox m_cmbLog;
	CEdit m_editRemark;
	CEdit m_editMdSize;
	CEdit m_editStTime;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	CStatic m_textCurTime;
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton11();
	CColorButton m_btnPass;
	afx_msg void OnClickedPasswordButton();
	CStatic m_textSG;
	afx_msg void OnSelsteelkind();
	afx_msg void OnUpdateSelsteelkind(CCmdUI *pCmdUI);
	afx_msg void OnMove(int x, int y);
	CButton m_chkAuto;
	CButton m_chkHighSpeed;
	afx_msg void OnTrendchg();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	CButton m_btnALvlCtl;
	CButton m_btnExeALvlCtl;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton m_btnLogPrev;
	CButton m_btnLogNext;
	CColorEdit m_textBoMsgFirst;
	CColorEdit m_textBoMsgFirstD;
	//afx_msg void OnTcposition();
	afx_msg void OnClickedFilterSw();
	CButton m_chkFilter;
	CEdit m_editTempFilter;
	CEdit m_editSnFilter;
	CEdit m_editLevelFilter;
	CEdit m_editVcFilter;
	CStatic m_txtConfName;
	CStatic m_txtConfNameD;
	CButton m_chkCsvOut;
	CButton m_btnStartSim;
	afx_msg void OnBnClickedButton6();
	CStatic m_labLogName;
	CButton m_btnLogList;
};
