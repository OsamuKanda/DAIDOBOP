// SimGraph.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "SimGraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CChartLib	&clib = theApp.m_simChart;


// CSimGraph
IMPLEMENT_DYNAMIC(CSimGraph, CWnd)
CSimGraph::CSimGraph()
{
	m_szFilePath = _T("");
	m_szSimFilePath = _T("");

	//
	clib.m_clClear = COL_GRAPH_BACK;
}

CSimGraph::~CSimGraph()
{
}


BEGIN_MESSAGE_MAP(CSimGraph, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE( WM_SIMLOAD, OnTrendLoad )
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CSimGraph message handler

void CSimGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	clib.GraphPaint(this, dc);
}

void CSimGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CClientDC dc(this);

	clib.GraphHScroll(this, dc, nSBCode, nPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CSimGraph::OnTrendLoad(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	if( m_szFilePath.GetLength() == 0 )
		return 0;

	clib.GraphIdmTrendLoad(this, dc, m_szFilePath, &m_gp, m_szSimFilePath);

	return 0;
}

void CSimGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
	clib.GraphLButtonDown(this, dc, point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CSimGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
	clib.GraphMouseMove(this, dc, point);

	CWnd::OnMouseMove(nFlags, point);
}

void CSimGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
	clib.GraphLButtonUp(dc);

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CSimGraph::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	static HCURSOR	hNormal = NULL;

	// mouse cursor type check
	if (clib.m_isSelecting == TRUE) {
		// selected cross cursor set
		hNormal = SetCursor(::LoadCursor(NULL, IDC_CROSS));
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////////////
// CSimGraph member function

void CSimGraph::TrendLoad(LPCTSTR name, StGraphParam *gp)
{
	m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_SIMLOAD);
}

void CSimGraph::TrendLoad(StChartAxisParam *cp)
{
	m_gp.timeAxis	= cp->timeAxis;
	m_gp.tempAxis	= cp->tempAxis;

	SendMessage(WM_SIMLOAD);
}

void CSimGraph::TrendLoad(int *pArgs)
{
	memcpy(m_gp.nTrendChArg, pArgs, sizeof(m_gp.nTrendChArg));

	SendMessage(WM_SIMLOAD);
}

void CSimGraph::TrendLoad(BOOL isNet, LPCTSTR name /*=NULL*/)
{
	if( isNet )
	{
		if( name != NULL )
			m_szSimFilePath = name;
		m_gp.isNeuralCal = TRUE;
		m_gp.neuralChannel = 2;
	}
	else
	{
		m_gp.isNeuralCal = FALSE;
		m_gp.neuralChannel = 0;
	}

	SendMessage(WM_SIMLOAD);
}

void CSimGraph::TrendLoad(void)
{
	SendMessage(WM_SIMLOAD);
}


