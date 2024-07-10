#pragma once
#include <windows.h>


typedef	struct {
	HGLOBAL	handle;
	DWORD	pointer;
} MemoryHandle;

typedef	struct {
	UINT			num;
	MemoryHandle	table[1];
} MemoryTable;


#define	DataUnitBufSize		(65536UL)

#define	PtrInMTab(type, mem, index)											\
			(type *)(mem->table[0].pointer + ((DWORD)sizeof(type) * (DWORD)(index)))

MemoryTable	*allocDataBuf(DWORD size, WORD flags);
int		lockDataBuf(MemoryTable *);
void	unlockDataBuf(MemoryTable *);
void	freeDataBuf(MemoryTable *);
