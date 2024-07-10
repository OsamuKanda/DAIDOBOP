// BosysDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BosysDlg.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;


// CBosysDlg dialog
IMPLEMENT_DYNAMIC(CBosysDlg, CDialog)
CBosysDlg::CBosysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBosysDlg::IDD, pParent)
{
	// initialize member data.
	m_nSamplePeriod = 500;
	m_bPass = FALSE;
	m_pDlgTcView = NULL;
	m_page = 0;
	memset(&m_stBsInfo, 0, sizeof(m_stBsInfo));

	// create dialog.
	Create(CBosysDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CBosysDlg::~CBosysDlg()
{
}

void CBosysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT0, m_textSamplePeriod);
	DDX_Control(pDX, IDC_EDIT1, m_editComment[0]);
	DDX_Control(pDX, IDC_EDIT2, m_editComment[1]);
	DDX_Control(pDX, IDC_EDIT3, m_editComment[2]);
	DDX_Control(pDX, IDC_EDIT4, m_editComment[3]);
	DDX_Control(pDX, IDC_EDIT5, m_editComment[4]);
	DDX_Control(pDX, IDC_EDIT6, m_editComment[5]);
	DDX_Control(pDX, IDC_EDIT7, m_editComment[6]);
	DDX_Control(pDX, IDC_EDIT8, m_editComment[7]);
	DDX_Control(pDX, IDC_EDIT9, m_editComment[8]);
	DDX_Control(pDX, IDC_EDIT11, m_editWatchTime[0]);
	DDX_Control(pDX, IDC_EDIT12, m_editWatchTime[1]);
	DDX_Control(pDX, IDC_EDIT13, m_editWatchTime[2]);
	DDX_Control(pDX, IDC_EDIT14, m_editWatchTime[3]);
	DDX_Control(pDX, IDC_EDIT15, m_editWatchTime[4]);
	DDX_Control(pDX, IDC_EDIT16, m_editWatchTime[5]);
	DDX_Control(pDX, IDC_EDIT17, m_editWatchTime[6]);
	DDX_Control(pDX, IDC_EDIT18, m_editWatchTime[7]);
	DDX_Control(pDX, IDC_EDIT19, m_editWatchTime[8]);
	DDX_Control(pDX, IDC_LIST1, m_listBoRec[0]);
	DDX_Control(pDX, IDC_LIST2, m_listBoRec[1]);
	DDX_Control(pDX, IDC_LIST3, m_listBoRec[2]);
	DDX_Control(pDX, IDC_LIST4, m_listBoRec[3]);
	DDX_Control(pDX, IDC_LIST5, m_listBoRec[4]);
	DDX_Control(pDX, IDC_LIST6, m_listBoRec[5]);
	DDX_Control(pDX, IDC_LIST7, m_listBoRec[6]);
	DDX_Control(pDX, IDC_LIST8, m_listBoRec[7]);
	DDX_Control(pDX, IDC_LIST9, m_listBoRec[8]);
	//DDX_Control(pDX, IDC_LIST10, m_listBoPow[0]);
	//DDX_Control(pDX, IDC_LIST11, m_listBoPow[1]);
	//DDX_Control(pDX, IDC_LIST12, m_listBoPow[2]);
	//DDX_Control(pDX, IDC_LIST13, m_listBoPow[3]);
	//DDX_Control(pDX, IDC_LIST14, m_listBoPow[4]);
	//DDX_Control(pDX, IDC_LIST15, m_listBoPow[5]);
	//DDX_Control(pDX, IDC_LIST16, m_listBoPow[6]);
	//DDX_Control(pDX, IDC_LIST17, m_listBoPow[7]);
	//DDX_Control(pDX, IDC_LIST18, m_listBoPow[8]);
	DDX_Control(pDX, IDC_EDIT31, m_editAlmCombi[0]);
	DDX_Control(pDX, IDC_EDIT32, m_editAlmCombi[1]);
	DDX_Control(pDX, IDC_EDIT33, m_editAlmCombi[2]);
	DDX_Control(pDX, IDC_EDIT34, m_editAlmCombi[3]);
	DDX_Control(pDX, IDC_EDIT35, m_editAlmCombi[4]);
	DDX_Control(pDX, IDC_EDIT36, m_editAlmCombi[5]);
	DDX_Control(pDX, IDC_EDIT37, m_editAlmCombi[6]);
	DDX_Control(pDX, IDC_EDIT38, m_editAlmCombi[7]);
	DDX_Control(pDX, IDC_EDIT39, m_editAlmCombi[8]);
	DDX_Control(pDX, IDC_EDIT41, m_editAlmCtlCombi[0]);
	DDX_Control(pDX, IDC_EDIT42, m_editAlmCtlCombi[1]);
	DDX_Control(pDX, IDC_EDIT43, m_editAlmCtlCombi[2]);
	DDX_Control(pDX, IDC_EDIT44, m_editAlmCtlCombi[3]);
	DDX_Control(pDX, IDC_EDIT45, m_editAlmCtlCombi[4]);
	DDX_Control(pDX, IDC_EDIT46, m_editAlmCtlCombi[5]);
	DDX_Control(pDX, IDC_EDIT47, m_editAlmCtlCombi[6]);
	DDX_Control(pDX, IDC_EDIT48, m_editAlmCtlCombi[7]);
	DDX_Control(pDX, IDC_EDIT49, m_editAlmCtlCombi[8]);
	DDX_Control(pDX, IDC_BUTTON10, m_btnPass);
	DDX_Control(pDX, IDC_EDIT10, m_editCondLen);
	DDX_Control(pDX, IDC_EDIT20, m_editCondVc);
	DDX_Control(pDX, IDC_AUTOTC_PRM1, m_editAutoTcPrm1);
	DDX_Control(pDX, IDC_AUTOTC_PRM2, m_editAutoTcPrm2);
	DDX_Control(pDX, IDC_AUTOTC_PRM3, m_editAutoTcPrm3);
	DDX_Control(pDX, IDC_AUTOTC_PRM4, m_editAutoTcPrm4);
	DDX_Control(pDX, IDC_AUTOTC_PRM5, m_editAutoTcPrm5);
	DDX_Control(pDX, IDC_PLCIP, m_ipPlc);
	DDX_Control(pDX, IDC_EDIT89, m_editConfName[0]);
	DDX_Control(pDX, IDC_EDIT90, m_editConfName[1]);
	DDX_Control(pDX, IDC_EDIT91, m_editConfName[2]);
	DDX_Control(pDX, IDC_EDIT92, m_editConfName[3]);
	DDX_Control(pDX, IDC_EDIT93, m_editConfName[4]);
	DDX_Control(pDX, IDC_EDIT94, m_editConfName[5]);
	DDX_Control(pDX, IDC_EDIT95, m_editConfName[6]);
	DDX_Control(pDX, IDC_EDIT96, m_editConfName[7]);
	DDX_Control(pDX, IDC_EDIT97, m_editConfName[8]);
	DDX_Control(pDX, IDC_EDIT80, m_editConfNameD[0]);
	DDX_Control(pDX, IDC_EDIT81, m_editConfNameD[1]);
	DDX_Control(pDX, IDC_EDIT82, m_editConfNameD[2]);
	DDX_Control(pDX, IDC_EDIT83, m_editConfNameD[3]);
	DDX_Control(pDX, IDC_EDIT84, m_editConfNameD[4]);
	DDX_Control(pDX, IDC_EDIT85, m_editConfNameD[5]);
	DDX_Control(pDX, IDC_EDIT86, m_editConfNameD[6]);
	DDX_Control(pDX, IDC_EDIT87, m_editConfNameD[7]);
	DDX_Control(pDX, IDC_EDIT88, m_editConfNameD[8]);
	DDX_Control(pDX, IDC_PAGE, m_txtPage);
	DDX_Control(pDX, IDC_BUTTON11, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON13, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON1, m_btnGradeNo[0]);
	DDX_Control(pDX, IDC_BUTTON2, m_btnGradeNo[1]);
	DDX_Control(pDX, IDC_BUTTON3, m_btnGradeNo[2]);
	DDX_Control(pDX, IDC_BUTTON4, m_btnGradeNo[3]);
	DDX_Control(pDX, IDC_BUTTON5, m_btnGradeNo[4]);
	DDX_Control(pDX, IDC_BUTTON6, m_btnGradeNo[5]);
	DDX_Control(pDX, IDC_BUTTON7, m_btnGradeNo[6]);
	DDX_Control(pDX, IDC_BUTTON8, m_btnGradeNo[7]);
	DDX_Control(pDX, IDC_BUTTON9, m_btnGradeNo[8]);
}


