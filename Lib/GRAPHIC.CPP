#include	"../include/graphic.h"
#include	<windowsx.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<stdarg.h>
#include	<TCHAR.H>

static int	numAllocColor;
static WORD	paletteSize;
static HPALETTE	palette;
static int	isPaletteEnab;

void	selectLogicalPalette(HDC dc);
static int	trendBase, msgBase, Sepia, Black, White, Green, Red, Blue, Yellow, moldColor;
//static int	colorTone[NumColorTone];


/*===================================================*/
/* log message into the file named "log".
*/
void	logPrintf(_TCHAR *fmt, ...)
{
	va_list	pvar;
	FILE	*fp;

	/* open the logging file in append mode. */
	if ((fp = fopen("log", "a")) == NULL)
		return;

	va_start(pvar, fmt);
	/* write a log string into the file. */
	_vftprintf(fp, fmt, pvar);
	va_end(pvar);

	/* close the logging file. */
	fclose(fp);
}
#if 0

/////////////////////////////////////////////////
/* 96/05/10 move */
/*====================================================*/
/* set up colors resources.
*/
void    initializeColors(void)
{
	/* set up logical color palette. */
	setupPalette();

	/* allocate trend base window color (light gray). */
	trendBase   = allocColor(232, 232, 232);

	/* allocate message base window color (wheat). */
	msgBase	= allocColor(245, 222, 179);

	/* allocate chart mesh drawing color (palegoldenrod). */
	Sepia   = allocColor(210, 180, 140);

	/* allocate black color. */
	Black   = allocColor(0, 0, 0);

	/* allocate white color. */
	White   = allocColor(255, 255, 255);

	/* allocate green color. */
	Green   = allocColor(0, 192, 0);

	/* allocate Red color. */
	Red     = allocColor(205, 0, 0);

	/* allocate Blue color. */
	Blue    = allocColor(0, 0, 205);

	/* allocate Yellow Color. */
	Yellow  = allocColor(128, 128, 0);

	/* allocate mold drawing color (goldenrod). */
	moldColor	= allocColor(218, 165, 32);

	/* allocate neural network drawing colors. */
	allocColorTone(NumColorTone, 150, 52, colorTone);
//	allocColorTone(NumColorTone, 128, 16, colorTone);
}
///////////////////////////////////////////////////








/*===================================================*/
/* set up logical color palette.
*/
void	setupPalette(void)
{
	LOCALHANDLE	areaID;
	static PLOGPALETTE	logicalPalette;

	/* 15 colors are initially allocated for application. */
	paletteSize		= 15;

	/* initialize the number of color. */
	numAllocColor	= 0;

	/* allocate local palette area for initial value setting. */
	areaID	= LocalAlloc(LMEM_MOVEABLE,
			sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));

	/* translate ID to a pointer of allocated area. */
	logicalPalette	= (PLOGPALETTE)LocalLock(areaID);

	/* store initial value into logical palette. */
	logicalPalette->palVersion		= 0x300;
	logicalPalette->palNumEntries	= paletteSize;

	/* create a new logical palette. */
	palette	= CreatePalette(logicalPalette);

	/* unlock allocated palette area. */
	LocalUnlock(areaID);

	/* free allocated palette area. */
	LocalFree(areaID);
}

/*===================================================*/
/* allocate new color from logical palette.
*/
int	allocColor(int r, int g, int b)
{
	static PALETTEENTRY	entry;

	/* if the number of allocated color is equal to the size of palette, */
	if (numAllocColor == paletteSize) {
		/* increase paletteSize. */
		paletteSize++;
		/* resize palette. */
		ResizePalette(palette, paletteSize);
	}

	/* set RGB element into new color. */
	entry.peRed		= (BYTE)r;
	entry.peGreen	= (BYTE)g;
	entry.peBlue	= (BYTE)b;
	entry.peFlags	= (BYTE)0;

	/* update palette entry. */
	SetPaletteEntries(palette, numAllocColor, 1, &entry);

	/* increase the number of allocated color. */
	numAllocColor++;

	/* return allocated color number. */
	return	numAllocColor - 1;
}

#endif

