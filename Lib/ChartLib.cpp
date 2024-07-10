#include "../include/ChartLib.h"
#include <math.h>


CChartLib::CChartLib()
{
	m_LeftX				= 0;
	m_RightX			= 0;
	m_LeftX_Bk			= 0;
	m_RightX_Bk			= 0;
	ChartWidth			= 900;
	ChartHeight			= 450;
	ChartMeshXCnt		= 34;
	ChartMeshYCnt		= 20;
	ChartMeshWidth		= 30;		
	ChartMeshHeight		= 30;		
	m_nDataCount		= 0;
	m_nStartDataNo		= 0;
	m_nNumTrendChannel	= 0;
	m_nNumSimChannel	= 0;
	m_nTimeCounter		= 0;
	m_nChLineWd			= 1;
	m_nTmChLineWd		= 1;
	m_nTmCh2LineWd		= 1;
	m_nScooterPos		= 0;
	m_strSelFileName	= _T("");
	m_strSimFileName	= _T("");
	m_strTrendOutFileName = _T("");
	m_isLineUp			= FALSE;
	m_isLineDown		= FALSE;
	m_isCutAreaSelect	= FALSE;
	m_isSelecting		= FALSE;
	m_bTrack			= FALSE;
	m_isNeuralCal		= FALSE;
	m_isScooter			= FALSE;
	m_bChScaleDisp		= TRUE;		
	m_clClear			= RGB(230,230,230);		// グラフ背景色
//	m_clClear			= RGB(30,30,30);		// グラフ背景色
	m_clAxis			= RGB(210,180,140);
	m_clText			= RGB(0,0,0);
	m_clRed				= RGB(255,0,0);
	m_bFilterSwitch		= FALSE;
	m_fFilterTemp		= 1.0f;
	m_fFilterSn			= 1.0f;
	m_fFilterLv			= 1.0f;
	m_fFilterPr			= 1.0f;
	m_fFilterLen		= 1.0f;
	m_nCutAreaLen		= 128;
	memset(&m_stDispPos, 0, sizeof(m_stDispPos));
	memset(&m_stPreDispPos, 0, sizeof(m_stPreDispPos));

	// allocate colors. 
//	allocColorTones(AllTrendArea, 150, 52, m_clTones);
//	allocColorTones(NumADChannel, 150, 52, m_clTonesTm);
//@	allocColorTones(AllTrendArea, 150, 52, m_clTones);
//@	allocColorTones(NumADChannelDisp, 150, 52, m_clTonesTm);
	allocColorTones(AllTrendArea, 150, 15, m_clTones);
	allocColorTones(NumADChannelDisp, 150, 15, m_clTonesTm);

	m_pDispBufPtr = allocDataBuf(DispDataBufSize, GMEM_MOVEABLE);
	lockDataBuf(m_pDispBufPtr);
}

CChartLib::~CChartLib()
{
	unlockDataBuf(m_pDispBufPtr);
	freeDataBuf(m_pDispBufPtr);
}

// scale
int	CChartLib::ScaleY(int arg, int val)
{
	double	v	= 0.0;

	if (arg >= TCTapID && arg < LevelID)
		v	= ChartMeshHeight * (PhisicalTempUnit * val - m_stTempAxis.zero) / m_stTempAxis.scale;
	else if (arg == LevelID)
		v	= ChartMeshHeight * (PhisicalLevelUnit * val - m_stLevelAxis.zero) / m_stLevelAxis.scale;
	else if (arg == VcID)
		v	= ChartMeshHeight * (PhisicalVcUnit * val - m_stVcAxis.zero) / m_stVcAxis.scale;
	else if (arg == SNID)
		v	= ChartMeshHeight * (PhisicalSNUnit * val - m_stSNAxis.zero) / m_stSNAxis.scale;
//	else if (arg == WidthID)
//		v	= ChartMeshHeight * (PhisicalWidthUnit * val - m_stWidthAxis.zero) / m_stWidthAxis.scale;

	if (v < 0)						v	= 0.0;
	else if (v > ChartHeight)		v	= ChartHeight;

	return	(int)(ChartHeight - v);
}

// draw trend chart in specified range.
void CChartLib::DrawChart(CDC &dc, int pen, int sIdx, int eIdx)
{
	DisplayData *p;
	DisplayData *q;
	int	idx, x, cnt, cntMax, lastIdx;
	BOOL lastDisp = FALSE;
	POINT *linePoint;

	if (eIdx - sIdx != ChartWidth && m_nNumDispData >= ChartWidth) {
		// right scroll ?
		if (sIdx == 0) {
			idx = m_nBufHeadIdx;
			lastIdx = idx;
			eIdx++;
			if (lastIdx + eIdx - sIdx > ChartWidth) {
				lastDisp = TRUE;
				eIdx--;
			}
		}
		// left scrool
		else {
			idx = m_nBufTailIdx - ScrollPitch;
			// head over ?
			if (idx < 0) {
				// ring buffer last point
				idx = ChartWidth - ScrollPitch;
			}
			lastIdx = idx - 1;
			if (lastIdx < 0)
				lastIdx = ChartWidth - 1;
		}
		// last data disp ?
		if (m_nReadDataCnt < ScrollPitch) {
			cntMax = m_nReadDataCnt;
		}
		// normal data disp
		else {
			cntMax = eIdx - sIdx + 1;
		}
	}
	// all chart width disp
	else {
		idx = sIdx;
		lastIdx = idx;
		idx++; eIdx--;
		// data count < chart width ?
		if (m_nNumDispData < ChartWidth)
			cntMax = m_nNumDispData;
		else
			cntMax = eIdx - sIdx + 1;
	}

	p = PtrInMTab(DisplayData, m_pDispBufPtr, lastIdx);
	linePoint = (POINT *)malloc(sizeof(POINT) * cntMax);
	linePStart(dc, PS_SOLID, m_nChLineWd, m_clTones[pen], R2_COPYPEN);
	for (x = sIdx, cnt = 0; x < eIdx && cnt < cntMax; x++, idx++, cnt++) {
		if (p->yPos[pen] >= 0) {
			linePoint[cnt].x = x; linePoint[cnt].y = p->yPos[pen];
		}
		else {
			linePoint[cnt].x = x; linePoint[cnt].y = 0;
		}
		p = PtrInMTab(DisplayData, m_pDispBufPtr, idx);
	}
	dc.Polyline(linePoint, cnt);
	free(linePoint);
	// last point disp right scroll
	// right scrool last point disp ?
	if (lastDisp == TRUE) {
		q = PtrInMTab(DisplayData, m_pDispBufPtr, 0);
		lineP(dc, x, p->yPos[pen], x + 1, q->yPos[pen]);
	}
	linePEnding(dc);
}

// file size pickup
DWORD CChartLib::FileSizePkup(LPCTSTR name)
{
	struct _stat statbuf;
	int result;

	// file info. pickup
	result = _tstat(name, &statbuf);
	// return chck
	if (result != 0) {
		// error return
		return (DWORD)0;
	}
	else {
		// normal return
		return (DWORD)statbuf.st_size;
	}
}