BEGIN_MESSAGE_MAP(CBosysDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_COMMAND(IDM_LOAD_DATA, OnLoadData)
	ON_COMMAND(IDM_SAVE_DATA, OnSaveData)
	ON_WM_SHOWWINDOW()
	ON_BN_DOUBLECLICKED(IDC_BUTTON1, OnDoubleclickedButton1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON2, OnDoubleclickedButton2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON3, OnDoubleclickedButton3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON4, OnDoubleclickedButton4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON5, OnDoubleclickedButton5)
	ON_BN_DOUBLECLICKED(IDC_BUTTON6, OnDoubleclickedButton6)
	ON_BN_DOUBLECLICKED(IDC_BUTTON7, OnDoubleclickedButton7)
	ON_BN_DOUBLECLICKED(IDC_BUTTON8, OnDoubleclickedButton8)
	ON_BN_DOUBLECLICKED(IDC_BUTTON9, OnDoubleclickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnClickedProtectButton)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_DATA, OnUpdateLoadData)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_DATA, OnUpdateSaveData)
	ON_UPDATE_COMMAND_UI(IDM_TCPOSITION, OnUpdateTcposition)
	ON_WM_MOVE()
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_BN_CLICKED(IDC_BUTTON11, &CBosysDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON13, &CBosysDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBosysDlg message handler

//It evades ending by Enter Key.
void CBosysDlg::OnOk(){}

//It evades ending by ESC Key.
void CBosysDlg::OnCancel(){}

void CBosysDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CBosysDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

BOOL CBosysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup dialog
	SetupBosysCondition();

	// Display bosys information.
	DispBosysInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CBosysDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		// setup dialog
		SetupBosysCondition();

		// Display bosys information.
		m_page = 0;
		DispBosysInfo();
	}
	else
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

