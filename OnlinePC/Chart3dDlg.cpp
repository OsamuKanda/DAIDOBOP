// Chart3dDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "Chart3dDlg.h"
#include ".\chart3ddlg.h"

///////////////////////////////////////////////////////////////////////////////
// definition
//#define New3DVer
static CConfig			&cnf		= theApp.m_cnfBoexe;

#define FixTapColumn	15
#define LooseTapColumn	15
#define N1TapColumn		1
#define N2TapColumn		1

#define	AREAGROUP	6

#ifdef New3DVer
	#define MAXTEMP		2000
#else
	#define MAXTEMP		3000
#endif

#define MINTEMP		0

int iDispType;
int iDisp;

_TCHAR *title[] = {	_T("Fix-1"),
					_T("Fix-2"),
					_T("Fix-3"),
					_T("Loose-1"),
					_T("Loose-2"),
					_T("Loose-3")};

int	iLEFTPos[TapRaw][N1TapColumn] = {	
						{ 0},
						{ 1},
						{ 2}};
int iFIXPos[TapRaw][FixTapColumn] = {	
						{ 3, 6, 9,12,15,18,21,24,27,30,33,36,39,42,45},
						{ 4, 7,10,13,16,19,22,25,28,31,34,37,40,43,46},
						{ 5, 8,11,14,17,20,23,26,29,32,35,38,41,44,47}};
int	iRIGHTPos[TapRaw][N2TapColumn] = {	
						{93},
						{94},
						{95}};
int iLOOSEPos[TapRaw][LooseTapColumn] = {
						{48,51,54,57,60,63,66,69,72,75,78,81,84,87,90},
						{49,52,55,58,61,64,67,70,73,76,79,82,85,88,91},
						{50,53,56,59,62,65,68,71,74,77,80,83,86,89,92}};

// CChart3dDlg ダイアログ

IMPLEMENT_DYNAMIC(CChart3dDlg, CDialog)
CChart3dDlg::CChart3dDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChart3dDlg::IDD, pParent)
{
	// initialize member data.
	m_pDlgTcView = NULL;
	m_isHideCloseButton = FALSE;
	memset(m_iData, 0, sizeof(m_iData));

	// create dialog.
	Create(CChart3dDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CChart3dDlg::~CChart3dDlg()
{
}

void CChart3dDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUR_TIME, m_textCurTime);

	// ***CFX***
	// Do not use class wizard to generate these. It will cause the generation of unecesary classes
	DDX_Control(pDX, IDC_CHART3D, m_ChartFX);
	if (!pDX->m_bSaveAndValidate) // Link Chart FX pointer to control window
		m_pChartFX = m_ChartFX.GetControlUnknown();
	// ***CFX***
	DDX_Control(pDX, IDC_BUTTON11, m_btnClose);

	//
	DDX_Control(pDX, IDC_STATIC_TC1, m_stTC1);
	DDX_Control(pDX, IDC_STATIC_TC2, m_stTC2);
	DDX_Control(pDX, IDC_STATIC_TC3, m_stTC3);
	DDX_Control(pDX, IDC_STATIC_TC4, m_stTC4);
	DDX_Control(pDX, IDC_STATIC_TC5, m_stTC5);
	DDX_Control(pDX, IDC_STATIC_TC6, m_stTC6);
	DDX_Control(pDX, IDC_STATIC_TC7, m_stTC7);
	DDX_Control(pDX, IDC_STATIC_TC8, m_stTC8);
	DDX_Control(pDX, IDC_STATIC_TC9, m_stTC9);
	DDX_Control(pDX, IDC_STATIC_TC10, m_stTC10);
	DDX_Control(pDX, IDC_STATIC_TC11, m_stTC11);
	DDX_Control(pDX, IDC_STATIC_TC12, m_stTC12);
	DDX_Control(pDX, IDC_STATIC_TC13, m_stTC13);
	DDX_Control(pDX, IDC_STATIC_TC14, m_stTC14);
	DDX_Control(pDX, IDC_STATIC_TC15, m_stTC15);
	DDX_Control(pDX, IDC_STATIC_TC16, m_stTC16);
	DDX_Control(pDX, IDC_STATIC_TC_NO1, m_stTCNo1);
	DDX_Control(pDX, IDC_STATIC_TC_NO2, m_stTCNo2);
	DDX_Control(pDX, IDC_STATIC_TC_NO3, m_stTCNo3);
	DDX_Control(pDX, IDC_STATIC_TC_NO4, m_stTCNo4);
	DDX_Control(pDX, IDC_STATIC_TC_NO5, m_stTCNo5);
	DDX_Control(pDX, IDC_STATIC_TC_NO6, m_stTCNo6);
	DDX_Control(pDX, IDC_STATIC_TC_NO7, m_stTCNo7);
	DDX_Control(pDX, IDC_STATIC_TC_NO8, m_stTCNo8);
	DDX_Control(pDX, IDC_STATIC_TC_NO9, m_stTCNo9);
	DDX_Control(pDX, IDC_STATIC_TC_NO10, m_stTCNo10);
	DDX_Control(pDX, IDC_STATIC_TC_NO11, m_stTCNo11);
	DDX_Control(pDX, IDC_STATIC_TC_NO12, m_stTCNo12);
	DDX_Control(pDX, IDC_STATIC_TC_NO13, m_stTCNo13);
	DDX_Control(pDX, IDC_STATIC_TC_NO14, m_stTCNo14);
	DDX_Control(pDX, IDC_STATIC_TC_NO15, m_stTCNo15);
	DDX_Control(pDX, IDC_STATIC_TC_NO16, m_stTCNo16);
	DDX_Control(pDX, IDC_CAST_VC, m_textCastVc);
	DDX_Control(pDX, IDC_MD_WIDTH, m_textWidth);
	DDX_Control(pDX, IDC_ACT_LEVEL, m_textActLevel);
	DDX_Control(pDX, IDC_CAST_LEN, m_textCastLen);
	DDX_Control(pDX, IDC_EMBR, m_textEMBr);
	DDX_Control(pDX, IDC_STATIC1, m_static[0]);
	DDX_Control(pDX, IDC_STATIC2, m_static[1]);
	DDX_Control(pDX, IDC_STATIC3, m_static[2]);
	DDX_Control(pDX, IDC_STATIC4, m_static[3]);
	DDX_Control(pDX, IDC_STATIC5, m_static[4]);
	DDX_Control(pDX, IDC_STATIC6, m_static[5]);
	DDX_Control(pDX, IDC_STATIC7, m_static[6]);
	DDX_Control(pDX, IDC_STATIC8, m_static[7]);
	DDX_Control(pDX, IDC_STATIC9, m_static[8]);
	DDX_Control(pDX, IDC_STATIC10, m_static[9]);
}