// allocate color tone
void allocColorTones(int n, int span, int offset, COLORREF *colors)
{
	int	i;
	int	r, g, b;
	double	t, dT;

	dT	= 1.0 / (n / 3);

	/* create color gradation from Red to Green. */
	for (i = 0, t = 0.0; i < (n+2)/3; i++, t += dT) {
		/* calculate RGB element. */
		r	= (int)((span * (1 - t) + offset) / 0.45);
		g	= (int)((span * t + offset) / 0.6);
		b	= (int)(offset / 0.5);
		if (r > 255)	r	= 255;
		if (g > 255)	g	= 255;
		if (b > 255)	b	= 255;
		/* allocate new color. */
		*colors++	= RGB(r, g, b);
	}

	/* create color gradation from Green to Blue. */
	offset += 10;
	for (t = 0.0; i < 2 * (n+2)/3; i++, t += dT) {
		/* calculate RGB element. */
		g	= (int)((span * (1 - t) + offset) / 0.6);
		b	= (int)((span * t + offset) / 0.5);
		r	= (int)(offset / 0.45);
		if (r > 255)	r	= 255;
		if (g > 255)	g	= 255;
		if (b > 255)	b	= 255;
		/* allocate new color. */
		*colors++	= RGB(r, g, b);
	}

	/* create color gradation from Blue to Red. */
	offset -= 10;
	for (t = 0.0; i < n; i++, t += dT) {
		/* calculate RGB element. */
		b	= (int)((span * (1 - t) + offset) / 0.5);
		r	= (int)((span * t + offset) / 0.45);
		g	= (int)(offset / 0.6);
		if (r > 255)	r	= 255;
		if (g > 255)	g	= 255;
		if (b > 255)	b	= 255;
		/* allocate new color. */
		*colors++	= RGB(r, g, b);
	}
}

// allocate color tone (64 fix color)
void allocColorTones64(COLORREF *colors)
{
}

#if 0

/*===================================================*/
/* delete palette
*/
void delPalette(void)
{
	DeletePalette(palette);
}

/*===================================================*/
/* add hock function at the first of WndProc.
*/
void	__beginWndProc(HWND win, UINT msg)
{
	/* set palette to be disable. */
	isPaletteEnab	= FALSE;
}

/*===================================================*/
/* add hock function at the end of WndProc.
*/
void	__endWndProc(HWND win, UINT msg)
{
}

/*===================================================*/
/* procedure for WM_QUERYNEWPALETTE message.
*/
void	queryNewPaletteProc(HWND win, HDC dc)
{
	/* execute SelectPalette. */
	SelectPalette(dc, palette, FALSE);

	/* execute RealizePalette. */
	if (RealizePalette(dc))
		InvalidateRect(win, NULL, TRUE);
}

/*===================================================*/
/* procedure for WM_PALETTECHANGED message.
*/
void	paletteChangedProc(HWND win, HDC dc, WPARAM wParam)
{
	if (wParam != (WPARAM)win) {
		SelectPalette(dc, palette, FALSE);
		if (RealizePalette(dc))
			UpdateColors(dc);
	}
}

/*===================================================*/
/* generate WM_PAINT message.
*/
void	redrawWindow(HWND win)
{
	RECT	rect;

	GetClientRect(win, &rect);
	InvalidateRect(win, &rect, FALSE);
	UpdateWindow(win);
}

/*===================================================*/
/* select logical palette.
*/
void	selectLogicalPalette(HDC dc)
{
	/* if palette is disable. */
	if (!isPaletteEnab) {
		/* set using palette. */
		SelectPalette(dc, palette, FALSE);

		/* realize the palette to system. */
		RealizePalette(dc);

		/* set palette enable. */
		isPaletteEnab	= TRUE;
	}
}

#endif

// draw a line.
void line(CDC &dc, int style, int width, COLORREF color, int func, int x1, int y1, int x2, int y2)
{
	CPen	pen, *hPenOld;

	// generate a pen for drawing.
	pen.CreatePen(style, width, color);

	// set drawing operation code.
	dc.SetROP2(func);

	// set using pen to device handle.
	hPenOld = dc.SelectObject(&pen);

	// draw a line.
	dc.MoveTo(x1, y1);
	dc.LineTo(x2, y2);

	// set stocked object to device handle for deleting.
	dc.SelectObject(hPenOld);
	pen.DeleteObject();
}

/*===================================================*/
/* Draw PolyLine
*/
void polyPline(CDC &dc, int style, int width, int color, int func, POINT *linePoint, int pointCount)
{
	CPen	pen, *pPenOld;
	BOOL	bl;

	/* generate a pen for drawing. */
	bl = pen.CreatePen(style, width, color);

	/* set drawing operation code. */
	dc.SetROP2(func);

	/* set using pen to device handle. */
	pPenOld = (CPen *)dc.SelectObject(&pen);
	/* draw a line. */                /* not saport Win32 MoveTo Function */

	dc.Polyline(linePoint, pointCount);

	dc.SelectObject(pPenOld);

	/* delete used pen. */
	pen.DeleteObject();
}