// load bosys data
void CBosysDlg::OnLoadData()
{
	m_page = 0;
	DispBosysInfo();
}

// save bosys data
void CBosysDlg::OnSaveData()
{
	int		i;

	if(IDNO == AfxMessageBox(IDS_BO_SYS01, MB_ICONEXCLAMATION | MB_YESNO))
		return;

	// check the Bosys Information.
	if(CheckBosysInfo() == TRUE)
	{
		// save the auto TC parameter
		if( cnf.WriteTcExpSetFile(&m_stTcExpInfo) == FALSE )
		{
			AfxMessageBox(IDS_BO_SYS02, MB_ICONEXCLAMATION | MB_OK);
		}
		// save the network parameter
		if( cnf.WriteBosysNetwork(&m_stNetInfo) == FALSE )
		{
			AfxMessageBox(IDS_BO_SYS02, MB_ICONEXCLAMATION | MB_OK);
		}
		// save the bo sys condition
		if( cnf.WriteBosysCondition(&m_stCondInfo) == FALSE )
		{
			AfxMessageBox(IDS_BO_SYS02, MB_ICONEXCLAMATION | MB_OK);
		}
		// save the Bosys Information.
		for( i = 0; i < NumSteelKind; i++ )
		{
			// save the Bosys Information.
			if( cnf.WriteSamplingPeriod(i, &m_nSamplePeriod) == FALSE )
			{
				AfxMessageBox(IDS_BO_SYS02, MB_ICONEXCLAMATION | MB_OK);
			}
			if( cnf.WriteBosysInfo(i, &m_stBsInfo[i]) == FALSE )
			{
				AfxMessageBox(IDS_BO_SYS02, MB_ICONEXCLAMATION | MB_OK);
			}
		}
	}
}

