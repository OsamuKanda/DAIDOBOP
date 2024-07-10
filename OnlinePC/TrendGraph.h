#pragma once

#include "../include/ChartLib.h"


#define	WM_TRENDLOAD	(WM_APP + 0)


// CTrendGraph
class CTrendGraph : public CWnd
{
	DECLARE_DYNAMIC(CTrendGraph)

// 
public:
	CTrendGraph();
	virtual ~CTrendGraph();

// member data
public:
	CString			m_szFilePath;
	StGraphParam	m_gp;

//member function
public:
	void TrendLoad(LPCTSTR name, StGraphParam *gp);
	void TrendLoad(StChartAxisParam *cp);
	void TrendLoad(int *ch);
	void TrendLoad(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnTrendLoad(WPARAM, LPARAM); 
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


