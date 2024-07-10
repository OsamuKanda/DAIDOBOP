// BoviewGraph.cpp : é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BoviewGraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CChartLib	&clib1 = theApp.m_boviewChartFz;	// mem dc for freeze
static CChartLib	&clib2 = theApp.m_boviewChart5;		// mem dc for 5 minutes
static CChartLib	&clib3 = theApp.m_boviewChart15;	// mem dc for 15 minutes
static CChartLib	&clib4 = theApp.m_boviewChart30;	// mem dc for 30 minutes
static CChartLib	&clib5 = theApp.m_boviewChart60;	// mem dc for 60 minutes


// CBoviewGraph
IMPLEMENT_DYNAMIC(CBoviewGraph, CWnd)
CBoviewGraph::CBoviewGraph()
{
	m_szFilePath	= _T("");
	m_nCurIdx		= 0;
	m_nCount		= 0;
	m_nCountFrz		= 0;
	m_nScX			= 0;
	m_cur			= 1;
	m_bFirst		= TRUE;

	//
	clib1.m_clClear	= COL_GRAPH_BACK;		// ÉOÉâÉtîwåiêF
	clib2.m_clClear	= COL_GRAPH_BACK;		// ÉOÉâÉtîwåiêF
	clib3.m_clClear	= COL_GRAPH_BACK;		// ÉOÉâÉtîwåiêF
	clib4.m_clClear	= COL_GRAPH_BACK;		// ÉOÉâÉtîwåiêF
	clib5.m_clClear	= COL_GRAPH_BACK;		// ÉOÉâÉtîwåiêF
}

CBoviewGraph::~CBoviewGraph()
{
}


BEGIN_MESSAGE_MAP(CBoviewGraph, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE( WM_BOVIEWLOAD, OnTrendLoad )
	ON_MESSAGE( WM_BOVIEWREDRAW, OnAppendData )
	ON_MESSAGE( WM_BOVIEWRELOAD, OnTrendReload )
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBoviewGraph message handler

void CBoviewGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CBoviewGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( m_bFirst )
		return;

	int		c = m_cur;

	CBitmap	*old;
	old = (CBitmap *)m_mdc[m_cur].SelectObject(&m_bitmap[m_cur]);
	switch(m_cur)
	{
	case 1:
		clib2.GraphPaint(this, m_mdc[c]);
		break;
	case 2:
		clib3.GraphPaint(this, m_mdc[c]);
		break;
	case 3:
		clib4.GraphPaint(this, m_mdc[c]);
		break;
	case 4:
		clib5.GraphPaint(this, m_mdc[c]);
		break;
	}
	CRect	rc;
	GetClientRect(&rc);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&m_mdc[c],0,0,SRCCOPY);
	m_mdc[c].SelectObject(old);

	// draw scooter line
	DrawScooterLine(dc);
}

LRESULT CBoviewGraph::OnTrendLoad(WPARAM, LPARAM)
{
	CClientDC	dc(this);
	CChartLib	*clib[] = {&clib1, &clib2, &clib3, &clib4, &clib5};
	int			n[] = {1, 1, 3, 2, 2};
	int		i;
	int		c = m_cur;

	if( m_bFirst )
	{
		for( i = 0; i < BOVIEW_GRAPH_NUM; i++ )
		{
			m_mdc[i].CreateCompatibleDC(&dc);
			m_bitmap[i].CreateCompatibleBitmap(&dc, clib[i]->ChartWidth, clib[i]->ChartHeight);
		}
		m_bFirst = FALSE;
	}
	for( i = 0; i < BOVIEW_GRAPH_NUM; i++ )
	{
		m_gp.timeAxis.scale *= n[i];
		clib[i]->InitTimeGraph2(this, dc, m_mdc[i], &m_gp);
	}

	return 0;
}

LRESULT CBoviewGraph::OnTrendReload(WPARAM, LPARAM)
{
	CClientDC	dc(this);
	int		i, j, k, idx, idxs, cnt;
	SampledData	sd;

	CChartLib	*clib[] = {&clib2, &clib3, &clib4, &clib5};
	int			n[] = {1, 3, 2, 2};
	int			m[] = {1, 3, 6, 12};

	for( i = 0; i < BOVIEW_GRAPH_NUM-1; i++)
	{
		m_gp.timeAxis.scale *= n[i];
		clib[i]->InitTimeGraph2(this, dc, m_mdc[i+1], &m_gp);
		idxs = m_nCurIdx - ((m_nCount >= m[i]*clib[i]->ChartWidth)? m[i]*clib[i]->ChartWidth: m_nCount);
		if( idxs < 0 )
			idxs = idxs + SAVE_DATA_NUM;
		cnt = (m_nCount >= m[i]*clib[i]->ChartWidth)? m[i]*clib[i]->ChartWidth: m_nCount;
		for(j = 0, idx = idxs; j < cnt; j++)
		{
			for(k = 0; k < m_gp.numChannel; k++ )
			{
				if( k < m_gp.numChannel - 3 )
					sd.val[k] = m_data[idx].val[k];
				else
					sd.val[k] = m_data[idx].val[k + m_gp.nCombiNo * TapRaw];
			}
			clib[i]->DrawTimeChart2(m_mdc[i+1], &sd);
			idx = (idx + 1) % SAVE_DATA_NUM;
		}
	}

	return 0;
}