// enable change load of menu.
void CBosysDlg::OnUpdateLoadData(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

// enable change save of menu.
void CBosysDlg::OnUpdateSaveData(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

// enable change save of menu.
void CBosysDlg::OnUpdateTcposition(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

// double clicked pattern no button. 
void CBosysDlg::OnDoubleclickedButton1(){ DoPatternNo(0); }
void CBosysDlg::OnDoubleclickedButton2(){ DoPatternNo(1); }
void CBosysDlg::OnDoubleclickedButton3(){ DoPatternNo(2); }
void CBosysDlg::OnDoubleclickedButton4(){ DoPatternNo(3); }
void CBosysDlg::OnDoubleclickedButton5(){ DoPatternNo(4); }
void CBosysDlg::OnDoubleclickedButton6(){ DoPatternNo(5); }
void CBosysDlg::OnDoubleclickedButton7(){ DoPatternNo(6); }
void CBosysDlg::OnDoubleclickedButton8(){ DoPatternNo(7); }
void CBosysDlg::OnDoubleclickedButton9(){ DoPatternNo(8); }

// clicked unlock ptotect
void CBosysDlg::OnClickedProtectButton()
{
	CPassDlg	dlg;

	if( m_bPass == FALSE && IDOK == dlg.DoModal() )
	{
		m_bPass = TRUE;
		EnableControl(m_bPass);
	}
}

// selected 'TC Position' menu.
void CBosysDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

// 次ページ
void CBosysDlg::OnBnClickedButton11()
{
	if( m_page+1 < MAX_PAGE )
		m_page++;
	DispBosysInfoMem();
}

// 前ページ
void CBosysDlg::OnBnClickedButton13()
{
	if( m_page+1 > 1 )
		m_page--;
	DispBosysInfoMem();
}

///////////////////////////////////////////////////////////////////////////////
// CBosysDlg message handler

// setup bo condition.
void CBosysDlg::SetupBosysCondition(void)
{
	CString	str;

	// read Sampling Period.(only 500ms)
	m_nSamplePeriod = 500;

	// set sampling period in edit box
	str.Format(_T("%d"), m_nSamplePeriod);
	m_textSamplePeriod.SetWindowText(str); 

	// set color for password button.
	#ifdef NO_PASSWD
	m_bPass = TRUE;
	#else
	m_bPass = CPassDlg::CheckValidPass();
	#endif
	EnableControl(m_bPass);
}

// display the Bosys Information.
void CBosysDlg::DispBosysInfo(void)
{
	CString	str;
	DWORD	dwIP;
	int		i;

	// read bo execute condition info.
	if( FALSE == cnf.ReadBosysCondition(&m_stCondInfo) )
	{
		// version <= 500
		//m_stCondInfo.fCondLen = 200.00F;
		m_stCondInfo.fCondLen = 2.00F;
		m_stCondInfo.fCondVc = 0.5F;
	}
	str.Format(_T("%.2f"), m_stCondInfo.fCondLen);
	m_editCondLen.SetWindowText(str);
	str.Format(_T("%.2f"), m_stCondInfo.fCondVc);
	m_editCondVc.SetWindowText(str);

	// read tc except info.
	if( FALSE == cnf.ReadTcExpSetFile(&m_stTcExpInfo) )
	{
		m_stTcExpInfo.dTempMin		= 0.0;
		m_stTcExpInfo.dTempMax		= 500.0;
		m_stTcExpInfo.nWatchTime	= 5;
		m_stTcExpInfo.nCheckTime	= 1;
		m_stTcExpInfo.dDeltaT		= 10.0;
	}
	str.Format(_T("%.1f"), m_stTcExpInfo.dTempMin);
	m_editAutoTcPrm1.SetWindowText(str);
	str.Format(_T("%.1f"), m_stTcExpInfo.dTempMax);
	m_editAutoTcPrm2.SetWindowText(str);
	str.Format(_T("%d"), m_stTcExpInfo.nWatchTime);
	m_editAutoTcPrm3.SetWindowText(str);
	str.Format(_T("%.1f"), m_stTcExpInfo.dDeltaT);
	m_editAutoTcPrm4.SetWindowText(str);
	str.Format(_T("%d"), m_stTcExpInfo.nCheckTime);
	m_editAutoTcPrm5.SetWindowText(str);

	// read bo system network info.
	if( FALSE == cnf.ReadBosysNetwork(&m_stNetInfo) )
	{
		m_stNetInfo.dwIP = 0xC0A86465;
	}
	dwIP = ntohl(m_stNetInfo.dwIP);
	m_ipPlc.SetAddress(dwIP);

	// 一旦全てをクリア
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		ClearBosysInfo(i);
	}

	// for each pattern,
	for( i = 0; i < NumSteelKind; i++ )
	{
		// Bosys information is read.
		if(FALSE == cnf.ReadBosysInfo(i, &m_stBsInfo[i]))
		{
			AfxMessageBox(IDS_BO_SYS03, MB_ICONEXCLAMATION | MB_OK);
			break;
		}
	}

	// 鋼種グループNo表示
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		StoreBosysInfo(i);
		str.Format(_T("%d"), m_page * NUM_PTN_NO + i + 1);
		m_btnGradeNo[i].SetWindowText(str);
	}

	// ページ表示
	str.Format(_T("[%d/%d]"), m_page+1, MAX_PAGE);
	m_txtPage.SetWindowText(str);
}

// display the Bosys Information. 
void CBosysDlg::DispBosysInfoMem(void)
{
	CString	str;
	int		i;

	// 一旦全てをクリア
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		ClearBosysInfo(i);
	}

	// for each pattern,
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		// set screen
		StoreBosysInfo(i);
	}

	// 鋼種グループNo表示
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		str.Format(_T("%d"), m_page * NUM_PTN_NO + i + 1);
		m_btnGradeNo[i].SetWindowText(str);
	}

	// ページ表示
	str.Format(_T("[%d/%d]"), m_page+1, MAX_PAGE);
	m_txtPage.SetWindowText(str);
}

