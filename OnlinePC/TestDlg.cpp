// TestDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TestDlg.h"
#include "PredicDlg.h"
#include ".\testdlg.h"


// CTestDlg ダイアログ

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	m_nTimerID = -1;

	// create dialog.
	Create(CTestDlg::IDD, pParent);				
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VCONOFF, m_rdoPrOn);
	DDX_Control(pDX, IDC_CHARGEONOFF, m_rdoChargeChange);
	DDX_Control(pDX, IDC_AUTOLCONOFF, m_rdoAutoLevel);
	DDX_Control(pDX, IDC_TDCHGONOFF, m_rdoTdChg);
	DDX_Control(pDX, IDC_L1ERRONOFF, m_rdoL1Err);
	DDX_Control(pDX, IDC_BOPCOND_RDO, m_rdoBopCond);
	DDX_Control(pDX, IDC_BOCOUT, m_chkBocOut);
	DDX_Control(pDX, IDC_BOOUT, m_chkBoOut);
	DDX_Control(pDX, IDC_BORUN, m_chkBoRun);
	DDX_Control(pDX, IDC_PLC_ERR1, m_txtPlcErr1);
	DDX_Control(pDX, IDC_PLC_ERR2, m_txtPlcErr2);
	DDX_Control(pDX, IDC_PLC_ERR3, m_txtPlcErr3);
	DDX_Control(pDX, IDC_PLC_ERR4, m_txtPlcErr4);
	DDX_Control(pDX, IDC_PLC_ERR5, m_txtPlcErr5);
	DDX_Control(pDX, IDC_PLC_ERR6, m_txtPlcErr6);
	DDX_Control(pDX, IDC_PLC_ERR7, m_txtPlcErr7);
	DDX_Control(pDX, IDC_PLC_ERR8, m_txtPlcErr8);
	DDX_Control(pDX, IDC_PLC_ERR9, m_txtPlcErr9);
	DDX_Control(pDX, IDC_PLC_ERR10, m_txtPlcErr10);
	DDX_Control(pDX, IDC_PLC_ERR11, m_txtPlcErr11);
	DDX_Control(pDX, IDC_PLC_ERR12, m_txtPlcErr12);
	DDX_Control(pDX, IDC_PLC_ERR13, m_txtPlcErr13);
	DDX_Control(pDX, IDC_PLC_ERR14, m_txtPlcErr14);
	DDX_Control(pDX, IDC_PLC_ERR15, m_txtPlcErr15);
	DDX_Control(pDX, IDC_PLC_ERR16, m_txtPlcErr16);
	DDX_Control(pDX, IDC_PLC_ERR17, m_txtPlcErr17);
	DDX_Control(pDX, IDC_PLC_ERR18, m_txtPlcErr18);
	DDX_Control(pDX, IDC_PLC_ERR19, m_txtPlcErr19);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDC_VCONOFF, OnClickedPrOn)
	ON_BN_CLICKED(IDC_CHARGEONOFF, OnClickedChargeChange)
	ON_BN_CLICKED(IDC_AUTOLCONOFF, OnClickedAutoLevel)
	ON_BN_CLICKED(IDC_TDCHGONOFF, OnClickedTdChange)
	ON_BN_CLICKED(IDC_L1ERRONOFF, OnClickedL1Error)
	ON_BN_CLICKED(IDC_ENABSIM, OnBnClickedEnabsim)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BOCOUT, &CTestDlg::OnBnClickedBocout)
	ON_BN_CLICKED(IDC_BOOUT, &CTestDlg::OnBnClickedBoout)
	ON_BN_CLICKED(IDC_BORUN, &CTestDlg::OnBnClickedBorun)
	ON_BN_CLICKED(IDCANCEL, &CTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTestDlg message handler

//It evades ending by Enter Key.
void CTestDlg::OnOK(){}

//It evades ending by ESC Key.
void CTestDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CTestDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// clicked the enable sim button.
void CTestDlg::OnBnClickedEnabsim()
{
	// update the execution condition.
	BoConditionManage(IDC_ENABSIM, -1);
}
\
// clicked P/R On button.
void CTestDlg::OnClickedPrOn()
{
	// update the execution condition.
	BoConditionManage(IDC_VCONOFF, -1);
}

// clicked charge change button.
void CTestDlg::OnClickedChargeChange()
{
	// update the execution condition.
	BoConditionManage(IDC_CHARGEONOFF, -1);
}

// clicked auto level button.
void CTestDlg::OnClickedAutoLevel()
{
	// update the execution condition.
	BoConditionManage(IDC_AUTOLCONOFF, -1);
}

// clicked auto level button.
void CTestDlg::OnClickedTdChange()
{
	// update the execution condition.
	BoConditionManage(IDC_TDCHGONOFF, -1);
}

// clicked L1 error button.
void CTestDlg::OnClickedL1Error()
{
	// update the execution condition.
	BoConditionManage(IDC_L1ERRONOFF, -1);
}

// B.O.警報出力
void CTestDlg::OnBnClickedBocout()
{
	BoConditionManage(IDC_BOCOUT, -1);
}

// B.O.注意報出力
void CTestDlg::OnBnClickedBoout()
{
	BoConditionManage(IDC_BOOUT, -1);
}

// B.O.装置稼働中
void CTestDlg::OnBnClickedBorun()
{
	BoConditionManage(IDC_BORUN, -1);
}

// WM_TIMER
void CTestDlg::OnTimer(UINT nIDEvent)
{
	static BOOL	sMemBopCond = FALSE;
	static BOOL sMemPlcErr1 = 0;
	static BOOL sMemPlcErr2 = 0;
	int		nErr1, nErr2;
	CString		str;
	CPredicDlg	*pDlg = (CPredicDlg *)GetParent();
	if( pDlg == NULL || pDlg->m_pShMem == NULL )
		return;

	if (sMemBopCond == FALSE && pDlg->m_pShMem->BopCondition == TRUE) {
		sMemBopCond = TRUE;
		m_rdoBopCond.SetCheck(BST_CHECKED);
	}
	else if (sMemBopCond == TRUE && pDlg->m_pShMem->BopCondition == FALSE) {
		sMemBopCond = FALSE;
		m_rdoBopCond.SetCheck(BST_UNCHECKED);
	}

	// PLC異常情報取得
	nErr1 = pDlg->m_pShMem->inPlcErr1;
	nErr2 = pDlg->m_pShMem->inPlcErr2;

	if( (nErr1 & 0x1) == 0x1 )
		m_txtPlcErr1.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr1.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x2) == 0x2 )
		m_txtPlcErr2.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr2.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x4) == 0x4 )
		m_txtPlcErr3.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr3.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x8) == 0x8 )
		m_txtPlcErr4.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr4.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x10) == 0x10 )
		m_txtPlcErr5.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr5.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x20) == 0x20 )
		m_txtPlcErr6.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr6.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x40) == 0x40 )
		m_txtPlcErr7.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr7.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x80) == 0x80 )
		m_txtPlcErr8.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr8.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x100) == 0x100 )
		m_txtPlcErr9.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr9.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x200) == 0x200 )
		m_txtPlcErr10.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr10.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x400) == 0x400 )
		m_txtPlcErr11.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr11.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x800) == 0x800 )
		m_txtPlcErr12.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr12.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x1000) == 0x1000 )
		m_txtPlcErr13.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr13.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x2000) == 0x2000 )
		m_txtPlcErr14.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr14.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x4000) == 0x4000 )
		m_txtPlcErr15.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr15.SetBackColor(RGB(128,128,128));
	if( (nErr1 & 0x8000) == 0x8000 )
		m_txtPlcErr16.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr16.SetBackColor(RGB(128,128,128));
	if( (nErr2 & 0x1) == 0x1 )
		m_txtPlcErr17.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr17.SetBackColor(RGB(128,128,128));
	if( (nErr2 & 0x2) == 0x2 )
		m_txtPlcErr18.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr18.SetBackColor(RGB(128,128,128));
	if( (nErr2 & 0x4) == 0x4 )
		m_txtPlcErr19.SetBackColor(RGB(255,0,0));
	else
		m_txtPlcErr19.SetBackColor(RGB(128,128,128));

	CDialog::OnTimer(nIDEvent);
}