// draw trend chart axis.
void CChartLib::DrawChartAxis(CDC &dc, int sIdx, int eIdx)
{
	int	x, y, xo;
	struct tm	*startT;
	time_t	t;
	int	roundSec;
	POINT	linePoint[2];

	xo = ChartMeshWidth*4 -
			((int)((m_nDispStartTime - m_nDispStartTimeIni) /
											(2 * 60 * m_stTimeAxis.scale)) % 2) * ScrollPitch;
	roundSec = (int)(xo/ScrollPitch * 2 * m_stTimeAxis.scale * 60);

	// draw the time of axis.
	linePStart(dc, PS_SOLID, 1, m_clAxis, R2_COPYPEN);
	linePoint[0].y = ChartHeight + 1; linePoint[1].y = ChartHeight + 3;
	for (t = m_nDispStartTime + roundSec, x = xo; x < ChartWidth;
							x += ChartMeshWidth*4, t += (time_t)(60 * 4 * m_stTimeAxis.scale)) {
		if (x >= sIdx && x <= eIdx) {
			startT	= localtime(&t);
			PrintText(dc, 0, m_clText, x - 30, ChartHeight + 5, _T("%02d.%2d.%2d"),
				startT->tm_year % 100, startT->tm_mon + 1, startT->tm_mday);
			PrintText(dc, 0, m_clText, x - 30, ChartHeight + 25, _T("  %02d:%02d"),
				startT->tm_hour, startT->tm_min);
			linePoint[0].x = linePoint[1].x = x;
			dc.Polyline(linePoint, 2);
		}
	}

	// draw vertical mesh line.
	linePoint[0].y = 0; linePoint[1].y = ChartHeight;
	for (x = sIdx; x < eIdx; x += ChartMeshWidth) {
		linePoint[0].x = linePoint[1].x = x;
		if( m_bChScaleDisp )
			dc.Polyline(linePoint, 2);
	}
	// draw horizontal mesh line.
	linePoint[0].x = sIdx; linePoint[1].x = eIdx;
	for (y = -1; y < ChartHeight; y += ChartMeshHeight) {
		linePoint[0].y = linePoint[1].y = y;
		if( m_bChScaleDisp )
			dc.Polyline(linePoint, 2);
	}
	linePEnding(dc);
}

// draw trend chart in specified area.
void CChartLib::DrawTrendWave(CDC &dc, int sx, int ex, int nChannel)
{
	int	i;
	CRect	rc;
//	DWORD	linePoint, pointMax;

	// clear memory area.
	rc.left = sx;
	rc.top = 0;
	rc.right = ex;
	rc.bottom = ChartPlaneHeight;
	dc.FillSolidRect(&rc, m_clClear);

	// draw trend chart axis.
	DrawChartAxis(dc, sx, ex);

	for (i = 0; i < nChannel; i++) {
		if (m_nTrendChArg[i] < 0)
			continue;
		DrawChart(dc, i, sx, ex);
	}

	// non stady area line display
/*	pointMax = (ChartMeshWidth*5) / m_nDisplayPitch;
	if (m_nFileStartIdx >= ChartWidth)
		linePoint = pointMax - (m_nFileStartIdx - ChartWidth);
	else
		linePoint = pointMax;

	if (linePoint <= pointMax) {
		line(dc, PS_SOLID, 1, m_clRed, R2_COPYPEN,
								(int)linePoint, 0, (int)linePoint, (int)ChartHeight);
	}*/
}

// PAINT message proc
void CChartLib::GraphPaint(CWnd *wnd, CDC &dc)
{
	CRect	rcPaint, setRect;
	CBitmap	*pOldBitmap;
	CDC		memDC;

	// selected bitmap
	if (wnd == NULL || (HBITMAP)NULL == m_bitmap) 
		return;

	// create memory dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = (CBitmap *)memDC.SelectObject(&m_bitmap);

	// get client area
	wnd->GetClientRect(rcPaint);
	dc.BitBlt(	rcPaint.left, rcPaint.top,
				rcPaint.right - rcPaint.left,
				rcPaint.bottom - rcPaint.top,
				&memDC, rcPaint.left, rcPaint.top, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);

	// cut area select?
	if (m_isCutAreaSelect == TRUE) {
		// pickup redisplay area
		setRect.left   = rcPaint.left;
		setRect.right  = rcPaint.right;
		setRect.top    = rcPaint.top;
		setRect.bottom = rcPaint.bottom;

		if (setRect.left < m_LeftX)
			setRect.left = m_LeftX;
		if (setRect.right > m_RightX)
			setRect.right = m_RightX;
		if (setRect.bottom > ChartHeight)
			setRect.bottom	= ChartHeight;

		// redisplay
		dc.InvertRect(&setRect);
	}

	// Is Scooter?
	if( m_isScooter == TRUE )
	{
		// draw scooter line
		line(dc, PS_SOLID, 1, m_clText, R2_COPYPEN, 
			rcPaint.left + m_nScooterPos, rcPaint.top, rcPaint.left + m_nScooterPos, rcPaint.top + ChartHeight );
	}
}

// IDM_TRENDLOAD message proc
void CChartLib::GraphIdmTrendLoad(CWnd *wnd, CDC &dc, LPCTSTR name, StGraphParam *pGPrm, LPCTSTR simName/*=NULL*/)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	CRect	rc, rcPaint;
	int		i;
//	DWORD	linePoint, pointMax;
	int		loadFileSize;

	if( wnd == NULL || (HBITMAP)NULL == m_bitmap )
	{
		wnd->GetClientRect(rcPaint);
		ChartWidth = rcPaint.Width();
		ChartHeight = rcPaint.Height() - 60;
		if ( FALSE == m_bitmap.CreateCompatibleBitmap(&dc, ChartPlaneWidth, ChartPlaneHeight) )
			return;
	}

	// input data
	m_nNumTrendChannel	= pGPrm->numChannel;
	m_nNumSimChannel	= pGPrm->neuralChannel;
	m_nSamplePeriod		= pGPrm->samplePeriod;
	m_nDispStartTime	= pGPrm->dispStartTime;
	m_isNeuralCal		= pGPrm->isNeuralCal;
	memcpy( m_nTrendChArg,	pGPrm->nTrendChArg, sizeof(m_nTrendChArg));
	memcpy( &m_stTimeAxis,	&pGPrm->timeAxis,	sizeof(m_stTimeAxis));
	memcpy( &m_stTempAxis,	&pGPrm->tempAxis,	sizeof(m_stTempAxis));
	memcpy( &m_stLevelAxis, &pGPrm->levelAxis,	sizeof(m_stLevelAxis));
	memcpy( &m_stVcAxis,	&pGPrm->vcAxis,		sizeof(m_stVcAxis));
	memcpy( &m_stSNAxis,	&pGPrm->snAxis,		sizeof(m_stSNAxis));
//	memcpy( &m_stWidthAxis, &pGPrm->widthAxis,	sizeof(m_stWidthAxis));

	// initialize data
	m_nBufHeadIdx		= 0;
	m_nBufTailIdx		= 0;
	m_nReadDataCnt		= ScrollPitch;
	m_nDispStartTimeIni = m_nDispStartTime;
	m_nDisplayPitch		= (int)(m_stTimeAxis.scale / (m_nSamplePeriod * 0.001 / 2.0) + 0.5);

	// load trend data into memory buffer.
	m_nNumDispData		= (int)LoadTrendFile(m_pDispBufPtr, name,
							0, 0, ChartWidth, m_nDisplayPitch);
	m_nFileStartIdx		= m_nNumDispData;

	// load simulation
	if (m_nNumSimChannel > 0 && m_isNeuralCal == TRUE) {
		// read simulation data
		LoadSimuFile(m_pDispBufPtr, simName, 0, 0, ChartWidth, m_nDisplayPitch);
	}

	// pickup load file size
	loadFileSize		= FileSizePkup(name);
	
#ifdef Compatibility
	FILE *fd;
	DataFileHeader	header;

	// open the trend data file. return 0 if failed.
	if ((fd = _tfopen( name, _T("rb") )) == NULL)
		return;

	// load data file header.
	fread(&header, sizeof(header), 1, fd);

	if( header.header.version <= 500 )
		// load file record size calc.
		m_nLoadFileRecordCount = (WORD)((loadFileSize -
								(DWORD)sizeof(DataFileHeader)) /
										(DWORD)sizeof(SampledDataV5));
	else
		// load file record size calc.
		m_nLoadFileRecordCount = (WORD)((loadFileSize -
								(DWORD)sizeof(DataFileHeader)) /
										(DWORD)sizeof(SampledData));

	fclose(fd);
#else
	// load file record size calc.
	m_nLoadFileRecordCount = (WORD)((loadFileSize -
							(DWORD)sizeof(DataFileHeader)) /
									(DWORD)sizeof(SampledData));
