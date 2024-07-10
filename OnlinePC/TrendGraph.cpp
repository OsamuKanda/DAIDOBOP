// TrendGraph.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TrendGraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CChartLib	&clib = theApp.m_trendChart;


// CTrendGraph
IMPLEMENT_DYNAMIC(CTrendGraph, CWnd)
CTrendGraph::CTrendGraph()
{
	m_szFilePath = _T("");

	//
	clib.m_clClear = COL_GRAPH_BACK;
}

CTrendGraph::~CTrendGraph()
{
}


BEGIN_MESSAGE_MAP(CTrendGraph, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE( WM_TRENDLOAD, OnTrendLoad )
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTrendGraph message handler

void CTrendGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CTrendGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	clib.GraphPaint(this, dc);
}

void CTrendGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CClientDC dc(this);

	clib.GraphHScroll(this, dc, nSBCode, nPos);

	CWnd *pParent = GetParent();
	if( pParent != NULL )
		pParent->SendMessage(WM_HSCROLL, (WPARAM)0, (LPARAM)pScrollBar);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CTrendGraph::OnTrendLoad(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	if( m_szFilePath.GetLength() == 0 )
		return 0;

	clib.GraphIdmTrendLoad(this, dc, m_szFilePath, &m_gp);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CTrendGraph member function

void CTrendGraph::TrendLoad(LPCTSTR name, StGraphParam *gp)
{
	m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_TRENDLOAD);
}

void CTrendGraph::TrendLoad(StChartAxisParam *cp)
{
	m_gp.timeAxis	= cp->timeAxis;
	m_gp.tempAxis	= cp->tempAxis;
	m_gp.levelAxis	= cp->levelAxis;
	m_gp.vcAxis		= cp->vcAxis;
	m_gp.snAxis		= cp->snAxis;
//	m_gp.widthAxis	= cp->widthAxis;

	SendMessage(WM_TRENDLOAD);
}

void CTrendGraph::TrendLoad(int *ch)
{
	memcpy(m_gp.nTrendChArg, ch, sizeof(m_gp.nTrendChArg));

	SendMessage(WM_TRENDLOAD);
}

void CTrendGraph::TrendLoad(void)
{
	SendMessage(WM_TRENDLOAD);
}

