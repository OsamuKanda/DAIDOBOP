#pragma once
#include "afxwin.h"
#include "BoBatch.h"
#include "TcVwDlg.h"

#define	WM_MAPDATA		(WM_APP + 0)

typedef struct
{
	CBoBatch	*pBatch;
	time_t		nStartTime;
	int			nOffsetTime;
}StLogInfo;


// CTMapDlgEx ダイアログ
class CTMapDlgEx : public CDialog
{
	#define PartitionCount	10

	DECLARE_DYNAMIC(CTMapDlgEx)

public:
	CTMapDlgEx(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTMapDlgEx();

// メンバ
public:
	IChartFXPtr m_pChartFIX;
	IChartFXPtr m_pChartLOOSE;
	CWnd		m_ChartFIX;
	CWnd		m_ChartLOOSE;
	BOOL		m_isHideCloseButton;
	int			m_averageData[NumTCTap][PartitionCount + 1][PartitionCount + 1];
	BOOL		m_bBatchStop;
	BOOL		m_bAutoRun;
	StLogInfo	m_stLog;
	int			m_nRecCur;
	int			m_nRecCnt;
	int			m_nStartTime;
	int			m_nOffsetTime;
	CBoBatch	*m_pBatch;
	CTcVwDlg	*m_pDlgTcView;				// TC View screen class

	void SetAverageData(int *data);
	void GetAverageData(int *pData);
	void CalcAverageData(int f, int *pData);
	void HideCloseButton(BOOL bHide);
	BOOL GetBatchStopReq(void);
	void SetBatchStopReq(BOOL bSet);
	void SetLogInfo(StLogInfo *pInfo);

	void DispCurrentMap(void);
	void DispCurrentTime(void);
	LPCTSTR GetTimeStr(time_t tm);

// dialog data
public:
	enum { IDD = IDD_TMAP_DLG_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg LRESULT OnAddone(WPARAM, LPARAM) ;
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButton1();
	CButton m_btnClose;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	void PostPaintChartfix(short w, short h, long lPaint, short* nRes);
	void PostPaintChartloose(short w, short h, long lPaint, short* nRes);
	afx_msg void OnBnClickedButton4();
	CScrollBar m_scr;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT nIDEvent);
	CStatic m_textCurTime;
	afx_msg void OnTcposition();
};