// update the execution condition.
void CTestDlg::BoConditionManage(WPARAM wParam, int cmd)
{
	CPredicDlg	*pDlg = (CPredicDlg *)GetParent();
	switch(wParam)
	{
	case IDC_ENABSIM:
		if (cmd < 0)
			pDlg->m_pShMem->isEnabSimSignal	= !pDlg->m_pShMem->isEnabSimSignal;
		break;
	case IDC_VCONOFF:
		if (cmd < 0)
			pDlg->m_pShMem->simVcOn			= !pDlg->m_pShMem->simVcOn;
		else
			pDlg->m_pShMem->simVcOn			= cmd;
		m_rdoPrOn.SetCheck(pDlg->m_pShMem->simVcOn);
		break;
	case IDC_CHARGEONOFF:
		if (cmd < 0)
			pDlg->m_pShMem->simChargeChange	= !pDlg->m_pShMem->simChargeChange;
		else
			pDlg->m_pShMem->simChargeChange	= cmd;
		m_rdoChargeChange.SetCheck(pDlg->m_pShMem->simChargeChange);
		break;
	case IDC_AUTOLCONOFF:
		if (cmd < 0)
			pDlg->m_pShMem->simAutoLVCtl	= !pDlg->m_pShMem->simAutoLVCtl;
		else
			pDlg->m_pShMem->simAutoLVCtl	= cmd;
		m_rdoAutoLevel.SetCheck(pDlg->m_pShMem->simAutoLVCtl);
		break;
	case IDC_TDCHGONOFF:
		if (cmd < 0)
			pDlg->m_pShMem->simCastStart	= !pDlg->m_pShMem->simCastStart;
		else
			pDlg->m_pShMem->simCastStart	= cmd;
		m_rdoTdChg.SetCheck(pDlg->m_pShMem->simCastStart);
		break;
	case IDC_L1ERRONOFF:
		if (cmd < 0)
			pDlg->m_pShMem->simL1Err	= !pDlg->m_pShMem->simL1Err;
		else
			pDlg->m_pShMem->simL1Err	= cmd;
		m_rdoL1Err.SetCheck(pDlg->m_pShMem->simL1Err);
		break;
	case IDC_BOCOUT:
		if (cmd < 0)
			pDlg->m_pShMem->simBocOut	= !pDlg->m_pShMem->simBocOut;
		else
			pDlg->m_pShMem->simBocOut	= cmd;
		m_chkBocOut.SetCheck(pDlg->m_pShMem->simBocOut);
		break;
	case IDC_BOOUT:
		if (cmd < 0)
			pDlg->m_pShMem->simBoOut	= !pDlg->m_pShMem->simBoOut;
		else
			pDlg->m_pShMem->simBoOut	= cmd;
		m_chkBoOut.SetCheck(pDlg->m_pShMem->simBoOut);
		break;
	case IDC_BORUN:
		if (cmd < 0)
			pDlg->m_pShMem->simBoRun	= !pDlg->m_pShMem->simBoRun;
		else
			pDlg->m_pShMem->simBoRun	= cmd;
		m_chkBoRun.SetCheck(pDlg->m_pShMem->simBoRun);
		break;
	}
}

