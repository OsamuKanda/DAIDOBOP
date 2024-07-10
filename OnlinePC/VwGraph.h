#pragma once

#include "../include/ChartLib.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	WM_APPENDTREND	(WM_APP + 0)
#define	WM_REDRAW_BP	(WM_APP + 1)


///////////////////////////////////////////////////////////////////////////////
// CVwGraph
class CVwGraph : public CWnd
{
	DECLARE_DYNAMIC(CVwGraph)

public:
	CVwGraph();
	virtual ~CVwGraph();

// member data
public:
	StGraphParam	m_gp;
	CBitmap			m_bitmap;
	int				m_nTrendXPos;
	int				m_nTrendShiftCnt;
	int				m_nPreYPos[4];
	COLORREF		m_clBase;
	COLORREF		m_clLine;
	COLORREF		m_clPattOrd;
	COLORREF		m_clPattAlm;
	COLORREF		m_clCorrOrd;
	COLORREF		m_clCorrAlm;

//member function
protected:
	void GraphPaint(CDC &dc);
	void ClearTrendWave(CDC &dc);
	void DrawTrendWave(CDC &dc, int color, int cmd, int yPos);
	void ShiftTrendWave(CDC &dc);

public:
	void AppendTrend(double dOrdPattOut, double dAlmPattOut, double dOrdCorrOut, double dAlmCorrOut);
	void RedrawTrend(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnAppendTrend(WPARAM, LPARAM); 
	afx_msg LRESULT OnRedrawTrend(WPARAM, LPARAM); 
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


