// BpnetDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "BpnetDlg.h"
#include ".\bpnetdlg.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf = theApp.m_cnf;
static CBolog		&lg = theApp.m_log;
static TcName		*tcnm	= &theApp.m_stTcNm;

#define	WM_APP_SIM_CHG		(WM_APP + 0x10)
#define	WM_APP_BPNET_CHG	(WM_APP + 0x11)


// CBpnetDlg dialog
IMPLEMENT_DYNAMIC(CBpnetDlg, CDialog)
CBpnetDlg::CBpnetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBpnetDlg::IDD, pParent)
{
	// initialize member data.
	m_nNetIdx = 0;
	m_nOrdArg1		= -1;
	m_nOrdArg2		= -1;
	m_nAlmArg1		= -1;
	m_nAlmArg2		= -1;
	m_dPattOrdTeachVal	= DEF_PATT_ORD;
	m_dPattAlmTeachVal	= DEF_PATT_ALM;
	m_dCorrOrdTeachVal	= DEF_CORR_ORD; 
	m_dCorrAlmTeachVal	= DEF_CORR_ALM;
	m_dEatha			= DEF_EATHA;
	m_dAlpha			= DEF_ALPHA;
	m_nTeachCondition	= DEF_CONDITION;
	m_isBPExecuting		= FALSE;
	m_isLearnedPending	= FALSE;
	m_bBpnetSecPass		= FALSE;
	m_nTimerID			= 0;
	m_bPass				= FALSE;
	m_bBpnetSecPass		= FALSE;
	m_strWkName			= _T("");
	m_pDlgTcView		= NULL;
	m_isCheckLearn		= FALSE;

	// create dialog.
	Create(CBpnetDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CBpnetDlg::~CBpnetDlg()
{
}

void CBpnetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONF_NAME, m_textConfName);
	DDX_Control(pDX, IDC_COMBO1, m_cmbNN);
	DDX_Control(pDX, IDC_PATT_ORD, m_textPattOrd);
	DDX_Control(pDX, IDC_PATT_ALM, m_textPattAlm);
	DDX_Control(pDX, IDC_CORR_ORD, m_textCorrOrd);
	DDX_Control(pDX, IDC_CORR_ALM, m_textCorrAlm);
	DDX_Control(pDX, IDC_CHECK1, m_btnPattOrd);
	DDX_Control(pDX, IDC_CHECK2, m_btnPattAlm);
	DDX_Control(pDX, IDC_CHECK3, m_btnCorrOrd);
	DDX_Control(pDX, IDC_CHECK4, m_btnCorrAlm);
	DDX_Control(pDX, IDC_EDIT1, m_editPattOrd);
	DDX_Control(pDX, IDC_EDIT2, m_editPattAlm);
	DDX_Control(pDX, IDC_EDIT3, m_editCorrOrd);
	DDX_Control(pDX, IDC_EDIT4, m_editCorrAlm);
	DDX_Control(pDX, IDC_EDIT5, m_editEatha);
	DDX_Control(pDX, IDC_EDIT6, m_editAlpha);
	DDX_Control(pDX, IDC_COMBO2, m_cmbOrd);
	DDX_Control(pDX, IDC_COMBO3, m_cmbAlm);
	DDX_Control(pDX, IDC_ORD_DATE, m_textOrdDate);
	DDX_Control(pDX, IDC_ORD_TIME, m_textOrdTime);
	DDX_Control(pDX, IDC_ALM_DATE, m_textAlmDate);
	DDX_Control(pDX, IDC_ALM_TIME, m_textAlmTime);
	DDX_Control(pDX, IDC_BUTTON10, m_btnPass);
	DDX_Control(pDX, IDC_EDIT7, m_editOrdinary);
	DDX_Control(pDX, IDC_EDIT8, m_editAlarm);
	DDX_Control(pDX, IDC_EDIT9, m_editOrdinaryFilter);
	DDX_Control(pDX, IDC_EDIT10, m_editAlarmFilter);
	DDX_Control(pDX, IDC_BUTTON1, m_btnOrdL);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOrdR);
	DDX_Control(pDX, IDC_BUTTON3, m_btnAlmL);
	DDX_Control(pDX, IDC_BUTTON4, m_btnAlmR);
	DDX_Control(pDX, IDC_ORDSFT, m_textOrdShift);
	DDX_Control(pDX, IDC_ALMSFT, m_textAlmShift);
	DDX_Control(pDX, IDC_LEARN_TIMES, m_labLearnTimes);
	DDX_Control(pDX, IDC_WEIOK_LP, m_labWeiLp);
	DDX_Control(pDX, IDC_WEIOK_BTN, m_btnWeiOk);
}


BEGIN_MESSAGE_MAP(CBpnetDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_COMMAND(IDM_SIMULATE, OnSimulate)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_TIMER()
	ON_COMMAND(IDM_BP_LOAD, OnBpLoad)
	ON_COMMAND(IDM_BP_SAVE, OnBpSave)
	ON_COMMAND(IDM_BP_INIT, OnBpInit)
	ON_COMMAND(IDM_START_BP, OnStartBp)
	ON_COMMAND(IDM_STOP_BP, OnStopBp)
	ON_COMMAND(IDM_RECOVER_BP, OnRecoverBp)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_UPDATE_COMMAND_UI(IDM_BP_LOAD, OnUpdateBpLoad)
	ON_UPDATE_COMMAND_UI(IDM_BP_SAVE, OnUpdateBpSave)
	ON_UPDATE_COMMAND_UI(IDM_BP_INIT, OnUpdateBpInit)
	ON_UPDATE_COMMAND_UI(IDM_START_BP, OnUpdateStartBp)
	ON_UPDATE_COMMAND_UI(IDM_STOP_BP, OnUpdateStopBp)
	ON_UPDATE_COMMAND_UI(IDM_RECOVER_BP, OnUpdateRecoverBp)
	ON_UPDATE_COMMAND_UI(IDM_SIMULATE, OnUpdateSimulate)
	ON_UPDATE_COMMAND_UI(IDM_TCPOSITION, OnUpdateTcposition)
	ON_BN_CLICKED(IDC_BUTTON10, OnClickedPasswordButton)
	ON_WM_MOVE()
	ON_MESSAGE( WM_APP_BPNET_CHG, OnBpnetChg )
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_WEIOK_BTN, &CBpnetDlg::OnBnClickedWeiokBtn)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBpnetDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CBpnetDlg::OnOk(){}

