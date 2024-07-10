// SelGraph.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "SelGraph.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf = theApp.m_cnf;
static CBolog		&bolog = theApp.m_log;

#define	MeshSize		40
#define	ChartWidth		(MeshSize*5)
#define	ChartHeight		(MeshSize*15)
#define	SelGraphData	128


///////////////////////////////////////////////////////////////////////////////
// CSelGraph
IMPLEMENT_DYNAMIC(CSelGraph, CWnd)
CSelGraph::CSelGraph()
{
	m_nCombiIdx		= -1;
	m_nArg1			= -1;
	m_nArg2			= -1;
	m_clBase		= COL_GRAPH_BACK;		// グラフ背景色
	m_clLine		= RGB(210, 180, 140);
	m_clRed			= RGB(205, 0, 0);
	m_clGreen		= RGB(0, 192, 0);
	m_nNumDispData	= 0;
	m_hDispData		= NULL;
	m_pDispData		= NULL;
	m_strTrendOutFilePath = _T("");
	m_nOffset		= 0;
	m_nLength		= 0;
	m_nShift		= 0;

	// setup display data buffer 
	SetupDispDataBuffer();
}

CSelGraph::~CSelGraph()
{
	// destroy display data buffer 
	DestroyDispDataBuffer();
}


BEGIN_MESSAGE_MAP(CSelGraph, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE( WM_REDRAW, OnRedraw )
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CSelGraph message handler

int CSelGraph::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	// clear ordinary chart plane.
	ClearChart(memDC);

	// draw ordinary chart plane.
//	if(m_nCombiIdx >= 0 && m_nCombiIdx < cnf.numCombination)
//		DrawChart(memDC, cnf.tcCombination[m_nCombiIdx].upper,
//						 cnf.tcCombination[m_nCombiIdx].lower);
	if( m_nArg1 >= 0 && m_nArg2 < NumTCTap &&
		m_nArg1 >= 0 && m_nArg2 < NumTCTap)
		DrawChart(memDC, m_nArg1, m_nArg2);

	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();

	return 0;
}

void CSelGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	GraphPaint(dc);
}

