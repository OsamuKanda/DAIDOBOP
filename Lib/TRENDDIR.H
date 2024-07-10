#ifndef	trenddir_h
# define	trenddir_h

/*----------------------------------------------------*/
/* general symbols
*/
#include	<TCHAR.H>

BOOL RegKeyGet( LPCTSTR lpValueName, LPTSTR lpRegKeyData );

int getTrendDirectory(TrendDirectory *table);


#endif	/* trenddir_h	*/