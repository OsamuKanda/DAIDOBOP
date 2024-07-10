// TcGraph.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TcGraph.h"


// CTcGraph
IMPLEMENT_DYNAMIC(CTcGraph, CWnd)
CTcGraph::CTcGraph(int no)
{
	m_pLib = NULL;
	if( no >= 0 && no < TC_NO_MAX ){
		m_pLib = &theApp.m_tcChart[no];
		m_pLib2 = &theApp.m_tcChart2[no];
		m_pLib->m_clClear = COL_GRAPH_BACK;
		m_pLib2->m_clClear = COL_GRAPH_BACK;
	}
	m_szFilePath = _T("");
	m_bAlarm = FALSE;
}

CTcGraph::~CTcGraph()
{
}

BEGIN_MESSAGE_MAP(CTcGraph, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE( WM_TCLOAD, OnTcLoad )
	ON_MESSAGE( WM_TCREDRAW, OnAppendData )
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTcGraph message handler

// Lbutton down.
void CTcGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

// on paint
void CTcGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( m_pLib != NULL && m_pLib2 != NULL )
	{
		if( m_bAlarm )
			m_pLib2->GraphPaint(this, dc);
		else
			m_pLib->GraphPaint(this, dc);
	}
}

// on TC load
LRESULT CTcGraph::OnTcLoad(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	if( m_pLib != NULL && m_pLib2 != NULL )
	{
		m_pLib->InitTimeGraph2(this, dc, dc, &m_gp);
		m_pLib2->InitTimeGraph2(this, dc, dc, &m_gp);
	}

	return 0;
}

// on append data
LRESULT CTcGraph::OnAppendData(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	if( m_pLib != NULL && m_pLib2 != NULL )
	{
		m_pLib->DrawTimeChart2(dc, &m_data);
		m_pLib2->DrawTimeChart2(dc, &m_data);
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// CTcGraph member function

// set graph parameter.
void CTcGraph::TcLoad(LPCTSTR name, StGraphParam *gp)
{
	if(name != NULL)
		m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_TCLOAD);
}

// append graph data.
void CTcGraph::AppendData(SampledData *data)
{
	memcpy(&m_data, data, sizeof(SampledData));

	SendMessage(WM_TCREDRAW);
}

