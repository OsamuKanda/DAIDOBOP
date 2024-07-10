/*************************************************************************

      File:  PALETTE.C

   Purpose:  Contains routines to display a palette in a popup window.
             Allows the user to scroll around the palette and dump info
             on particular colors in the palette.  Has much the same
             look as the Clipboard's palette dump.

 Functions:  PaletteWndProc
             HighlightSquare   
             UnHighlightSquare
             PalRowsAndColumns 
             SetPaletteWindowsPal
             PalEntriesOnDevice
             GetSystemPalette
             ColorsInPalette
             MyAnimatePalette
             CopyPaletteChangingFlags

  Comments:  The routines in this module are provided mostly for
             debugging purposes.  There are definite improvements
             that can be made.  For example, the scroll bars now
             let you scroll way beyond the existing colors in a
             palette.  They should probably be limited to the number
             of rows to be displayed in the palette window.

   History:   Date      Reason
             6/ 1/91     Created
            11/15/91     Use LoadString instead of a hardcoded
                         array of strings.  This frees up some
                         DS, and is better for localizing for
                         international markets.
             1/28/92     Added WM_QUERYNEWPALETTE message,
                         and always select palette as
                         a background palette in WM_PAINT.

*************************************************************************/


#define		STRICT
#include	<windows.h>
#include	<assert.h>
#include	"errors.h"
#include	"math.h"
#include	"palette.h"



// Useful magic numbers.
#define		PAL_SIZE_DEF       PALSIZE_MEDIUM    // Default palette square size.
#define		ID_INFO_CHILD      1                 // Palette information window ID


// Local function prototypes.
int	PalEntriesOnDevice( HDC hDC );


//---------------------------------------------------------------------
//
// Function:   PalEntriesOnDevice
//
// Purpose:    Returns the number of colors a device supports.
//
// Parms:      hDC == DC for the device we want # of colors for.
//
// Returns:    # of colors that the given device can represent.
//
// History:   Date      Reason
//             6/01/91  Created
//
//---------------------------------------------------------------------

int PalEntriesOnDevice( HDC hDC )
{
	int nColors;

	// Find out the number of palette entries on this
	//  defice.
	nColors = GetDeviceCaps( hDC, SIZEPALETTE );


	// For non-palette devices, we'll use the # of system
	//  colors for our palette size.

	if ( !nColors )
		nColors = GetDeviceCaps(hDC, NUMCOLORS );

	assert( nColors );

	return nColors;
}
#if 0
//---------------------------------------------------------------------
//
// Function:   GetSystemPalette
//
// Purpose:    This routine returns a handle to a palette which represents
//             the system palette (each entry is an offset into the system
//             palette instead of an RGB with a flag of PC_EXPLICIT).
//             Useful for dumping the system palette.
//
// Parms:      None
//
// Returns:    Handle to a palette consisting of the system palette
//             colors.
//
// History:   Date      Reason
//             6/01/91  Created
//
//---------------------------------------------------------------------

HPALETTE GetSystemPalette( void )
{
	HDC           hDC;
	HPALETTE      hPal = NULL;
	HANDLE        hLogPal;
	LPLOGPALETTE  lpLogPal;
	int           i, nColors;


	// Find out how many palette entries we want.

	hDC = GetDC( NULL );
	if ( !hDC )
	{
		DIBError( ERR_GETDC );
		return NULL;
	}

	nColors = PalEntriesOnDevice( hDC );
	ReleaseDC( NULL, hDC );


	  // Allocate room for the palette and lock it.

	hLogPal = GlobalAlloc( GHND, sizeof( LOGPALETTE ) +
						   nColors * sizeof( PALETTEENTRY ) );

	if ( !hLogPal )
	{
		DIBError( ERR_CREATEPAL );
		return NULL;
	}

	lpLogPal = ( LPLOGPALETTE )GlobalLock( hLogPal );

	lpLogPal->palVersion    = PALVERSION;
	lpLogPal->palNumEntries = ( WORD )nColors;

	for ( i = 0; i < nColors; i++ )
	{
		lpLogPal->palPalEntry[i].peBlue  = 0;
		*((LPWORD) (&lpLogPal->palPalEntry[i].peRed)) = (WORD)i;
		lpLogPal->palPalEntry[i].peFlags = PC_EXPLICIT;
	}

	// Go ahead and create the palette.  Once it's created,
	//  we no longer need the LOGPALETTE, so free it.

	hPal = CreatePalette( lpLogPal );

	GlobalUnlock( hLogPal );
	GlobalFree( hLogPal );

	return hPal;
}
#endif

// Create a copy of the current system palette.
HPALETTE GetSystemPalette( void )
{
	HDC hDC;
	HPALETTE hPal;
//	HANDLE hLogPal;
	LPLOGPALETTE lpLogPal;

	// Get a DC for the desktop.
	hDC = GetDC( NULL );
	if ( !hDC )
	{
		DIBError( ERR_GETDC );
		return NULL;
	}

	// Check to see if you are a running in a palette-based video mode.
	if ( !( GetDeviceCaps( hDC, RASTERCAPS ) & RC_PALETTE ) )
	{
		ReleaseDC( NULL, hDC );
		return NULL;
	}
	// Allocate memory for the palette.
	lpLogPal = GlobalAlloc( GPTR, sizeof( LOGPALETTE ) + 256 *
								sizeof( PALETTEENTRY ) );
	if ( !lpLogPal )
	{
		DIBError( ERR_CREATEPAL );
		return NULL;
	}

	// Initialize.
	lpLogPal->palVersion = 0x300;
	lpLogPal->palNumEntries = 256;

	// Copy the current system palette into the logical palette.
	GetSystemPaletteEntries( hDC, 0, 256,
							(LPPALETTEENTRY)( lpLogPal->palPalEntry ) );

	// Create the palette.
	hPal = CreatePalette( lpLogPal );

	// Clean up
	GlobalFree( lpLogPal );
	ReleaseDC( NULL, hDC );
	return hPal;
}
