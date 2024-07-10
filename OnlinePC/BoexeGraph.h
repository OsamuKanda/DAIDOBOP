#pragma once

#include "../include/ChartLib.h"


#define	WM_BOEXELOAD	(WM_APP + 0)
#define	WM_BOEXEREDRAW	(WM_APP + 1)
#define	WM_BOEXEFETCH	(WM_APP + 2)
#define	WM_BOEXECHG		(WM_APP + 3)
#define	WM_BOEXECHGTM	(WM_APP + 4)

#define	EXEC_DATA_NUM	(5*60*2)

// CBoexeGraph
class CBoexeGraph : public CWnd
{
	DECLARE_DYNAMIC(CBoexeGraph)

// 
public:
	CBoexeGraph();
	virtual ~CBoexeGraph();

// member data
public:
	CString			m_szFilePath;
	StGraphParam	m_gp;
	SampledData		m_data[EXEC_DATA_NUM];
	COLORREF		m_clFetchColor;
	int				m_nCurIdx;
	int				m_nCount;
	CToolTipCtrl	m_tool;						// tool tip control
	CFont			m_fontToolTip;

//member function
public:
	void BatchLoad(LPCTSTR name, StGraphParam *gp);
	void AppendData(SampledData *data);
	void Fetch(void);
	void SetFetchColor(COLORREF col);
	void ChangeArg(int *pArg);
	void ChangeChArgTm(int *pArg);
	void ChangeAxis(StGraphParam *pPrm);
	void PlotReset(void);
	int	ChartHitTest(POINT pt, StChartHitInfo *chi);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnTrendLoad(WPARAM, LPARAM); 
	afx_msg LRESULT OnAppendData(WPARAM, LPARAM);
	afx_msg LRESULT OnFetchDraw(WPARAM, LPARAM);
	afx_msg LRESULT OnTrendChg(WPARAM, LPARAM);
	afx_msg LRESULT OnTrendChgTm(WPARAM, LPARAM);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


