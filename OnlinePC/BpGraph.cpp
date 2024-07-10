// BpGraph.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BpGraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	MeshSize		40


// CBpGraph
IMPLEMENT_DYNAMIC(CBpGraph, CWnd)
CBpGraph::CBpGraph()
{
	m_nTrendXPos = -1;
	m_nTrendShiftCnt = 0;
	m_clBase = COL_GRAPH_BACK;		// グラフ背景色
	m_clLine = RGB(210, 180, 140);
	m_clPattOrd  = RGB(0, 192, 0);
	m_clPattAlm  = RGB(205, 0, 0);
	m_clCorrOrd  = RGB(0, 0, 205);
	m_clCorrAlm  = RGB(128, 128, 0);
	memset(m_nPreYPos, 0, sizeof(m_nPreYPos));
}

CBpGraph::~CBpGraph()
{
}

BEGIN_MESSAGE_MAP(CBpGraph, CWnd)
	ON_WM_PAINT()
	ON_MESSAGE( WM_APPENDTREND, OnAppendTrend )
	ON_MESSAGE( WM_REDRAW_BP, OnRedrawTrend )
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBpGraph message handler

int CBpGraph::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CClientDC	dc(this);
	CRect		rcPaint;
	CDC			memDC;
	CBitmap		*oldBitmap;

	GetClientRect(rcPaint);

	if ( FALSE == m_bitmap.CreateCompatibleBitmap(&dc, rcPaint.Width(), rcPaint.Height()) )
		return FALSE;
	
	memDC.CreateCompatibleDC(&dc);
	oldBitmap = memDC.SelectObject(&m_bitmap);
	ClearTrendWave(memDC);
	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();

	return 0;
}

void CBpGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	GraphPaint(dc);
}

void CBpGraph::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CClientDC dc(this);

//	clib.GraphHScroll(this, dc, nSBCode, nPos);

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CBpGraph::OnAppendTrend(WPARAM wParam, LPARAM lParam)
{
	CClientDC	dc(this);
	CDC			memDC;
	CBitmap		*oldBitmap;
	CRect		rc;

	GetClientRect(rc);
	memDC.CreateCompatibleDC(&dc);
	oldBitmap = memDC.SelectObject(&m_bitmap);

	switch(wParam) {
	case 0:
		// if the tail of graph, shift the window.
		if (m_nTrendXPos == rc.Width() - 1)
			ShiftTrendWave(memDC),	m_nTrendXPos--,	m_nTrendShiftCnt++;
		break;
	case 1:
		// append output of ordinary pattern neural network.
		DrawTrendWave(memDC, m_clPattOrd, wParam, LOWORD(lParam));
		break;
	case 2:
		// append output of alarm pattern neural network.
		DrawTrendWave(memDC, m_clPattAlm, wParam, LOWORD(lParam));
		break;
	case 3:
		// append output of ordinary correlation neural network.
		DrawTrendWave(memDC, m_clCorrOrd, wParam, LOWORD(lParam));
		break;
	case 4:
		// append output of alarm correlation neural network.
		DrawTrendWave(memDC, m_clCorrAlm, wParam, LOWORD(lParam));
		break;
	case 5:
		// increase drawing position.
		m_nTrendXPos++;
		// map the graph plane.
		dc.BitBlt(0, 0, rc.Width()-2, rc.Height()-1, &memDC, 0, 0, SRCCOPY);
		break;
	}
	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();

	return 0;
}

LRESULT CBpGraph::OnRedrawTrend(WPARAM wParam, LPARAM lParam)
{
	CClientDC	dc(this);
	CRect		rcPaint;
	CDC			memDC;
	CBitmap		*oldBitmap;

	GetClientRect(rcPaint);

	memDC.CreateCompatibleDC(&dc);
	oldBitmap = memDC.SelectObject(&m_bitmap);
	ClearTrendWave(memDC);
	dc.BitBlt(0, 0, rcPaint.Width()-2, rcPaint.Height()-1, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();

	// initialize trend display position.
	m_nTrendXPos		= -1;
	m_nTrendShiftCnt	= 0;

	return 0;
}

void CBpGraph::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
//	clib.GraphLButtonDown(this, dc, point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CBpGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
//	clib.GraphMouseMove(this, dc, point);

	CWnd::OnMouseMove(nFlags, point);
}

void CBpGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
//	clib.GraphLButtonUp(dc);

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CBpGraph::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// mouse cursor type check
	SetCursor(::LoadCursor(NULL, IDC_CROSS));
	return TRUE;

//	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////////////
// CBpGraph member function(protected)

// PAINT message proc
void CBpGraph::GraphPaint(CDC &dc)
{
	CRect	rcPaint, setRect;
	CBitmap	*pOldBitmap;
	CDC		memDC;

	// selected bitmap
	if ((HBITMAP)NULL == m_bitmap) 
		return;

	// create mem dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = (CBitmap *)memDC.SelectObject(&m_bitmap);

	//
	GetClientRect(rcPaint);
	dc.BitBlt(	rcPaint.left, rcPaint.top,
				rcPaint.right - rcPaint.left,
				rcPaint.bottom - rcPaint.top,
				&memDC, rcPaint.left, rcPaint.top, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);
}

