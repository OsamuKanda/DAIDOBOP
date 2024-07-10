#pragma once

#include "atlfile.h"
#include "Bodef.h"

class SharedMemSet
{
public:
	SharedMemSet(void);
	~SharedMemSet(void);

	BOOL createSharedMem(void);
	void closeSharedMem(void);

	CAtlFileMappingBase m_atlFileMapSCOWrite;
	CAtlFileMappingBase m_atlFileMapBOPWrite;

	void writeSharedData(short *vt);
	void readSharedData(StSampledData *stData, void *sg);

};