#endif

	// file siez pickup & scroll count calc.
	m_nScrollCount = (int)(m_nLoadFileRecordCount /
							(ScrollPitch * (DWORD)m_nDisplayPitch) -
									(ChartWidth / ScrollPitch));
	// set scroll count(width)
	if( m_nScrollCount <= 0 )
		m_nScrollCount = 0;
	wnd->SetScrollRange(SB_HORZ, 0, m_nScrollCount, FALSE);

	// set scroll position
	m_nScrollVal = 0;
	wnd->SetScrollPos(SB_HORZ, m_nScrollVal, TRUE);

	// create compatible dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = memDC.SelectObject(&m_bitmap);

	// clear memory plane.
	wnd->GetClientRect(rc);
	memDC.FillSolidRect(rc, m_clClear);

	// draw chart axis.
	DrawChartAxis(memDC, 0, ChartWidth);

	// draw trend chart in specified range.
	for (i = 0; i < m_nNumTrendChannel; i++) {
		if (m_nTrendChArg[i] < 0)
			continue;
		DrawChart(memDC, i, 0, m_nNumDispData);
	}

	// neural cal end ?
	if (m_isNeuralCal) {
		// neural data disp
		for (i = m_nNumTrendChannel; i < m_nNumTrendChannel + m_nNumSimChannel; i++) {
			DrawChart(memDC, i, 0, m_nNumDispData);
		}
	}

	// non stady area line display
/*	pointMax = (ChartMeshWidth*5) / m_nDisplayPitch;
	if (m_nFileStartIdx >= ChartWidth)
		linePoint = pointMax - (m_nFileStartIdx - ChartWidth);
	else
		linePoint = pointMax;

	if (linePoint <= pointMax) {
		line(memDC, PS_SOLID, 1, m_clRed, R2_COPYPEN,
						(int)linePoint, 0, (int)linePoint, (int)ChartHeight);
	}*/

	dc.BitBlt(0, 0, ChartPlaneWidth, ChartPlaneHeight, &memDC, 0, 0, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);

	// select area clear
	GraphSelectAreaClear(wnd);
}

// SB_LINEUP message proc
void CChartLib::GraphSbLineUp(CWnd *wnd)
{
	int		mIdx;
	DWORD	fIdx;

	// set scroll flag ng
	m_isLineUp = FALSE;
	if (m_isCutAreaSelect == TRUE)
	{
		// select area clear
		GraphSelectAreaClear(wnd);
	}
	// data count > chart width ?
	if (m_nNumDispData >= ChartWidth && m_nFileStartIdx - ChartWidth > 0) {
		// buffer head index calc. scroll pitch dec.
		m_nBufHeadIdx -= ScrollPitch;
		if (m_nBufHeadIdx < 0)
			m_nBufHeadIdx = ChartWidth - ScrollPitch;
		// buffer tail index calc. scroll pitch dec.
		m_nBufTailIdx -= ScrollPitch;
		if (m_nBufTailIdx < 0)
			m_nBufTailIdx = ChartWidth - ScrollPitch;
		// file read index next
		m_nFileStartIdx -= ScrollPitch;
		// file read record calc.
		fIdx = (m_nFileStartIdx - ChartWidth) * m_nDisplayPitch;
		// data set memory index calc.
		mIdx = m_nBufHeadIdx;
		// data read */
		m_nReadDataCnt	= (int)LoadTrendFile(m_pDispBufPtr, m_strSelFileName,
				fIdx, mIdx, (DWORD)ScrollPitch, m_nDisplayPitch);
		if (m_nNumSimChannel > 0 && m_isNeuralCal == TRUE) {
			// read simulation data
			LoadSimuFile(m_pDispBufPtr, m_strSimFileName,
				fIdx, mIdx, (DWORD)ScrollPitch, m_nDisplayPitch);
		}
		if (--m_nScrollVal < 0) m_nScrollVal = 0;
		// set scroll flag ok
		m_isLineUp = TRUE;
		m_nDispStartTime -=
					(time_t)(m_stTimeAxis.scale * 60 * ScrollPitch / ChartMeshWidth);
	}
}

// SB_LINEDOWN message proc
void CChartLib::GraphSbLineDown(CWnd *wnd)
{
	int		mIdx;
	DWORD	fIdx;

	// set scroll flag ng
	m_isLineDown = FALSE;
	if (m_isCutAreaSelect == TRUE) {
		// select area clear
		GraphSelectAreaClear(wnd);
	}
	// data count > chart width ?
	if (m_nNumDispData >= ChartWidth && m_nReadDataCnt >= ScrollPitch) {
		// file read record calc.
		fIdx = m_nFileStartIdx * m_nDisplayPitch;
		// data set memory index calc.
		mIdx = m_nBufTailIdx;
		// data read
		m_nReadDataCnt	= (int)LoadTrendFile(m_pDispBufPtr, m_strSelFileName,
				fIdx, mIdx, (DWORD)ScrollPitch, m_nDisplayPitch);
		if (m_nNumSimChannel > 0 && m_isNeuralCal == TRUE) {
			// read simulation data
			LoadSimuFile(m_pDispBufPtr, m_strSimFileName,
				fIdx, mIdx, (DWORD)ScrollPitch, m_nDisplayPitch);
		}
		// non next data
		if (m_nReadDataCnt <= 0)
			return;
		// buffer head index calc. scroll pitch inc.
		m_nBufHeadIdx += ScrollPitch;
		if ( m_nBufHeadIdx >= ChartWidth )
			m_nBufHeadIdx = 0;
		// buffer tail index calc. scroll pitch inc.
		m_nBufTailIdx += ScrollPitch;
		if ( m_nBufTailIdx >= ChartWidth )
			m_nBufTailIdx = 0;
		// file read index next
		m_nFileStartIdx += ScrollPitch;
		if (++m_nScrollVal > m_nScrollCount) m_nScrollVal = m_nScrollCount;
		// set scroll flag ok
		m_isLineDown = TRUE;
		m_nDispStartTime += (time_t)(m_stTimeAxis.scale * 60 * ScrollPitch / ChartMeshWidth);
	}
}

// HSCROLL message proc
void CChartLib::GraphHScroll(CWnd *wnd, CDC &dc, UINT nSBCode, int nPos)
{
	CRect	winRect, clipRect, updateRect;
	int 	i, dx;
	int 	scrollCalc;	
	CBitmap	*hbmpOld;	
	CDC		memDC;

	wnd->GetClientRect(&winRect);

	clipRect.left	= winRect.left;
	clipRect.right	= winRect.right;
	clipRect.top	= winRect.top;
	clipRect.bottom	= winRect.left + ChartPlaneHeight - 1;

	switch(nSBCode) 
	{
	// push page right
	case SB_PAGERIGHT:
		for(i = 0; i < ChartWidth / ScrollPitch; i++){
			wnd->SendMessage(WM_HSCROLL, SB_LINEDOWN, 0L);
		}
		break;
	// push page left
	case SB_PAGELEFT:
		for(i = 0; i < ChartWidth / ScrollPitch; i++){
			wnd->SendMessage(WM_HSCROLL, SB_LINEUP, 0L);
		}
		break;
	// move scroll bottom
	case SB_THUMBTRACK:
		scrollCalc = nPos - m_nScrollVal;
		if (scrollCalc > 0) {
			for(i = 0; i < scrollCalc; i++){
				wnd->SendMessage(WM_HSCROLL, SB_LINEDOWN, 0L);
			}
		}
		else if (scrollCalc < 0) {
			for(i = scrollCalc; i < 0; i++){
				wnd->SendMessage(WM_HSCROLL, SB_LINEUP, 0L);
			}
		}
		break;
	case SB_LINEDOWN:
		GraphSbLineDown(wnd);
		dx = -ScrollPitch;
		break;

	case SB_LINEUP:
		GraphSbLineUp(wnd);
		dx = ScrollPitch;
		break;
	}
	if ( ((HBITMAP)NULL != m_bitmap) && (m_isLineDown == TRUE || m_isLineUp == TRUE) ) {
		wnd->SetScrollPos(SB_HORZ, m_nScrollVal, TRUE);
		memDC.CreateCompatibleDC(&dc);
		// bitmap select
		hbmpOld = (CBitmap *)memDC.SelectObject(&m_bitmap);
		memDC.ScrollDC(dx, 0,
				&winRect, &clipRect,
				NULL, &updateRect);
		DrawTrendWave(memDC, updateRect.left, updateRect.right, m_nNumTrendChannel);
		if (m_nNumSimChannel > 0) {
			DrawNeuralTrendWave(memDC, updateRect.left, updateRect.right);
		}
		dc.BitBlt(0, 0, ChartWidth, ChartPlaneHeight, &memDC, 0, 0, SRCCOPY);
		// old bitmap restore
		memDC.SelectObject(hbmpOld);
		memDC.DeleteDC();
		// stop down scroll
		m_isLineDown = FALSE;
		// stop up scroll
		m_isLineUp = FALSE;
	}
}

