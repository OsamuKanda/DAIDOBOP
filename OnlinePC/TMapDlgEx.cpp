// TMapDlgEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TMapDlgEx.h"
#include ".\tmapdlgex.h"

///////////////////////////////////////////////////////////////////////////////
// definition

#define MAXTEMP		3000
#define MINTEMP		0

// CTMapDlgEx ダイアログ

IMPLEMENT_DYNAMIC(CTMapDlgEx, CDialog)
CTMapDlgEx::CTMapDlgEx(CWnd* pParent /*=NULL*/)
	: CDialog(CTMapDlgEx::IDD, pParent)
{
	// initialize member data.
	m_isHideCloseButton = FALSE;
	m_bBatchStop		= FALSE;
	m_bAutoRun			= FALSE;
	m_nRecCur			= 0;
	m_nRecCnt			= 0;
	m_nStartTime		= 0;
	m_nOffsetTime		= 0;
	m_pBatch			= NULL;
	m_pDlgTcView		= NULL;

	// create dialog.
	Create(CTMapDlgEx::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CTMapDlgEx::~CTMapDlgEx()
{
}

//It evades ending by Enter Key.
void CTMapDlgEx::OnOK(){}

//It evades ending by ESC Key.
void CTMapDlgEx::OnCancel(){}

void CTMapDlgEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// ***CFX***
	// Do not use class wizard to generate these. It will cause the generation of unecesary classes
	DDX_Control(pDX, IDC_CHARTFIX,   m_ChartFIX);
	DDX_Control(pDX, IDC_CHARTLOOSE, m_ChartLOOSE);
	if ( !pDX->m_bSaveAndValidate ) // Link Chart FX pointer to control window
	{
		m_pChartFIX = m_ChartFIX.GetControlUnknown();
		m_pChartLOOSE = m_ChartLOOSE.GetControlUnknown();
	}
	// ***CFX***
	DDX_Control(pDX, IDC_BUTTON1, m_btnClose);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scr);
	DDX_Control(pDX, IDC_CUR_TIME, m_textCurTime);
}


BEGIN_MESSAGE_MAP(CTMapDlgEx, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_MESSAGE(WM_MAPDATA, OnAddone)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_TIMER()
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTMapDlgEx, CDialog)
	ON_EVENT(CTMapDlgEx, IDC_CHARTFIX, 20, PostPaintChartfix, VTS_I2 VTS_I2 VTS_I4 VTS_PI2)
	ON_EVENT(CTMapDlgEx, IDC_CHARTLOOSE, 20, PostPaintChartloose, VTS_I2 VTS_I2 VTS_I4 VTS_PI2)
END_EVENTSINK_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTMapDlgEx メッセージ ハンドラ

