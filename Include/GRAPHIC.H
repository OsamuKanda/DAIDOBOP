#ifndef	graphic_h
# define	graphic_h

#include	"afxwin.h"

/*----------------------------------------------------*/
/* general symbols
*/
#ifndef	ERROR
/* caution!, the value of ERROR is not -1 in windows apprication. */
# define	ERROR	0
#endif
#ifndef	OK
/* caution!, the value of OK is not 0 in windows apprication. */
# define	OK		(!ERROR)
#endif
#ifndef	M_PI
# define	M_PI	3.14159265358979323846
#endif
#ifndef	newcell
# include	<malloc.h>
# define	newcell(type)		(type *)calloc(1, sizeof(type))
#endif

//#define	Window	HWND

/*---------------------------------------------------*/
/* Mouse geometry clculate function.
*/
#define	eventX(p)	(LOWORD((DWORD)(p)))
#define	eventY(p)	(HIWORD((DWORD)(p)))

/*---------------------------------------------------*/
/* the first declaration of WndProc.
*/
#define	beginGraphic(win, msg)											\
			PAINTSTRUCT	ps;		HDC	dc;									\
			void	__beginWndProc(HWND, UINT);							\
			void	__endWndProc(HWND, UINT);							\
			__beginWndProc(win, msg);									\
			if (msg == WM_PAINT)	dc	= BeginPaint(win, &ps);			\
			else					dc	= GetDC(win)

/*---------------------------------------------------*/
/* the last declaration of WndProc.
*/
#define	endGraphic(win, msg)											\
			if (msg == WM_PAINT)	EndPaint(win, &ps);					\
			else					ReleaseDC(win, dc);					\
			__endWndProc(win, msg)										\

/*---------------------------------------------------*/
/* the procedure for default Messages in parent WndProc.
*/
#define	defaultProcsParent(win,msg,wParam,lParam)						\
			case WM_QUERYNEWPALETTE: {									\
				beginGraphic(win, msg);									\
				queryNewPaletteProc(win, dc);							\
				endGraphic(win, msg);									\
				break;													\
			}															\
			case WM_PALETTECHANGED: {									\
				beginGraphic(win, msg);									\
				paletteChangedProc(win, dc, wParam);					\
				endGraphic(win, msg);									\
				break;													\
			}															\
			default:													\
				return	DefWindowProc(win, msg, wParam, lParam)

/*---------------------------------------------------*/
/* the procedure for default Messages in child WndProc.
*/
#define	defaultProcsChild(win,msg,wParam,lParam)						\
			default:													\
				return	DefWindowProc(win, msg, wParam, lParam)

/*---------------------------------------------------*/
/* functions for color allocation.
*/
void    initializeColors(void);
void	setupPalette(void);
int		allocColor(int r, int g, int b);
void	allocColorTones(int n, int span, int offset, COLORREF *colors);
void	allocColorTones64(COLORREF *colors);

void	queryNewPaletteProc(HWND, HDC);
void	paletteChangedProc(HWND, HDC, WPARAM);

void	redrawWindow(HWND);

void	line(CDC &dc,int style,int width,COLORREF color,int func,int,int,int,int);
void	box(CDC &dc, int style, int penwidth, COLORREF color, int func, int x, int y, int width, int height);
void	boxFillSolid(CDC &dc,int color,int x,int y,int width,int height);
void	circle(HDC,int style,int width,int color,int func,int,int,int);
void	circleFillSolid(CDC &dc, COLORREF color, int x, int y, int r);
void	gprintf(HDC,HFONT,COLORREF,int x,int y,_TCHAR *fmt,...);

void	logPrintf(_TCHAR *fmt,...);
void 	delPalette(void);

void	polyPline(CDC &dc,int style,int width,int color,int func, POINT *, int);
void	linePStart(CDC &,int style,int width,COLORREF color,int func);
void	lineP(CDC&,int,int,int,int);
void	linePEnding(CDC&);
void	PrintText(CDC &dc, CFont *font, COLORREF color, int x, int y, LPCTSTR fmt, ...);

/* draw a poligon.	2003/10/27 new*/
void	polygonFillSolid(CDC &dc, COLORREF color, POINT *lineP, int count );
void	polygon(HDC dc, int style, int pWidth, int color, int func, POINT *lineP, int count );

//#define	NumColorTone	16
//#define	NumColorTone	64

void	gtextout(HDC dc, HFONT font, COLORREF color, int x, int y, _TCHAR *buf );

#endif	/* graphic_h	*/