// load trend data into memory buffer.
DWORD CChartLib::LoadSimuFile(MemoryTable *mem, LPCTSTR name, DWORD fileIdx,
								int memIdx, DWORD numData, int pitch)
{
	DisplayData	*q;
	FILE		*fd;
	DWORD		n;
	double		patternOut, corrOut;

	if( name == NULL || (name != NULL && _tcslen(name) == 0) )
		return 0;
	m_strSimFileName = name;

	// open the sim data file. return 0 if failed.
	if ((fd = _tfopen( name, _T("rb") )) == NULL)
		return	0L;

	// for each data,
	q	= PtrInMTab(DisplayData, mem, memIdx);
	for (n = 0; n < numData; n++, memIdx++, fileIdx += pitch, q++) {
		// seek the file to starting position.
		fseek(fd, fileIdx * (sizeof(patternOut) + sizeof(corrOut)), 0);

		// read the values into simulation output file.
		if (fread(&patternOut, sizeof(patternOut), 1, fd) != 1)
			break;
		if (fread(&corrOut, sizeof(corrOut), 1, fd) != 1)
			break;

		// make up the display data structure.
		q->yPos[m_nNumTrendChannel] = ScaleSimY(patternOut);
		q->yPos[m_nNumTrendChannel + 1] = ScaleSimY(corrOut);
	}

	// close the file.
	fclose(fd);

	// return the number of read data.
	return	n;
}

// sim Y scale
int CChartLib::ScaleSimY(double val)
{
	double v = 0.0;

	v	= ChartMeshHeight * 10.0 * val;

	if (v < 0.0) v = 0.0;
	else if (v > ChartHeight - ChartMeshHeight * 5)	v = ChartHeight - ChartMeshHeight * 5;

	return (int)(ChartHeight - v);
}

// draw trend chart in specified area.
void CChartLib::DrawNeuralTrendWave(CDC &dc, int sx, int ex)
{
	int i;

	// neural calc. end ?
	if (m_isNeuralCal == TRUE) {
		for (i = m_nNumTrendChannel; i < m_nNumTrendChannel + m_nNumSimChannel; i++) {
			DrawChart(dc, i, sx, ex);
		}
	}
}

