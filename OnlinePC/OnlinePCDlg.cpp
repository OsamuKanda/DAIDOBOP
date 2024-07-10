// OnlinePCDlg.cpp : implementation file
//
#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include ".\onlinepcdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CConfig			&cnf		= theApp.m_cnfBoexe;

// local function
//void RunChart3D(void);
//BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
//BOOL CALLBACK EnumWndProc2(HWND hWnd, LPARAM lParam);
//BOOL g_bRun3D = FALSE;


///////////////////////////////////////////////////////////////////////////////
// COnlinePCDlg dialog
COnlinePCDlg::COnlinePCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnlinePCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// initialize member
	m_pDlgActive	= NULL;
	m_pDlgBoView	= NULL;
	m_pDlgPredic	= NULL;
	m_pDlgTMap		= NULL;
	m_pDlgChart3d	= NULL;
	m_pDlgBpnet		= NULL;
	m_pDlgSim		= NULL;
	m_pDlgBatchSim	= NULL;
	m_pDlgTrend		= NULL;
	m_pDlgConfig	= NULL;
	m_pDlgBosys		= NULL;
//	m_pInfoOut		= NULL;
//	m_pDlgCastInfo	= NULL;
//	g_bRun3D		= FALSE;
	m_bInfoOutBk	= FALSE;
	m_nTimer1		= -1;
//	m_nTimer2		= -1;
	m_isAppEnd		= FALSE;
}

void COnlinePCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STR_SEL,			m_txtStrand);
	DDX_Control(pDX, IDC_MSG_OUT,			m_txtMsgOut);
	DDX_Control(pDX, IDC_INFO_OUT_BTN,		m_btnInfoOut);
	DDX_Control(pDX, IDC_QUIT,				m_btnQuit);
	DDX_Control(pDX, IDC_PIC000, m_picLogo);
}

BEGIN_MESSAGE_MAP(COnlinePCDlg, CDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_PREDIC_BTN, OnClickedPredicBtn)
	ON_BN_CLICKED(IDC_TMAP_BTN, OnClickedTmapBtn)
	ON_BN_CLICKED(IDC_3DCHART_BTN, OnClicked3dchartBtn)
	ON_BN_CLICKED(IDC_BOVIEW_BTN, OnClickedBoviewBtn)
	ON_BN_CLICKED(IDC_BPNET_BTN, OnClickedBpnetBtn)
	ON_BN_CLICKED(IDC_BATCHSIM_BTN, OnClickedBatchsimBtn)
	ON_BN_CLICKED(IDC_TRENDC_BTN, OnClickedTrendBtn)
	ON_BN_CLICKED(IDC_CONF_BTN, OnBnClickedConfigBtn)
	ON_BN_CLICKED(IDC_BOSYS_BTN, OnClickedBosysBtn)
	#define	WM_APP_TMS_QUIT					(WM_APP+0xFF6)
	ON_MESSAGE( WM_APP_TMS_QUIT,			OnTMSQuit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_INFO_OUT_BTN, OnBnClickedInfoOutBtn)
	ON_BN_CLICKED(IDC_CASTINFO_BTN, OnBnClickedCastinfoBtn)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// COnlinePCDlg message handlers
BOOL COnlinePCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	str;
	BOOL	ret;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// 
//	m_txtMsgOut.SetBackColor(RGB(120,120,120));

	// 説明画像の読み込み
	str.Format(_T("%s%s%s"), cnf.WorkRootDir, _T("\\..\\Config"), STFLogoFileName);
	m_imgPaint.Load(str);

	// set title
	str.LoadString(IDS_MAIN00_MSG);
	SetWindowText(str);

	// set password valid sec
	CPassDlg::SetPassTime(PASS_VALID_SEC);

#ifdef USE_PLC
	// 通信処理の起動
	m_plccomm.Initialize(this);

	// 通信起動待ち
	Sleep(2000);
#endif

	// open sub windows
	ret = OpenSubWindows();
	if( !ret )
	{
		AfxMessageBox(IDS_MAIN01_MSG);
	}

	// dispay strand lamp
	m_txtStrand.SetFontInfo(30, TRUE);
	DispStrandLamp();
