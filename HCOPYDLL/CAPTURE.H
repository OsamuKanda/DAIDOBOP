/*
 *  capture.h
 *
 */

/* Handle to a DIB */
#define HDIB HANDLE

/* Print Area selection */
#define PW_WINDOW        1
#define PW_CLIENT        2


/* Function prototypes */
HDIB    CopyScreenToDIB(LPRECT);
WORD    DestroyDIB(HDIB);
HANDLE  CaptureFullScreen (HWND hWndParent);



