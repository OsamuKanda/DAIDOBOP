#include <afxmt.h>
#include "bodef.h"

static SharedMemory		sharedMemory;
static ViewData			sharedMemory2;
static CCriticalSection	viewLock;

// 共有メモリのアドレスを返す
SharedMemory *GetSharedMemory()
{
	return &sharedMemory;
}

// Viewデータのアドレスを返す
ViewData *GetSharedMemory2()
{
	return &sharedMemory2;
}

// Viewデータのロック
void ViewLock(void)
{
	viewLock.Lock();
}

// Viewデータのロック
void ViewUnlock(void)
{
	viewLock.Unlock();
}
