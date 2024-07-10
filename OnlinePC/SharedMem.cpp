#include <afxmt.h>
#include "bodef.h"

static SharedMemory		sharedMemory;
static ViewData			sharedMemory2;
static CCriticalSection	viewLock;

// ���L�������̃A�h���X��Ԃ�
SharedMemory *GetSharedMemory()
{
	return &sharedMemory;
}

// View�f�[�^�̃A�h���X��Ԃ�
ViewData *GetSharedMemory2()
{
	return &sharedMemory2;
}

// View�f�[�^�̃��b�N
void ViewLock(void)
{
	viewLock.Lock();
}

// View�f�[�^�̃��b�N
void ViewUnlock(void)
{
	viewLock.Unlock();
}
