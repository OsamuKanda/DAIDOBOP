// BatchGraph.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BatchGraph.h"
#include ".\batchgraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CChartLib	&clib = theApp.m_batchChart;
static TcName		*tcnm	= &theApp.m_stTcNm;


// CBatchGraph
IMPLEMENT_DYNAMIC(CBatchGraph, CWnd)
CBatchGraph::CBatchGraph()
{
	m_szFilePath = _T("");
	m_nCurIdx		= 0;
	m_nCount		= 0;

	//
	clib.m_clClear = COL_GRAPH_BACK;
}

CBatchGraph::~CBatchGraph()
{
}


BEGIN_MESSAGE_MAP(CBatchGraph, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE( WM_BATCHLOAD, OnTrendLoad )
	ON_MESSAGE( WM_BATCHREDRAW, OnAppendData )
	ON_MESSAGE( WM_BATCHFETCH, OnFetchDraw )
	ON_MESSAGE( WM_BATCHCHG, OnTrendChg )
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBatchGraph message handler

void CBatchGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CBatchGraph::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE )
	{
		CClientDC		dc(this);
		StChartHitInfo	chi;
		CString			str, strV;
		CPoint			point;
		CRect			rect;
		double			dVal;

		// ToolTip未作成なら作成
		if( m_tool.GetSafeHwnd() == NULL )
		{
			// initialize tool tip
			m_tool.Create(this);
			m_tool.AddTool(this);
			m_tool.SetTipBkColor(RGB(255,255,255));
			m_fontToolTip.CreateFont( -14, 0, 0, 0,	FW_NORMAL, FALSE, FALSE, FALSE,						
								DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,		
								ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("MS Shell Dlg")); 
			m_tool.SetFont(&m_fontToolTip);		
		}

		// 座標取得
		point.x = pMsg->lParam & 0xffff;
		point.y = (pMsg->lParam >> 16) & 0xffff;
		GetClientRect(&rect);

		// 指定ポイントのHitテスト
		if( ChartHitTest(point, &chi) )
		{
			// ToolTip文字の設定
			switch(m_gp.nTrendChArgTm[chi.idx])
			{
			case SNID:		str.Format(_T("SN開度"));
				dVal = chi.val * PhisicalSNUnit;
				strV.Format(_T("=%.0lf"), dVal);
				break;
			case LevelID:	str.Format(_T("レベル"));
				dVal = chi.val * PhisicalLevelUnit;
				strV.Format(_T("=%.1lf"), dVal);
				break;
			case VcID:		str.Format(_T("鋳込速度"));
				dVal = chi.val * PhisicalVcUnit;
				strV.Format(_T("=%.2lf"), dVal);
				break;
			//case WidthID:	str.Format(_T("モールド幅"));
			//	dVal = chi.val * PhisicalWidthUnit;
			//	strV.Format(_T("=%.0lf"), dVal);
			//	break;
			//case WidthLID:	str.Format(_T("WidthL"));
			//	dVal = chi.val * PhisicalWidthUnit;
			//	break;
			//case WidthRID:	str.Format(_T("WidthR"));
			//	dVal = chi.val * PhisicalWidthUnit;
			//	break;
			case LengthID:	str.Format(_T("鋳込長"));
				dVal = chi.val * PhisicalLengthUnit;
				strV.Format(_T("=%.2lf"), dVal);
				break;
			default:		
				if( m_gp.nTrendChArgTm[chi.idx] >= 0 && m_gp.nTrendChArgTm[chi.idx] < NumTCTap )
					str.Format(_T("%s"), tcnm->name[m_gp.nTrendChArgTm[chi.idx]]);
				else
					str = _T("");
				dVal = chi.val * PhisicalTempUnit;
				strV.Format(_T("=%.1lf"), dVal);
				break;
			}
			str += strV;
			m_tool.UpdateTipText(str,this);
		}
		// Hit無しならToolTip非表示設定
		else
			m_tool.UpdateTipText(_T(""),this);
		
		// ToolTipにイベントを渡す
		m_tool.RelayEvent(pMsg);
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CBatchGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	clib.GraphPaint(this, dc);
}

void CBatchGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CClientDC dc(this);

	clib.GraphHScroll(this, dc, nSBCode, nPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CBatchGraph::OnTrendLoad(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	clib.InitTimeGraph(this, dc, &m_gp);

	return 0;
}

LRESULT CBatchGraph::OnAppendData(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	clib.DrawTimeChart(dc, &m_data[m_nCurIdx]);

	if( ++m_nCurIdx >= BATCH_DATA_NUM )
	{
		m_nCurIdx = 0;
	}

	if( m_nCount < BATCH_DATA_NUM )
		m_nCount++;

	return 0;
}

LRESULT CBatchGraph::OnFetchDraw(WPARAM, LPARAM)
{
	CClientDC	dc(this);

	clib.DrawFetchLine(dc, m_clFetchColor);

	return 0;
}

LRESULT CBatchGraph::OnTrendChg(WPARAM, LPARAM)
{
	memcpy( clib.m_isTrendIndicate, m_gp.isTrendIndicate, sizeof(clib.m_isTrendIndicate) );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CBatchGraph member function

void CBatchGraph::BatchLoad(LPCTSTR name, StGraphParam *gp)
{
	if(name != NULL)
		m_szFilePath = name;
	memcpy( &m_gp, gp, sizeof(m_gp) );

	SendMessage(WM_BATCHLOAD);
	m_nCurIdx		= 0;
	m_nCount		= 0;
}

void CBatchGraph::AppendData(SampledData *data)
{
	int		idx = m_nCurIdx;
	memcpy(&m_data[idx], data, sizeof(SampledData));

	SendMessage(WM_BATCHREDRAW);
}

void CBatchGraph::Fetch(void)
{
	SendMessage(WM_BATCHFETCH);
}

void CBatchGraph::SetFetchColor(COLORREF col)
{
	m_clFetchColor = col;
}

void CBatchGraph::ChangeArg(int	*pArg)
{
	if(pArg == NULL)
		return;

	memcpy( m_gp.isTrendIndicate, pArg, sizeof(m_gp.isTrendIndicate) );

	SendMessage(WM_BATCHCHG);
}

// Hit Test for displayed chart
int	CBatchGraph::ChartHitTest(POINT pt, StChartHitInfo *chi)
{
	POINT		p1, p2;
	DPOINT		cp;
	double		dis, dis_min = 2.0e0;
	double		ptx, pty;
	BOOL		bCalc;
	int			i, j, idx, i_min = -1;

	// get the mouse coordinates
	ptx = (double)pt.x;
	pty = (double)pt.y;

	// For each trend channel number,
	for (i = 0; i < clib.m_nNumTrendChannel; i++) {
		if (clib.m_nTrendChArgTm[i] < 0 || m_gp.isTrendIndicate[i] == FALSE)
			continue;

		// For each data,
		for( j = 0; j < m_nCount-1; j++ )
		{
			// get the two point.
			p1.x = j; 
			p2.x = j+1; 
			idx = m_nCurIdx - m_nCount + j;
			if( idx < 0 )
				idx += BATCH_DATA_NUM;
			p1.y = clib.ScaleY(clib.m_nTrendChArgTm[i], m_data[idx].val[i]);
			idx = m_nCurIdx - m_nCount + j+1;
			if( idx < 0 )
				idx += BATCH_DATA_NUM;
			p2.y = clib.ScaleY(clib.m_nTrendChArgTm[i], m_data[idx].val[i]);

			// calculate cross point.
			bCalc = clib.CalcNearPoint(p1, p2, pt, &cp);
			if( bCalc )
			{
				// calculate distance from the mouse point to cross point.
				dis = sqrt((((double)ptx-cp.x)*(ptx-cp.x) + ((double)pt.y-cp.y)*(pt.y-cp.y)));

				// if the calculation distance is less than the minimum distance, update information.
				if( dis < dis_min )
				{
					dis_min = dis;
					i_min = i;
					chi->val = m_data[idx].val[i];
//					chi->val = (short)(((clib.ChartHeight-cp.y) * m_gp.tempAxis.scale / clib.ChartMeshHeight + m_gp.tempAxis.zero) / PhisicalTempUnit);
				}
			}
		}
	}
	
	// set hit index
	chi->idx = i_min;
	if( i_min != -1 )
		return TRUE;
	else
		return FALSE;
}