// check the Bosys Information.
BOOL CBosysDlg::CheckBosysInfo(void)
{
	CString		str;
	_TCHAR		buf[32];
	DWORD		dwIP;

	// get bo system condition information
	m_editCondLen.GetWindowText(buf, 32);
	m_stCondInfo.fCondLen = (float)_tstof(buf);
	m_editCondVc.GetWindowText(buf, 32);
	m_stCondInfo.fCondVc = (float)_tstof(buf);

	// get auto tc param information.
	m_editAutoTcPrm1.GetWindowText(buf, 32);
	m_stTcExpInfo.dTempMin = (float)_tstof(buf);
	m_editAutoTcPrm2.GetWindowText(buf, 32);
	m_stTcExpInfo.dTempMax = (float)_tstof(buf);
	m_editAutoTcPrm3.GetWindowText(buf, 32);
	m_stTcExpInfo.nWatchTime = _tstoi(buf);
	m_editAutoTcPrm4.GetWindowText(buf, 32);
	m_stTcExpInfo.dDeltaT = (float)_tstof(buf);
	m_editAutoTcPrm5.GetWindowText(buf, 32);
	m_stTcExpInfo.nCheckTime = _tstoi(buf);
	if( m_stTcExpInfo.nCheckTime > (TCEXP_CHK_TIME_MAX-1) )
	{
		m_stTcExpInfo.nCheckTime = (TCEXP_CHK_TIME_MAX-1);
		str.Format(_T("%d"), m_stTcExpInfo.nCheckTime);
		m_editAutoTcPrm5.SetWindowText(str);
	}

	// get bo network information.
	m_ipPlc.GetAddress(dwIP);
	m_stNetInfo.dwIP = htonl(dwIP);

	// for each steel grade group
	//for(i = 0; i < NUM_PTN_NO; i++ )
	//{
	//	// check sg name
	//	m_editComment[i].GetWindowText(buf, 32);
	//	if( 0 == _tcslen(buf) )
	//	{
	//		memset(&m_stBsInfo[i], 0, sizeof(m_stBsInfo[i]));
	//		continue;
	//	}

	//	if( !SetBosysInfo(i) )
	//		return FALSE;
	//}

	// Setup Data is adopted.
	m_nSamplePeriod = 500;

	return TRUE;
}