static CPen	m_tmpPen, *tmpPenOld;
/*===================================================*/
/* draw a line start.
*/
void linePStart(CDC &dc, int style, int width, COLORREF color, int func)
{
	// generate a pen for drawing.
	m_tmpPen.CreatePen(style, width, color);

	// set drawing operation code.
	dc.SetROP2(func);

	// set using pen to device handle.
	tmpPenOld = dc.SelectObject(&m_tmpPen);
}

/*===================================================*/
/* draw a line.
*/
void lineP(CDC &dc, int x1, int y1, int x2, int y2)
{
	POINT	linePoint[2];

	// draw a line.
	linePoint[0].x = x1,linePoint[1].x = x2;
	linePoint[0].y = y1,linePoint[1].y = y2;
	dc.Polyline(linePoint, 2);
}
/*===================================================*/
/* draw a line ending.
*/
void linePEnding(CDC &dc)
{
	dc.SelectObject(tmpPenOld);
	// delete used pen.
	m_tmpPen.DeleteObject();
}

/*===================================================*/
/* fill a specified rectangle area by solid color.
	96.04.04	å√Ç¢ÇoÇdÇmï€ë∂ñYÇÍ
*/
void	boxFillSolid(CDC &dc, int color, int x, int y, int width, int height)
{
	CBrush	brush, *pBrushOld;
	CPen	*pPenOld;

	/* generate a brush for drawing. */
	brush.CreateSolidBrush(color);

	/* set drawing operation code. */
	dc.SetROP2(R2_COPYPEN);

	/* set using brush. */
	pBrushOld = dc.SelectObject(&brush);
	/* set using pen, for no outline drawing. */
	pPenOld = dc.SelectObject(CPen::FromHandle(GetStockPen(NULL_PEN)));

	/* draw rectangle. */
	dc.Rectangle(x, y, x + width + 1, y + height + 1);

	/* set stocked object to device handle for deleting.
		DON'T FORGET !!!!!
	*/
	dc.SelectObject(pBrushOld);
	dc.SelectObject(pPenOld); 
	/* delete used brush. */
	brush.DeleteObject();
}

// draw a rectangle.
void box(CDC &dc, int style, int penwidth, COLORREF color, int func, int x, int y, int width, int height)
{
	CPen	pen, *hPenOld;
	CBrush	*brush, *hBrushOld;

	// generate a pen for drawing.
	pen.CreatePen(style, penwidth, color);

	// set drawing operation code.
	dc.SetROP2(func);

	// set using pen.
	hPenOld = (CPen *)dc.SelectObject(&pen);

	// set using brush.
	brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	hBrushOld = (CBrush *)dc.SelectObject(brush);

	// draw rectangle.
	dc.Rectangle(x, y, x + width + 1, y + height + 1);

	// set stocked object to device handle for deleting.
	dc.SelectObject(hBrushOld);
	dc.SelectObject(hPenOld);

	// delete used pen.
	pen.DeleteObject();
}

/*===================================================*/
/* fill an area specified by a circle.
*/
void	circleFillSolid(CDC &dc, COLORREF color, int x, int y, int r)
{
	CBrush	brush, *pBrushOld;
	CPen	*pPenOld;

	/* generate a brush for drawing. */
	brush.CreateSolidBrush(color);

	/* set drawing operation code. */
	dc.SetROP2(R2_COPYPEN);

	/* set using brush. */
	pBrushOld = dc.SelectObject(&brush);
	/* set using pen, for no outline drawing. */
	pPenOld = dc.SelectObject(CPen::FromHandle(GetStockPen(NULL_PEN)));

	/* draw circle. */
	Chord(dc, x - r, y - r, x + r, y + r, x, y - r, x, y - r);

	/* set stocked object to device handle for deleting.
		DON'T FORGET !!!!!
	*/
	dc.SelectObject(pBrushOld);
	dc.SelectObject(pPenOld); 
	/* delete used brush. */
	brush.DeleteObject();
}

#if 0

/*===================================================*/
/* draw a circle.
*/
void	circle(HDC dc, int style, int pWidth, int color, int func, int x, int y, int r)
{
	HPEN	pen, hPenOld;
	HBRUSH	hBrushOld;

	/* select logical palette. */
	selectLogicalPalette(dc);

	/* generate a pen for drawing. */
	pen	= CreatePen(style, pWidth, PALETTEINDEX(color));

	/* set drawing operation code. */
	SetROP2(dc, func);

	/* set using pen. */
	hPenOld = SelectPen(dc, pen);

	/* set using brush. */
	hBrushOld = SelectBrush(dc, GetStockPen(NULL_BRUSH));

	/* draw a circle. */
	Chord(dc, x - r, y - r, x + r, y + r, x, y - r, x, y - r);

	/* set stocked object to device handle for deleting.
		DON'T FORGET !!!!!
	*/
	SelectPen(dc, hPenOld);
	SelectBrush(dc, hBrushOld);

	/* delete used pen. */
	DeletePen(pen);
}