BEGIN_MESSAGE_MAP(CChart3dDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_COUNTER, OnCounter)
	ON_BN_CLICKED(IDC_SURFACE, OnSurface)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonUP)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonDOWN)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonLEFT)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonRIGHT)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonS1)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonS2)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonS3)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonN1)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonN2)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonN3)
	ON_MESSAGE(WM_CHARTDATA, OnAddone)
	ON_MESSAGE(WM_ACTINFO, OnActInfo)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CChart3dDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CChart3dDlg::OnOk(){}

//It evades ending by ESC Key.
void CChart3dDlg::OnCancel(){}

void CChart3dDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CChart3dDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

BOOL CChart3dDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_TCHAR buffer[128];
	ICfxSeriesPtr pSerie;
	int i,j;
	
	OnButtonS1();
	OnCounter();
	OnSurface();               

	m_pChartFX->MultipleColors = TRUE;

	m_pChartFX->Scheme = CHART_CSSOLID;
    m_pChartFX->PaletteBar = FALSE;
	m_pChartFX->Palette = "Sunset";

        
    //Set smooth data
    m_pChartFX->OpenDataEx(COD_VALUES, MAXTIME, N1TapColumn+FixTapColumn );
    for ( i = 0; i < MAXTIME; i++ )
	{
		if (i % 5 == 0)
		{
//M			_stprintf( buffer, _T("-%d"), i );
			_stprintf( buffer, _T("-%.1f"), (double)i*0.5 );
			m_pChartFX->KeySer[(short)i] = buffer;
		}

		pSerie = m_pChartFX->Series->Item[(short)i];
		for ( j = 0; j < N1TapColumn+FixTapColumn; j++ )
		{			
			pSerie->Yvalue[j] = 0;
		}
	}
	m_pChartFX->KeySer[(short)(MAXTIME - 5)] = "[sec]";			//M


    m_pChartFX->CloseData(COD_VALUES);
  