LRESULT CSelGraph::OnRedraw(WPARAM wParam, LPARAM lParam)
{
	CClientDC	dc(this);
	CDC			memDC;
	CBitmap		*oldBitmap;
	CRect		rc;

	GetClientRect(rc);
	memDC.CreateCompatibleDC(&dc);
	oldBitmap = memDC.SelectObject(&m_bitmap);

	// clear chart plane. 
	ClearChart(memDC);

	// draw chart plane
	//if(m_nCombiIdx >= 0 && m_nCombiIdx < cnf.numCombination)
	//	DrawChart(memDC, cnf.tcCombination[m_nCombiIdx].upper,
	//					 cnf.tcCombination[m_nCombiIdx].lower);
	if( m_nArg1 >= 0 && m_nArg2 < NumTCTap &&
		m_nArg1 >= 0 && m_nArg2 < NumTCTap)
		DrawChart(memDC, m_nArg1, m_nArg2);

	// map the graph plane.
	dc.BitBlt(0, 0, rc.Width()-2, rc.Height()-2, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CSelGraph member function(protected)

// set up buffer for teaching data
void CSelGraph::SetupDispDataBuffer(void)
{
	// allocate display data buffer from global heap. 
	m_hDispData	= GlobalAlloc(GMEM_MOVEABLE,
							(DWORD)sizeof(SelDispData) * ChartWidth);
	// map the memory block. 
	m_pDispData	= (SelDispData *)GlobalLock(m_hDispData);

}

// destroy buffers of teaching data.
void CSelGraph::DestroyDispDataBuffer(void)
{
	// unmap display data buffer. 
	GlobalUnlock(m_pDispData);
	// release the memory block. 
	GlobalFree(m_hDispData);
}

// PAINT message proc
void CSelGraph::GraphPaint(CDC &dc)
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

// clear ordinary chart plane.
void CSelGraph::ClearChart(CDC &dc)
{
	int	x, y;
	POINT	linePoint[2];
	CRect	rc;

	GetClientRect(&rc);

	// clear memory plane.
	boxFillSolid(dc, m_clBase, 0, 0, rc.Width(), rc.Height());

	//@ 網かけ追加
	boxFillSolid(dc, RGB(200,200,200), m_nOffset + m_nShift, 0, m_nLength, ChartHeight - 1);

	// draw chart mesh.
	linePStart(dc, PS_SOLID, 1, m_clLine, R2_COPYPEN);
	linePoint[0].y = 0; linePoint[1].y = ChartHeight;
	for (x = MeshSize; x < ChartWidth; x += MeshSize) {
		linePoint[0].x = linePoint[1].x = x;
		dc.Polyline(linePoint, 2);
	}
	linePoint[0].x = 0; linePoint[1].x = ChartWidth;
	for (y = MeshSize; y < ChartHeight; y += MeshSize) {
		linePoint[0].y = linePoint[1].y = y;
		dc.Polyline(linePoint, 2);
	}
	linePEnding(dc);
}

// draw ordinary chart plane.
void CSelGraph::DrawChart(CDC &dc, int upper, int lower)
{
	int	x;
	POINT	linePoint[2];

	linePStart(dc, PS_SOLID, 1, m_clRed, R2_COPYPEN);
	// for each data of upper thermo couple in display buffer,
	for (x = 0; x < m_nNumDispData - 1 && m_pDispData[x].yPos[upper] >= 0; x++) {
		// draw a chart line.
		linePoint[0].x = x; linePoint[1].x = x + 1;
		linePoint[0].y = m_pDispData[x].yPos[upper]; linePoint[1].y = m_pDispData[x+1].yPos[upper];
		dc.Polyline(linePoint, 2);
	}
	linePEnding(dc);
	linePStart(dc, PS_SOLID, 1, m_clGreen, R2_COPYPEN);
	// for each data of lower thermo couple in display buffer,
	for (x = 0; x < m_nNumDispData - 1 && m_pDispData[x].yPos[lower] >= 0; x++) {
		// draw a chart line.
		linePoint[0].x = x; linePoint[1].x = x + 1;
		linePoint[0].y = m_pDispData[x].yPos[lower]; linePoint[1].y = m_pDispData[x+1].yPos[lower];
		dc.Polyline(linePoint, 2);
	}
	linePEnding(dc);
}

// calculate of scale y cordinate for temperature displaying.
int	CSelGraph::ScaleY(int temp)
{
	double	v;

	v	= MeshSize * (0.1 * temp - cnf.m_stTempAxis.zero) / cnf.m_stTempAxis.scale;
	if (v < 0)						v	= 0.0;
	else if (v > ChartHeight)		v	= ChartHeight;

	return	(int)(ChartHeight - v);
}


///////////////////////////////////////////////////////////////////////////////
// CSelGraph member function(public)

// redraw chart
void CSelGraph::RedrawChart(int nArg1, int nArg2)
{
	m_nArg1		= nArg1;
	m_nArg2		= nArg2;
	if( m_nArg1 < 0 || m_nArg1 >= NumTCTap ||
		m_nArg2 < 0 || m_nArg2 >= NumTCTap)
		m_nNumDispData = 0;

	if( IsWindow(m_hWnd) )
		SendMessage(WM_REDRAW);
}

// load display data.
void CSelGraph::LoadDispData(LPCTSTR pName, int offset, int length, float fFilterTemp, int shift)
{
	FILE	*fd;
	FILE	*fdBk;
	DataFileHeader	header;
	SampledData		buf;
	SampledData		bufBk;
	SampledData		data;
	float	fFilter;
	long	idx, n;
	int		i, j, ch;

	//@ 網かけ追加
	m_nOffset = 0;
	m_nLength = 0;
	m_nShift = 0;

	// initialize the numbers of data in buffer.
	m_nNumDispData	= 0;

	// open logging data file. return if failed.
	if ((fd = _tfopen(pName, _T("rb"))) == NULL) 
	{
		// Tempフォルダも探す
		pName = bolog.GetTempLogFilePath(pName);
		if(pName == NULL) return;
		if ((fd = _tfopen(pName, _T("rb"))) == NULL) 
			return;
	}

	// calculate reading point.
	idx	= offset + length - SelGraphData;
	if( idx < 0 ) idx = 0;

	// read data file header.
	fread(&header, sizeof(header), 1, fd);

	/////////////////////////////////////////////////////////////////////////

	// filter calc for ver6.20
	if( header.header.version >= 620 && m_strTrendOutFilePath.GetLength() > 0 )
	{
		// open the trend data file. return 0 if failed.
		if ((fdBk = _tfopen( m_strTrendOutFilePath, _T("wb+") )) != NULL)
		{
			fwrite(&header, sizeof(header), 1, fdBk);
			// read data
			if( fread(&bufBk, sizeof(SampledData), 1, fd) == 1)
			{
				fwrite(&bufBk, sizeof(SampledData), 1, fdBk);
				for( i = 0; i < (offset + ChartWidth); i++ )
				{
					if( fread(&buf, sizeof(SampledData), 1, fd) != 1)
						break;

					// for each A/D channel,
					for(j = 0; j < NumADChannel; j++)
					{
						switch(header.header.arg[j])
						{
						case -1:
						case SNID:		// SN Channel No.
						case LevelID:	// Level Channel No.
						case VcID: 		// P/R Speed Channel No.
						case LengthID:	// Length Channel No.
							fFilter = 1.0f;	break;
						default:		// Temp Channel No.
							fFilter = fFilterTemp;break;
						}
						if( header.header.arg[j] >= LastID )
							fFilter = 1.0f;
						bufBk.val[j] =  (short)((float)bufBk.val[j] * (1.0f - fFilter) +
												(float)buf.val[j] * fFilter);
					}
					fwrite(&bufBk, sizeof(SampledData), 1, fdBk);
				}
			}
			fclose(fd);
			fd = fdBk;
		}
	}

	/////////////////////////////////////////////////////////////////////////
#ifdef Compatibility
	if( header.header.version <= 500 )
	{
		// seek to the head of data.
		fseek(fd, idx * sizeof(SampledDataV5) + sizeof(DataFileHeader), 0);

		// for each sampled data,
		for (n = 0; n < ChartWidth; n++) {
			// if no more data, break.
			if (fread(&data, sizeof(SampledDataV5), 1, fd) != 1)
				break;
			for( i = NumADChannelV5; i < NumADChannel; i++ )
				data.val[i] = 0;
			// for each field in teaching data,
			for (i = 0; i < NumInput; i++) {
				// get the assigned A/D channel.
				ch	= header.header.adCh[i];
				// store the field.
				m_pDispData[n].yPos[i]	= (ch >= 0) ?	ScaleY(data.val[ch]) : -1;
			}
		}
	}
	else
	{
		// seek to the head of data.
		fseek(fd, idx * sizeof(SampledData) + sizeof(DataFileHeader), 0);

		// for each sampled data,
		for (n = 0; n < ChartWidth; n++) {
			// if no more data, break.
			if (fread(&data, sizeof(data), 1, fd) != 1)
				break;
			// for each field in teaching data,
			for (i = 0; i < NumInput; i++) {
				// get the assigned A/D channel.
				ch	= header.header.adCh[i];
				// store the field.
				m_pDispData[n].yPos[i]	= (ch >= 0) ?	ScaleY(data.val[ch]) : -1;
			}
		}
	}
#else
	// seek to the head of data.
	fseek(fd, idx * sizeof(SampledData) + sizeof(DataFileHeader), 0);

	// for each sampled data,
	for (n = 0; n < ChartWidth; n++) {
		// if no more data, break.
		if (fread(&data, sizeof(data), 1, fd) != 1)
			break;
		// for each field in teaching data,
		for (i = 0; i < NumInput; i++) {
			// get the assigned A/D channel.
			ch	= header.header.adCh[i];
			// store the field.
			m_pDispData[n].yPos[i]	= (ch >= 0) ?	ScaleY(data.val[ch]) : -1;
		}
	}
#endif

	// close the file.
	fclose(fd);

	// store the number of read data.
	m_nNumDispData	= n;

	//@ 網かけ追加
	m_nOffset = offset - idx;
	m_nLength = length;
	m_nShift = shift;
}

// 網かけを+方向にシフト
void CSelGraph::DataShiftP(long& shift)
{
	m_nShift = shift;
    m_nShift++;
	if( (m_nOffset + m_nShift + m_nLength + 2) > m_nNumDispData )
	{
		m_nShift = (m_nNumDispData - m_nOffset - m_nLength - 2);
	}
	if( IsWindow(m_hWnd) )
		SendMessage(WM_REDRAW);
	shift = m_nShift;
}

// 網かけを-方向にシフト
void CSelGraph::DataShiftM(long& shift)
{
	m_nShift = shift;
	m_nShift--;
	if( (m_nOffset + m_nShift) < 0 )
	{
		m_nShift = (0 - m_nOffset);
	}
	if( IsWindow(m_hWnd) )
		SendMessage(WM_REDRAW);
	shift = m_nShift;
}