/*===================================================*/
/* draw string.
*/
void	gtextout(HDC dc, HFONT font, COLORREF color, int x, int y, _TCHAR *buf )
{
	HFONT	orgFont = NULL;
	COLORREF	oldColor;					/* */

	oldColor = SetTextColor(dc, color);		/* */
	SetBkMode(dc, TRANSPARENT);

	if (font)
		orgFont	= SelectFont(dc, font);
	TextOut( dc, x, y, buf, lstrlen( buf ) );

	SetTextColor(dc, oldColor);				/* */

	if (font)
		SelectFont(dc, orgFont);
}

/*===================================================*/
/* draw formatted string.
	96.04.04	å√Ç¢ï∂éöêFï€ë∂ÇµÇ»Ç¢Ç≈ã≠êßìIÇ…çïÇ…ÇµÇƒÇ¢ÇΩ
*/
void	gprintf(HDC dc, HFONT font, COLORREF color, int x, int y, _TCHAR *fmt, ...)
{
	_TCHAR	buf[256];
	int	len;
	HFONT	orgFont = NULL;
	va_list	pvar;
	COLORREF	oldColor;					/*  96.04.04í«â¡ */

	va_start(pvar, fmt);
	len	= _vstprintf(buf, fmt, pvar);
	va_end(pvar);

	oldColor = SetTextColor(dc, color);		/*  96.04.04ïœçX */
	SetBkMode(dc, TRANSPARENT);

	if (font)
		orgFont	= SelectFont(dc, font);
	TextOut(dc, x, y, buf, len);

	SetTextColor(dc, oldColor);				/*  96.04.04ïœçX */

	if (font)
		SelectFont(dc, orgFont);
}
#endif
/*===================================================*/
/* draw formatted string. By Windows
*/
void PrintText(CDC &dc, CFont *font, COLORREF color, int x, int y, LPCTSTR fmt, ...)
{
	_TCHAR	buf[256];
	int	len;
	CFont	*orgFont = NULL;
	va_list	pvar;
	COLORREF	oldColor;

	va_start(pvar, fmt);
	len	= _vstprintf(buf, fmt, pvar);
	va_end(pvar);

	oldColor = dc.SetTextColor(color);
	dc.SetBkMode(TRANSPARENT);

	if(font != NULL)
		orgFont	= dc.SelectObject(font);
	
	dc.TextOut(x, y, buf, len);

	dc.SetTextColor(oldColor);

	if(font != NULL)
		dc.SelectObject(orgFont);
}

/*===================================================*/
/* fill a specified rectangle area by solid color.
	2003/10/27 new
*/
void	polygonFillSolid(CDC &dc, COLORREF color, POINT *lineP, int count )
{
	CBrush	brush, *pBrushOld;
	CPen	*pPenOld;

	/* generate a brush for drawing. */
	brush.CreateSolidBrush(color);

	/* set drawing operation code. */
	dc.SetROP2(R2_COPYPEN);

	/* set using brush. */
	pBrushOld = dc.SelectObject(&brush);
	/* set using pen, for no outline drawing. */
	pPenOld = dc.SelectObject(CPen::FromHandle(GetStockPen(NULL_PEN)));

	/* draw Polygon. */
	Polygon( dc, lineP, count );

	/* set stocked object to device handle for deleting.
		DON'T FORGET !!!!!
	*/
	dc.SelectObject(pBrushOld);
	dc.SelectObject(pPenOld); 
	/* delete used brush. */
	brush.DeleteObject();
}

#if 0
/*===================================================*/
/* draw a poligon.
	2003/10/27 new
*/
void	polygon(HDC dc, int style, int pWidth, int color, int func, POINT *lineP, int count )
{
	HPEN	pen, hPenOld;
	HBRUSH	hBrushOld;

	/* select logical palette. */
	selectLogicalPalette(dc);

	/* generate a pen for drawing. */
	pen	= CreatePen( style, pWidth, PALETTEINDEX( color ) );

	/* set drawing operation code. */
	SetROP2( dc, func );

	/* set using pen. */
	hPenOld = SelectPen( dc, pen );

	/* set using brush. */
	hBrushOld = SelectBrush( dc, GetStockBrush( NULL_BRUSH ) );

	/* draw rectangle. */
	Polygon( dc, lineP, count );

	/* set stocked object to device handle for deleting.
	*/
	SelectBrush( dc, hBrushOld );
	SelectPen( dc, hPenOld );

	/* delete used pen. */
	DeletePen( pen );
}
#endif