void CTMapDlgEx::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CTMapDlgEx::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// WM_INITDIALOG
BOOL CTMapDlgEx::OnInitDialog()
{
	CDialog::OnInitDialog();
	
 	ICfxSeriesPtr pSerie;
	int i,j;

	//-------------------------------------------------------------------------
	// initialize for Fix side
	//-------------------------------------------------------------------------
	m_pChartFIX->MultipleColors = FALSE;
	m_pChartFIX->Scheme = CHART_CSSOLID;
    m_pChartFIX->PaletteBar = FALSE;
	m_pChartFIX->Palette = "Sunset";

    //Set smooth data
    m_pChartFIX->OpenDataEx(COD_VALUES, (PartitionCount+1)*(TapRaw-1), (PartitionCount+1)*(NumTCTap / TapRaw / 2 -1) );
    for ( i = 0; i < (PartitionCount+1)*(TapRaw-1); i++ )
	{
		pSerie = m_pChartFIX->Series->Item[i];
		for ( j = 0; j < (PartitionCount+1)*(NumTCTap / TapRaw / 2 -1); j++ )
		{
			pSerie->Yvalue[j] = 0.0f;
		}
	}
    m_pChartFIX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));
	m_pChartFIX->OpenDataEx(COD_COLORS, 15, 0);

	m_pChartFIX->Color[ 0] = RGB( 55, 55,255);
	m_pChartFIX->Color[ 1] = RGB( 55,155,255);
	m_pChartFIX->Color[ 2] = RGB( 55,255,255);
	m_pChartFIX->Color[ 3] = RGB( 55,255,155);
	m_pChartFIX->Color[ 4] = RGB( 55,255, 55);
	m_pChartFIX->Color[ 5] = RGB(155,255, 55);
	m_pChartFIX->Color[ 6] = RGB(255,255, 55);
	m_pChartFIX->Color[ 7] = RGB(255,230, 55);
	m_pChartFIX->Color[ 8] = RGB(255,205, 55);
	m_pChartFIX->Color[ 9] = RGB(255,180, 55);
	m_pChartFIX->Color[10] = RGB(255,155, 55);
	m_pChartFIX->Color[11] = RGB(255,130, 55);
	m_pChartFIX->Color[12] = RGB(255,105, 55);
	m_pChartFIX->Color[13] = RGB(255, 80, 55);
	m_pChartFIX->Color[14] = RGB(255, 55, 55);

    m_pChartFIX->CloseData(COD_COLORS);

    m_pChartFIX->Axis->Item[AXIS_X]->Decimals = 0;
    m_pChartFIX->Axis->Item[AXIS_Y]->Decimals = 0;

	m_pChartFIX->Axis->Item[AXIS_Y]->STEP = 20;
	m_pChartFIX->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartFIX->Axis->Item[AXIS_Y]->Max = 300;

	// 3D Settings
	m_pChartFIX->View3D = FALSE;

    //Constrains
    m_pChartFIX->ShowTips = FALSE;
    m_pChartFIX->AllowDrag = FALSE;
    m_pChartFIX->SerLegBoxObj->BorderStyle = BBS_NONE;

	m_pChartFIX->LeftGap = 0;
    m_pChartFIX->RightGap = 0;
    m_pChartFIX->TopGap = 0;
	m_pChartFIX->BottomGap = 0;
        
    m_pChartFIX->Gallery = CONTOUR;
    m_pChartFIX->SetContourLabels(20);
	m_pChartFIX->SerLegBoxObj->Docked = TGFP_RIGHT;	
        
    m_pChartFIX->Axis->Item[AXIS_Y]->Style = (CfxAxisStyle) (m_pChartFIX->Axis->Item[AXIS_Y]->Style & ~AS_BREAKZERO);
	m_pChartFIX->RGB3DBk = CHART_TRANSPARENT;    //No background
	//m_pChartFIX->Title[CHART_TOPTIT]   = "EAST                                                    FIX                                                 ";
	m_pChartFIX->Title[CHART_TOPTIT]   = "WEST                                                    FIX                                                 ";

	//-------------------------------------------------------------------------
	// initialize for Loose side
	//-------------------------------------------------------------------------
	m_pChartLOOSE->MultipleColors = FALSE;
	m_pChartLOOSE->Scheme = CHART_CSSOLID;
    m_pChartLOOSE->PaletteBar = FALSE;
	m_pChartLOOSE->Palette = "Sunset";

    //Set smooth data
    m_pChartLOOSE->OpenDataEx(COD_VALUES, (PartitionCount+1)*(TapRaw-1), (PartitionCount+1)*(NumTCTap / TapRaw / 2 -1) );
    for ( i = 0; i < (PartitionCount+1)*(TapRaw-1); i++ )
	{
		pSerie = m_pChartLOOSE->Series->Item[i];
		for ( j = 0; j < (PartitionCount+1)*(NumTCTap / TapRaw / 2 -1); j++ )
		{
			pSerie->Yvalue[j] = 0.0f;
		}
	}
    m_pChartLOOSE->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));
	m_pChartLOOSE->OpenDataEx(COD_COLORS, 15, 0);

	m_pChartLOOSE->Color[ 0] = RGB( 55, 55,255);
	m_pChartLOOSE->Color[ 1] = RGB( 55,155,255);
	m_pChartLOOSE->Color[ 2] = RGB( 55,255,255);
	m_pChartLOOSE->Color[ 3] = RGB( 55,255,155);
	m_pChartLOOSE->Color[ 4] = RGB( 55,255, 55);
	m_pChartLOOSE->Color[ 5] = RGB(155,255, 55);
	m_pChartLOOSE->Color[ 6] = RGB(255,255, 55);
	m_pChartLOOSE->Color[ 7] = RGB(255,230, 55);
	m_pChartLOOSE->Color[ 8] = RGB(255,205, 55);
	m_pChartLOOSE->Color[ 9] = RGB(255,180, 55);
	m_pChartLOOSE->Color[10] = RGB(255,155, 55);
	m_pChartLOOSE->Color[11] = RGB(255,130, 55);
	m_pChartLOOSE->Color[12] = RGB(255,105, 55);
	m_pChartLOOSE->Color[13] = RGB(255, 80, 55);
	m_pChartLOOSE->Color[14] = RGB(255, 55, 55);

    m_pChartLOOSE->CloseData(COD_COLORS);

    m_pChartLOOSE->Axis->Item[AXIS_X]->Decimals = 0;
    m_pChartLOOSE->Axis->Item[AXIS_Y]->Decimals = 0;

	m_pChartLOOSE->Axis->Item[AXIS_Y]->STEP = 20;
	m_pChartLOOSE->Axis->Item[AXIS_Y]->Min = 0;
	m_pChartLOOSE->Axis->Item[AXIS_Y]->Max = 300;

	// 3D Settings
	m_pChartLOOSE->View3D = FALSE;

    //Constrains
    m_pChartLOOSE->ShowTips = FALSE;
    m_pChartLOOSE->AllowDrag = FALSE;
    m_pChartLOOSE->SerLegBoxObj->BorderStyle = BBS_NONE;

	m_pChartLOOSE->LeftGap = 0;
    m_pChartLOOSE->RightGap = 0;
    m_pChartLOOSE->TopGap = 0;
    m_pChartLOOSE->BottomGap = 0;
        
    m_pChartLOOSE->Gallery = CONTOUR;
    m_pChartLOOSE->SetContourLabels(20);
 	m_pChartLOOSE->SerLegBoxObj->Docked = TGFP_RIGHT;	
									// TGFP_LEFT            513  ツール画面を左マージンにドック表示する 
									// TGFP_BOTTOM 258  ツール画面を下マージンにドック表示する 
									// TGFP_RIGHT 515 ツール画面を右マージンにドック表示する 
									// TGFP_FIXED 1028 凡例ボックスをチャート領域内の固定位置で表示する（凡例ボックスのみに適用） 
									// TGFP_FLOAT 1029 エンドユーザに浮動ツールを提供する 
        
    m_pChartLOOSE->Axis->Item[AXIS_Y]->Style = (CfxAxisStyle) (m_pChartFIX->Axis->Item[AXIS_Y]->Style & ~AS_BREAKZERO);
	m_pChartLOOSE->RGB3DBk = CHART_TRANSPARENT;    //No background
	//m_pChartLOOSE->Title[CHART_TOPTIT]   = "                                                 LOOSE                                                WEST ";
	m_pChartLOOSE->Title[CHART_TOPTIT]   = "                                                 LOOSE                                                EAST ";

	// start timer
	SetTimer( 1, 500, NULL );

	return TRUE;
}