#ifdef New3DVer
	m_pChartFX->OpenDataEx(COD_COLORS, 20, 0);

	m_pChartFX->Color[ 0] = RGB( 55, 55,255);
	m_pChartFX->Color[ 1] = RGB( 55,105,255);
	m_pChartFX->Color[ 2] = RGB( 55,155,255);
	m_pChartFX->Color[ 3] = RGB( 55,200,255);
	m_pChartFX->Color[ 4] = RGB( 55,255,255);
	m_pChartFX->Color[ 5] = RGB( 55,255,205);
	m_pChartFX->Color[ 6] = RGB( 55,255,155);
	m_pChartFX->Color[ 7] = RGB( 55,255, 55);
	m_pChartFX->Color[ 8] = RGB(105,255, 55);
	m_pChartFX->Color[ 9] = RGB(155,255, 55);
	m_pChartFX->Color[10] = RGB(205,255, 55);
	m_pChartFX->Color[11] = RGB(255,255, 55);
	m_pChartFX->Color[12] = RGB(255,230, 55);
	m_pChartFX->Color[13] = RGB(255,205, 55);
	m_pChartFX->Color[14] = RGB(255,180, 55);
	m_pChartFX->Color[15] = RGB(255,155, 55);
	m_pChartFX->Color[16] = RGB(255,130, 55);
	m_pChartFX->Color[17] = RGB(255,105, 55);
	m_pChartFX->Color[18] = RGB(255, 80, 55);
	m_pChartFX->Color[19] = RGB(255, 55, 55);	
#else
	m_pChartFX->OpenDataEx(COD_COLORS, 15, 0);

	m_pChartFX->Color[ 0] = RGB( 55, 55,255);
	m_pChartFX->Color[ 1] = RGB( 55,155,255);
	m_pChartFX->Color[ 2] = RGB( 55,255,255);
	m_pChartFX->Color[ 3] = RGB( 55,255,155);
	m_pChartFX->Color[ 4] = RGB( 55,255, 55);
	m_pChartFX->Color[ 5] = RGB(155,255, 55);
	m_pChartFX->Color[ 6] = RGB(255,255, 55);
	m_pChartFX->Color[ 7] = RGB(255,230, 55);
	m_pChartFX->Color[ 8] = RGB(255,205, 55);
	m_pChartFX->Color[ 9] = RGB(255,180, 55);
	m_pChartFX->Color[10] = RGB(255,155, 55);
	m_pChartFX->Color[11] = RGB(255,130, 55);
	m_pChartFX->Color[12] = RGB(255,105, 55);
	m_pChartFX->Color[13] = RGB(255, 80, 55);
	m_pChartFX->Color[14] = RGB(255, 55, 55);
#endif

    m_pChartFX->CloseData(COD_COLORS);

    m_pChartFX->Axis->Item[AXIS_X]->Decimals = 0;
    m_pChartFX->Axis->Item[AXIS_Y]->Decimals = 0;

    // 3D Settings
	m_pChartFX->View3D = TRUE;
    m_pChartFX->View3DDepth = 60;
    m_pChartFX->AngleX = 30;
    m_pChartFX->AngleY = 5;
	m_pChartFX->Perspective = 50;

    //Constrains
    m_pChartFX->ShowTips = FALSE;
    m_pChartFX->AllowDrag = FALSE;
	m_pChartFX->ContextMenus = FALSE;
        
    m_pChartFX->SerLegBoxObj->BorderStyle = BBS_NONE;

	return TRUE;
}

// close my window.
void CChart3dDlg::OnBnClickedButton11()
{
	ShowWindow(SW_HIDE);
}

void CChart3dDlg::OnCounter( void ) 
{
	iDispType = 1;
	m_pChartFX->LeftGap = 0;
//    m_pChartFX->RightGap = 20;
    m_pChartFX->RightGap = 35;
    m_pChartFX->TopGap = 0;
    m_pChartFX->BottomGap = 20;
        
    m_pChartFX->Gallery = CONTOUR;
    m_pChartFX->SerLegBoxObj->Docked = TGFP_RIGHT;
        
    m_pChartFX->Axis->Item[AXIS_Y]->Style = (CfxAxisStyle) (m_pChartFX->Axis->Item[AXIS_Y]->Style & ~AS_BREAKZERO);
	m_pChartFX->RGB3DBk = CHART_TRANSPARENT;    //No background
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];    

#ifdef New3DVer
	m_pChartFX->Axis->Item[AXIS_Y]->STEP = 10;
	m_pChartFX->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartFX->Axis->Item[AXIS_Y]->Max = 200;
	m_pChartFX->Axis->Item[AXIS_Y]->Title = "[deg]";
	m_pChartFX->Axis->Item[AXIS_X]->Title = "[TC]";
    m_pChartFX->SetContourLabels(10);
