#ifndef	TMSParameter_h
//#pragma pack(1)

#include "Bodef.h"
#include "comdrv.h"


// Smart Casting Observer status Data
typedef struct {
	short		index;
} SCOStatusStruct;


//---------------------------------------
// Smart Casting Observer HMI Write Data
typedef struct {
	SampledData			sd;
	ViewData			vd;
	StInSignal			sg;

	SCOStatusStruct		st;
} SCOWriteDataStruct;

//---------------------------------------
// TMS(BOP) Write Data
typedef struct {
	short	val[2];
} TMSWriteDataStruct;

//#pragma pack()
#endif	/* TMSParameter_h	*/