// close own window
void CTMapDlgEx::OnBnClickedButton1()
{
	ShowWindow(SW_HIDE);
	m_bBatchStop = FALSE;
}

LRESULT CTMapDlgEx::OnAddone(WPARAM wParam, LPARAM lParam) 
{
    int l,k,i,j,m;
	int nData[NumTCTap];
	ICfxSeriesPtr pSerie;

	m_nRecCur = (int)wParam;
	m_scr.SetScrollPos(m_nRecCur);
	memcpy(nData, (int *)lParam, sizeof(nData));
	SetAverageData(nData);
	GetAverageData(nData);

	//-------------------------------------------------------------------------
    //Set smooth data for FIX
	//-------------------------------------------------------------------------
    m_pChartFIX->OpenDataEx( COD_VALUES, (PartitionCount+1)*(TapRaw-1), (PartitionCount+1)*(NumTCTap / TapRaw / 2-1) );
	m = NumTCTap / TapRaw / 2 -1;
	for ( l = 0; l < m; l++ )
	{
		for ( k = 0; k < TapRaw-1; k++ )	// 縦のチャンネル数
		{
			for ( i = 0; i < (PartitionCount+1); i++ )// 縦のデータ数
			{
				pSerie = m_pChartFIX->Series->Item[(TapRaw-1-1-k)*(PartitionCount+1)+((PartitionCount+1)-1-i)];
				for ( j = 0; j < (PartitionCount+1); j++ )	// 横のデータ数
				{
					pSerie->Yvalue[l*(PartitionCount+1)+j] = m_averageData[(m-l-1)*TapRaw+k][i][PartitionCount-j] / 10.0f;
				}
			}
		}
	}
    m_pChartFIX->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));

	//-------------------------------------------------------------------------
    //Set smooth data for LOOSE
	//-------------------------------------------------------------------------
    m_pChartLOOSE->OpenDataEx( COD_VALUES, (PartitionCount+1)*(TapRaw-1), (PartitionCount+1)*(NumTCTap / TapRaw / 2 -1) );
	for ( l = 0; l < NumTCTap / TapRaw / 2 -1; l++ )
	{
		for ( k = 0; k < TapRaw-1; k++ )	// 縦のチャンネル数
		{
			for ( i = 0; i < (PartitionCount+1); i++ )// 縦のデータ数
			{
				pSerie = m_pChartLOOSE->Series->Item[(TapRaw-1-1-k)*(PartitionCount+1)+((PartitionCount+1)-1-i)];
				for ( j = 0; j < (PartitionCount+1); j++ )	// 横のデータ数
				{
					int idx = l*TapRaw+k+NumTCTap/2;
					pSerie->Yvalue[l*(PartitionCount+1)+j] = m_averageData[idx][i][j] / 10.0f;
				}
			}
		}
	}
    m_pChartLOOSE->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));

	return 0;
}