// initialize dialog
BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_txtPlcErr1.SetBackColor(RGB(128,128,128));
	m_txtPlcErr2.SetBackColor(RGB(128,128,128));
	m_txtPlcErr3.SetBackColor(RGB(128,128,128));
	m_txtPlcErr4.SetBackColor(RGB(128,128,128));
	m_txtPlcErr5.SetBackColor(RGB(128,128,128));
	m_txtPlcErr6.SetBackColor(RGB(128,128,128));
	m_txtPlcErr7.SetBackColor(RGB(128,128,128));
	m_txtPlcErr8.SetBackColor(RGB(128,128,128));
	m_txtPlcErr9.SetBackColor(RGB(128,128,128));
	m_txtPlcErr10.SetBackColor(RGB(128,128,128));
	m_txtPlcErr11.SetBackColor(RGB(128,128,128));
	m_txtPlcErr12.SetBackColor(RGB(128,128,128));
	m_txtPlcErr13.SetBackColor(RGB(128,128,128));
	m_txtPlcErr14.SetBackColor(RGB(128,128,128));
	m_txtPlcErr15.SetBackColor(RGB(128,128,128));
	m_txtPlcErr16.SetBackColor(RGB(128,128,128));
	m_txtPlcErr17.SetBackColor(RGB(128,128,128));
	m_txtPlcErr18.SetBackColor(RGB(128,128,128));
	m_txtPlcErr19.SetBackColor(RGB(128,128,128));

	m_nTimerID = SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// destroy dialog
void CTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if( m_nTimerID != -1 )
		KillTimer(m_nTimerID);
}

void CTestDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

// PLC異常ﾘｾｯﾄ
void CTestDlg::OnBnClickedButton1()
{
	CPredicDlg	*pDlg = (CPredicDlg *)GetParent();

	if( pDlg != NULL && pDlg->m_pShMem != NULL )
		pDlg->m_pShMem->outPlcErrReset	= 1;
}