// initialize output trend graph plane.
void CBpGraph::ClearTrendWave(CDC &dc)
{
	int	x, y;
	CPoint	linePoint[2];
	CRect	rc;

	GetClientRect(&rc);

	// clear memory plane. 
	boxFillSolid(dc, m_clBase, 0, 0, rc.Width(), rc.Height());

	linePStart(dc, PS_SOLID, 1, m_clLine, R2_COPYPEN);
	// draw graph mesh.
	linePoint[0].y = 0; linePoint[1].y = rc.Height();
	for (x = MeshSize; x < rc.Width(); x += MeshSize) {
		linePoint[0].x = linePoint[1].x = x;
		Polyline(dc, linePoint, 2);
	}
	linePoint[0].x = 0; linePoint[1].x = rc.Width();
	for (y = MeshSize; y < rc.Height(); y += MeshSize) {
		linePoint[0].y = linePoint[1].y = y;
		Polyline(dc, linePoint, 2);
	}
	linePEnding(dc);
}

// append output neural network.
void CBpGraph::DrawTrendWave(CDC &dc, int color, int cmd, int yPos)
{
	POINT	linePoint[2];

	if (m_nTrendXPos >= 0) {
		linePoint[0].x = m_nTrendXPos - 1;  linePoint[1].x = m_nTrendXPos;
		linePoint[1].y = yPos;
		switch(cmd) {
		case 1:
			linePoint[0].y = m_nPreYPos[0];
			polyPline(dc, PS_SOLID, 1, color, R2_COPYPEN, linePoint, 2);
			m_nPreYPos[0]	= yPos;
			break;
		case 2:
			linePoint[0].y = m_nPreYPos[1];
			polyPline(dc, PS_SOLID, 1, color, R2_COPYPEN, linePoint, 2);
			m_nPreYPos[1]	= yPos;
			break;
		case 3:
			linePoint[0].y = m_nPreYPos[2];
			polyPline(dc, PS_SOLID, 1, color, R2_COPYPEN, linePoint, 2);
			m_nPreYPos[2]	= yPos;
			break;
		case 4:
			linePoint[0].y = m_nPreYPos[3];
			polyPline(dc, PS_SOLID, 1, color, R2_COPYPEN, linePoint, 2);
			m_nPreYPos[3]	= yPos;
			break;
		}
	}
	else {
		switch(cmd) {
		case 1:		m_nPreYPos[0]	= yPos;		break;
		case 2:		m_nPreYPos[1]	= yPos;		break;
		case 3:		m_nPreYPos[2]	= yPos;		break;
		case 4:		m_nPreYPos[3]	= yPos;		break;
		}
	}
}

// shift the window.
void CBpGraph::ShiftTrendWave(CDC &dc)
{
	CRect	winRect, clipRect, updateRect;
	int	x;
	int	y;
	CPoint	linePoint[2];

	GetClientRect(&winRect);
	x = winRect.Width() - 1;
	clipRect	= winRect;
	dc.ScrollDC(-1, 0, &winRect, &clipRect, NULL, &updateRect);
	linePoint[0].x = 	linePoint[1].x = x;
	linePoint[0].y = 0; linePoint[1].y = winRect.Height();
	polyPline(dc, PS_SOLID, 1, m_clBase, R2_COPYPEN, linePoint, 2);

	linePStart(dc, PS_SOLID, 1, m_clLine, R2_COPYPEN);
	linePoint[0].x = x - 1;	linePoint[1].x = x;

	for (y = MeshSize; y < winRect.Height(); y += MeshSize) {
		linePoint[0].y = linePoint[1].y = y;
		Polyline(dc, linePoint, 2);
	}
	if ((m_nTrendShiftCnt + x) % MeshSize == 0) {
		linePoint[0].x =  	linePoint[1].x = x;
		linePoint[0].y = 0; linePoint[1].y = winRect.Height();
		Polyline(dc, linePoint, 2);
	}
	linePEnding(dc);
}

///////////////////////////////////////////////////////////////////////////////
// CBpGraph member function(public)

void CBpGraph::AppendTrend(	double dOrdPattOut, double dAlmPattOut, 
						    double dOrdCorrOut, double dAlmCorrOut)
{
	CRect	rc;
	GetClientRect(&rc);

	SendMessage(WM_APPENDTREND, 0, 0L);
	SendMessage(WM_APPENDTREND, 1, (long)(rc.Height() * (1 - dOrdPattOut)));
	SendMessage(WM_APPENDTREND, 2, (long)(rc.Height() * (1 - dAlmPattOut)));
	SendMessage(WM_APPENDTREND, 3, (long)(rc.Height() * (1 - dOrdCorrOut)));
	SendMessage(WM_APPENDTREND, 4, (long)(rc.Height() * (1 - dAlmCorrOut)));
	SendMessage(WM_APPENDTREND, 5, 0L);
}

void CBpGraph::RedrawTrend(void)
{
	SendMessage(WM_REDRAW_BP);
}