// setAverageData.
void CTMapDlgEx::SetAverageData(int *data)
{
	int l[] = {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 31, 
		        29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 30};
	int	dmData[NumTCTap];
	int i, j;

	// 配列変数の初期化
	memcpy( dmData, data, sizeof(dmData) );

	for( i = 0; i < NumTCTap / TapRaw; i++ )
		for( j = 0; j < TapRaw; j++ )
			data[i * TapRaw + j] = dmData[l[i] * TapRaw + j];

}

// get Average Data.
void CTMapDlgEx::GetAverageData(int *pData)
{
	int	i, j;

	// 配列変数の初期化
	memset( m_averageData, 0, sizeof( m_averageData ) );
	for( i = 0; i < NumTCTap / TapRaw; i++ )
	{
		for( j = 0; j < TapRaw-1; j++)
			CalcAverageData(i * TapRaw + j, pData);
	}
}

// calc Average Data.
void CTMapDlgEx::CalcAverageData(int f, int *pData)
{
	int j, k, z;
	float a1, a2, a3, q, r, s;

	/*データ点数を設定*/
	z = PartitionCount + 1;

	/*各ポイント値を設定*/
	m_averageData[f][0][0] = pData[f];				// 中心点
	m_averageData[f][0][z - 1] = pData[f + 3];		// 右横
	m_averageData[f][z - 1][0] = pData[f + 1];		// 真下
	m_averageData[f][z - 1][z - 1] = pData[f + 4];	// 右斜め下

	/*p[f]-p[f + 3]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 3] - pData[f]) / z;

	/*p[f + 1]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 1]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（横方向）のポイント値を設定*/
		m_averageData[f][0][j] = int(a1 * j + pData[f]);
		m_averageData[f][z - 1][j] = int(a2 * j + pData[f + 1]);
	}

	/*p[f]-p[f + 1]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 1] - pData[f]) / z;

	/*p[f + 3]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 3]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（縦方向）のポイント値を設定*/
		m_averageData[f][j][0] = int(a1 * j + pData[f]);
		m_averageData[f][j][z - 1] = int(a2 * j + pData[f + 3]);
	}

	/*p[f]-p[f + 4]ポイント間の傾きを計算*/
	a3 = (float)(pData[f + 4] - pData[f]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*p[f]-p[f + 3]ポイント間の傾きを計算*/
		a1 = (float)(m_averageData[f][j][z - 1] - m_averageData[f][j][0]) / z;

		for(k = 1;k < z - 1;k++)
		{
			/*p[f]-p[f + 1]ポイント間の傾きを計算*/
			a2 = (float)(m_averageData[f][z - 1][k] - m_averageData[f][0][k]) / z;

			/*p[f]-p[f + 3]ポイント間のポイント値を計算*/
			q = a1 * k + m_averageData[f][j][0];

			/*p[f]-p[f + 1]ポイント間のポイント値を計算*/
			r = a2 * j + m_averageData[f][0][k];

			/*対角線上のデータの場合*/
			if (k == j)
			{
				/*p[f]-p[f + 4]ポイント間のポイント値を計算*/
				s = a3 * k + pData[f];

				/*3つのポイント値の平均をポイント値として設定*/
				m_averageData[f][j][k] = int((q + r + s) / 3);
			}
			/*対角線上以外のデータの場合*/
			else
			{
				/*2つのポイント値の平均をポイント値として設定*/
				m_averageData[f][j][k] = int((q + r) / 2);
			}
		}
	}
}

