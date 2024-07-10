// OnlinePC.h : main header file for the BO Online PC application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Config.h"					// configuration interface
#include "Bolog.h"					// bo log interface
#include "BoColor.h"				// color interface
#include "../include/ChartLib.h"	// chart library
#include "NetConfig.h"				// Network configuration
#include "resource.h"				// main symbols

typedef struct
{
//	double	dVc;
//	double	dSn;
//	double	dLevel;
//	double	dWidth;
//	double	dWidthL;
//	double	dWidthR;
	double	dLen;
	double	dTdWeight;
	double	dCoolWtFlow;
	double	dCoolWtFlow2;
	double	dCoolWtFlow3;
	double	dCoolWtTemp;
//	double	dEMBr;
}StActiveInfo;


// COnlinePCApp class:
// See OnlinePC.cpp for the implementation of this class
//
class COnlinePCApp : public CWinApp
{
public:
	COnlinePCApp();

// member data
public:
	CConfig		m_cnfBoexe;		// configuration info for boexe
	CConfig		m_cnfBoexeD;	// configuration info for boexe dep
	CConfig		m_cnfBatch;		// configuration info for batchsim
	CConfig		m_cnfBatchD;	// configuration info for batchsim dep
	CConfig		m_cnf;			// configuration info
	CBolog		m_log;			// bo log info
	CBoColor	m_col;			// color info

	CChartLib	m_trendChart;	// chartlib for trend
	CChartLib	m_simChart;		// chartlib for sim
	CChartLib	m_batchChart;	// chartlib for batchsim
	CChartLib	m_boexeChart;	// chartlib for boexe
	CChartLib	m_boviewChart5;	// chartlib for boview
	CChartLib	m_boviewChart15;// chartlib for boview
	CChartLib	m_boviewChart30;// chartlib for boview
	CChartLib	m_boviewChart60;// chartlib for boview
	CChartLib	m_boviewChartFz;// chartlib for boview
	CChartLib	m_tcChart[32];	// chartlib for tc
	CChartLib	m_tcChart2[32];	// chartlib for tc(for alarm)

	CNetConfig	m_netcnf;		// net config

	BOOL		m_bTcExp[NumADChannel];			// cut off TC.
	int			m_nCutOffTimer[NumADChannel];	// cut off timer.

	CRITICAL_SECTION	m_lnLock;	// 学習一時ファイル操作用

	TcName		m_stTcNm;		// TC名

// member function
public:

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation
	DECLARE_MESSAGE_MAP()
};

extern COnlinePCApp theApp;
