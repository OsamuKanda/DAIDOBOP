#pragma once

#include "../include/ChartLib.h"

#define	WM_SIMLOAD	(WM_APP + 0)

// CSimGraph
class CSimGraph : public CWnd
{
	DECLARE_DYNAMIC(CSimGraph)

public:
	CSimGraph();
	virtual ~CSimGraph();

// member data
public:
	CString			m_szFilePath;
	CString			m_szSimFilePath;
	StGraphParam	m_gp;

//member function
public:
	void TrendLoad(LPCTSTR name, StGraphParam *gp);
	void TrendLoad(StChartAxisParam *cp);
	void TrendLoad(int *pArgs);
	void TrendLoad(BOOL isNet, LPCTSTR name=NULL);
	void TrendLoad(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnTrendLoad(WPARAM, LPARAM); 
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


