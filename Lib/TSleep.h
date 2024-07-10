// TSleep.h: CTSleep �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLEEP_H_)
#define AFX_TSLEEP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������

class CTSleep  
{
public:
	CTSleep();
	virtual ~CTSleep();

public:
	void Sleep(DWORD dwMsec);
	void Sleep(DWORD dwMsec, LPDWORD pdwStop, DWORD dwTick = 1);	// ��~�t���O�t

private:
	void StartSleep(void);
	void WatchSleep(LPDWORD pdwStop, DWORD dwTick);
};

#endif // !defined(AFX_TSLEEP_H_)