//	m_nTimer2 = SetTimer(2, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void COnlinePCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//---------------------------------------------------------------------
		// ロゴ画像の描画
		CPaintDC	dc(this); // 描画のデバイス コンテキスト
		CRect		rcPaint;
		CBitmap		*bmp, *oldBmp;
		int			mode;

		// 描画領域の取得
		m_picLogo.GetClientRect(&rcPaint);
		m_picLogo.MapWindowPoints(this, rcPaint);
		
		// ロゴ画像の情報取得
		bmp = CBitmap::FromHandle(m_imgPaint);

		// メモリDC生成を生成し選択
		m_dcPaint.CreateCompatibleDC(&dc);
		oldBmp = m_dcPaint.SelectObject(bmp);

		// ロゴ画像を画面に描画
		mode = dc.SetStretchBltMode(COLORONCOLOR);
		dc.StretchBlt(	rcPaint.left, 
						rcPaint.top, 
						rcPaint.Width(), 
						rcPaint.Height(), 
						&m_dcPaint, 0, 0, 2914, 160, SRCCOPY);

		// 後始末
		dc.SetStretchBltMode(mode);
		dc.SelectObject(oldBmp);
		m_dcPaint.DeleteDC();

		CDialog::OnPaint();
	}
}

//It evades ending by Enter Key.
void COnlinePCDlg::OnOK(){}

//It evades ending by ESC Key.
void COnlinePCDlg::OnCancel(){}

//pushed EXIT button.
void COnlinePCDlg::OnBnClickedQuit()
{
	CString	str;
	int		ret;

	str.LoadString(IDS_MAIN02_MSG);
	ret = m_pDlgActive->MessageBox(str, _T(""), MB_YESNO);
	if( ret == IDYES )
	{
		m_isAppEnd = TRUE;
		m_pDlgPredic->EndRequest();
		m_nTimer1 = SetTimer(1, 1000, NULL);
	}
}

