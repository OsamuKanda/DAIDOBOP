// TMapDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TMapDlg.h"
#include ".\tmapdlg.h"

///////////////////////////////////////////////////////////////////////////////
// definition

#define PartitionCount	10
int averageData[NumTCTap][PartitionCount + 1][PartitionCount + 1];
#define MAXTEMP		3000
#define MINTEMP		0

// CTMapDlg ダイアログ

IMPLEMENT_DYNAMIC(CTMapDlg, CDialog)
CTMapDlg::CTMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMapDlg::IDD, pParent)
{
	// initialize member data.
	m_isHideCloseButton = FALSE;
	m_pDlgTcView = NULL;

	// create dialog.
	Create(CTMapDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CTMapDlg::~CTMapDlg()
{
}

//It evades ending by Enter Key.
void CTMapDlg::OnOK(){}

//It evades ending by ESC Key.
void CTMapDlg::OnCancel(){}

void CTMapDlg::DoDataExchange(CDataExchange* pDX)
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
}


BEGIN_MESSAGE_MAP(CTMapDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_MESSAGE(WM_MAPDATA, OnAddone)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTMapDlg, CDialog)
	ON_EVENT(CTMapDlg, IDC_CHARTFIX, 20, PostPaintChartfix, VTS_I2 VTS_I2 VTS_I4 VTS_PI2)
	ON_EVENT(CTMapDlg, IDC_CHARTLOOSE, 20, PostPaintChartloose, VTS_I2 VTS_I2 VTS_I4 VTS_PI2)
END_EVENTSINK_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTMapDlg メッセージ ハンドラ

void CTMapDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CTMapDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// WM_INITDIALOG
BOOL CTMapDlg::OnInitDialog()
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


	return TRUE;
}

// close own window
void CTMapDlg::OnBnClickedButton1()
{
	ShowWindow(SW_HIDE);
}

LRESULT CTMapDlg::OnAddone(WPARAM, LPARAM lParam) 
{
    int l,k,i,j,m;
	int nData[NumTCTap];
	ICfxSeriesPtr pSerie;

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
					pSerie->Yvalue[l*(PartitionCount+1)+j] = averageData[(m-l-1)*TapRaw+k][i][PartitionCount-j] / 10.0f;
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
					pSerie->Yvalue[l*(PartitionCount+1)+j] = averageData[idx][i][j] / 10.0f;
				}
			}
		}
	}
    m_pChartLOOSE->CloseData((CfxCod)(COD_VALUES | COD_SMOOTH));

	return 0;
}

// setAverageData.
void CTMapDlg::SetAverageData(int *data)
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
void CTMapDlg::GetAverageData(int *pData)
{
	int	i, j;

	// 配列変数の初期化
	memset( averageData, 0, sizeof( averageData ) );
	for( i = 0; i < NumTCTap / TapRaw; i++ )
	{
		for( j = 0; j < TapRaw-1; j++)
			CalcAverageData(i * TapRaw + j, pData);
	}
}

// calc Average Data.
void CTMapDlg::CalcAverageData(int f, int *pData)
{
	int j, k, z;
	float a1, a2, a3, q, r, s;

	/*データ点数を設定*/
	z = PartitionCount + 1;

	/*各ポイント値を設定*/
	averageData[f][0][0] = pData[f];				// 中心点
	averageData[f][0][z - 1] = pData[f + 3];		// 右横
	averageData[f][z - 1][0] = pData[f + 1];		// 真下
	averageData[f][z - 1][z - 1] = pData[f + 4];	// 右斜め下

	/*p[f]-p[f + 3]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 3] - pData[f]) / z;

	/*p[f + 1]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 1]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（横方向）のポイント値を設定*/
		averageData[f][0][j] = int(a1 * j + pData[f]);
		averageData[f][z - 1][j] = int(a2 * j + pData[f + 1]);
	}

	/*p[f]-p[f + 1]ポイント間の傾きを計算*/
	a1 = (float)(pData[f + 1] - pData[f]) / z;

	/*p[f + 3]-p[f + 4]ポイント間の傾きを計算*/
	a2 = (float)(pData[f + 4] - pData[f + 3]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*外周（縦方向）のポイント値を設定*/
		averageData[f][j][0] = int(a1 * j + pData[f]);
		averageData[f][j][z - 1] = int(a2 * j + pData[f + 3]);
	}

	/*p[f]-p[f + 4]ポイント間の傾きを計算*/
	a3 = (float)(pData[f + 4] - pData[f]) / z;

	for(j = 1;j < z - 1;j++)
	{
		/*p[f]-p[f + 3]ポイント間の傾きを計算*/
		a1 = (float)(averageData[f][j][z - 1] - averageData[f][j][0]) / z;

		for(k = 1;k < z - 1;k++)
		{
			/*p[f]-p[f + 1]ポイント間の傾きを計算*/
			a2 = (float)(averageData[f][z - 1][k] - averageData[f][0][k]) / z;

			/*p[f]-p[f + 3]ポイント間のポイント値を計算*/
			q = a1 * k + averageData[f][j][0];

			/*p[f]-p[f + 1]ポイント間のポイント値を計算*/
			r = a2 * j + averageData[f][0][k];

			/*対角線上のデータの場合*/
			if (k == j)
			{
				/*p[f]-p[f + 4]ポイント間のポイント値を計算*/
				s = a3 * k + pData[f];

				/*3つのポイント値の平均をポイント値として設定*/
				averageData[f][j][k] = int((q + r + s) / 3);
			}
			/*対角線上以外のデータの場合*/
			else
			{
				/*2つのポイント値の平均をポイント値として設定*/
				averageData[f][j][k] = int((q + r) / 2);
			}
		}
	}
}

// hide close button on/off.
void CTMapDlg::HideCloseButton(BOOL bHide)
{
	m_isHideCloseButton = bHide;
}


void CTMapDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( m_isHideCloseButton )
	{
		m_btnClose.ShowWindow(SW_HIDE);
		m_btnClose.EnableWindow(FALSE);
	}
	if( !bShow )
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

void CTMapDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 描画メッセージで CDialog::OnPaint() を呼び出さないでください。
}

void CTMapDlg::PostPaintChartfix(short w, short h, long lPaint, short* nRes)
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

void CTMapDlg::PostPaintChartloose(short w, short h, long lPaint, short* nRes)
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

// selected 'TC Position' menu.
void CTMapDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}
