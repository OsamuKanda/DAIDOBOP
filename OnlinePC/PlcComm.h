#pragma once
#include <afxwin.h>
#include "../Include/Bodef.h"
#include "../Lib/Plc.h"

typedef enum
{
    PLC_CLOSE,
	PLC_WAIT,
	PLC_CONNECTED
}NmPlcState;

typedef enum
{
	idCmdNone	= 0,
	idCmdSend	= 1,
	idCmdNum	= 2
}NmPlcCommand;

class CPlcComm
{
public:
	CPlcComm();
	~CPlcComm();

// �����o�ϐ�
public:
	CPlc		m_plcd, m_plcc;
	uintptr_t	m_thData, m_thCmd;
	NmPlcState	m_nmPoll, m_nmCmd;
	BoNetInfo	m_stNetInfo;

protected:

// �����o�[�֐�
public:
	BOOL Initialize(void *pParent);
	BOOL CommandProc(NmPlcCommand nmCmd, WORD *pVal, int nSize);
};

// �O���֐�
BOOL SendPlc(NmPlcCommand nmCmd, void *pDat, int nSize);