// BoSys情報を画面に設定
void CBosysDlg::StoreBosysInfo(int no)
{
	CString	str;
	int		idx;

	if( no < 0 || no >= NUM_PTN_NO )
		return;
	idx = m_page * NUM_PTN_NO + no;

	// display comment.
	str = m_stBsInfo[idx].szComment;
	m_editComment[no].SetWindowText(str);

	//// display the powder name.
	//m_listBoPow[no].ResetContent();
	//for(j = 0;j < m_stBsInfo[no].numRegPow;j++)
	//{
	//	m_listBoPow[no].InsertString(-1, m_stBsInfo[no].szPowder[j]);
	//}

	// display configuration name
	m_editConfName[no].SetWindowText(m_stBsInfo[idx].szConfName);
	m_editConfNameD[no].SetWindowText(m_stBsInfo[idx].szConfNameD);

	// display Number of combinations.
	str.Format(_T("%d"), m_stBsInfo[idx].numCombiBA);
	m_editAlmCombi[no].SetWindowText(str);
	str.Format(_T("%d"), m_stBsInfo[idx].numCombiBAC);
	m_editAlmCtlCombi[no].SetWindowText(str);
}

// BoSys情報画面クリア
void CBosysDlg::ClearBosysInfo(int no)
{
	CString	str;

	if( no < 0 || no >= NUM_PTN_NO )
		return;

	// display comment.
	m_editComment[no].SetWindowText(_T(""));

	// display configuration name
	m_editConfName[no].SetWindowText(_T(""));
	m_editConfNameD[no].SetWindowText(_T(""));

	// display Number of combinations.
	m_editAlmCombi[no].SetWindowText(_T("0"));
	m_editAlmCtlCombi[no].SetWindowText(_T("0"));
}

//BOOL CBosysDlg::SetBosysInfo(int no)
//{
//	CString	str;
//	_TCHAR	buf[256];
//	int		numCount;
//	int		i = no, j;
//
//	// check sg name
//	m_editComment[i].GetWindowText(buf, 32);
//	_tcscpy(m_stBsInfo[i].szComment, buf);
//
//	// check B.O watch time
//	m_editWatchTime[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].boWatchTime) != 1 ||
//		m_stBsInfo[i].boWatchTime < 300)
//	{
//		m_editWatchTime[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS06, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].boWatchTime);
//	m_editWatchTime[i].SetWindowText(buf);
//
//	// check Relations between sampling period and the BO watch time
//	if(m_stBsInfo[i].boWatchTime % 500 != 0)
//	{
//		AfxMessageBox(IDS_BO_SYS07, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//
//	// check B.O Occurrence recognition
//	numCount = (int)(m_stBsInfo[i].boWatchTime / 500);
//
//	if(m_stBsInfo[i].numRegister <= 0)
//	{
//		AfxMessageBox(IDS_BO_SYS08, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//
//	for(j = 0;j < m_stBsInfo[i].numRegister;j++)
//	{
//		if(m_stBsInfo[i].boTimes[j] > numCount)
//		{
//			AfxMessageBox(IDS_BO_SYS09, MB_ICONEXCLAMATION | MB_OK);
//			return FALSE;
//		}
//	}
//
//	// check Number of times for BO Alarm.
//	m_editAlmCombi[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].numCombiBA) != 1)
//	{
//		m_editAlmCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	if(m_stBsInfo[i].numCombiBA < 1)
//	{
//		m_editAlmCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].numCombiBA);
//	m_editAlmCombi[i].SetWindowText(buf);
//
//	// check Number of times for BO Alarm Ctl.
//	m_editAlmCtlCombi[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].numCombiBAC) != 1)
//	{
//		m_editAlmCtlCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	if(m_stBsInfo[i].numCombiBAC < 1)
//	{
//		m_editAlmCtlCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].numCombiBAC);
//	m_editAlmCtlCombi[i].SetWindowText(buf);
//
//	return TRUE;
//}