LRESULT COnlinePCDlg::OnTMSQuit(WPARAM wParam, LPARAM lParam)
{
	m_pDlgPredic->EndRequest();
	m_nTimer1 = SetTimer(1, 1000, NULL);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//user function

// open sub windows (first time)
BOOL COnlinePCDlg::OpenSubWindows(void) 
{
	//bo main view window
	if( m_pDlgBoView == NULL )
	{
		m_pDlgBoView = new CBoViewDlg(this);
		if( !m_pDlgBoView )
			return FALSE;
		m_pDlgBoView->ShowWindow(SW_HIDE);
	}

	//bo prediction window
	if( m_pDlgPredic == NULL )
	{
		m_pDlgPredic = new CPredicDlg(this);
		if( !m_pDlgPredic )
			return FALSE;
		m_pDlgPredic->ShowWindow(SW_HIDE);
	}

	//thermal mapping window
	if( m_pDlgTMap == NULL )
	{
		m_pDlgTMap = GetTMapDlg(this, FALSE);
		if( !m_pDlgTMap )
			return FALSE;
		m_pDlgTMap->ShowWindow(SW_HIDE);
	}

	//3d chart window
	if( m_pDlgChart3d == NULL )
	{
		m_pDlgChart3d = GetChart3DDlg(this, FALSE);
		if( !m_pDlgChart3d )
			return FALSE;
		m_pDlgChart3d->ShowWindow(SW_HIDE);
	}

	//learning window
	if( m_pDlgBpnet == NULL )
	{
		m_pDlgBpnet = new CBpnetDlg(this);
		if( !m_pDlgBpnet )
			return FALSE;
		m_pDlgBpnet->ShowWindow(SW_HIDE);
	}
	//simulation window
	if( m_pDlgSim == NULL )
	{
		m_pDlgSim = new CSimDlg(this);
		if( !m_pDlgSim )
			return FALSE;
		m_pDlgSim->ShowWindow(SW_HIDE);
	}
	//batchsim window
	if( m_pDlgBatchSim == NULL )
	{
		m_pDlgBatchSim = new CBatchSimDlg(this);
		if( !m_pDlgBatchSim )
			return FALSE;
		m_pDlgBatchSim->ShowWindow(SW_HIDE);
	}
	//trend window
	if( m_pDlgTrend == NULL )
	{
		m_pDlgTrend = new CTrendDlg(this);
		if( !m_pDlgTrend )
			return FALSE;
		m_pDlgTrend->ShowWindow(SW_HIDE);
	}
	//config setting window
	if( m_pDlgConfig == NULL )
	{
		m_pDlgConfig = new CConfigDlg(this);
		if( !m_pDlgConfig )
			return FALSE;
		m_pDlgConfig->ShowWindow(SW_HIDE);
	}
	//bo system setting window
	if( m_pDlgBosys == NULL )
	{
		m_pDlgBosys = new CBosysDlg(this);
		if( !m_pDlgBosys )
			return FALSE;
		m_pDlgBosys->ShowWindow(SW_HIDE);
	}

	////casting information output
	//if( m_pInfoOut == NULL )
	//{
	//	m_pInfoOut = new CInfoOut(this);
	//	if( !m_pInfoOut )
	//		return FALSE;
	//	m_pInfoOut->ShowWindow(SW_HIDE);
	//}
	////casting information window
	//if( m_pDlgCastInfo == NULL )
	//{
	//	m_pDlgCastInfo = new CCastInfoDlg(this);
	//	if( !m_pDlgCastInfo )
	//		return FALSE;
	//	m_pDlgCastInfo->ShowWindow(SW_HIDE);
	//}

	//open default screen
	if( m_pDlgBoView != NULL )
	{
		m_pDlgActive = m_pDlgBoView;
		m_pDlgActive->ShowWindow(SW_SHOW);
	}

	CButton	*pBtn = (CButton *)GetDlgItem(IDC_BOVIEW_BTN);

	//set default highlight button
	if( pBtn != NULL )
	{
		pBtn->SetCheck(BST_CHECKED);
	}

	return TRUE;
}

//clicked bo main view screen button.
void COnlinePCDlg::OnClickedBoviewBtn()
{
	//open bo main view window
	OpenScreen(m_pDlgBoView);
//	ChgButtonHighlight();
}

//clicked bo execute screen button.
void COnlinePCDlg::OnClickedPredicBtn()
{
	//open bo prediction window
	OpenScreen(m_pDlgPredic);
//	ChgButtonHighlight();
}

//clicked thermal mapping screen button.
void COnlinePCDlg::OnClickedTmapBtn()
{
	//open thermal mapping window
	OpenScreen3D(m_pDlgTMap);
//	ChgButtonHighlight();
}

//clicked 3d chart screen button.
void COnlinePCDlg::OnClicked3dchartBtn()
{
	//open 3d chart window
	OpenScreen3D(m_pDlgChart3d);
//	ChgButtonHighlight();
}

//clicked bpnet screen button.
void COnlinePCDlg::OnClickedBpnetBtn()
{
	//open larning window
	OpenScreen(m_pDlgBpnet);
//	ChgButtonHighlight();
}

//clicked batchsim screen button.
void COnlinePCDlg::OnClickedBatchsimBtn()
{
	//open batchsim window
	OpenScreen(m_pDlgBatchSim);
//	ChgButtonHighlight();
}

//clicked trend chart screen button.
void COnlinePCDlg::OnClickedTrendBtn()
{
	//open trend window
	OpenScreen(m_pDlgTrend);
//	ChgButtonHighlight();
}

//clicked config setting screen button.
void COnlinePCDlg::OnBnClickedConfigBtn()
{
	//open config setting window
	OpenScreen(m_pDlgConfig);
//	ChgButtonHighlight();
}

//clicked system setting screen button.
void COnlinePCDlg::OnClickedBosysBtn()
{
	//open system setting window
	OpenScreen(m_pDlgBosys);
//	ChgButtonHighlight();
}

// clicked 'Casting Information' button.
void COnlinePCDlg::OnBnClickedCastinfoBtn()
{
	//open system setting window
	//OpenScreen(m_pDlgCastInfo);
//	ChgButtonHighlight();
}

// clicked 'Information Output' button.
void COnlinePCDlg::OnBnClickedInfoOutBtn()
{
	//if( m_pInfoOut != NULL )
	//{
	//	m_pInfoOut->ShowWindow(SW_SHOW);
	//	m_pInfoOut->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//}
//	RunChart3D();
}

template<class T>
void COnlinePCDlg::OpenScreen(T *pScreen)
{
//	if(m_pDlgActive == pScreen)
//		return;

	// 移動先が学習でなく現在の画面が学習又はシミュレーション画面の場合
	if( (CDialog *)pScreen != m_pDlgBpnet && (m_pDlgActive == m_pDlgBpnet || m_pDlgActive == m_pDlgSim) )
	{
		// 学習が未確定で画面移動しないなら完了確認
		if( m_pDlgBpnet != NULL )
			m_pDlgBpnet->CheckLearningFinish();
	}

	//open larning window
	if( pScreen == NULL )
	{
		pScreen = new T(this);
		if( pScreen != NULL )
			pScreen->ShowWindow(SW_SHOW);
	}
	else
		pScreen->ShowWindow(SW_SHOW);

	//hide active screen
	if( m_pDlgActive != NULL && m_pDlgActive != pScreen )
		m_pDlgActive->ShowWindow(SW_HIDE);
	
	m_pDlgActive = pScreen;
}

void COnlinePCDlg::OpenScreen3D(CDialog *pScreen)
{
	// 現在の画面が学習又はシミュレーション画面の場合
	if( m_pDlgActive == m_pDlgBpnet || m_pDlgActive == m_pDlgSim )
	{
		// 学習が未確定で画面移動しないなら完了確認
		if( m_pDlgBpnet != NULL )
			m_pDlgBpnet->CheckLearningFinish();
	}

	pScreen->ShowWindow(SW_SHOW);

	//hide active screen
	if( m_pDlgActive != NULL && m_pDlgActive != pScreen )
		m_pDlgActive->ShowWindow(SW_HIDE);
	
	m_pDlgActive = pScreen;
}

// set all button off
void COnlinePCDlg::SetAllButtonOff(void)
{
	int		nIDs[] = {
		IDC_BOVIEW_BTN,
		IDC_PREDIC_BTN,
		IDC_3DCHART_BTN,
		IDC_TMAP_BTN,
		IDC_BPNET_BTN,
		IDC_BATCHSIM_BTN,
		IDC_TREND_BTN,
		IDC_CONFIG_BTN,
		IDC_BOSYS_BTN,
		0
	};
	CButton		*pBtn;
	int			i;

	// set all button off
	for( i = 0; nIDs[i] != 0; i++ )
	{
		pBtn = (CButton *)GetDlgItem(nIDs[i]);
		if( pBtn != NULL )
		{
			pBtn->SetCheck(BST_UNCHECKED);
		}
	}
}

// change button highlight
void COnlinePCDlg::ChgButtonHighlight(void)
{
	/*
	int		nIDs[] = {
		IDC_QUIT,
		IDC_PREDIC_BTN,
		IDC_TMAP_BTN,
		IDC_3DCHART_BTN,
		IDC_BOVIEW_BTN,
		IDC_BPNET_BTN,
		IDC_BATCHSIM_BTN,
		IDC_TREND_BTN,
		IDC_CONFIG_BTN,
		IDC_BOSYS_BTN,
		0
	};
	const MSG	*pMsg;
	CButton		*pBtn;
	int			nID, i;
	UINT		uiSts;

	// set highlight off
	for( i = 0; nIDs[i] != 0; i++ )
	{
		pBtn = (CButton *)GetDlgItem(nIDs[i]);
		if( pBtn != NULL )
		{
			uiSts = pBtn->GetState();
			if( (uiSts & 0x0004) != 0 )
			{
				pBtn->SetState(FALSE);
			}
		}
	}

	// get current message
	pMsg = GetCurrentMessage();
	if( pMsg == NULL )
		return;

	// get button handle
	nID = LOWORD(pMsg->wParam);
	pBtn = (CButton *)GetDlgItem(nID);
	if( pBtn == NULL )
		return;

	// set highlight on
	pBtn->SetState(TRUE);
	*/
}

// タイマー処理
void COnlinePCDlg::OnTimer(UINT nIDEvent)
{
	if( m_nTimer1 == nIDEvent && m_pDlgPredic->IsFinish() )
	{
		KillTimer(m_nTimer1);
//		KillTimer(m_nTimer2);
		EndDialog(IDOK);
	}

	//if( m_nTimer2 == nIDEvent && m_pDlgCastInfo != NULL )
	//{
	//	if( m_bInfoOutBk != m_pDlgCastInfo->m_bInfoMsg )
	//	{
	//		if( m_pDlgCastInfo->m_bInfoMsg )
	//			m_txtMsgOut.BlinkOn(1000, RGB(255,0,0), RGB(120,120,120));
	//		else
	//		{
	//			m_txtMsgOut.BlinkOff();
	//			m_txtMsgOut.SetBackColor(RGB(120,120,120));
	//		}
	//	}
	//	m_bInfoOutBk = m_pDlgCastInfo->m_bInfoMsg;
	//}

	CDialog::OnTimer(nIDEvent);
}

//void RunChart3D(void)
//{
//	CString strCommand;
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//
//	EnumWindows((WNDENUMPROC)EnumWndProc, (LPARAM)0);
//
//	if( g_bRun3D )
//	{
//		EnumWindows((WNDENUMPROC)EnumWndProc2, (LPARAM)0);
//	}
//	else
//	{
//		memset( &si, 0, sizeof(si) );
//		memset( &pi, 0, sizeof(pi) );
//		si.cb = sizeof(si);
//		strCommand.Format( _T("%s/../Bin%s"), cnf.WorkRootDir, Chart3DName );
//		if (CreateProcess(NULL, strCommand.GetBuffer(255), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) ) 
//		{
//			WaitForInputIdle(pi.hProcess, 5000);
//			EnumWindows((WNDENUMPROC)EnumWndProc2, (LPARAM)0);
//			CloseHandle(pi.hProcess);
//		}
//	}
//}

//BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	_TCHAR szClassName[256];
//	_TCHAR szWindowName[256];
//
//	GetClassName(hWnd, szClassName, sizeof(szClassName));
//	GetWindowText(hWnd, szWindowName, 256);
//
//	if (IsWindowVisible(hWnd) && _tcscmp(szWindowName, _T("Historical Chart 3D")) == 0 )
//	{
//		g_bRun3D = TRUE;
//	}
//
//	return TRUE;
//}
//
//BOOL CALLBACK EnumWndProc2(HWND hWnd, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	_TCHAR szClassName[256];
//	_TCHAR szWindowName[256];
//
//	GetClassName(hWnd, szClassName, sizeof(szClassName));
//	GetWindowText(hWnd, szWindowName, 256);
//
//	if (IsWindowVisible(hWnd) && _tcscmp(szWindowName, _T("Historical Chart 3D")) == 0 )
//	{
//		::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
//		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
//	}
//
//	return TRUE;
//}

// display strand lamp.
void COnlinePCDlg::DispStrandLamp( void )
{
	CString	str, str2;
	str = _T("ST");
	if( m_plccomm.m_stNetInfo.dwST > 0 )
	{
		str2.Format(_T("%d"), m_plccomm.m_stNetInfo.dwST);
		str2 += str;
		m_txtStrand.SetWindowText(str2);
		m_txtStrand.SetTextColor(RGB(0,0,0));
		m_txtStrand.SetBackColor(RGB(0,255,0));
	}
	else
	{
		m_txtStrand.SetWindowText(_T(""));
		m_txtStrand.SetTextColor(RGB(200,200,200));
		m_txtStrand.SetBackColor(RGB(100,100,100));
	}
}