// hide close button on/off.
void CTMapDlgEx::HideCloseButton(BOOL bHide)
{
	m_isHideCloseButton = bHide;
}


void CTMapDlgEx::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		if( m_isHideCloseButton )
			m_btnClose.ShowWindow(SW_HIDE);

		m_nRecCnt = m_pBatch->GetRecordCount();
		m_scr.SetScrollRange(0, m_nRecCnt-1);
	}
	else
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

void CTMapDlgEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 描画メッセージで CDialog::OnPaint() を呼び出さないでください。
}

void CTMapDlgEx::PostPaintChartfix(short w, short h, long lPaint, short* nRes)
{
	_variant_t vt = m_pChartFIX->PaintInfo(CPI_GETDC);
	HDC	hDC = (HDC)(long)vt;
	if( hDC != NULL )
	{
		CDC	*pDC = CDC::FromHandle(hDC);
		CPen	pen, *oldPen;
		pen.CreatePen(PS_SOLID, 2, RGB(0,0,0));
		oldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->MoveTo(67, 46);
		pDC->LineTo(67, h-3);
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
	}
}

void CTMapDlgEx::PostPaintChartloose(short w, short h, long lPaint, short* nRes)
{
	_variant_t vt = m_pChartLOOSE->PaintInfo(CPI_GETDC);
	HDC	hDC = (HDC)(long)vt;
	if( hDC != NULL )
	{
		CDC	*pDC = CDC::FromHandle(hDC);
		CPen	pen, *oldPen;
		pen.CreatePen(PS_SOLID, 2, RGB(0,0,0));
		oldPen = (CPen *)pDC->SelectObject(&pen);
		pDC->MoveTo(w-60, 46);
		pDC->LineTo(w-60, h-3);
		pDC->SelectObject(oldPen);
		pen.DeleteObject();
	}
}

// stop batchsim
void CTMapDlgEx::OnBnClickedButton4()
{
	m_bBatchStop = TRUE;
}

BOOL CTMapDlgEx::GetBatchStopReq(void)
{
	return m_bBatchStop;
}

void CTMapDlgEx::SetBatchStopReq(BOOL bSet)
{
	m_bBatchStop = bSet;
	m_bAutoRun	 = FALSE;
}


void CTMapDlgEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( m_bBatchStop && !m_bAutoRun && (&m_scr == pScrollBar) )
	{
		int	nCurPos;
		nCurPos = m_scr.GetScrollPos();	
		switch(nSBCode)
		{
		case SB_LINELEFT:
			if( nCurPos > 0 )
			{
				nCurPos-=2;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_LINERIGHT:
			if( nCurPos < (m_nRecCnt - 1) )
			{
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_PAGELEFT:
			if( nCurPos > 0 )
			{
				nCurPos-=11;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_PAGERIGHT:
			if( nCurPos < (m_nRecCnt - 1) )
			{
				nCurPos+=9;
				if( nCurPos > m_nRecCnt - 1 )
					nCurPos = m_nRecCnt - 1;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_THUMBTRACK:
			if( nPos >= 0 && nPos < (m_nRecCnt - 1) )
			{
				nCurPos = nPos - 1;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		case SB_THUMBPOSITION:
			if( nPos >= 0 && nPos < (m_nRecCnt - 1) )
			{
				nCurPos = nPos - 1;
				if( nCurPos < 0 )
					nCurPos = 0;
				m_pBatch->SetLogPointer(nCurPos);
				DispCurrentMap();
			}
			break;
		default:
			break;
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTMapDlgEx::SetLogInfo(StLogInfo *pInfo)
{
	m_pBatch = pInfo->pBatch;
	m_nStartTime = pInfo->nStartTime;
	m_nOffsetTime = pInfo->nOffsetTime;
	m_pBatch->SetLogPointer(m_nOffsetTime);
}

// start
void CTMapDlgEx::OnBnClickedButton2()
{
	int	nCurRec;

	if( !m_bBatchStop || m_bAutoRun )
		return;

	// 
	nCurRec = m_pBatch->GetCurrentRecord();

	// setup log
	m_pBatch->SetLogPointer(nCurRec);

	// auto run flag on.
	m_bAutoRun = TRUE;
}

// stop
void CTMapDlgEx::OnBnClickedButton3()
{
	if( !m_bBatchStop || !m_bAutoRun )
		return;

	m_bAutoRun = FALSE;
}

void CTMapDlgEx::OnTimer(UINT nIDEvent)
{
	if( m_bBatchStop && m_bAutoRun )
	{
		DispCurrentMap();
	}
	else
		DispCurrentTime();


	CDialog::OnTimer(nIDEvent);
}

void CTMapDlgEx::DispCurrentMap(void)
{
	int		nData[NumTCTap];
	int		nRecCnt, nCurRec;

	SampledData	data;

	nRecCnt = m_pBatch->GetRecordCount();
	nCurRec = m_pBatch->GetCurrentRecord();
	if( nCurRec >= (nRecCnt - 1) )
		return;

	m_pBatch->GetSampledData(&data);
	//
	m_pBatch->SetAverageData(&data, nData);

	SendMessage(WM_MAPDATA, (WPARAM)nCurRec, (LPARAM)nData);

	// disp current time
	DispCurrentTime();
}

void CTMapDlgEx::DispCurrentTime(void)
{
	static time_t	preTime;
	time_t			t;
	int				nCurRec;
	_TCHAR			buf[64];

	// get current time.
	nCurRec = m_pBatch->GetCurrentRecord();
	t = (time_t)(m_nStartTime + nCurRec * m_pBatch->m_header.header.samplePeriod * 0.001);
	if (t != preTime) {
		// make up the display string.
//		_tcscpy(buf, _tctime(&t)),	buf[_tcslen(buf) - 1] = _T('\0');

		// clear displaying area.
		m_textCurTime.SetWindowText(GetTimeStr(t));

		// update current time.
		preTime	= t;
	}
}

LPCTSTR CTMapDlgEx::GetTimeStr(time_t tm)
{
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	tp	= localtime(&tm);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

// selected 'TC Position' menu.
void CTMapDlgEx::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}
