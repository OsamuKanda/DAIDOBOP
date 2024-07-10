#include	"../include/DataBuf.h"


// allocate data buffer.
MemoryTable	*allocDataBuf(DWORD size, WORD flags)
{
	UINT			i, n;
	MemoryTable		*table;
	MemoryHandle	*p;

	// calculate the number of unit buffer.
	n = 1; //@ 
	//@ n	= (unsigned int)((size + DataUnitBufSize - 1) / DataUnitBufSize);

	// allocate handler table.
	table	= (MemoryTable *)calloc(1,
				sizeof(MemoryTable) + (n - 1) * sizeof(MemoryHandle));
	// if not enough memory, return NULL.
	if (!table)
		return	NULL;

	// store table size.
	table->num	= n;

	// for each element of ID array, */
	for (i = 0, p = table->table; i < n; i++, p++) {
		// allocate global memory. if failed, break.
		//@ if ((p->handle = GlobalAlloc(flags, DataUnitBufSize)) == NULL)
		if ((p->handle = GlobalAlloc(flags, size)) == NULL)
			break;
	}

	// if total memory blocks were allocated.
	if (i == n) {
		// return the pointer of handler table.
		return	table;
	}

	// otherwise,
	// free allocated memory.
	for (i = 0, p = table->table; i < n; i++, p++) {
		if (p->handle)
			GlobalFree(p->handle);
	}
	free(table);

	// return NULL.
	return	NULL;
}

// lock data buffers.
BOOL lockDataBuf(MemoryTable *table)
{
	UINT			i, n;
	MemoryHandle	*p;

	// get the memory table size. 
	n	= table->num;

	// for each unit buffer,
	for (i = 0, p = table->table; i < n; i++, p++) {
		// lock allocated memory. if failed, break.
		if (p->handle) {
			p->pointer = (DWORD)GlobalLock(p->handle);
			if ( !p->pointer )
				break;
		}
		else
			p->pointer	= (DWORD)NULL;
	}
	// if total memory blocks were locked, return OK.
	if (i == n)
		return	TRUE;

	// otherwise,
	// unlock data buffers.
	for (i = 0, p = table->table; i < n; i++, p++) {
		if (p->handle) {
			GlobalUnlock(p->handle);
			p->pointer	= (DWORD)NULL;
		}
	}

	// return FALSE.
	return	FALSE;
}

// unlock data buffer.
void unlockDataBuf(MemoryTable *table)
{
	UINT			i, n;
	MemoryHandle	*p;

	// get the memory table size.
	n	= table->num;

	// for each data buffer,
	for (i = 0, p = table->table; i < n; i++, p++) {
		// unlock memory block.
		if (p->pointer) {
			GlobalUnlock(p->handle);
			p->pointer	= (DWORD)NULL;
		}
	}
}

// free data buffer.
void freeDataBuf(MemoryTable *table)
{
	UINT			i, n;
	MemoryHandle	*p;

	// get the memory table size.
	n	= table->num;

	// for each data buffer.
	for (i = 0, p = table->table; i < n; i++, p++) {
		// free memory block.
		if (p->handle)
			GlobalFree(p->handle);
	}

	// free the memory table.
	free(table);
}

