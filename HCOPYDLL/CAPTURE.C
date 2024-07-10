/*
 * Capture.c
 *
 * Contains code for the following functions used in DibView:
 *
 *  CopyWindowToDIB()   - Copies a window to a DIB
 *  CopyScreenToDIB()   - Copies entire screen to a DIB
 *  DestroyDIB()        - Deletes DIB when finished using it
 *  SelectWindow()      - Allows user to point to a window on the screen
 *
 * Copyright (c) 1991 Microsoft Corporation. All rights reserved.
 *
 */

#define		STRICT
#include	<windows.h>
#include	"capture.h"
#include	"palette.h"
#include	"dib.h"

// Prototypes for local functions
HBITMAP CopyScreenToBitmap( LPRECT lpRect );
void HighlightWindow( HWND hwnd, BOOL fDraw );

#if 1	// kawai
// Create a 24-bit-per-pixel surface.
HBITMAP Create24BPPDIBSection( HDC hDC, int iWidth, int iHeight )
{
	BITMAPINFO bmi;
	HBITMAP hbm;
	LPBYTE pBits;

	// Initialize to 0s.
	ZeroMemory(&bmi, sizeof( bmi ) );
	// Initialize the header.
	bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = iHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	// Create the surface.
	hbm = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0 );
	return hbm;
}
#endif

/*************************************************************************
 *
 * CopyScreenToDIB()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-independent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * independent bitmap.
 *
 ************************************************************************/

HDIB CopyScreenToDIB( LPRECT lpRect )
{
	HBITMAP hBitmap;		// handle to device-dependent bitmap
	HPALETTE hPalette;		// handle to palette
	HDIB hDIB=NULL;			// handle to DIB

	/*  get the device-dependent bitmap in lpRect by calling
	 *  CopyScreenToBitmap and passing it the rectangle to grab
	 */
	hBitmap = CopyScreenToBitmap( lpRect );

	/* check for a valid bitmap handle */
	if ( !hBitmap )
		return NULL;

	/* get the current palette */
	hPalette = GetSystemPalette();

	/* convert the bitmap to a DIB */
	hDIB = BitmapToDIB( hBitmap, hPalette );

	/* clean up */
	DeleteObject( hBitmap );
	DeleteObject( hPalette );

	/* return handle to the packed-DIB */
	return hDIB;
}


/*************************************************************************
 *
 * DestroyDIB ()
 *
 * Purpose:  Frees memory associated with a DIB
 *
 * Returns:  Nothing
 *
 *************************************************************************/

WORD DestroyDIB( HDIB hDib )
{
	GlobalFree( hDib );
	return 0;
}


/*************************************************************************
 *
 * CopyScreenToBitmap()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * dependent bitmap.
 *
 ************************************************************************/

HBITMAP CopyScreenToBitmap( LPRECT lpRect )
{
	HDC hScrDC, hMemDC;				// screen DC and memory DC
	HBITMAP hBitmap, hOldBitmap;	// handles to deice-dependent bitmaps
	int nX, nY, nX2, nY2;			// coordinates of rectangle to grab
	int nWidth, nHeight;			// DIB width and height
	int xScrn, yScrn;				// screen resolution

	BOOL bRet;
	/* check for an empty rectangle */
	if ( IsRectEmpty( lpRect ) )
		return NULL;

	/*  create a DC for the screen and create
	 *  a memory DC compatible to screen DC
	 */
	hScrDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
	if ( hScrDC == NULL )
		return NULL;

	hMemDC = CreateCompatibleDC( hScrDC );
	if ( hMemDC == NULL )
		return NULL;

	/* get points of rectangle to grab */
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	/* get screen resolution */
	xScrn = GetDeviceCaps( hScrDC, HORZRES );
	yScrn = GetDeviceCaps( hScrDC, VERTRES );

	/* make sure bitmap rectangle is visible */
	if ( nX < 0 )
		nX = 0;
	if ( nY < 0 )
		nY = 0;
	if ( nX2 > xScrn )
		nX2 = xScrn;
	if ( nY2 > yScrn )
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
#if 1	// kawai
	hBitmap = Create24BPPDIBSection( hScrDC, lpRect->right, lpRect->bottom );
#else
	/* create a bitmap compatible with the screen DC */
	hBitmap = CreateCompatibleBitmap( hScrDC, nWidth, nHeight );
#endif
	if ( hBitmap == NULL )
		return NULL;

	/* select new bitmap into memory DC */
	hOldBitmap = ( HBITMAP )SelectObject( hMemDC, hBitmap );
	if ( hOldBitmap == (HBITMAP)NULL || hOldBitmap == (HBITMAP)GDI_ERROR )
		return NULL;

	/* bitblt screen DC to memory DC */
	bRet = BitBlt( hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY );
	if ( bRet == FALSE )
		return NULL;

	/*  select old bitmap back into memory DC and get handle to
	 *  bitmap of the screen
	 */
	hBitmap = (HBITMAP)SelectObject( hMemDC, hOldBitmap );
	if ( hBitmap == (HBITMAP)NULL || hBitmap == (HBITMAP)GDI_ERROR )
		return NULL;

	/* clean up */
	bRet = DeleteDC( hScrDC );
	if ( bRet == FALSE )
		return NULL;

	bRet = DeleteDC( hMemDC );
	if ( bRet == FALSE )
		return NULL;

	/* return handle to the bitmap */
	return hBitmap;
}


/*************************************************************************
 *
 * CaptureFullScreen
 *
 * Parameters:
 *
 * HWND hWndParent  - specifies the "parent" window.  If bHide, this window
 *                    is hidden during the capture operation.
 *
 *
 * Return Value:
 *
 * HDIB             - identifies the device-independent bitmap
 *
 * Description:
 *
 * This function copies the entire screen into a DIB, returning a
 * handle to the global memory with the DIB in it.
 *
 ************************************************************************/

HANDLE CaptureFullScreen( HWND hWndParent )
{
	RECT rScreen;			// Rect containing entire screen
	HDC  hDC;				// DC to screen
	MSG  msg;				// Message for the PeekMessage()
	HDIB hDib;				// Handle to DIB

	// Create a DC for the display

	hDC = CreateDC( "DISPLAY", NULL, NULL, NULL );
	if ( hDC == NULL )
		return NULL;

	rScreen.left = rScreen.top = 0;
	rScreen.right = GetDeviceCaps( hDC, HORZRES );
	rScreen.bottom = GetDeviceCaps( hDC, VERTRES );

	/* clean up '070612 tanno add */
	if ( DeleteDC( hDC ) == FALSE )
		return NULL;

	// Wait here until the apps on the screen have a chance to
	// repaint themselves.  Make our own message loop, and process
	// the messages we see.  When PeekMessage() returns, we know
	// that all the other app's WM_PAINT messages have been
	// processed.

	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// Call the function which captures screen
	hDib = CopyScreenToDIB( &rScreen );

	// Restore the DIBVIEW window
	return hDib;
}