#else
	m_pChartFX->Axis->Item[AXIS_Y]->STEP = 20;
	m_pChartFX->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartFX->Axis->Item[AXIS_Y]->Max = 300;
	m_pChartFX->Axis->Item[AXIS_Y]->Title = "[deg]";
	m_pChartFX->Axis->Item[AXIS_X]->Title = "[TC]";
    m_pChartFX->SetContourLabels(20);
#endif
}

void CChart3dDlg::OnSurface( void )
{
	iDispType = 0;
	m_pChartFX->LeftGap = 30;
    m_pChartFX->RightGap = 30;
    m_pChartFX->TopGap = 0;
    m_pChartFX->BottomGap = 20;
        
    m_pChartFX->Gallery = SURFACE;
    m_pChartFX->SerLegBoxObj->Docked = TGFP_RIGHT;//TGFP_BOTTOM;
        
    m_pChartFX->Axis->Item[AXIS_Y]->Style = (CfxAxisStyle) (m_pChartFX->Axis->Item[AXIS_Y]->Style | AS_BREAKZERO);
    m_pChartFX->RGB3DBk = CHART_PALETTECOLOR | 0;  // Standard background
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];

#ifdef New3DVer
	m_pChartFX->Axis->Item[AXIS_Y]->STEP = 10;
	m_pChartFX->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartFX->Axis->Item[AXIS_Y]->Max = 200;
 	m_pChartFX->Axis->Item[AXIS_Y]->Title = "[deg]";
	m_pChartFX->Axis->Item[AXIS_X]->Title = "[TC]";
	m_pChartFX->SetContourLabels(10);
#else
	m_pChartFX->Axis->Item[AXIS_Y]->STEP = 20;
	m_pChartFX->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartFX->Axis->Item[AXIS_Y]->Max = 300;
 	m_pChartFX->Axis->Item[AXIS_Y]->Title = "[deg]";
	m_pChartFX->Axis->Item[AXIS_X]->Title = "[TC]";
	m_pChartFX->SetContourLabels(20);
#endif
}