//BOOL CBosysDlg::SetBosysInfo2(int no)
//{
//	CString	str;
//	_TCHAR	buf[256];
//	int		numCount;
//	int		i = no, j;
//
//	// check sg name
//	m_editComment[i].GetWindowText(buf, 32);
//	_tcscpy(m_stBsInfo[i].szComment, buf);
//
//	// check B.O watch time
//	m_editWatchTime[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].boWatchTime) != 1 ||
//		m_stBsInfo[i].boWatchTime < 300)
//	{
//		m_editWatchTime[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS06, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].boWatchTime);
//	m_editWatchTime[i].SetWindowText(buf);
//
//	// check Relations between sampling period and the BO watch time
//	if(m_stBsInfo[i].boWatchTime % 500 != 0)
//	{
//		AfxMessageBox(IDS_BO_SYS07, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//
//	// check B.O Occurrence recognition
//	numCount = (int)(m_stBsInfo[i].boWatchTime / 500);
//
//	for(j = 0;j < m_stBsInfo[i].numRegister;j++)
//	{
//		if(m_stBsInfo[i].boTimes[j] > numCount)
//		{
//			AfxMessageBox(IDS_BO_SYS09, MB_ICONEXCLAMATION | MB_OK);
//			return FALSE;
//		}
//	}
//
//	// check Number of times for BO Alarm.
//	m_editAlmCombi[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].numCombiBA) != 1)
//	{
//		m_editAlmCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	if(m_stBsInfo[i].numCombiBA < 1)
//	{
//		m_editAlmCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].numCombiBA);
//	m_editAlmCombi[i].SetWindowText(buf);
//
//	// check Number of times for BO Alarm Ctl.
//	m_editAlmCtlCombi[i].GetWindowText(buf, 32);
//	if(_stscanf(buf, _T("%d"), &m_stBsInfo[i].numCombiBAC) != 1)
//	{
//		m_editAlmCtlCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	if(m_stBsInfo[i].numCombiBAC < 1)
//	{
//		m_editAlmCtlCombi[i].SetWindowText(_T(""));
//		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
//		return FALSE;
//	}
//	_stprintf(buf, _T("%d"), m_stBsInfo[i].numCombiBAC);
//	m_editAlmCtlCombi[i].SetWindowText(buf);
//
//	return TRUE;
//}

void CBosysDlg::DoPatternNo(int no)
{
	// 
	CBoRec	dlg;
	int		idx;

//	if( !m_bPass || !SetBosysInfo2(no) )
	if( !m_bPass )
		return;

	idx = m_page * NUM_PTN_NO + no;
	dlg.SetInfo( idx, &m_stBsInfo[idx] );
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetInfo( &m_stBsInfo[idx] );
		StoreBosysInfo(no);
	}
}

// コントロールの有効／無効設定
void CBosysDlg::EnableControl(BOOL bEnable)
{
	int		i;

	if( bEnable )
	{
		m_btnPass.SetTextColor(RGB(0,0,0));
		m_btnPass.SetBackColor(RGB(0,255,0));
	}
	else
	{
		m_btnPass.SetTextColor(RGB(255,255,255));
		m_btnPass.SetBackColor(RGB(255,0,0));
	}
	m_editCondLen.EnableWindow(bEnable);
	m_editCondVc.EnableWindow(bEnable);
	m_editAutoTcPrm1.EnableWindow(bEnable);
	m_editAutoTcPrm2.EnableWindow(bEnable);
	m_editAutoTcPrm3.EnableWindow(bEnable);
	m_editAutoTcPrm4.EnableWindow(bEnable);
	m_editAutoTcPrm5.EnableWindow(bEnable);
	m_btnNext.EnableWindow(bEnable);
	m_btnPrev.EnableWindow(bEnable);
	m_ipPlc.EnableWindow(bEnable);
	for( i = 0; i < NUM_PTN_NO; i++ )
	{
		m_editComment[i].EnableWindow(bEnable);
		m_editWatchTime[i].EnableWindow(bEnable);
		m_listBoRec[i].EnableWindow(bEnable);
		//m_listBoPow[i].EnableWindow(bEnable);
		m_editConfName[i].EnableWindow(bEnable);
		m_editConfNameD[i].EnableWindow(bEnable);
		m_editAlmCombi[i].EnableWindow(bEnable);
		m_editAlmCtlCombi[i].EnableWindow(bEnable);
	}
}
