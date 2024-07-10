// OnlinePC.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "TcName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COnlinePCApp
BEGIN_MESSAGE_MAP(COnlinePCApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// COnlinePCApp construction
COnlinePCApp::COnlinePCApp()
{
	memset(m_bTcExp, 0, sizeof(m_bTcExp));
	memset(m_nCutOffTimer, 0, sizeof(m_nCutOffTimer));
	memset(&m_stTcNm, 0, sizeof(m_stTcNm));
}

// The one and only COnlinePCApp object
COnlinePCApp theApp;

// COnlinePCApp initialization
BOOL COnlinePCApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	// set priority
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// 一時ファイル操作用同期Object初期化
	InitializeCriticalSection(&m_lnLock);

	// Tc Name 読み込み
	CTcName	cls(-1);
	// real tc name from file.
	if( !cls.ReadTcNameFile(&m_stTcNm) )
	{
		// if err,
		cls.SetDefaultTCName(&m_stTcNm);
		cls.WriteTcNameFile(&m_stTcNm);
	}

	// ソケットの初期化
	if (!AfxSocketInit())
	{
		return FALSE;
	}

	// initialize color
	if( !m_col.Initialize() )
	{
		return FALSE;
	}

	// initialize configuration
	if( !m_cnf.Initialize() )
	{
		return FALSE;
	}

	// initialize configuration for batchsim
	if( !m_cnfBatch.Initialize() )
	{
		return FALSE;
	}

	// initialize configuration for batchsim dep.
	if( !m_cnfBatchD.Initialize() )
	{
		return FALSE;
	}

	// initialize configuration for boexecute
	if( !m_cnfBoexe.Initialize() )
	{
		return FALSE;
	}

	// initialize configuration for boexecute dep.
	if( !m_cnfBoexeD.Initialize() )
	{
		return FALSE;
	}

	// initialize log
	if( !m_log.Initialize() )
	{
		return FALSE;
	}

	// open main menu
	COnlinePCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		//  dismissed with Cancel
	}

	// uninitialize
	m_cnf.Uninitialize();

	// 一時ファイル操作用同期Object削除
	DeleteCriticalSection(&m_lnLock);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