LRESULT CChart3dDlg::OnAddone(WPARAM, LPARAM lParam) 
{
    int			i,j;
	//int nData[NumTCTap];
	int nData[NumADChannel];
	int dispADData[NumADChannel];
	ICfxSeriesPtr pSerie;
	char buf[10];

	memcpy(nData, (int *)lParam, sizeof(nData));
	memcpy(dispADData, nData, sizeof(dispADData));
	SetAverageData(nData);

	MoveMemory( &m_iData[1][0], &m_iData[0][0], sizeof(m_iData) - sizeof(m_iData[0]));
	for( i = 0; i < NumTCTap; i++ )
		m_iData[0][i] = nData[i];

	//Set smooth data
	if ( iDisp == 0 )					// F-1
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N1TapColumn+FixTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLEFTPos[0][j]] / 10.0f;
			}
			for ( j = N1TapColumn; j < FixTapColumn+N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iFIXPos[0][j-N1TapColumn]] / 10.0f;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));

		m_stTCNo1.SetWindowText("#1");
		sprintf(buf, "%d", dispADData[0]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#4");
		sprintf(buf, "%d", dispADData[3]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#7");
		sprintf(buf, "%d", dispADData[6]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#10");
		sprintf(buf, "%d", dispADData[9]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#13");
		sprintf(buf, "%d", dispADData[12]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#16");
		sprintf(buf, "%d", dispADData[15]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#19");
		sprintf(buf, "%d", dispADData[18]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#22");
		sprintf(buf, "%d", dispADData[21]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#25");
		sprintf(buf, "%d", dispADData[24]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#28");
		sprintf(buf, "%d", dispADData[27]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#31");
		sprintf(buf, "%d", dispADData[30]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#34");
		sprintf(buf, "%d", dispADData[33]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#37");
		sprintf(buf, "%d", dispADData[36]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#40");
		sprintf(buf, "%d", dispADData[39]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#43");
		sprintf(buf, "%d", dispADData[42]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#94");
		sprintf(buf, "%d", dispADData[93]/10);
		m_stTC16.SetWindowText(buf);
	}
	else if ( iDisp == 1 )				// F-2
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N1TapColumn+FixTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLEFTPos[1][j]] / 10.0;
			}
			for ( j = N1TapColumn; j < FixTapColumn+N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iFIXPos[1][j-N1TapColumn]] / 10.0f;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));
		m_stTCNo1.SetWindowText("#2");
		sprintf(buf, "%d", dispADData[1]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#5");
		sprintf(buf, "%d", dispADData[4]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#8");
		sprintf(buf, "%d", dispADData[7]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#11");
		sprintf(buf, "%d", dispADData[10]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#14");
		sprintf(buf, "%d", dispADData[13]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#17");
		sprintf(buf, "%d", dispADData[16]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#20");
		sprintf(buf, "%d", dispADData[19]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#23");
		sprintf(buf, "%d", dispADData[22]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#26");
		sprintf(buf, "%d", dispADData[25]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#29");
		sprintf(buf, "%d", dispADData[28]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#32");
		sprintf(buf, "%d", dispADData[31]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#35");
		sprintf(buf, "%d", dispADData[34]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#38");
		sprintf(buf, "%d", dispADData[37]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#41");
		sprintf(buf, "%d", dispADData[40]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#44");
		sprintf(buf, "%d", dispADData[43]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#95");
		sprintf(buf, "%d", dispADData[94]/10);
		m_stTC16.SetWindowText(buf);

	}
	else if ( iDisp == 2 )				// F-3
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N1TapColumn+FixTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLEFTPos[2][j]] / 10.0;
			}
			for ( j = N1TapColumn; j < FixTapColumn+N1TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iFIXPos[2][j-N1TapColumn]] / 10.0;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));
		m_stTCNo1.SetWindowText("#3");
		sprintf(buf, "%d", dispADData[2]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#6");
		sprintf(buf, "%d", dispADData[5]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#9");
		sprintf(buf, "%d", dispADData[8]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#12");
		sprintf(buf, "%d", dispADData[11]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#15");
		sprintf(buf, "%d", dispADData[14]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#18");
		sprintf(buf, "%d", dispADData[19]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#21");
		sprintf(buf, "%d", dispADData[20]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#24");
		sprintf(buf, "%d", dispADData[23]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#27");
		sprintf(buf, "%d", dispADData[26]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#30");
		sprintf(buf, "%d", dispADData[29]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#33");
		sprintf(buf, "%d", dispADData[32]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#36");
		sprintf(buf, "%d", dispADData[35]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#39");
		sprintf(buf, "%d", dispADData[38]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#42");
		sprintf(buf, "%d", dispADData[41]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#45");
		sprintf(buf, "%d", dispADData[44]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#96");
		sprintf(buf, "%d", dispADData[95]/10);
		m_stTC16.SetWindowText(buf);

	}
	else if ( iDisp == 3 )				// L-1
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N2TapColumn+LooseTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < LooseTapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLOOSEPos[0][j]] / 10.0;
			}
			for ( j = LooseTapColumn; j < LooseTapColumn+N2TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iRIGHTPos[0][j-LooseTapColumn]] / 10.0;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));

		m_stTCNo1.SetWindowText("#91");
		sprintf(buf, "%d", dispADData[90]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#46");
		sprintf(buf, "%d", dispADData[45]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#49");
		sprintf(buf, "%d", dispADData[48]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#52");
		sprintf(buf, "%d", dispADData[51]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#55");
		sprintf(buf, "%d", dispADData[54]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#58");
		sprintf(buf, "%d", dispADData[57]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#61");
		sprintf(buf, "%d", dispADData[60]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#64");
		sprintf(buf, "%d", dispADData[63]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#67");
		sprintf(buf, "%d", dispADData[66]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#70");
		sprintf(buf, "%d", dispADData[69]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#73");
		sprintf(buf, "%d", dispADData[72]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#76");
		sprintf(buf, "%d", dispADData[75]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#79");
		sprintf(buf, "%d", dispADData[78]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#82");
		sprintf(buf, "%d", dispADData[81]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#85");
		sprintf(buf, "%d", dispADData[84]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#88");
		sprintf(buf, "%d", dispADData[87]/10);
		m_stTC16.SetWindowText(buf);

	}
	else if ( iDisp == 4 )				// L-2
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N2TapColumn+LooseTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < LooseTapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLOOSEPos[1][j]] / 10.0;
			}
			for ( j = LooseTapColumn; j < LooseTapColumn+N2TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iRIGHTPos[1][j-LooseTapColumn]] / 10.0;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));


		m_stTCNo1.SetWindowText("#92");
		sprintf(buf, "%d", dispADData[91]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#47");
		sprintf(buf, "%d", dispADData[46]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#50");
		sprintf(buf, "%d", dispADData[49]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#53");
		sprintf(buf, "%d", dispADData[52]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#56");
		sprintf(buf, "%d", dispADData[55]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#59");
		sprintf(buf, "%d", dispADData[58]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#62");
		sprintf(buf, "%d", dispADData[61]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#65");
		sprintf(buf, "%d", dispADData[64]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#68");
		sprintf(buf, "%d", dispADData[67]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#71");
		sprintf(buf, "%d", dispADData[70]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#74");
		sprintf(buf, "%d", dispADData[73]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#77");
		sprintf(buf, "%d", dispADData[76]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#80");
		sprintf(buf, "%d", dispADData[79]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#83");
		sprintf(buf, "%d", dispADData[82]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#86");
		sprintf(buf, "%d", dispADData[85]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#89");
		sprintf(buf, "%d", dispADData[88]/10);
		m_stTC16.SetWindowText(buf);

	}
	else if ( iDisp == 5 )				// L-3
	{
	    m_pChartFX->OpenDataEx( COD_VALUES, MAXTIME, N2TapColumn+LooseTapColumn );
		for ( i = 0; i < MAXTIME ; i++ )
		{
			pSerie = m_pChartFX->Series->Item[(short)i];
			for ( j = 0; j < LooseTapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iLOOSEPos[2][j]] / 10.0;
			}
			for ( j = LooseTapColumn; j < LooseTapColumn+N2TapColumn; j++ )
			{
				pSerie->Yvalue[j] = m_iData[i][iRIGHTPos[2][j-LooseTapColumn]] / 10.0;
			}
		}
	    m_pChartFX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));


		m_stTCNo1.SetWindowText("#93");
		sprintf(buf, "%d", dispADData[92]/10);
		m_stTC1.SetWindowText(buf);

		m_stTCNo2.SetWindowText("#48");
		sprintf(buf, "%d", dispADData[47]/10);
		m_stTC2.SetWindowText(buf);

		m_stTCNo3.SetWindowText("#51");
		sprintf(buf, "%d", dispADData[50]/10);
		m_stTC3.SetWindowText(buf);

		m_stTCNo4.SetWindowText("#54");
		sprintf(buf, "%d", dispADData[53]/10);
		m_stTC4.SetWindowText(buf);

		m_stTCNo5.SetWindowText("#57");
		sprintf(buf, "%d", dispADData[56]/10);
		m_stTC5.SetWindowText(buf);

		m_stTCNo6.SetWindowText("#60");
		sprintf(buf, "%d", dispADData[59]/10);
		m_stTC6.SetWindowText(buf);

		m_stTCNo7.SetWindowText("#63");
		sprintf(buf, "%d", dispADData[62]/10);
		m_stTC7.SetWindowText(buf);

		m_stTCNo8.SetWindowText("#66");
		sprintf(buf, "%d", dispADData[65]/10);
		m_stTC8.SetWindowText(buf);

		m_stTCNo9.SetWindowText("#69");
		sprintf(buf, "%d", dispADData[68]/10);
		m_stTC9.SetWindowText(buf);

		m_stTCNo10.SetWindowText("#72");
		sprintf(buf, "%d", dispADData[71]/10);
		m_stTC10.SetWindowText(buf);

		m_stTCNo11.SetWindowText("#75");
		sprintf(buf, "%d", dispADData[74]/10);
		m_stTC11.SetWindowText(buf);

		m_stTCNo12.SetWindowText("#78");
		sprintf(buf, "%d", dispADData[77]/10);
		m_stTC12.SetWindowText(buf);

		m_stTCNo13.SetWindowText("#81");
		sprintf(buf, "%d", dispADData[80]/10);
		m_stTC13.SetWindowText(buf);

		m_stTCNo14.SetWindowText("#84");
		sprintf(buf, "%d", dispADData[83]/10);
		m_stTC14.SetWindowText(buf);

		m_stTCNo15.SetWindowText("#87");
		sprintf(buf, "%d", dispADData[86]/10);
		m_stTC15.SetWindowText(buf);

		m_stTCNo16.SetWindowText("#90");
		sprintf(buf, "%d", dispADData[89]/10);
		m_stTC16.SetWindowText(buf);

	}

	return 0;
}

LRESULT CChart3dDlg::OnActInfo(WPARAM, LPARAM lParam) 
{
	StActiveInfo	info = *((StActiveInfo *)lParam);
	CString			str;

	str.Format(_T("%.2lf"), info.dVc );
	m_textCastVc.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dWidthR+info.dWidthL );
	m_textWidth.SetWindowText(str);
	str.Format(_T("%.1lf"), info.dLevel );
	m_textActLevel.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dLen );
	m_textCastLen.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dEMBr );
	m_textEMBr.SetWindowText(str);

	return 0;
}

void CChart3dDlg::OnButtonUP() 
{
	if ( iDispType == 0 && m_pChartFX->AngleX <= 175 )
		m_pChartFX->AngleX += 5;
}

void CChart3dDlg::OnButtonDOWN() 
{
	if ( iDispType == 0 && 5 <= m_pChartFX->AngleX )
		m_pChartFX->AngleX -= 5;
}

void CChart3dDlg::OnButtonLEFT() 
{
	if ( iDispType == 0 && 5 <= m_pChartFX->AngleY )
		m_pChartFX->AngleY -= 5;
}

void CChart3dDlg::OnButtonRIGHT() 
{
	if ( iDispType == 0 && m_pChartFX->AngleY <= 175 )
		m_pChartFX->AngleY += 5;	
}

void CChart3dDlg::OnButtonS1() 
{
	register int i;
	char buf[32];

	iDisp = 0;					// F-1
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];
		
	m_pChartFX->Legend[(short)0] = "#94";//M
	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 43 - i*3 );
		m_pChartFX->Legend[(short)(i + 1)] = buf;//M
	}
	SetTcBackColor();
}

void CChart3dDlg::OnButtonS2() 
{
	register int i;
	char buf[32];

	iDisp = 1;					// F-2
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];

	m_pChartFX->Legend[(short)0] = "#95";//M
	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 44 - i*3 );
		m_pChartFX->Legend[(short)(i + 1)] = buf;//M
	}
	SetTcBackColor();
}


void CChart3dDlg::OnButtonS3() 
{
	register int i;
	char buf[32];

	iDisp = 2;					// F-3
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];

	m_pChartFX->Legend[(short)0] = "#96";//M
	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 45 - i*3 );
		m_pChartFX->Legend[(short)(i + 1)] = buf;//M
	}
	SetTcBackColor();
}

void CChart3dDlg::OnButtonN1() 
{
	register int i;
	char buf[32];

	iDisp = 3;					// L-1
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];

	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 88 - i*3 );
		m_pChartFX->Legend[(short)i] = buf;//M
	}
	m_pChartFX->Legend[(short)i] = "#91";//M
		
	SetTcBackColor();
}

void CChart3dDlg::OnButtonN2() 
{
	register int i;
	char buf[32];

	iDisp = 4;					// L-2
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];

	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 89 - i*3 );
		m_pChartFX->Legend[(short)i] = buf;//M
	}
	m_pChartFX->Legend[(short)i] = "#92";//M
		
	SetTcBackColor();
}

void CChart3dDlg::OnButtonN3() 
{
	register int i;
	char buf[32];

	iDisp = 5;					// L-3
	m_pChartFX->Title[CHART_TOPTIT] = title[iDisp];


	for(i = 0; i < 15; i++){
		sprintf(buf, "#%d", 90 - i*3 );
		m_pChartFX->Legend[(short)i] = buf;//M
	}
	m_pChartFX->Legend[(short)i] = "#93";//M
	SetTcBackColor();
}

// setAverageData.
void CChart3dDlg::SetAverageData(int *data)
{
	int l[] = { 31, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 30};
	int	dmData[NumTCTap];
	int i, j;

	// 配列変数の初期化
	memcpy( dmData, data, sizeof(dmData) );

	for( i = 0; i < NumTCTap / TapRaw; i++ )
		for( j = 0; j < TapRaw; j++ )
			data[i * TapRaw + j] = dmData[l[i] * TapRaw + j];

}

// hide close button on/off.
void CChart3dDlg::HideCloseButton(BOOL bHide)
{
	m_isHideCloseButton = bHide;
}

void CChart3dDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	int		i;

	if( m_isHideCloseButton )
		m_btnClose.ShowWindow(SW_HIDE);
	else
	{
		m_textCastVc.ShowWindow(SW_HIDE);
		m_textWidth.ShowWindow(SW_HIDE);
		m_textActLevel.ShowWindow(SW_HIDE);
		m_textCastLen.ShowWindow(SW_HIDE);
		m_textEMBr.ShowWindow(SW_HIDE);
		for( i = 0; i < 10; i++ )
			m_static[i].ShowWindow(SW_HIDE);
	}
	if( !bShow )
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
	else
	{
		SetTcBackColor();
	}
}

// selected 'TC Position' menu.
void CChart3dDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

void CChart3dDlg::SetTcBackColor(void)
{
	SharedMemory	*pShMem = GetSharedMemory();
	COLORREF		col1, col2;
	int				nWidthR, nWidthL;

	if( pShMem == NULL )
		return;

	nWidthR = pShMem->moldSizeR;
	nWidthL = pShMem->moldSizeL;
	col1	= RGB(208,208,208);
	col2	= RGB(80,80,80);

	m_stTC1.SetBackColor(col1);
	m_stTC2.SetBackColor(col1);
	m_stTC3.SetBackColor(col1);
	m_stTC4.SetBackColor(col1);
	m_stTC5.SetBackColor(col1);
	m_stTC6.SetBackColor(col1);
	m_stTC7.SetBackColor(col1);
	m_stTC8.SetBackColor(col1);
	m_stTC9.SetBackColor(col1);
	m_stTC10.SetBackColor(col1);
	m_stTC11.SetBackColor(col1);
	m_stTC12.SetBackColor(col1);
	m_stTC13.SetBackColor(col1);
	m_stTC14.SetBackColor(col1);
	m_stTC15.SetBackColor(col1);
	m_stTC16.SetBackColor(col1);

	//Set smooth data
	if ( iDisp == 0 || iDisp == 1 || iDisp == 2 )
	{
		if( cnf.nTpPosData[0] > nWidthR ) m_stTC1.SetBackColor(col2);
		if( cnf.nTpPosData[1] > nWidthR ) m_stTC2.SetBackColor(col2);
		if( cnf.nTpPosData[2] > nWidthR ) m_stTC3.SetBackColor(col2);
		if( cnf.nTpPosData[3] > nWidthR ) m_stTC4.SetBackColor(col2);
		if( cnf.nTpPosData[4] > nWidthR ) m_stTC5.SetBackColor(col2);
		if( cnf.nTpPosData[5] > nWidthR ) m_stTC6.SetBackColor(col2);
		if( cnf.nTpPosData[6] > nWidthR ) m_stTC7.SetBackColor(col2);
		if( cnf.nTpPosData[8] > nWidthL ) m_stTC9.SetBackColor(col2);
		if( cnf.nTpPosData[9] > nWidthL ) m_stTC10.SetBackColor(col2);
		if( cnf.nTpPosData[10] > nWidthL ) m_stTC11.SetBackColor(col2);
		if( cnf.nTpPosData[11] > nWidthL ) m_stTC12.SetBackColor(col2);
		if( cnf.nTpPosData[12] > nWidthL ) m_stTC13.SetBackColor(col2);
		if( cnf.nTpPosData[13] > nWidthL ) m_stTC14.SetBackColor(col2);
		if( cnf.nTpPosData[14] > nWidthL ) m_stTC15.SetBackColor(col2);
	}
	else if ( iDisp == 3 || iDisp == 4 || iDisp == 5 )
	{
		if( cnf.nTpPosData[15] > nWidthR ) m_stTC2.SetBackColor(col2);
		if( cnf.nTpPosData[16] > nWidthR ) m_stTC3.SetBackColor(col2);
		if( cnf.nTpPosData[17] > nWidthR ) m_stTC4.SetBackColor(col2);
		if( cnf.nTpPosData[18] > nWidthR ) m_stTC5.SetBackColor(col2);
		if( cnf.nTpPosData[19] > nWidthR ) m_stTC6.SetBackColor(col2);
		if( cnf.nTpPosData[20] > nWidthR ) m_stTC7.SetBackColor(col2);
		if( cnf.nTpPosData[21] > nWidthR ) m_stTC9.SetBackColor(col2);
		if( cnf.nTpPosData[23] > nWidthL ) m_stTC10.SetBackColor(col2);
		if( cnf.nTpPosData[24] > nWidthL ) m_stTC11.SetBackColor(col2);
		if( cnf.nTpPosData[25] > nWidthL ) m_stTC12.SetBackColor(col2);
		if( cnf.nTpPosData[26] > nWidthL ) m_stTC13.SetBackColor(col2);
		if( cnf.nTpPosData[27] > nWidthL ) m_stTC14.SetBackColor(col2);
		if( cnf.nTpPosData[28] > nWidthL ) m_stTC15.SetBackColor(col2);
		if( cnf.nTpPosData[29] > nWidthL ) m_stTC16.SetBackColor(col2);
	}
}