LRESULT CBoviewGraph::OnAppendData(WPARAM, LPARAM)
{
	CClientDC	dc(this);
	SampledData	sd;
	int			i;

	for(i = 0; i < m_gp.numChannel; i++ )
	{
		if( i < m_gp.numChannel - 3 )
			sd.val[i] = m_data[m_nCurIdx].val[i];
		else
			sd.val[i] = m_data[m_nCurIdx].val[i + m_gp.nCombiNo * TapRaw];
	}

	CChartLib	*clib[] = {&clib2, &clib3, &clib4, &clib5};
	for( i = 0; i < BOVIEW_GRAPH_NUM-1; i++)
	{
		clib[i]->DrawTimeChart2(m_mdc[i+1], &sd);
	}

	if( ++m_nCurIdx >= SAVE_DATA_NUM )
	{
		m_nCurIdx = 0;
	}

	if( m_nCount < SAVE_DATA_NUM )
		m_nCount++;

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// CBoviewGraph member function

void CBoviewGraph::BoviewLoad(LPCTSTR name, StGraphParam *gp)
{
	if(name != NULL)
		m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_BOVIEWLOAD);
}

void CBoviewGraph::BoviewReload(LPCTSTR name, StGraphParam *gp)
{
	if(name != NULL)
		m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_BOVIEWRELOAD);
}

void CBoviewGraph::AppendData(SampledData *data)
{
	int		idx = m_nCurIdx;
	memcpy(&m_data[idx], data, sizeof(SampledData));

	SendMessage(WM_BOVIEWREDRAW);
}

void CBoviewGraph::ChangeView(int no)
{
	if( m_cur != no )
	{
		if( no == 0 )
		{
			CBitmap	*old1, *old2;
			CRect	rc;
			old1 = (CBitmap *)m_mdc[m_cur].SelectObject(&m_bitmap[m_cur]);
			old2 = (CBitmap *)m_mdc[no].SelectObject(&m_bitmap[no]);
			GetClientRect(&rc);
			m_mdc[no].BitBlt(0,0,rc.Width(),rc.Height(),&m_mdc[m_cur],0,0,SRCCOPY);
			m_mdc[m_cur].SelectObject(old1);
			m_mdc[no].SelectObject(old2);

			int i;
			int	c = m_cur;
			int	cnt = clib1.ChartWidth;
			int	fcnt;
			int	idx = m_nCurIdx-1;
			int	idx2, idx3;
			int	m[] = {1, 1, 3, 6, 12};

			for( i = 0, fcnt = 0, idx2 = 0, idx3 = 0; i < cnt && idx2 < m_nCount; i++ )
			{
				idx3 = idx - idx2;
				if( idx3 < 0 )
					idx3 += SAVE_DATA_NUM;
//				for( j = 0; j < m_gp.numChannel; j++ )
//					m_fdat[i].val[j] = m_data[idx3].val[j];
				m_fdat[i] = m_data[idx3];
				idx2 += m[c];
				fcnt++;
			}
			m_nCountFrz = fcnt;
		}

		m_cur = no;
	}
}

void CBoviewGraph::SetScooterX(int x)
{
	if( x < 0 )
		x = 0;
	if( x >= clib1.ChartWidth )
		x = clib1.ChartWidth-1;
	m_nScX = x;
}

void CBoviewGraph::DrawScooterLine(CDC &dc)
{
	CRect	rc;
	CPen	pen, *pOldPen;

	GetClientRect(&rc);
	pen.CreatePen(PS_DOT, 1, RGB(0,0,255));
	pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(m_nScX, rc.top);
	dc.LineTo(m_nScX, rc.bottom);
	dc.SelectObject(pOldPen);
}

BOOL CBoviewGraph::GetScooterVal(SampledData *pDat)
{
	CRect		rc;
	int			idx, x, c;
	CChartLib	*clib[] = {&clib1, &clib2, &clib3, &clib4, &clib5};

	c = m_cur;
	x = m_nScX;

	switch(c)
	{
	case 0:
		if( m_nCountFrz < m_nScX + 1 )
			return FALSE;
		x = m_nCountFrz - x - 1;
		memcpy(pDat, &m_fdat[x], sizeof(SampledData));
		return TRUE;
		break;
	case 1:
		x = m_nScX;
		if( m_nCount <= x )
			return FALSE;
		if( m_nCount > clib[c]->ChartWidth )
			idx = m_nCurIdx - clib[c]->ChartWidth + x;
		else
			idx = m_nCurIdx - m_nCount + x;
		break;
	case 2:
		x = m_nScX * 3;
		if( m_nCount <= x )
			return FALSE;
		if( m_nCount > (clib[c]->ChartWidth*3) )
			idx = m_nCurIdx - clib[c]->ChartWidth*3 + x;
		else
			idx = m_nCurIdx - m_nCount + x;
		break;
	case 3:
		x = m_nScX * 6;
		if( m_nCount <= x )
			return FALSE;
		if( m_nCount > (clib[c]->ChartWidth*6) )
			idx = m_nCurIdx - clib[c]->ChartWidth*6 + x;
		else
			idx = m_nCurIdx - m_nCount + x;
		break;
	case 4:
		x = m_nScX * 12;
		if( m_nCount <= x )
			return FALSE;
		if( m_nCount > (clib[c]->ChartWidth*12) )
			idx = m_nCurIdx - clib[c]->ChartWidth*12 + x;
		else
			idx = m_nCurIdx - m_nCount + x;
		break;
	}

	if( idx < 0 )
		idx = SAVE_DATA_NUM + idx;

	memcpy(pDat, &m_data[idx], sizeof(SampledData));

	return TRUE;
}