// load trend data into memory buffer.
DWORD CChartLib::LoadTrendFile(MemoryTable *mem, LPCTSTR name, DWORD fileIdx,
									int memIdx, DWORD numData, int pitch)
{
	SampledData		buf;
	SampledData		bufBk;
	SampledData		*p;
	DisplayData		*q;
	FILE			*fd;
	FILE			*fdBk;
	DataFileHeader	header;
	int				i, j, ch, arg;
	DWORD			n;
	size_t			tes;
	float			fFilter;

	if( name == NULL || (name != NULL && _tcslen(name) == 0) )
		return 0;
	m_strSelFileName = name;

	// open the trend data file. return 0 if failed.
	if ((fd = _tfopen( name, _T("rb") )) == NULL)
		return	0L;

	// load data file header. */
	fread(&header, sizeof(header), 1, fd);

	/////////////////////////////////////////////////////////////////////////

	// filter calc for ver6.20
	if( header.header.version >= 620 && m_bFilterSwitch == TRUE && m_strTrendOutFileName.GetLength() > 0 )
	{
		// open the trend data file. return 0 if failed.
		if ((fdBk = _tfopen( m_strTrendOutFileName, _T("wb+") )) != NULL)
		{
			fwrite(&header, sizeof(header), 1, fdBk);
			// read data
			if( fread(&bufBk, sizeof(SampledData), 1, fd) == 1)
			{
				fwrite(&bufBk, sizeof(SampledData), 1, fdBk);
				for( i = 0; i < (int)((fileIdx + ChartWidth * pitch)); i++ )
				{
					if( fread(&buf, sizeof(SampledData), 1, fd) != 1)
						break;

					// for each A/D channel,
					for(j = 0; j < NumADChannel; j++)
					{
						switch(header.header.arg[j])
						{
						case -1:
							fFilter = 1.0; break;
						case SNID:		// SN Channel No.
							fFilter = m_fFilterSn;	break;
						case LevelID:	// Level Channel No.
							fFilter = m_fFilterLv;	break;
						case VcID: 		// P/R Speed Channel No.
							fFilter = m_fFilterPr;	break;
						case LengthID:	// Length Channel No.
							fFilter = m_fFilterLen;	break;
						default:		// Temp Channel No.
							fFilter = m_fFilterTemp;break;
						}
						bufBk.val[j] =	(short)((float)bufBk.val[j] * (1.0f - fFilter) +
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

	// for each data,
	q = PtrInMTab(DisplayData, mem, memIdx);
	for (n = 0; n < numData; n++, memIdx++, fileIdx += pitch, q++) {

#ifdef Compatibility
		// read sample data for Ver5.00
		if( header.header.version <= 500 )
		{
			// seek the file to starting position.
			fseek(fd, fileIdx * sizeof(SampledDataV5) + sizeof(DataFileHeader), 0);
			// read the data from file. if fetch eof, break.
			if ((tes = fread(&buf, sizeof(SampledDataV5), 1, fd)) != 1)
				break;
			for( i = NumADChannelV5; i < NumADChannel; i++ )
				buf.val[i] = 0;
		}
		// read sample data for Ver6.00
		else //if( header.header.version == 600 )
		{
			// seek the file to starting position.
			fseek(fd, fileIdx * sizeof(SampledData) + sizeof(DataFileHeader), 0);
			// read the data from file. if fetch eof, break.
			if ((tes = fread(&buf, sizeof(SampledData), 1, fd)) != 1)
				break;
		}
#else
		// read sample data for Ver6.00
		if( header.header.version == 600 )
		{
			// seek the file to starting position.
			fseek(fd, fileIdx * sizeof(SampledData) + sizeof(DataFileHeader), 0);
			// read the data from file. if fetch eof, break.
			if ((tes = fread(&buf, sizeof(SampledData), 1, fd)) != 1)
				break;
		}
#endif

		// make up the display data structure.
		p	= &buf;
		for (i = 0; i < m_nNumTrendChannel; i++) {
			if ( (arg = m_nTrendChArg[i]) >= 0 &&
				 (ch = header.header.adCh[arg]) >= 0 )
				q->yPos[i]	= ScaleY(arg, p->val[ch]);
			else
				q->yPos[i]	= -1;
		}
	}

	// close the file.
	fclose(fd);

	// return the number of read data.
	return	n;
}

// load trend data into memory buffer.
BOOL CChartLib::LoadTrendData(LPCTSTR name, DWORD fileIdx, SampledData	*data)
{
	SampledData		buf;
	SampledData		*p;
	FILE			*fd;
	FILE			*fdBk;
	DataFileHeader	header;
	int				i, ch, arg;
	size_t			tes;

	if( name == NULL || (name != NULL && _tcslen(name) == 0) )
		return FALSE;

	// open the trend data file. return 0 if failed.
	if ((fd = _tfopen( name, _T("rb") )) == NULL)
		return FALSE;

	// load data file header. */
	fread(&header, sizeof(header), 1, fd);

	// filter calc for ver6.20
	if( header.header.version >= 620 && m_bFilterSwitch == TRUE && m_strTrendOutFileName.GetLength() > 0 )
	{
		// open the trend data file. return 0 if failed.
		if ((fdBk = _tfopen( m_strTrendOutFileName, _T("rb") )) != NULL)
		{
			fclose(fd);
			fd = fdBk;
		}
	}

#ifdef Compatibility
	// read sample data for Ver5.00
	if( header.header.version <= 500 )
	{
		// seek the file to starting position.
		fseek(fd, fileIdx * sizeof(SampledDataV5) + sizeof(DataFileHeader), 0);
		// read the data from file. if fetch eof, break.
		if ((tes = fread(&buf, sizeof(SampledDataV5), 1, fd)) != 1)
		{
			for( i = 0; i < NumADChannelV5; i++ )
				buf.val[i] = 0;
		}
		for( i = NumADChannelV5; i < NumADChannel; i++ )
			buf.val[i] = 0;
	}
	// read sample data for Ver6.00
	else //if( header.header.version == 600 )
	{
		// seek the file to starting position.
		fseek(fd, fileIdx * sizeof(SampledData) + sizeof(DataFileHeader), 0);
		// read the data from file. if fetch eof, break.
		if ((tes = fread(&buf, sizeof(SampledData), 1, fd)) != 1)
		{
			for( i = 0; i < NumADChannel; i++ )
				buf.val[i] = 0;
		}
	}
#else
	// read sample data for Ver6.00
	if( header.header.version == 600 )
	{
		// seek the file to starting position.
		fseek(fd, fileIdx * sizeof(SampledData) + sizeof(DataFileHeader), 0);
		// read the data from file. if fetch eof, break.
		if ((tes = fread(&buf, sizeof(SampledData), 1, fd)) != 1)
		{
			for( i = 0; i < NumADChannel; i++ )
				buf.val[i] = 0;
		}
	}
#endif

	// make up the display data structure.
	p	= &buf;
	for (i = 0; i < m_nNumTrendChannel; i++) {
		if ( (arg = m_nTrendChArg[i]) >= 0 &&
				(ch = header.header.adCh[arg]) >= 0 )
			data->val[i] = p->val[ch];
		else
			data->val[i] = -1;
	}

	// close the file.
	fclose(fd);

	// return result.
	return	TRUE;
}

// select area clear
void CChartLib::GraphSelectAreaClear(CWnd *wnd)
{
	// cut area select off
	m_isCutAreaSelect = FALSE;
	// select area clear
	wnd->Invalidate(FALSE);
	wnd->UpdateWindow();
}

// set select start color
void CChartLib::SelectStart(void)
{
	// select on
	m_isSelecting = TRUE;
}

// LBUTTONDOWN message proc
void CChartLib::GraphLButtonDown(CWnd *wnd, CDC &dc, CPoint pt)
{
	// screen select
	if (m_isSelecting == TRUE) {
		SelectStart();

		m_bTrack = TRUE;

		// calc first point
		m_LeftX = pt.x - (int)((m_nCutAreaLen / m_nDisplayPitch) * 0.5);
		m_RightX = m_LeftX + (int)(m_nCutAreaLen / m_nDisplayPitch);
		m_LeftX_Bk = m_LeftX;
		m_RightX_Bk = m_RightX;

		// select area clear
		GraphSelectAreaClear(wnd);
		box(dc, PS_SOLID, 0, m_clClear, R2_NOT, m_LeftX, 0,
									(m_nCutAreaLen / m_nDisplayPitch), ChartHeight - 1);
		// mouse out of window to all input capture
		wnd->SetCapture();
	}
}

// MOUSEMOVE message proc
void CChartLib::GraphMouseMove(CWnd *wnd, CDC &dc, CPoint pt)
{
	RECT	rectClient;

	// screen select
	if (m_isSelecting == TRUE) {
		if (m_bTrack) {
			m_LeftX = pt.x - (int)((m_nCutAreaLen / m_nDisplayPitch) * 0.5);

			// painting is client only
			wnd->GetClientRect(&rectClient);
			if (m_LeftX < rectClient.left)
				m_LeftX = rectClient.left;
			m_RightX = m_LeftX + (int)(m_nCutAreaLen / m_nDisplayPitch);

			// painting is client only
			if (m_RightX > rectClient.right)
			{
				m_RightX = rectClient.right;
				m_LeftX = m_RightX - (int)(m_nCutAreaLen / m_nDisplayPitch);
			}

			// old box clear
			if (m_LeftX != m_LeftX_Bk || m_RightX != m_RightX_Bk) 
			{
				box(dc, PS_SOLID, 0, m_clClear, R2_NOT, m_LeftX_Bk, 0,
											(m_nCutAreaLen / m_nDisplayPitch), ChartHeight - 1);
				// now mouse position pickup
				m_LeftX_Bk = m_LeftX;
				m_RightX_Bk = m_RightX;

				// new box paint
				box(dc, PS_SOLID, 0, m_clClear, R2_NOT, m_LeftX, 0,
											(m_nCutAreaLen / m_nDisplayPitch), ChartHeight - 1);
			}
		}
	}
}

// LBUTTONUP message proc
void CChartLib::GraphLButtonUp(CDC &dc)
{
	RECT	setRect;
	int		startPoint;

	// screen select
	if (m_isSelecting == TRUE) {
		// select end
		m_bTrack = FALSE;
		// capture free
		ReleaseCapture();

		box(dc, PS_SOLID, 0, m_clClear, R2_NOT, m_LeftX, 0, (m_nCutAreaLen / m_nDisplayPitch), ChartHeight - 1);
		setRect.left	= m_LeftX;
		setRect.right	= m_RightX;
		setRect.top		= 0;
		setRect.bottom	= ChartHeight;
		dc.InvertRect(&setRect);
		// mouse point
		if (m_LeftX != m_RightX) 
		{
			// cut area select on
			m_isCutAreaSelect = TRUE;
			// Start Data No. calc.
			m_nDataCount = (abs(m_LeftX - m_RightX) + 1) * m_nDisplayPitch;
			// Data Count calc.
			startPoint = m_LeftX;
			if (m_nFileStartIdx >= ChartWidth)	{
				m_nStartDataNo =
						(m_nFileStartIdx - ChartWidth + startPoint) * m_nDisplayPitch;
			}
			else {
				m_nStartDataNo = startPoint * m_nDisplayPitch;
			}
		}
		else {
			// cut area select off
			m_isCutAreaSelect = FALSE;
		}
		// selecting off
		m_isSelecting = FALSE;
	}
}

#define	Y_WIDTH		6
#define Y_WIDTH1	4
void CChartLib::DrawYAxis(CDC &dc, LPCTSTR pName, LPCTSTR pUnit, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height)
{
	double scale;
	int	y, disp_cnt;
	int MapModePrevious;
	_TCHAR buf[32],oldBuf[32];
	CString	str, str2;

	// mapping mode set
	MapModePrevious = dc.SetMapMode(MM_ANISOTROPIC);

	PrintText(dc, font, color, xx - 4, yy - 45, _T("%s"), pName);
	PrintText(dc, font, color, xx - 2, yy - 30, _T("%s"), pUnit);

	// draw Y line
	linePStart(dc, PS_SOLID, 1, color, R2_COPYPEN);
	lineP(dc, xx, yy, xx, yy + height);

	// draw X line
	_tcscpy(oldBuf, _T(""));
	for (y = yy + height, scale = pCA->zero, disp_cnt = 0;
			y >= yy; y -= ChartMeshHeight, scale += pCA->scale, disp_cnt++) {
		switch (disp_cnt%2) {
		case	0:
			_stprintf(buf, pFmt, scale);
			if (_tcscmp(buf, oldBuf)) {
				PrintText(dc, font, color,	xx + Y_WIDTH + 5, y - 5, buf);
				_tcscpy(oldBuf, buf);
				lineP(dc, xx, y, xx + Y_WIDTH, y );
				break;
			}
		default:
			lineP(dc, xx, y, xx + Y_WIDTH1, y );
			break;
		}
	}
	linePEnding(dc);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

// initialize time graph
void CChartLib::InitTimeGraph(CWnd *wnd, CDC &dc, StGraphParam *pGPrm)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	CRect	rc, rcPaint;
	int		i;

	if( wnd == NULL || (HBITMAP)NULL == m_bitmap )
	{
		wnd->GetClientRect(rcPaint);
		ChartWidth = ChartMeshXCnt * ChartMeshWidth;
		ChartHeight = ChartMeshYCnt * ChartMeshHeight;
		int iii = ChartMeshWidth;
		if ( FALSE == m_bitmap.CreateCompatibleBitmap(&dc, ChartWidth, ChartHeight) )
			return;
	}

	// input data
	m_nNumTrendChannel	= pGPrm->numChannel;
	m_nNumSimChannel	= pGPrm->neuralChannel;
	m_nSamplePeriod		= pGPrm->samplePeriod;
	m_nDispStartTime	= pGPrm->dispStartTime;
	m_isNeuralCal		= pGPrm->isNeuralCal;
	memcpy( m_nTrendChArgTm,	pGPrm->nTrendChArgTm, sizeof(m_nTrendChArgTm));
	memcpy( m_isTrendIndicate,	pGPrm->isTrendIndicate, sizeof(m_isTrendIndicate));
	
	memcpy( &m_stTimeAxis,	&pGPrm->timeAxis,	sizeof(m_stTimeAxis));
	memcpy( &m_stTempAxis,	&pGPrm->tempAxis,	sizeof(m_stTempAxis));
	memcpy( &m_stLevelAxis, &pGPrm->levelAxis,	sizeof(m_stLevelAxis));
	memcpy( &m_stVcAxis,	&pGPrm->vcAxis,		sizeof(m_stVcAxis));
	memcpy( &m_stSNAxis,	&pGPrm->snAxis,		sizeof(m_stSNAxis));
//	memcpy( &m_stWidthAxis, &pGPrm->widthAxis,	sizeof(m_stWidthAxis));

	// initialize data
	m_nBufHeadIdx		= 0;
	m_nBufTailIdx		= 0;
	m_nReadDataCnt		= 0;
	m_nDispStartTimeIni = m_nDispStartTime;
	m_nDisplayPitch		= (int)(m_stTimeAxis.scale / (m_nSamplePeriod * 0.001 / 2.0) + 0.5);

	// load trend data into memory buffer.
	m_nNumDispData		= 0;
	m_nFileStartIdx		= m_nNumDispData;

	// set scroll count.
	m_nScrollCount		= ChartWidth;

	// set scroll position
	m_nScrollVal		= 0;
	memset(&m_stDispPos, 0, sizeof(m_stDispPos));

	// create compatible dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = memDC.SelectObject(&m_bitmap);

	// clear memory plane.
	wnd->GetClientRect(rc);
	memDC.FillSolidRect(rc, m_clClear);

	// draw chart axis.
	DrawChartAxis(memDC, 0, ChartWidth);

	// draw trend chart in specified range.
	for (i = 0; i < m_nNumTrendChannel; i++) {
		if (m_nTrendChArg[i] < 0)
			continue;
		DrawChart(memDC, i, 0, m_nNumDispData);
	}

	dc.BitBlt(0, 0, ChartWidth, ChartHeight, &memDC, 0, 0, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);
}

// draw time chart plane.
void CChartLib::DrawTimeChart(CDC &dc, SampledData *data)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	CRect	winRect, clipRect, updateRect;
    CPoint	linePoint[2];
	int		x, y;
	int		i;

	// create compatible dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = memDC.SelectObject(&m_bitmap);

	// increase display counter.
	m_nScrollVal++;

	// shift the displayed geometry.
	m_stPreDispPos	= m_stDispPos;

	// if display area is full,
	if (m_nScrollVal >= m_nScrollCount) {
		x	= m_nScrollCount-1;

		// scroll the screen.
		winRect.top = winRect.left = 0;
		winRect.right = ChartWidth;
		winRect.bottom = ChartHeight;
		clipRect	= winRect;
		memDC.ScrollDC(-1, 0, &winRect, &clipRect, NULL, &updateRect);
		linePoint[0].x = 	linePoint[1].x = x;
		linePoint[0].y = 0; linePoint[1].y = ChartHeight;
		polyPline(memDC, PS_SOLID, 1, m_clClear, R2_COPYPEN, linePoint, 2);
		linePStart(memDC, PS_SOLID, m_nTmChLineWd, m_clAxis, R2_COPYPEN);
		linePoint[0].x = x - 1; linePoint[1].x = x;
		for (y = ChartMeshHeight-1; y < ChartHeight; y += ChartMeshHeight) {
			linePoint[0].y = linePoint[1].y = y;
			if( m_bChScaleDisp )
				Polyline(memDC, linePoint, 2);
		}
		if ((m_nScrollVal + x) % ChartMeshWidth == 0) {
			linePoint[0].x = 	linePoint[1].x = x;
			linePoint[0].y = 0; linePoint[1].y = ChartHeight;
			if( m_bChScaleDisp )
				memDC.Polyline(linePoint, 2);
		}
		linePEnding(memDC);
	}
	else {
		x	= m_nScrollVal;
	}

	// for each A/D channel,
	for (i = 0; i < NumADChannel; i++) {
		if (m_nTrendChArgTm[i] < 0)
			continue;
		// calculate the displaying geometry.
		m_stDispPos.yPos[i]	= ScaleY(m_nTrendChArgTm[i], data->val[i]);
	}

	// if the first data, do nothing.
	if (m_nScrollVal == 1)		return;

	// for each A/D channel, 
	linePoint[0].x = x - 1;linePoint[1].x = x;
	for (i = 0; i < NumADChannel; i++) 
	{
		if (m_nTrendChArgTm[i] < 0 || !m_isTrendIndicate[i])
			continue;
		// draw a line segment.
		linePoint[0].y = m_stPreDispPos.yPos[i]; linePoint[1].y = m_stDispPos.yPos[i];
		polyPline(memDC, PS_SOLID, m_nTmChLineWd, m_clTonesTm[i], R2_COPYPEN, linePoint, 2);
	}

//	dc.BitBlt(0, 0, ChartWidth, ChartHeight, &memDC, 0, 0, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);
}

void CChartLib::DrawFetchLine(CDC &dc, COLORREF col)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	POINT	linePoint[2];
	int		x, y;

	// create compatible dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = memDC.SelectObject(&m_bitmap);

	x	= (m_nScrollVal >= ChartWidth) ? ChartWidth - 3 : (int)m_nScrollVal;
	y	= ChartHeight - 1;

//MMM	linePoint[0].x = 	linePoint[1].x = x;
//MMM	linePoint[0].y = 1; linePoint[1].y = y;

	linePoint[0].x = 	linePoint[1].x = x + 2;
	linePoint[0].y = 1; linePoint[1].y = y;


	polyPline(memDC, PS_SOLID, 1, col, R2_COPYPEN, linePoint, 2);
//MMM	polyPline(memDC, PS_SOLID, 1, col, R2_MERGEPEN, linePoint, 2);
//MMM	polyPline(memDC, PS_SOLID, 1, col, R2_XORPEN, linePoint, 2);
//mmm Good	polyPline(memDC, PS_SOLID, 1, col, R2_NOTXORPEN, linePoint, 2);
//MMM	polyPline(memDC, PS_SOLID, 1, col, R2_MASKPEN, linePoint, 2);
//MMM	polyPline(memDC, PS_SOLID, 1, col, R2_NOTMASKPEN, linePoint, 2);

	dc.BitBlt(0, 0, ChartWidth, ChartHeight, &memDC, 0, 0, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);
}







// initialize time graph
void CChartLib::InitTimeGraph2(CWnd *wnd, CDC &dc, CDC &mdc, StGraphParam *pGPrm)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	CRect	rc, rcPaint;
	int		i;

	if( wnd == NULL || (HBITMAP)NULL == m_bitmap )
	{
		wnd->GetClientRect(rcPaint);
		ChartWidth = ChartMeshXCnt * ChartMeshWidth;
		ChartHeight = ChartMeshYCnt * ChartMeshHeight;
		int iii = ChartMeshWidth;
		if ( FALSE == m_bitmap.CreateCompatibleBitmap(&dc, ChartWidth, ChartHeight) )
			return;
	}

	// input data
	m_nNumTrendChannel	= pGPrm->numChannel;
	m_nNumSimChannel	= pGPrm->neuralChannel;
	m_nSamplePeriod		= pGPrm->samplePeriod;
	m_nDispStartTime	= pGPrm->dispStartTime;
	m_isNeuralCal		= pGPrm->isNeuralCal;
	memcpy( m_nTrendChArgTm,	pGPrm->nTrendChArgTm, sizeof(m_nTrendChArgTm));
	memcpy( &m_stTimeAxis,	&pGPrm->timeAxis,	sizeof(m_stTimeAxis));
	memcpy( m_stAxisTm2,	pGPrm->stAxisTm2, sizeof(m_stAxisTm2));
	memcpy( m_dUnitTm, pGPrm->dUnitTm, sizeof(m_dUnitTm));

	// initialize data
	m_nBufHeadIdx		= 0;
	m_nBufTailIdx		= 0;
	m_nReadDataCnt		= 0;
	m_nDispStartTimeIni = m_nDispStartTime;
	m_nDisplayPitch		= (int)(m_stTimeAxis.scale / (m_nSamplePeriod * 0.001 / 2.0) + 0.5);

	// load trend data into memory buffer.
	m_nNumDispData		= 0;
	m_nFileStartIdx		= m_nNumDispData;

	// set scroll count.
	m_nScrollCount		= ChartWidth;

	// set scroll position
	m_nTimeCounter		= 0;
	m_nScrollVal		= 0;
	memset(&m_stDispPos, 0, sizeof(m_stDispPos));

	// create compatible dc
	memDC.CreateCompatibleDC(&dc);

	// select bitmap
	pOldBitmap = memDC.SelectObject(&m_bitmap);

	// clear memory plane.
	wnd->GetClientRect(rc);
	memDC.FillSolidRect(rc, m_clClear);

	// draw chart axis.
	DrawChartAxis(memDC, 0, ChartWidth);

	// draw trend chart in specified range.
	for (i = 0; i < m_nNumTrendChannel; i++) {
		if (m_nTrendChArgTm[i] < 0)
			continue;
		DrawChart(memDC, i, 0, m_nNumDispData);
	}

	mdc.BitBlt(0, 0, ChartWidth, ChartHeight, &memDC, 0, 0, SRCCOPY);

	// old bitmap restore
	memDC.SelectObject(pOldBitmap);
}

// draw time chart plane.
void CChartLib::DrawTimeChart2(CDC &dc, SampledData *data)
{
	CBitmap	*pOldBitmap;
	CDC		memDC;
	CRect	winRect, clipRect, updateRect;
    CPoint	linePoint[2];
	int		x, y;
	int		i;

	// increase display counter.
	if( ++m_nTimeCounter >= m_nDisplayPitch )
	{
		m_nScrollVal++;
		m_nTimeCounter = 0;

		// create compatible dc
		memDC.CreateCompatibleDC(&dc);

		// select bitmap
		pOldBitmap = memDC.SelectObject(&m_bitmap);

		// shift the displayed geometry.
		m_stPreDispPos	= m_stDispPos;

		// if display area is full,
		if (m_nScrollVal >= m_nScrollCount) {
			x	= m_nScrollCount-1;

			// scroll the screen.
			winRect.top = winRect.left = 0;
			winRect.right = ChartWidth;
			winRect.bottom = ChartHeight;
			clipRect	= winRect;
			memDC.ScrollDC(-1, 0, &winRect, &clipRect, NULL, &updateRect);
			linePoint[0].x = 	linePoint[1].x = x;
			linePoint[0].y = 0; linePoint[1].y = ChartHeight;
			polyPline(memDC, PS_SOLID, 1, m_clClear, R2_COPYPEN, linePoint, 2);
			linePStart(memDC, PS_SOLID, 1, m_clAxis, R2_COPYPEN);
			linePoint[0].x = x - 1; linePoint[1].x = x;
			for (y = ChartMeshHeight-1; y < ChartHeight; y += ChartMeshHeight) {
				linePoint[0].y = linePoint[1].y = y;
				if( m_bChScaleDisp )
					Polyline(memDC, linePoint, 2);
			}
			if ((m_nScrollVal + x) % ChartMeshWidth == 0) {
				linePoint[0].x = 	linePoint[1].x = x;
				linePoint[0].y = 0; linePoint[1].y = ChartHeight;
				if( m_bChScaleDisp )
					memDC.Polyline(linePoint, 2);
			}
			linePEnding(memDC);
		}
		else {
			x	= m_nScrollVal;
		}

		// for each A/D channel,
		for (i = 0; i < m_nNumTrendChannel; i++) {
			if (m_nTrendChArgTm[i] < 0)
				continue;
			// calculate the displaying geometry.
			m_stDispPos.yPos[i]	= ScaleY2(m_nTrendChArgTm[i], data->val[i]);
		}

		// if the first data, do nothing.
		if (m_nScrollVal == 1) return;

		// for each A/D channel, 
		linePoint[0].x = x - 1;linePoint[1].x = x;
		for (i = 0; i < m_nNumTrendChannel; i++) 
		{
			if (m_nTrendChArgTm[i] < 0)
				continue;
			// draw a line segment.
			linePoint[0].y = m_stPreDispPos.yPos[i]; linePoint[1].y = m_stDispPos.yPos[i];
			polyPline(memDC, PS_SOLID, m_nTmCh2LineWd, m_clTonesTm[i], R2_COPYPEN, linePoint, 2);
		}

//		dc.BitBlt(0, 0, ChartWidth, ChartHeight, &memDC, 0, 0, SRCCOPY);

		// old bitmap restore
		memDC.SelectObject(pOldBitmap);
	}
}

// scale
int	CChartLib::ScaleY2(int arg, int val)
{
	double	v	= 0.0;

	if(arg < NumADChannel)
	{
		v	= ChartMeshHeight * (m_dUnitTm[arg] * val - m_stAxisTm2[arg].zero) / m_stAxisTm2[arg].scale;
	}

	if (v < 0)						v	= 0.0;
	else if (v > ChartHeight)		v	= ChartHeight;

	return	(int)(ChartHeight - v);
}

#define Y_WIDTH2	4
#define X_OFFSET2	20
void CChartLib::DrawYAxis2(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height)
{
	double scale;
	int	y, disp_cnt;
	int MapModePrevious;
	_TCHAR buf[32];
	CString	str, str2;

	// mapping mode set
	MapModePrevious = dc.SetMapMode(MM_ANISOTROPIC);

	// draw Y line
	linePStart(dc, PS_SOLID, 2, color, R2_COPYPEN);
	lineP(dc, xx + X_OFFSET2, yy, xx + X_OFFSET2, yy + height);

	// draw X line
	for (y = yy + height, scale = pCA->zero, disp_cnt = 0;
			y >= yy; y -= ChartMeshHeight, scale += pCA->scale, disp_cnt++) {
		switch (disp_cnt) {
		case	0:
			_stprintf(buf, pFmt, scale);
			PrintText(dc, font, color,	xx + 5, y + 4, buf);
			lineP(dc, xx + X_OFFSET2, y, xx + X_OFFSET2 + Y_WIDTH, y );
			break;
		default:
			lineP(dc, xx + X_OFFSET2, y, xx + X_OFFSET2 + Y_WIDTH2, y );
			break;
		}
	}
	_stprintf(buf, pFmt, pCA->scale * (disp_cnt - 1) + pCA->zero);
	PrintText(dc, font, color,	xx + 5, yy - 15, buf);
	lineP(dc, xx + X_OFFSET2, yy, xx + X_OFFSET2 + Y_WIDTH, yy );
	linePEnding(dc);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

#define Y_WIDTH3	3
void CChartLib::DrawYAxis3(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height)
{
	double scale;
	int	y, disp_cnt;
	int MapModePrevious;
	_TCHAR buf[32],oldBuf[32];
	CString	str, str2;

	// mapping mode set
	MapModePrevious = dc.SetMapMode(MM_ANISOTROPIC);

	// draw Y line
	linePStart(dc, PS_SOLID, 1, color, R2_COPYPEN);
	lineP(dc, xx, yy, xx, yy + height);

	// draw X line
	_tcscpy(oldBuf, _T(""));
	for (y = yy + height, scale = pCA->zero, disp_cnt = 0;
			y >= yy; y -= ChartMeshHeight, scale += pCA->scale, disp_cnt++) {
		_stprintf(buf, pFmt, scale);
		if (_tcscmp(buf, oldBuf)) {
			PrintText(dc, font, color,	xx + Y_WIDTH3 + 3, y - 5, buf);
			_tcscpy(oldBuf, buf);
			lineP(dc, xx, y, xx + Y_WIDTH3, y );
		}
	}
	linePEnding(dc);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

#define Y_WIDTH4	4
void CChartLib::DrawYAxis4(CDC &dc, LPCTSTR pFmt, ChartAxis *pCA, CFont *font, COLORREF color, int xx, int yy, int height)
{
	double scale;
	int	y, disp_cnt, wd;
	int MapModePrevious;
	_TCHAR buf[32],oldBuf[32];
	CString	str, str2;

	// mapping mode set
	MapModePrevious = dc.SetMapMode(MM_ANISOTROPIC);

	// draw Y line
	linePStart(dc, PS_SOLID, 1, color, R2_COPYPEN);
	lineP(dc, xx, yy, xx, yy + height);

	// set text width
	wd = 20;

	// draw X line
	_tcscpy(oldBuf, _T(""));
	for (y = yy + height, scale = pCA->zero, disp_cnt = 0;
			y >= yy; y -= ChartMeshHeight, scale += pCA->scale, disp_cnt++) {
		_stprintf(buf, pFmt, scale);
		if (_tcscmp(buf, oldBuf)) {
			PrintText(dc, font, color,	xx - wd - Y_WIDTH4 - 3, y - 5, buf);
			_tcscpy(oldBuf, buf);
			lineP(dc, xx, y, xx - Y_WIDTH4, y );
		}
	}
	linePEnding(dc);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

// p1とp2間の直線上でp3に一番近い点を求める
// (1) p1とp2間の直線を直線Aとする
// (2) p3から直線Aに垂直におろした線を直線Bとする
// (3) 直線ABの交点を求める
// (4) 交点が直線A上ならそのまま返す
// (5) 交点が直線A外ならp1とp2の近い点を返す
BOOL CChartLib::CalcNearPoint(POINT p1, POINT p2, POINT p3, pDPOINT cp)
{
	double p1x, p1y, p2x, p2y, p3x, p3y;
	double a1, a2, b1, b2, c1, c2;
	double dis1, dis2;

	//---------------------
	// データ入力(int->double)
	p1x = p1.x;
	p1y = p1.y;
	p2x = p2.x;
	p2y = p2.y;
	p3x = p3.x;
	p3y = p3.y;

	//---------------------
	// 直線ABの交点を求める

	// 直線Aが水平である場合
	if( p1y == p2y ) 
	{
		// 直線ABの交点を返す
		cp->x = p3x;
		cp->y = p1y;
	}
	// 直線Aが垂直である場合
	else if( p1x == p2x )
	{
		// 直線ABの交点を返す
		cp->x = p1x;
		cp->y = p3y;
	}
	// 直線Aが斜め場合
	else 
	{
		// 直線ABの交点を求める
		a1 = -(p1x-p2x)/(p1y-p2y);
		b1 = -1;
		c1 = p3y - a1*p3x;

		a2 = p2y-p1y;
		b2 = -(p2x-p1x);
		c2 = -p1x*(p2y-p1y)+p1y*(p2x-p1x);

		cp->x = (b1*c2 - b2*c1 ) / ( a1*b2 - a2*b1 );
		cp->y = (a2*c1 - a1*c2 ) / ( a1*b2 - a2*b1 );
	}

	//---------------------
	// 近い点を求める

	// 交点が直線A上に存在するなら
	if( (cp->x >= p1.x && cp->x <= p2.x) && ((cp->y-p1.y) * (p2.y-cp->y) >= 0.0) )
	{
		//そのまま交点を返す
	}
	// 交点が直線A上に無いなら
	else
	{
		// p1とp2の近い方の点を返す
		dis1 = sqrt((cp->x - p1.x)*(cp->x - p1.x)+(cp->x - p1.y)*(cp->x - p1.y));
		dis2 = sqrt((cp->x - p2.x)*(cp->x - p2.x)+(cp->x - p2.y)*(cp->x - p2.y));
		if( dis1 < dis2 )
		{
			cp->x = p1.x;
			cp->y = p1.y;
		}
		else
		{
			cp->x = p2.x;
			cp->y = p2.y;
		}
	}

	return TRUE;
}

// draw trend chart in specified range.
BOOL CChartLib::GetChartData(int nIdx, SampledData *data)
{
	if (m_nFileStartIdx >= ChartWidth)	{
		m_nStartDataNo =
				(m_nFileStartIdx - ChartWidth + nIdx) * m_nDisplayPitch;
	}
	else {
		m_nStartDataNo = nIdx * m_nDisplayPitch;
	}

	return LoadTrendData(m_strSelFileName, m_nStartDataNo, data);
}

// draw trend chart in specified range.
BOOL CChartLib::GetChartTime(int nIdx, time_t *data)
{
	*data = (m_nDispStartTime + (int)(nIdx * m_nDisplayPitch / 2));

	return TRUE;
}

// Hit Test for displayed chart
int	CChartLib::ChartHitTest(POINT pt, StChartHitInfo *chi)
{
/*	ChartData	*cd;
	POINT		p1, p2;
	DPOINT		cp;
	double		dis, dis_min = 2.0e0;
	double		ptx, pty;
	BOOL		bCalc;
	int			i, j, i_min = -1;

	// get the mouse coordinates
	ptx = (double)pt.x;
	pty = (double)pt.y;

	// For each trend channel number,
	for (i = 0; i < m_nNumTrendChannel; i++) {
		if (m_nTrendChArg[i] < 0)
			continue;

		// get the trend data which is displayed in the screen.
		cd = GetChartData(i, m_nNumDispData);
		if( cd != NULL )
		{
			// For each data,
			for( j = 0; j < cd->cntMax-1; j++ )
			{
				// get the two point.
				p1 = cd->linePoint[j];
				p2 = cd->linePoint[j+1];

				// calculate cross point.
				bCalc = CalcNearPoint(p1, p2, pt, &cp);
				if( bCalc )
				{
					// calculate distance from the mouse point to cross point.
					dis = sqrt((((double)ptx-cp.x)*(ptx-cp.x) + ((double)pt.y-cp.y)*(pt.y-cp.y)));

					// if the calculation distance is less than the minimum distance, update information.
					if( dis < dis_min )
					{
						dis_min = dis;
						i_min = i;
						chi->val = (short)(((ChartHeight-cp.y) * m_stTempAxis.scale / ChartMeshHeight + m_stTempAxis.zero) / PhisicalTempUnit);
					}
				}
			}
			// free calculation area.
			free(cd->linePoint);
			free(cd);
		}
	}
	
	// set hit index
	chi->idx = i_min;
	if( i_min != -1 )
		return TRUE;
	else
		return FALSE;
		*/
	return TRUE;
}