//It evades ending by ESC Key.
void CBpnetDlg::OnCancel(){}

void CBpnetDlg::PostNcDestroy() 
{
	m_bp.ClearBpBONetwork();
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CBpnetDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// It is only 1 execute when the dialog opens.
BOOL CBpnetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set teach condition message of text color
	m_textPattOrd.SetTextColor(RGB(0,128,0),FALSE);
	m_textPattAlm.SetTextColor(RGB(205,0,0),FALSE);
	m_textCorrOrd.SetTextColor(RGB(0,0,205),FALSE);
	m_textCorrAlm.SetTextColor(RGB(128,128,0),TRUE);

	// set teach condition in check box
	m_btnPattOrd.SetCheck(BST_CHECKED);
	m_btnPattAlm.SetCheck(BST_CHECKED);
	m_btnCorrOrd.SetCheck(BST_CHECKED);
	m_btnCorrAlm.SetCheck(BST_CHECKED);

	// set value in edit box
	SetupTargetValueColumn();
	SetupParameterColumn();

	// set trend.out path
	m_bp.m_strTrendOutFilePath.Format(_T("%s/../trendf.out"), lg.LogRootDir);
	m_ordGraph.m_strTrendOutFilePath.Format(_T("%s/../trendf.out"), lg.LogRootDir);
	m_almGraph.m_strTrendOutFilePath.Format(_T("%s/../trendf.out"), lg.LogRootDir);

	// setup bpnet dialog.
	SetupBpnetDlg();

	// create bp trend window
	CRect	rc;
	CWnd	*wnd = GetDlgItem(IDC_BP_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("BpGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_BP_GRAPH_WIN);
	m_graph.ShowWindow(SW_SHOW);

	// create ordinary chart window
	wnd = GetDlgItem(IDC_ORD_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_ordGraph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("OrdSelGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_ORD_GRAPH_WIN);
	m_ordGraph.ShowWindow(SW_SHOW);

	// create alarm chart window
	wnd = GetDlgItem(IDC_ALM_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_almGraph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("AlmSelGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_ALM_GRAPH_WIN);
	m_almGraph.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// show window
void CBpnetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString	strWk;
	int	ret;

	if( bShow )
	{
		// stop the backpropagation process.
		m_isBPExecuting = FALSE;

		// disp work dir name
		strWk = cnf.GetWorkDirName();
		if( strWk != m_strWkName )
		{
			m_strWkName = strWk;
			m_nNetIdx = 0;
		}
		m_textConfName.SetWindowText(m_strWkName);

		if( cnf.numCombination > 0 )
		{
			// ニューラルネット情報（一時ファイル）を読み込む
			ret = m_bp.LoadBONeuralNetworkTmp(m_nNetIdx);

			// 読み込み異常なら通常のニューラルネット情報を読み込む
			if( ret != CBpOpe::NORMAL )
			{
				// load specified BO neural networks.
				ret = m_bp.LoadBONeuralNetwork(m_nNetIdx);

				// if anything happened, show the message.
				if( ret != CBpOpe::NORMAL )
				{
					DispBpErrMsg(ret);
				}
			}

			// 学習未確定状態表示
			DispWeiLp();
		}
		else
		{
			m_nNetIdx = 0;
			m_nOrdArg1 = -1;
			m_nOrdArg2 = -1;
			m_nAlmArg1 = -1;
			m_nAlmArg2 = -1;
			m_textConfName.SetWindowText(cnf.GetWorkDirName());
		}

		// setup bpnet dialog
		SetupBpnetDlg();

		// set color for password button.
		#ifdef NO_PASSWD
		m_bPass = TRUE;
		#else
		if( m_bBpnetSecPass )
			m_bPass = TRUE;
		else
			m_bPass = CPassDlg::CheckValidPass();
		#endif
		m_bBpnetSecPass = FALSE;
		EnableControl(m_bPass);
	}
	else
	{
		OnStopBp();

		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}

	CDialog::OnShowWindow(bShow, nStatus);
}

// selected load of menu
void CBpnetDlg::OnBpLoad()
{
	int	ret;

	if( m_cmbNN.GetCount() <= 0 )
		return;

	// stop step execution of backpropagation.
	m_isBPExecuting	= FALSE;

	// show message and query the action.
	if (AfxMessageBox(IDS_SIM_DLG10, MB_YESNO) == IDYES) 
	{
		// ニューラルネット情報（一時ファイル）を読み込む
		ret = m_bp.LoadBONeuralNetworkTmp(m_nNetIdx);

		// 読み込み異常なら通常のニューラルネット情報を読み込む
		if( ret != CBpOpe::NORMAL )
		{
			// load specified BO neural networks.
			ret = m_bp.LoadBONeuralNetwork(m_nNetIdx);

			// if anything happened, show the message.
			if( ret != CBpOpe::NORMAL )
				DispBpErrMsg(ret);
		}

		// 学習回数の表示
		DispLearningTimes();

		// 学習未確定状態表示
		DispWeiLp();
	}
}

// selected save of menu
void CBpnetDlg::OnBpSave()
{
	int	ret;

	if( m_cmbNN.GetCount() <= 0 )
		return;

	// stop step execution of backpropagation.
	m_isBPExecuting		= FALSE;

	// show message and query the action.
	if (AfxMessageBox(IDS_SIM_DLG11, MB_YESNO) == IDYES) 
	{
		// clear the pending status.
		m_isLearnedPending	= FALSE;
		// save these BO neural networks for tempo.
		ret = m_bp.SaveBONeuralNetworkTmp();
		// if anything happened, show the message.
		if(ret != CBpOpe::NORMAL)
			DispBpErrMsg(ret);

		// 学習未確定状態表示
		DispWeiLp();
	}
}

// selected initialize of menu
void CBpnetDlg::OnBpInit()
{
	if( m_cmbNN.GetCount() <= 0 )
		return;

	// stop step execution of backpropagation.
	m_isBPExecuting		= FALSE;

	// show message and query the action.
	if (AfxMessageBox(IDS_SIM_DLG08, MB_YESNO) == IDYES) 
	{
		// set the pending status.
		m_isLearnedPending	= TRUE;

		// if confirmed, reset the weights of BO neural networks.
		m_bp.ResetBONeuralNetwork();

		// 学習回数の表示
		DispLearningTimes();
	}
}

// selected start b.p. of menu
void CBpnetDlg::OnStartBp()
{
	double	dOrdPattOut, dAlmPattOut, dOrdCorrOut, dAlmCorrOut;
	int		ret;

	if( m_cmbNN.GetCount() <= 0 )
		return;

	if( m_isBPExecuting )
		return;

	// get&set editted target values of neural networks.
	GetTargetValue();
	m_bp.SetTeachVal(
			m_dPattOrdTeachVal,
			m_dPattAlmTeachVal,
			m_dCorrOrdTeachVal,
			m_dCorrAlmTeachVal);

	// get&set convergence parameters from editting columns.
	GetParameter();
	m_bp.SetNeuralParam(m_dEatha, m_dAlpha);

	// get&set teach condition.
	GetTeachCondition();
	m_bp.SetTeachCondition(m_nTeachCondition);

	// backpropagate BO neural networks. if succeess, 
	if( (ret = m_bp.DoBPBONeuralNetwork(m_nOrdArg1, m_nOrdArg2, m_nAlmArg1, m_nAlmArg2)) == CBpOpe::NORMAL )
	{
		// start step execution of backpropagation.
		m_isBPExecuting = TRUE;

		// set the pending status.
		m_isLearnedPending = TRUE;

		// clear backpropagation trend window.
		m_graph.RedrawTrend();

		// execute neural network with backpropagated weights.
		m_bp.CalcNeuralNetwork(&dOrdPattOut, &dAlmPattOut, &dOrdCorrOut, &dAlmCorrOut);

		// plot backpropagation trend.
		m_graph.AppendTrend(dOrdPattOut, dAlmPattOut, dOrdCorrOut, dAlmCorrOut);

		// disable control
		EnableControl(FALSE);

		// start timer
		m_nTimerID = SetTimer(1, 1, NULL);

		// 学習回数の表示
		DispLearningTimes();
	}
	// if anything happened, show the message.
	else
	{
		DispBpErrMsg(ret);
	}
}

// selected stop b.p. of menu
void CBpnetDlg::OnStopBp()
{
	if( m_cmbNN.GetCount() <= 0 )
		return;

	if( !m_isBPExecuting )
		return;

	// start step execution of backpropagation.
	m_isBPExecuting = FALSE;

	// stop timer
	KillTimer(m_nTimerID);

	// enable control
	EnableControl(TRUE);
}

// selected recover of menu
void CBpnetDlg::OnRecoverBp()
{
	if( m_cmbNN.GetCount() <= 0 )
		return;

	// stop step execution of backpropagation.
	m_isBPExecuting		= FALSE;

	CString		strPath;
	CWeiList	dlg;

	// make up pattern neural network backup file name.
	strPath.Format(_T("%s\\%s"), cnf.WorkRootDir, cnf.workDir);
	dlg.m_szConfPath = strPath;
	dlg.m_nNetIdx = m_nNetIdx;

	// 学習結果履歴画面を表示し、学習結果データが選択されたら
	if( IDOK == dlg.DoModal() )
	{
		// set the pending status.
		m_isLearnedPending	= TRUE;

		// recover BO neural networks.
		LPTSTR	pPathP = dlg.m_szSavePathP.GetBuffer();
		LPTSTR	pPathC = dlg.m_szSavePathC.GetBuffer();
		m_bp.RecoverBONeuralNetwork(pPathP, pPathC);
		dlg.m_szSavePathP.ReleaseBuffer();
		dlg.m_szSavePathC.ReleaseBuffer();
	}

	// 学習回数の表示
	DispLearningTimes();

	//// recover BO neural networks.
	//if( AfxMessageBox(IDS_SIM_DLG09, MB_YESNO) == IDYES )
	//{
	//	// set the pending status.
	//	m_isLearnedPending	= TRUE;
	//	// recover BO neural networks.
	//	m_bp.RecoverBONeuralNetwork();
	//}
}

// selected simulate of menu
void CBpnetDlg::OnSimulate()
{
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();

	if( m_cmbNN.GetCount() <= 0 )
		return;
	
	// stop the backpropagation process.
	m_isBPExecuting	= FALSE;
	// if the pending staus is set,
	if (m_isLearnedPending) 
	{
		// save B.O. neural networks for tempo.
		m_bp.SaveBONeuralNetworkTmp();
	}

	if( dlg != NULL ){
		dlg->m_pDlgSim->ShowWindow(SW_SHOW);
		dlg->m_pDlgActive = dlg->m_pDlgSim;
		dlg->m_pDlgSim->SendMessage(WM_APP_SIM_CHG, m_nNetIdx, ((short)m_nAlmArg1<<16)|(short)m_nAlmArg2);
	}
	ShowWindow(SW_HIDE);
}

// selectd change neural network combo box
void CBpnetDlg::OnSelchangeCombo1()
{
	CString			str;
	TeachDataSpec	*pOrdSpec, *pAlmSpec;
	//int				nOrdCmbID, nAlmCmbID;
	int				sel, ret, ret2, numTrend, offset;
	CString s;

	// if neural network list is valid,
	if (m_cmbNN.GetCount() > 0) {
		sel = m_cmbNN.GetCurSel();

		// if specified neural networks are changed,
		if (m_nNetTab[sel] != m_nNetIdx) {
			// free the now neural networks.
			m_bp.FreeBONeuralNetwork();
		}
		// get neural network identifier from the selection.
		m_nNetIdx	= m_nNetTab[sel];

		// ニューラルネット情報（一時ファイル）を読み込む
		ret = m_bp.LoadBONeuralNetworkTmp(m_nNetIdx);

		// 読み込み異常なら通常のニューラルネット情報を読み込む
		if( ret != CBpOpe::NORMAL )
		{
			// load specified BO neural networks.
			ret = m_bp.LoadBONeuralNetwork(m_nNetIdx);

			// if anything happened, show the message.
			if( ret != CBpOpe::NORMAL ){
				DispBpErrMsg(ret);
			}
		}

		// 学習未確定状態表示
		DispWeiLp();

		// change neural network selection from spec file.
		ChangeNeuralnetFromFile(m_nNetIdx);

		// load teaching data and get combination ID
		ret = m_bp.LoadTeachingDataOrd(m_nNetIdx);
		if( ret != CBpOpe::NORMAL)
		{
			m_nOrdArg1 = m_nOrdArg2 = -1;
			m_editOrdinaryFilter.SetWindowText(_T(""));
		}
		else
		{
			pOrdSpec = &m_bp.m_stOrdDataSpec;
			m_nOrdArg1	= pOrdSpec->arg1;
			m_nOrdArg2	= pOrdSpec->arg2;

			// if combinations are specified, 
			if (m_nOrdArg1 >= 0 && m_nOrdArg1 < NumTCTap &&
				m_nOrdArg2 >= 0 && m_nOrdArg2 < NumTCTap )
			{
				if( m_bp.m_pPattNN != NULL )
				{
					numTrend = m_bp.m_pPattNN->wMatrix[0]->numIn - 1;
					offset = pOrdSpec->fIdx + pOrdSpec->length - numTrend;
				}
				else
				{
					numTrend = pOrdSpec->length;
					offset = pOrdSpec->fIdx;
				}

				// load disp data in the graph
				m_ordGraph.LoadDispData(pOrdSpec->path,
										offset,
										numTrend,
										pOrdSpec->fFilter,
										pOrdSpec->shift);
			}
			// otherwise,
			else {
				m_nOrdArg1 = m_nOrdArg2 = -1;
			}

			// display filter value
			if( pOrdSpec->dtHead.header.version >= 620 )
			{
				str.Format(_T("%.1g"), pOrdSpec->fFilter);
				m_editOrdinaryFilter.SetWindowText(str);
			}
			else
			{
				m_editOrdinaryFilter.SetWindowText(_T(""));
			}
		}

		ret2 = m_bp.LoadTeachingDataAlm(m_nNetIdx);
		if( ret2 != CBpOpe::NORMAL)
		{
			m_nAlmArg1 = m_nAlmArg2 = -1;
			m_editAlarmFilter.SetWindowText(_T(""));
		}
		else
		{
			pAlmSpec = &m_bp.m_stAlmDataSpec;
			m_nAlmArg1	= pAlmSpec->arg1;
			m_nAlmArg2	= pAlmSpec->arg2;

			// if combinations are specified, 
			if (m_nAlmArg1 >= 0 && m_nAlmArg1 < NumTCTap &&
				m_nAlmArg2 >= 0 && m_nAlmArg2 < NumTCTap) 
			{
				if( m_bp.m_pPattNN != NULL )
				{
					numTrend = m_bp.m_pPattNN->wMatrix[0]->numIn - 1;
					offset = pAlmSpec->fIdx + pAlmSpec->length - numTrend;
				}
				else
				{
					numTrend = pAlmSpec->length;
					offset = pAlmSpec->fIdx;
				}

				// load disp data in the graph
				m_almGraph.LoadDispData(pAlmSpec->path,
										offset,
										numTrend,
										pAlmSpec->fFilter,
										pAlmSpec->shift);
			}
			// otherwise,
			else {
				m_nAlmArg1 = m_nAlmArg2 = -1;
			}

			// display filter value
			if( pAlmSpec->dtHead.header.version >= 620 )
			{
				str.Format(_T("%.1g"), pAlmSpec->fFilter);
				m_editAlarmFilter.SetWindowText(str);
			}
			else
			{
				m_editAlarmFilter.SetWindowText(_T(""));
			}
		}

		if( ret != CBpOpe::NORMAL || ret2 != CBpOpe::NORMAL )
		{
			DispBpErrMsg((ret != CBpOpe::NORMAL)? ret: ret2);
		}

		// redraw chart
		m_ordGraph.RedrawChart(m_nOrdArg1, m_nOrdArg2);
		m_almGraph.RedrawChart(m_nAlmArg1, m_nAlmArg2);

		// set chart datetime
		DispTeachTime();

		// 学習回数の表示
		DispLearningTimes();
	}
}

// selectd change ordinary combo box
void CBpnetDlg::OnSelchangeCombo2()
{
}

// selectd change alarm combo box
void CBpnetDlg::OnSelchangeCombo3()
{
}

// WM_TIMER proc
void CBpnetDlg::OnTimer(UINT nIDEvent)
{
	double	dOrdPattOut, dAlmPattOut, dOrdCorrOut, dAlmCorrOut;
	int	ret;

	// if step execution of backpropagation is specified,
	if(m_isBPExecuting) 
	{
		for( ret = 0; ret < REPEAT_COUNT; ret++ )
		{
			// do step execution of backpropagation.
			m_bp.StepBPBONeuralNetwork();

			// execute neural network with backpropagated weights.
			m_bp.CalcNeuralNetwork(&dOrdPattOut, &dAlmPattOut, &dOrdCorrOut, &dAlmCorrOut);

			// plot backpropagation trend.
			m_graph.AppendTrend(dOrdPattOut, dAlmPattOut, dOrdCorrOut, dAlmCorrOut);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CBpnetDlg::OnUpdateBpLoad(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateBpSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateBpInit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateStartBp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateStopBp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateRecoverBp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateSimulate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnUpdateTcposition(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CBpnetDlg::OnClickedPasswordButton()
{
	CPassDlg	dlg;

	if( m_bPass == FALSE && IDOK == dlg.DoModal() )
	{
		m_bPass = TRUE;
		EnableControl(m_bPass);
	}
}

// selected 'TC Position' menu.
void CBpnetDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

// 左矢印（正常時選択データ左１秒シフト）
void CBpnetDlg::OnBnClickedButton1()
{
	if( m_nOrdArg1 == -1 || m_nOrdArg2 == -1 )
		return;

	m_ordGraph.DataShiftM(m_bp.m_stOrdDataSpec.shift);
	DispTeachTime();
	OrdinarySave();
}

// 右矢印（正常時選択データ右１秒シフト）
void CBpnetDlg::OnBnClickedButton2()
{
	if( m_nOrdArg1 == -1 || m_nOrdArg2 == -1 )
		return;

	m_ordGraph.DataShiftP(m_bp.m_stOrdDataSpec.shift);
	DispTeachTime();
	OrdinarySave();
}

// 左矢印（異常時選択データ左１秒シフト）
void CBpnetDlg::OnBnClickedButton3()
{
	if( m_nAlmArg1 == -1 || m_nAlmArg2 == -1 )
		return;

	m_almGraph.DataShiftM(m_bp.m_stAlmDataSpec.shift);
	DispTeachTime();
	AlarmSave();
}

// 右矢印（異常時選択データ右１秒シフト）
void CBpnetDlg::OnBnClickedButton4()
{
	if( m_nAlmArg1 == -1 || m_nAlmArg2 == -1 )
		return;

	m_almGraph.DataShiftP(m_bp.m_stAlmDataSpec.shift);
	DispTeachTime();
	AlarmSave();
}

///////////////////////////////////////////////////////////////////////////////
// CBpnetDlg member function

// setup bpnet dialog
void CBpnetDlg::SetupBpnetDlg(void)
{
	CString			str;
	int				ret, ret2, numTrend, offset;
	TeachDataSpec	*pOrdSpec, *pAlmSpec;
	CString s;

	// setup network list.
	SetupNetworkList();
	
	// setting teach value
	m_bp.SetTeachVal(
		m_dPattOrdTeachVal,
		m_dPattAlmTeachVal,
		m_dCorrOrdTeachVal,
		m_dCorrAlmTeachVal );

	// setting teach condition
	m_bp.SetTeachCondition(m_nTeachCondition);

	// change neural network selection from spec file.
	ChangeNeuralnetFromFile(m_nNetIdx);

	// load teaching data and get combination ID
	ret = m_bp.LoadTeachingDataOrd(m_nNetIdx);
	if( ret != CBpOpe::NORMAL)
	{
		m_nOrdArg1 = m_nOrdArg2 = -1;
	}
	else
	{
		pOrdSpec = &m_bp.m_stOrdDataSpec;
		m_nOrdArg1	= pOrdSpec->arg1;
		m_nOrdArg2	= pOrdSpec->arg2;

		// if combinations are specified, 
		if (m_nOrdArg1 >= 0 && m_nOrdArg1 < NumTCTap &&
			m_nOrdArg2 >= 0 && m_nOrdArg2 < NumTCTap )
		{
			if( m_bp.m_pPattNN != NULL )
			{
				numTrend = m_bp.m_pPattNN->wMatrix[0]->numIn - 1;
				offset = pOrdSpec->fIdx + pOrdSpec->length - numTrend;
			}
			else
			{
				numTrend = pOrdSpec->length;
				offset = pOrdSpec->fIdx;
			}

			// load disp data in the graph
			m_ordGraph.LoadDispData(pOrdSpec->path,
									offset,
									numTrend,
									pOrdSpec->fFilter,
									pOrdSpec->shift);
		}
		// otherwise,
		else {
			m_nOrdArg1 = m_nOrdArg2 = -1;
		}

		// display filter value
		if( pOrdSpec->dtHead.header.version >= 620 )
		{
			str.Format(_T("%.1g"), pOrdSpec->fFilter);
			m_editOrdinaryFilter.SetWindowText(str);
		}
		else
		{
			m_editOrdinaryFilter.SetWindowText(_T(""));
		}
	}

	ret2 = m_bp.LoadTeachingDataAlm(m_nNetIdx);
	if( ret2 != CBpOpe::NORMAL)
	{
		m_nAlmArg1 = m_nAlmArg2 = -1;
	}
	else
	{
		pAlmSpec = &m_bp.m_stAlmDataSpec;
		m_nAlmArg1	= pAlmSpec->arg1;
		m_nAlmArg2	= pAlmSpec->arg2;

		// if combinations are specified, 
		if (m_nAlmArg1 >= 0 && m_nAlmArg1 < NumTCTap &&
			m_nAlmArg2 >= 0 && m_nAlmArg2 < NumTCTap) 
		{
			if( m_bp.m_pPattNN != NULL )
			{
				numTrend = m_bp.m_pPattNN->wMatrix[0]->numIn - 1;
				offset = pAlmSpec->fIdx + pAlmSpec->length - numTrend;
			}
			else
			{
				numTrend = pAlmSpec->length;
				offset = pAlmSpec->fIdx;
			}

			// load disp data in the graph
			m_almGraph.LoadDispData(pAlmSpec->path,
									offset,
									numTrend,
									pAlmSpec->fFilter,
									pAlmSpec->shift);
		}
		// otherwise,
		else {
			m_nAlmArg1 = m_nAlmArg2 = -1;
		}

		// display filter value
		if( pAlmSpec->dtHead.header.version >= 620 )
		{
			str.Format(_T("%.1g"), pAlmSpec->fFilter);
			m_editAlarmFilter.SetWindowText(str);
		}
		else
		{
			m_editAlarmFilter.SetWindowText(_T(""));
		}
	}

	if( ret != CBpOpe::NORMAL || ret2 != CBpOpe::NORMAL )
	{
		DispBpErrMsg((ret != CBpOpe::NORMAL)? ret: ret2);
	}

	// redraw chart
	m_ordGraph.RedrawChart(m_nOrdArg1, m_nOrdArg2);
	m_almGraph.RedrawChart(m_nAlmArg1, m_nAlmArg2);

	// set chart datetime
	DispTeachTime();

	// 学習回数の表示
	DispLearningTimes();
}

// set up Network list.
void CBpnetDlg::SetupNetworkList(void)
{
	int	i;
	int	sel	= -1;
	int	numNNList;

	// reset neural network list. 
	m_cmbNN.SetRedraw(FALSE);
	m_cmbNN.ResetContent();

	// for each neural network entry, 
	for (i = 0, numNNList = 0; i < NumNeuralNet; i++) {
		// if neural network structure definition is valid, 
		if (cnf.neuralnetConf[i].patternNN.numLayer &&
			cnf.neuralnetConf[i].corrNN.numLayer) {
			// test index. 
			if (m_nNetIdx == i)
				sel	= numNNList;
			// store the identifier of neural network. 
			m_nNetTab[numNNList++]	= i;
			// append neural net entry. 
			m_cmbNN.AddString(cnf.neuralnetConf[i].remark);
		}
	}

	// terminate neural network list. 
	m_cmbNN.SetRedraw(TRUE);

	// set high light now selection. 
	if (sel >= 0)
		m_cmbNN.SetCurSel(sel);
}

// change neural network selection from spec file.
void CBpnetDlg::ChangeNeuralnetFromFile(int nId)
{
	int		i;

	// search selection key from network identifier.
	for (i = 0; i < m_cmbNN.GetCount() && m_nNetTab[i] != nId; i++)
		;

	// if neural network list is valid,
	if (i < m_cmbNN.GetCount()) {
		// if specified neural networks are changed,
		if (nId != m_nNetIdx) {
			// free the now neural networks.
			m_bp.FreeBONeuralNetwork();
		}
		// get neural network identifier from the selection.
		m_nNetIdx	= nId;

		m_cmbNN.SetCurSel(i);
	}
}

// set up target values of neural network backpropagation.
void CBpnetDlg::SetupTargetValueColumn(void)
{
	CString	str;

	// set string of target values into editting column.
	str.Format(_T("%5.3lf"), m_dPattOrdTeachVal);
	m_editPattOrd.SetWindowText(str);

	str.Format(_T("%5.3lf"), m_dPattAlmTeachVal);
	m_editPattAlm.SetWindowText(str);

	str.Format(_T("%5.3lf"), m_dCorrOrdTeachVal);
	m_editCorrOrd.SetWindowText(str);

	str.Format(_T("%5.3lf"), m_dCorrAlmTeachVal);
	m_editCorrAlm.SetWindowText(str);
}

// set up parameter editting dialog box.
void CBpnetDlg::SetupParameterColumn(void)
{
	CString	str;

	// set string of target values into editting column.
	str.Format(_T("%5.3lf"), m_dEatha);
	m_editEatha.SetWindowText(str);

	str.Format(_T("%5.3lf"), m_dAlpha);
	m_editAlpha.SetWindowText(str);
}

// get editted target values of neural network.
void CBpnetDlg::GetTargetValue(void)
{
	_TCHAR	buf[32];

	// get string of target values from editting column,
	//	and store the values into variables.
	m_editPattOrd.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lf"), &m_dPattOrdTeachVal);

	m_editPattAlm.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lf"), &m_dPattAlmTeachVal);

	m_editCorrOrd.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lg"), &m_dCorrOrdTeachVal);

	m_editCorrAlm.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lf"), &m_dCorrAlmTeachVal);
}

// get convergence parameters from editting column.
void CBpnetDlg::GetParameter(void)
{
	_TCHAR	buf[32];

	// get convergence parameters from editting column,
	//  and store the values into variables.
	m_editEatha.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lf"), &m_dEatha);

	m_editAlpha.GetWindowText(buf, sizeof(buf));
	_stscanf(buf, _T("%lf"), &m_dAlpha);
}

// get teaching condition from check boxes.
void CBpnetDlg::GetTeachCondition(void)
{
	m_nTeachCondition = 0;

	if (m_btnPattOrd.GetCheck())
		m_nTeachCondition	|= CBpOpe::IsTeachPatOrdinary;
	if (m_btnPattAlm.GetCheck())
		m_nTeachCondition	|= CBpOpe::IsTeachPatAlarm;
	if (m_btnCorrOrd.GetCheck())
		m_nTeachCondition	|= CBpOpe::IsTeachCorOrdinary;
	if (m_btnCorrAlm.GetCheck())
		m_nTeachCondition	|= CBpOpe::IsTeachCorAlarm;
}

// enable/disable control
void CBpnetDlg::EnableControl(BOOL blEnable)
{
	CString s;

	if( m_bPass )
	{
		m_btnPass.SetTextColor(RGB(0,0,0));
		m_btnPass.SetBackColor(RGB(0,255,0));
	}
	else
	{
		m_btnPass.SetTextColor(RGB(255,255,255));
		m_btnPass.SetBackColor(RGB(255,0,0));
	}
	m_cmbNN.EnableWindow(blEnable);
	m_cmbOrd.EnableWindow(blEnable);
	m_cmbAlm.EnableWindow(blEnable);
	m_btnPattOrd.EnableWindow(blEnable);
	m_btnPattAlm.EnableWindow(blEnable);
	m_btnCorrOrd.EnableWindow(blEnable);
	m_btnCorrAlm.EnableWindow(blEnable);
	m_btnOrdL.EnableWindow(blEnable);
	m_btnOrdR.EnableWindow(blEnable);
	m_btnAlmL.EnableWindow(blEnable);
	m_btnAlmR.EnableWindow(blEnable);
	m_editPattOrd.EnableWindow(blEnable);
	m_editPattAlm.EnableWindow(blEnable);
	m_editCorrOrd.EnableWindow(blEnable);
	m_editCorrAlm.EnableWindow(blEnable);
	m_editEatha.EnableWindow(blEnable);
	m_editAlpha.EnableWindow(blEnable);
	m_btnWeiOk.EnableWindow(blEnable);
}

// display bp error message
int CBpnetDlg::DispBpErrMsg(int ret)
{
	CString	str;
	UINT	nType;

	switch(ret)
	{
	case CBpOpe::ERR_NO_NET:	str.LoadString(IDS_SIM_BP01);nType = MB_OK;break;
	case CBpOpe::ERR_PATT:		str.LoadString(IDS_SIM_BP02);nType = MB_OK;break;
	case CBpOpe::ERR_CORR:		str.LoadString(IDS_SIM_BP03);nType = MB_OK;break;
	case CBpOpe::ERR_PATT_CORR:	str.LoadString(IDS_SIM_BP04);nType = MB_OK;break;
	case CBpOpe::ERR_LOAD_NN:	str.LoadString(IDS_SIM_BP05);nType = MB_OK;break;
	case CBpOpe::ERR_NO_INIT:	str.LoadString(IDS_SIM_BP06);nType = MB_OK;break;
	case CBpOpe::ERR_NUM_DATA:	str.LoadString(IDS_SIM_BP07);nType = MB_OK;break;
	case CBpOpe::ERR_ORD_PREP:	str.LoadString(IDS_SIM_BP08);nType = MB_OK;break;
	case CBpOpe::ERR_ALM_PREP:	str.LoadString(IDS_SIM_BP09);nType = MB_OK;break;
	case CBpOpe::ERR_SPEC_FILE:	str.LoadString(IDS_SIM_BP10);nType = MB_OK;break;
	case CBpOpe::ERR_LOG_FILE:	str.LoadString(IDS_SIM_BP11);nType = MB_OK;break;
	default:					str.LoadString(IDS_SIM_BP00);nType = MB_OK;break;
	}
	return AfxMessageBox(str, nType);
}

void CBpnetDlg::OpenBpnetSecurityPass(void)
{
	m_bBpnetSecPass = TRUE;
}

LRESULT CBpnetDlg::OnBpnetChg(WPARAM wParam, LPARAM lParam)
{
	int	nNetIdx = wParam;

	if( nNetIdx != -1 && nNetIdx != m_nNetIdx )
	{
		m_nNetIdx = nNetIdx;
	}

	return 0;
}

// 学習回数を表示
void CBpnetDlg::DispLearningTimes(void)
{
	CString	str;

	//
	if( m_bp.m_pPattNN != NULL )
	{
		str.Format(_T("%d"), m_bp.m_pPattNN->times);
		m_labLearnTimes.SetWindowText(str);
	}
}

// 学習データの開始時間表示
void CBpnetDlg::DispTeachTime(void)
{
	CString			str;
	struct tm		*p;
	time_t			stt;

	// 開始時間が不正な時は表示しない
	if( m_bp.m_stOrdDataSpec.startTime >= 0 )
	{
		// 開始時間
		stt = m_bp.m_stOrdDataSpec.startTime + m_bp.m_stOrdDataSpec.shift/2;
		p	= localtime(&stt);
		str.Format(_T("'%2d.%2d.%2d"),		p->tm_year % 100, p->tm_mon + 1, p->tm_mday);
		m_textOrdDate.SetWindowText(str);
		str.Format(_T("%02d:%02d:%02d"),	p->tm_hour, p->tm_min, p->tm_sec);
		m_textOrdTime.SetWindowText(str);
	}
	else
	{
		m_textOrdDate.SetWindowText(_T("'99.99.99"));
		m_textOrdTime.SetWindowText(_T("99:99:99"));
	}
	if( m_bp.m_stAlmDataSpec.startTime >= 0 )
	{
		// 開始時間
		stt = m_bp.m_stAlmDataSpec.startTime + m_bp.m_stAlmDataSpec.shift/2;
		p	= localtime(&stt);
		str.Format(_T("'%2d.%2d.%2d"),		p->tm_year % 100, p->tm_mon + 1, p->tm_mday);
		m_textAlmDate.SetWindowText(str);
		str.Format(_T("%02d:%02d:%02d"),	p->tm_hour, p->tm_min, p->tm_sec);
		m_textAlmTime.SetWindowText(str);
	}
	else
	{
		m_textAlmDate.SetWindowText(_T("'99.99.99"));
		m_textAlmTime.SetWindowText(_T("99:99:99"));
	}

	// 選択コンビネーション
	if( m_nOrdArg1 == -1 || m_nOrdArg2 == -1 || m_nOrdArg1 >= NumTCTap || m_nOrdArg2 >= NumTCTap )
	{
		m_editOrdinary.SetWindowText(_T(""));
	}
	else
	{
		str.Format(_T("%s,%s"), tcnm->name[m_nOrdArg1], tcnm->name[m_nOrdArg2]);
		m_editOrdinary.SetWindowText(str);
	}
	if( m_nAlmArg1 == -1 || m_nAlmArg2 == -1 || m_nOrdArg1 >= NumTCTap || m_nOrdArg2 >= NumTCTap )
	{
		m_editAlarm.SetWindowText(_T(""));
	}
	else
	{
		str.Format(_T("%s,%s"), tcnm->name[m_nAlmArg1], tcnm->name[m_nAlmArg2]);
		m_editAlarm.SetWindowText(str);
	}

	// シフト時間
	str.Format(_T("%.1lf"),	m_bp.m_stOrdDataSpec.shift/2.0);
	m_textOrdShift.SetWindowText(str);
	str.Format(_T("%.1lf"),	m_bp.m_stAlmDataSpec.shift/2.0);
	m_textAlmShift.SetWindowText(str);
	
}

// save alarm position.
BOOL CBpnetDlg::AlarmSave(void)
{
	_TCHAR	name[256];

	// コンビ未選択なら何もしない
	if( m_nAlmArg1 == -1 || m_nAlmArg2 == -1 )
		return FALSE;

	// 書き込むファイルパス生成
	_stprintf(name, _T("%s\\%s") BPAlarmDataFile, cnf.WorkRootDir, cnf.workDir, m_nNetIdx);

	// データ保存
	m_bp.WriteTeachDataSpec(name, &m_bp.m_stAlmDataSpec);

	return TRUE;
}

// save ordinary position.
BOOL CBpnetDlg::OrdinarySave(void)
{
	_TCHAR	name[256];

	// コンビ未選択なら何もしない
	if( m_nOrdArg1 == -1 || m_nOrdArg2 == -1 )
		return FALSE;

	// 書き込むファイルパス生成
	_stprintf(name, _T("%s\\%s") BPOrdinaryDataFile, cnf.WorkRootDir, cnf.workDir, m_nNetIdx);

	// データ保存
	m_bp.WriteTeachDataSpec(name, &m_bp.m_stOrdDataSpec);

	return TRUE;
}

// 学習が確定されているかチェックする
BOOL CBpnetDlg::CheckLearningFinish(void)
{
	int	i, nNetId;
	int	numNNList;

	// チェック中は再入禁止
	if( m_isCheckLearn )
		return FALSE;

	// チェック処理中ON
	m_isCheckLearn = TRUE;

	// reset neural network list. 
	numNNList = m_cmbNN.GetCount();

	// ネットワークがなければ何もしない
	if( numNNList <= 0 )
	{
		// チェック処理中OFF
		m_isCheckLearn = FALSE;
		return FALSE;	// 何もなし
	}

	// 全ネットワークについて
	for( i = 0; i < numNNList; i++ )
	{
		// 一時ファイルが存在するか？
		if( m_bp.IsBONeuralNetworkTmp(m_nNetTab[i]) )
			break;
	}
	// 存在していないなら
	if( numNNList == i )
	{
		// チェック処理中OFF
		m_isCheckLearn = FALSE;
		return FALSE;	// 何もなし
	}

	// 学習確定確認
	if( IDYES == AfxMessageBox(_T("学習結果が確定されていません。確定しますか？\r\n「はい」を選択すると全てのネットワークが確定されます"), MB_YESNO) )
	{
		// 全ネットワークについて
		for( i = 0; i < numNNList; i++ )
		{
			nNetId = m_nNetTab[i];
			// 一時ファイルが存在するか？
			if( m_bp.IsBONeuralNetworkTmp(nNetId) )
			{
				// 学習一時ファイル操作同期区間開始------------
				EnterCriticalSection(&theApp.m_lnLock);

				// 一時ファイルのコピー
				m_bp.CopyBONeuralNetworkTmp(nNetId);

				// 学習一時ファイル操作同期区間終了------------
				LeaveCriticalSection(&theApp.m_lnLock);

				// clear the pending status.
				m_isLearnedPending	= FALSE;

				// 一時ファイルの削除
				m_bp.DelBONeuralNetworkTmp(nNetId);
			}
		}
	}

	// チェック処理中OFF
	m_isCheckLearn = FALSE;

	return TRUE;	// 正常
}

// 学習確定ボタン
void CBpnetDlg::OnBnClickedWeiokBtn()
{
	if( IDYES == AfxMessageBox(_T("学習データを確定してもよろしいですか？"), MB_YESNO) )
	{
		// 一時ファイルが存在するなら
		if( m_bp.IsBONeuralNetworkTmp(m_nNetIdx) )
		{
			// 学習一時ファイル操作同期区間開始------------
			EnterCriticalSection(&theApp.m_lnLock);

			// 一時ファイルのコピー
			m_bp.CopyBONeuralNetworkTmp(m_nNetIdx);

			// 学習一時ファイル操作同期区間終了------------
			LeaveCriticalSection(&theApp.m_lnLock);

			// clear the pending status.
			m_isLearnedPending	= FALSE;

			// 一時ファイルの削除
			m_bp.DelBONeuralNetworkTmp(m_nNetIdx);
		}
	}
	else
	{
	}

	// 学習未確定状態表示
	DispWeiLp();
}

// 学習データ未確定ランプ表示
void CBpnetDlg::DispWeiLp(void)
{
	// ネット番号が不正なら何もしない
	if( m_nNetIdx < 0 )
		return;

	// 学習データ（一時ファイル）が存在するなら
	if( m_bp.IsBONeuralNetworkTmp(m_nNetIdx) )
		m_labWeiLp.SetBackColor(RGB(0,255,0));
	else
		m_labWeiLp.SetBackColor(RGB(128,128,128));
}
