// ExeConf.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "ExeConf.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	TcTapSize		15
#define	NNLineSize		7
#define	TPPosMax		1250
#define	TPPosMin		-1250

// reference data
static CConfig			&cnf		= theApp.m_cnfBoexe;
static InputPoint		*inputPoint	= theApp.m_cnfBoexe.inputPoint;
static int				*nTpPosData = theApp.m_cnfBoexe.nTpPosData;
static COLORREF			MoldColor	= RGB(0, 200, 0);
static COLORREF			&Black		= theApp.m_col.clBlack;
static COLORREF			&White		= theApp.m_col.clWhite;
static COLORREF			*ToneColor	= theApp.m_col.clTones;
static TCCombination	*tcCombi	= theApp.m_cnfBoexe.tcCombination;
static int				&numCombi	= theApp.m_cnfBoexe.numCombination;
static BOOL				*pbTcExp	= theApp.m_cnfBoexe.bTcExp;


// CExeConf dialog

IMPLEMENT_DYNAMIC(CExeConf, CDialog)
CExeConf::CExeConf(CWnd* pParent /*=NULL*/)
	: CDialog(CExeConf::IDD, pParent)
{
	// initialize member
	m_nAsnArg		= -1;

	int i;
	for( i = 0; i < NumInput; i++ )
		m_pTcButton[i] = NULL;
}

CExeConf::~CExeConf()
{
}

void CExeConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIX_FRAME, m_frameFIX);
	DDX_Control(pDX, IDC_LOOSE_FRAME, m_frameLOOSE);
	DDX_Control(pDX, IDC_WEST_FRAME, m_frameWEST);
	DDX_Control(pDX, IDC_EAST_FRAME, m_frameEAST);
	DDX_Control(pDX, IDC_FIX_NAME, m_textFIX);
	DDX_Control(pDX, IDC_LOOSE_NAME, m_textLOOSE);
	DDX_Control(pDX, IDC_WEST_NAME, m_textWEST);
	DDX_Control(pDX, IDC_EAST_NAME, m_textEAST);
	DDX_Control(pDX, IDC_MOLD_FRAME, m_frameMOLD);
	DDX_Control(pDX, IDC_MOLD_FIX, m_moldFIX);
	DDX_Control(pDX, IDC_MOLD_LOOSE, m_moldLOOSE);
	DDX_Control(pDX, IDC_MOLD_WEST, m_moldWEST);
	DDX_Control(pDX, IDC_MOLD_EAST, m_moldEAST);
	DDX_Control(pDX, IDC_MOLD_RTNO, m_moldRTNO);
	DDX_Control(pDX, IDC_MOLD_LTNO, m_moldLTNO);
	DDX_Control(pDX, IDC_MOLD_LBNO, m_moldLBNO);
	DDX_Control(pDX, IDC_MOLD_RBNO, m_moldRBNO);
	DDX_Control(pDX, IDC_MOLD_WTNO, m_moldWTNO);
	DDX_Control(pDX, IDC_MOLD_ETNO, m_moldETNO);
	DDX_Control(pDX, IDC_MOLD_EYE, m_moldEye);
	DDX_Control(pDX, IDC_RADIO1, m_tc01);
	DDX_Control(pDX, IDC_RADIO2, m_tc02);
	DDX_Control(pDX, IDC_RADIO3, m_tc03);
	DDX_Control(pDX, IDC_RADIO4, m_tc04);
	DDX_Control(pDX, IDC_RADIO5, m_tc05);
	DDX_Control(pDX, IDC_RADIO6, m_tc06);
	DDX_Control(pDX, IDC_RADIO7, m_tc07);
	DDX_Control(pDX, IDC_RADIO8, m_tc08);
	DDX_Control(pDX, IDC_RADIO9, m_tc09);
	DDX_Control(pDX, IDC_RADIO10, m_tc10);
	DDX_Control(pDX, IDC_RADIO11, m_tc11);
	DDX_Control(pDX, IDC_RADIO12, m_tc12);
	DDX_Control(pDX, IDC_RADIO13, m_tc13);
	DDX_Control(pDX, IDC_RADIO14, m_tc14);
	DDX_Control(pDX, IDC_RADIO15, m_tc15);
	DDX_Control(pDX, IDC_RADIO16, m_tc16);
	DDX_Control(pDX, IDC_RADIO17, m_tc17);
	DDX_Control(pDX, IDC_RADIO18, m_tc18);
	DDX_Control(pDX, IDC_RADIO19, m_tc19);
	DDX_Control(pDX, IDC_RADIO20, m_tc20);
	DDX_Control(pDX, IDC_RADIO21, m_tc21);
	DDX_Control(pDX, IDC_RADIO22, m_tc22);
	DDX_Control(pDX, IDC_RADIO23, m_tc23);
	DDX_Control(pDX, IDC_RADIO24, m_tc24);
	DDX_Control(pDX, IDC_RADIO25, m_tc25);
	DDX_Control(pDX, IDC_RADIO26, m_tc26);
	DDX_Control(pDX, IDC_RADIO27, m_tc27);
	DDX_Control(pDX, IDC_RADIO28, m_tc28);
	DDX_Control(pDX, IDC_RADIO29, m_tc29);
	DDX_Control(pDX, IDC_RADIO30, m_tc30);
	DDX_Control(pDX, IDC_RADIO31, m_tc31);
	DDX_Control(pDX, IDC_RADIO32, m_tc32);
	DDX_Control(pDX, IDC_RADIO33, m_tc33);
	DDX_Control(pDX, IDC_RADIO34, m_tc34);
	DDX_Control(pDX, IDC_RADIO35, m_tc35);
	DDX_Control(pDX, IDC_RADIO36, m_tc36);
	DDX_Control(pDX, IDC_RADIO37, m_tc37);
	DDX_Control(pDX, IDC_RADIO38, m_tc38);
	DDX_Control(pDX, IDC_RADIO39, m_tc39);
	DDX_Control(pDX, IDC_RADIO40, m_tc40);
	DDX_Control(pDX, IDC_RADIO41, m_tc41);
	DDX_Control(pDX, IDC_RADIO42, m_tc42);
	DDX_Control(pDX, IDC_RADIO43, m_tc43);
	DDX_Control(pDX, IDC_RADIO44, m_tc44);
	DDX_Control(pDX, IDC_RADIO45, m_tc45);
	DDX_Control(pDX, IDC_RADIO46, m_tc46);
	DDX_Control(pDX, IDC_RADIO47, m_tc47);
	DDX_Control(pDX, IDC_RADIO48, m_tc48);
	DDX_Control(pDX, IDC_RADIO49, m_tc49);
	DDX_Control(pDX, IDC_RADIO50, m_tc50);
	DDX_Control(pDX, IDC_RADIO51, m_tc51);
	DDX_Control(pDX, IDC_RADIO52, m_tc52);
	DDX_Control(pDX, IDC_RADIO53, m_tc53);
	DDX_Control(pDX, IDC_RADIO54, m_tc54);
	DDX_Control(pDX, IDC_RADIO55, m_tc55);
	DDX_Control(pDX, IDC_RADIO56, m_tc56);
	DDX_Control(pDX, IDC_RADIO57, m_tc57);
	DDX_Control(pDX, IDC_RADIO58, m_tc58);
	DDX_Control(pDX, IDC_RADIO59, m_tc59);
	DDX_Control(pDX, IDC_RADIO60, m_tc60);
	DDX_Control(pDX, IDC_RADIO61, m_tc61);
	DDX_Control(pDX, IDC_RADIO62, m_tc62);
	DDX_Control(pDX, IDC_RADIO63, m_tc63);
	DDX_Control(pDX, IDC_RADIO64, m_tc64);
	DDX_Control(pDX, IDC_RADIO65, m_tc65);
	DDX_Control(pDX, IDC_RADIO66, m_tc66);
	DDX_Control(pDX, IDC_RADIO67, m_tc67);
	DDX_Control(pDX, IDC_RADIO68, m_tc68);
	DDX_Control(pDX, IDC_RADIO69, m_tc69);
	DDX_Control(pDX, IDC_RADIO70, m_tc70);
	DDX_Control(pDX, IDC_RADIO71, m_tc71);
	DDX_Control(pDX, IDC_RADIO72, m_tc72);
	DDX_Control(pDX, IDC_RADIO73A, m_tc73);
	DDX_Control(pDX, IDC_RADIO74A, m_tc74);
	DDX_Control(pDX, IDC_RADIO75A, m_tc75);
	DDX_Control(pDX, IDC_RADIO76A, m_tc76);
	DDX_Control(pDX, IDC_RADIO77A, m_tc77);
	DDX_Control(pDX, IDC_RADIO78A, m_tc78);
	DDX_Control(pDX, IDC_RADIO79A, m_tc79);
	DDX_Control(pDX, IDC_RADIO80A, m_tc80);
	DDX_Control(pDX, IDC_RADIO81A, m_tc81);
	DDX_Control(pDX, IDC_RADIO82A, m_tc82);
	DDX_Control(pDX, IDC_RADIO83A, m_tc83);
	DDX_Control(pDX, IDC_RADIO84A, m_tc84);
	DDX_Control(pDX, IDC_RADIO85A, m_tc85);
	DDX_Control(pDX, IDC_RADIO86A, m_tc86);
	DDX_Control(pDX, IDC_RADIO87A, m_tc87);
	DDX_Control(pDX, IDC_RADIO88A, m_tc88);
	DDX_Control(pDX, IDC_RADIO89A, m_tc89);
	DDX_Control(pDX, IDC_RADIO90A, m_tc90);
	DDX_Control(pDX, IDC_RADIO91A, m_tc91);
	DDX_Control(pDX, IDC_RADIO92A, m_tc92);
	DDX_Control(pDX, IDC_RADIO93A, m_tc93);
	DDX_Control(pDX, IDC_RADIO94A, m_tc94);
	DDX_Control(pDX, IDC_RADIO95A, m_tc95);
	DDX_Control(pDX, IDC_RADIO96A, m_tc96);
	DDX_Control(pDX, IDC_CONF_NAME, m_textConfName);
	DDX_Control(pDX, IDC_EDIT1, m_tp01);
	DDX_Control(pDX, IDC_EDIT2, m_tp02);
	DDX_Control(pDX, IDC_EDIT3, m_tp03);
	DDX_Control(pDX, IDC_EDIT4, m_tp04);
	DDX_Control(pDX, IDC_EDIT5, m_tp05);
	DDX_Control(pDX, IDC_EDIT6, m_tp06);
	DDX_Control(pDX, IDC_EDIT7, m_tp07);
	DDX_Control(pDX, IDC_EDIT8, m_tp08);
	DDX_Control(pDX, IDC_EDIT9, m_tp09);
	DDX_Control(pDX, IDC_EDIT10, m_tp10);
	DDX_Control(pDX, IDC_EDIT11, m_tp11);
	DDX_Control(pDX, IDC_EDIT12, m_tp12);
	DDX_Control(pDX, IDC_EDIT13, m_tp13);
	DDX_Control(pDX, IDC_EDIT14, m_tp14);
	DDX_Control(pDX, IDC_EDIT15, m_tp15);
	DDX_Control(pDX, IDC_EDIT16, m_tp16);
	DDX_Control(pDX, IDC_EDIT17, m_tp17);
	DDX_Control(pDX, IDC_EDIT18, m_tp18);
	DDX_Control(pDX, IDC_EDIT19, m_tp19);
	DDX_Control(pDX, IDC_EDIT20, m_tp20);
	DDX_Control(pDX, IDC_EDIT21, m_tp21);
	DDX_Control(pDX, IDC_EDIT22, m_tp22);
	DDX_Control(pDX, IDC_EDIT23, m_tp23);
	DDX_Control(pDX, IDC_EDIT24, m_tp24);
	DDX_Control(pDX, IDC_EDIT25A, m_tp25);
	DDX_Control(pDX, IDC_EDIT26A, m_tp26);
	DDX_Control(pDX, IDC_EDIT27A, m_tp27);
	DDX_Control(pDX, IDC_EDIT28A, m_tp28);
	DDX_Control(pDX, IDC_EDIT29A, m_tp29);
	DDX_Control(pDX, IDC_EDIT30A, m_tp30);
	DDX_Control(pDX, IDC_EDIT31A, m_tp31);
	DDX_Control(pDX, IDC_EDIT32A, m_tp32);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
}


BEGIN_MESSAGE_MAP(CExeConf, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButton1)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CExeConf message handler

// paint screen
void CExeConf::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawCanvas(dc);
}

// It is only 1 execute when the dialog opens.
BOOL CExeConf::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup input point for screen
	SetupInputPoints();

	// setup TC pos for screen
	SetupTCPos();

	// disp work directry name
	m_textConfName.SetWindowText(cnf.GetWorkDirName());

	return TRUE;
}

// clicked double left mouse button.
void CExeConf::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
	int x, y;

	// search A/D assign point and if found it, 
	if (GetADAssignPoint(point.x, point.y, &m_nAsnArg, &x, &y) == TRUE && 
		pbTcExp[m_nAsnArg] == FALSE && 
		inputPoint[m_nAsnArg].adCh != -1) 
	{
		// change color to white. 
		DrawADAssignPoint(dc, White, x, y, TcTapSize);

		// pop up A/D assignment Dialog Box.
		// ここで割付を表面上外す
		if( IDOK == AfxMessageBox(IDS_EXE_CNF01, MB_OKCANCEL) )
		{
			// save tc exception info.
			pbTcExp[m_nAsnArg] = TRUE;
			cnf.WriteTcExpFile();
		}
		m_nAsnArg = -1;
		Invalidate(TRUE);
	}
	else 
	{
		m_nAsnArg = -1;
		// Beep. 
		MessageBeep(0xFFFFFFFF);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

// pushed close button.
void CExeConf::OnClickedButton1()
{
	EndDialog(IDOK);
}

// clicked recover button.
void CExeConf::OnBnClickedButton2()
{
	int		i;

	// recover confirmation.
	if( IDNO == AfxMessageBox(IDS_EXE_CNF02, MB_YESNO) )
		return;

	// clear tc exception.
	for( i = 0; i < NumInput; i++ )
		pbTcExp[i] = FALSE;

	// save tc exception info.
	cnf.WriteTcExpFile();

	// redraw screen.
	Invalidate(TRUE);
}


///////////////////////////////////////////////////////////////////////////////
// CConfigDlg user function

// draw canvas.
void CExeConf::DrawCanvas(CDC &dc)
{
	// draw mold surface. 
	DrawMold(dc);

	// draw mold model. 
	DrawMoldModel(dc);

	// draw thermo-couple combination. 
	DrawTCCombination(dc);

	// draw TC Taps. 
	DrawInputPoints(dc);
}

// draw mold.
void CExeConf::DrawMold(CDC &dc)
{
	CStatic*	sf[] = {&m_frameFIX,&m_frameLOOSE,&m_frameWEST,&m_frameEAST,NULL};
	CStatic*	sn[] = {&m_textFIX, &m_textLOOSE, &m_textWEST, &m_textEAST, NULL};
	CBrush		brBox(Black);
	CString		str;
	CRect		rc;
	int			i;

	i = 0;
	while( sf[i] != NULL && sn[i] != NULL )
	{
		// draw Mold surface
		sf[i]->GetClientRect(rc);
		sf[i]->MapWindowPoints(this, rc);
		dc.FillSolidRect(rc, MoldColor);
		dc.FrameRect(rc, &brBox);

		// draw 'Name'
		sn[i]->GetClientRect(rc);
		sn[i]->MapWindowPoints(this, rc);
		sn[i]->GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, rc, DT_CENTER);
		i++;
	}
}

// draw mold model.
void CExeConf::DrawMoldModel(CDC &dc)
{
	CStatic*	sf[] = {&m_frameMOLD,NULL};
	CStatic*	sn[] = {&m_moldFIX, &m_moldLOOSE, &m_moldWEST, &m_moldEAST, 
						&m_moldRTNO, &m_moldLTNO, &m_moldLBNO, &m_moldRBNO, 
						&m_moldWTNO, &m_moldETNO, &m_moldEye, NULL};
	CBrush		brBox(Black);
	CString		str;
	CRect		rc;
	CPoint		line;
	int			i;

	i = 0;
	while( sf[i] != NULL )
	{
		// draw Mold surface
		sf[i]->GetClientRect(rc);
		sf[i]->MapWindowPoints(this, rc);
		dc.FillSolidRect(rc, MoldColor);
		dc.FrameRect(rc, &brBox);
		i++;
	}

	i = 0;
	while( sn[i] != NULL )
	{
		// draw 'Name'
		sn[i]->GetClientRect(rc);
		sn[i]->MapWindowPoints(this, rc);
		sn[i]->GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, rc, DT_CENTER);
		i++;
	}

	// draw eye line
	m_moldEye.GetClientRect(rc);
	m_moldEye.MapWindowPoints(this, rc);
	line.x = rc.left + rc.Width()/2;
	line.y = rc.top+20;
	dc.MoveTo(line);
	line.y += 60;	// arrow length
	dc.LineTo(line);
	dc.LineTo(line.x+5, line.y-5);
	dc.LineTo(line);
	dc.LineTo(line.x-5, line.y-5);
}

// draw tc combination.
void CExeConf::DrawTCCombination(CDC &dc)
{
	int	i;

	// for each thermo couple combination, 
	for (i = 0; i < numCombi; i++) {
		// draw combination line. 
		DrawTCCombiLine(dc, &tcCombi[i]);
	}
}

// draw combination line.
void CExeConf::DrawTCCombiLine(CDC &dc, TCCombination *cp)
{
	int	color	= ToneColor[4 * cp->netID];
	int	sx		= inputPoint[cp->upper].gx;
	int	sy		= inputPoint[cp->upper].gy;
	int	ex		= inputPoint[cp->lower].gx;
	int	ey		= inputPoint[cp->lower].gy;

	// if link next of lower neighbor. 
	if (cp->lower == cp->upper + 2) 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black,	sx,   sy, sx-15, sy+5);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx-15, sy+5, ex-15, ey-5);
		DrawLine(dc, PS_SOLID, NNLineSize,	 Black, ex-15, ey-5,    ex,   ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color,    sx,   sy, sx-15, sy+5);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx-15, sy+5, ex-15, ey-5);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, ex-15, ey-5,    ex,   ey);
	}
	// if link next of left neighbor. 
	else if (cp->lower == cp->upper - 6) 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black,   sx,    sy, sx-5, sy-15);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx-5, sy-15, ex+5, ey-15);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, ex+5, ey-15,   ex,    ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color,   sx,    sy, sx-5, sy-15);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx-5, sy-15, ex+5, ey-15);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, ex+5, ey-15,   ex,    ey);
	}
	// the other case, 
	else 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx, sy, ex, ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx, sy, ex, ey);
	}
}

// draw line.
void CExeConf::DrawLine(CDC &dc, int ps, int width, COLORREF col, int x1, int y1, int x2, int y2)
{
	CPen	pen(ps, width, col);
	CPen	*pPen;

	pPen = dc.SelectObject(&pen);
	dc.MoveTo(x1, y1);
	dc.LineTo(x2, y2);
	dc.SelectObject(pPen);
}

// draw input points.
void CExeConf::DrawInputPoints(CDC &dc)
{
	COLORREF	color;
	CRect		rc;
	CString		str;
	TCHAR		*pBuf;
	int			i;

	// for each TC Taps.
	for (i = 0; i < NumInput; i++)
	{
		// skip if disable.
		if (inputPoint[i].gx < 0 || inputPoint[i].gy < 0) continue;

		// get display color.
		if (m_nAsnArg == i)
			color	= White;
		else if (inputPoint[i].adCh >= 0 && pbTcExp[i] == FALSE)
			color	= ToneColor[inputPoint[i].adCh];
		else
			color	= Black;

		// draw A/D assign point. 
		DrawADAssignPoint(dc, color, inputPoint[i].gx, inputPoint[i].gy, TcTapSize);

		// if now drawing TCTap
		if (i < NumTCTap) 
		{
			// draw TC Tap name.
			if( m_pTcButton[i] != NULL )
			{
				m_pTcButton[i]->GetWindowText(str);
				pBuf = str.GetBuffer(256);
				PrintText(dc, NULL, Black,
					inputPoint[i].gx + 3, inputPoint[i].gy - 25, pBuf);
				str.ReleaseBuffer(256);
			}
		}
	}
}

// draw assign point.
void CExeConf::DrawADAssignPoint(CDC &dc, COLORREF col, int x, int y, int size)
{
	CPen		pen, *ppen;
	CBrush		brush, *pbrush;

	pen.CreatePen(PS_SOLID, 1, Black);
	brush.CreateSolidBrush(col);
	ppen = dc.SelectObject(&pen);
	pbrush = dc.SelectObject(&brush);
	dc.Ellipse(	x-size/2, y-size/2, 
				x+size/2, y+size/2);
	dc.SelectObject(ppen);
	dc.SelectObject(pbrush);
	pen.DeleteObject();
	brush.DeleteObject();
}

// initialize input points for screen.
void CExeConf::SetupInputPoints(void)
{
	CButton*	bn[] = {
					&m_tc01,&m_tc02,&m_tc03,&m_tc04,&m_tc05,&m_tc06,&m_tc07,&m_tc08,
					&m_tc09,&m_tc10,&m_tc11,&m_tc12,&m_tc13,&m_tc14,&m_tc15,&m_tc16,
					&m_tc17,&m_tc18,&m_tc19,&m_tc20,&m_tc21,&m_tc22,&m_tc23,&m_tc24,
					&m_tc25,&m_tc26,&m_tc27,&m_tc28,&m_tc29,&m_tc30,&m_tc31,&m_tc32,
					&m_tc33,&m_tc34,&m_tc35,&m_tc36,&m_tc37,&m_tc38,&m_tc39,&m_tc40,
					&m_tc41,&m_tc42,&m_tc43,&m_tc44,&m_tc45,&m_tc46,&m_tc47,&m_tc48,
					&m_tc49,&m_tc50,&m_tc51,&m_tc52,&m_tc53,&m_tc54,&m_tc55,&m_tc56,
					&m_tc57,&m_tc58,&m_tc59,&m_tc60,&m_tc61,&m_tc62,&m_tc63,&m_tc64,
					&m_tc65,&m_tc66,&m_tc67,&m_tc68,&m_tc69,&m_tc70,&m_tc71,&m_tc72,
					&m_tc73,&m_tc74,&m_tc75,&m_tc76,&m_tc77,&m_tc78,&m_tc79,&m_tc80,
					&m_tc81,&m_tc82,&m_tc83,&m_tc84,&m_tc85,&m_tc86,&m_tc87,&m_tc88,
					&m_tc89,&m_tc90,&m_tc91,&m_tc92,&m_tc93,&m_tc94,&m_tc95,&m_tc96,
				NULL};
	CRect	rc;
	int i, idx;

	idx = 0;
	// input data fix tc
	for( i = 0; i < NumFixTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = FixSide;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data loose tc
	for( i = 0; i < NumLooseTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = LooseSide;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data narrow west tc
	for( i = 0; i < NumWestTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = NarrowWest;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data narrow east tc
	for( i = 0; i < NumEastTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = NarrowEast;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
}

// initialize TC pos for screen.
void CExeConf::SetupTCPos(void)
{
	CEdit*	ed[] = {
					&m_tp01,&m_tp02,&m_tp03,&m_tp04,&m_tp05,&m_tp06,&m_tp07,&m_tp08,
					&m_tp09,&m_tp10,&m_tp11,&m_tp12,&m_tp13,&m_tp14,&m_tp15,&m_tp16,
					&m_tp17,&m_tp18,&m_tp19,&m_tp20,&m_tp21,&m_tp22,&m_tp23,&m_tp24,
					&m_tp25,&m_tp26,&m_tp27,&m_tp28,&m_tp29,&m_tp30,&m_tp31,&m_tp32,
				NULL};
	CString	str;
	int idx;

	// input tc pos.
	for( idx = 0; ed[idx] != NULL; idx++ )
	{
		str.Format("%d", nTpPosData[idx]);
		ed[idx]->SetWindowText(str);
	}
}

// store TC pos for screen.
BOOL CExeConf::StoreTCPos(void)
{
	CEdit*	ed[] = {
					&m_tp01,&m_tp02,&m_tp03,&m_tp04,&m_tp05,&m_tp06,&m_tp07,&m_tp08,
					&m_tp09,&m_tp10,&m_tp11,&m_tp12,&m_tp13,&m_tp14,&m_tp15,&m_tp16,
					&m_tp17,&m_tp18,&m_tp19,&m_tp20,&m_tp21,&m_tp22,&m_tp23,&m_tp24,
					&m_tp25,&m_tp26,&m_tp27,&m_tp28,&m_tp29,&m_tp30,&m_tp31,&m_tp32,
				NULL};
	CString	str;
	int idx, data;

	// output tc pos.
	for( idx = 0; ed[idx] != NULL; idx++ )
	{
		ed[idx]->GetWindowText(str);
		data = _tstoi(str);
		nTpPosData[idx] = data;
		if ( TPPosMin > data || data > TPPosMax || _tcsspn( str, _T("0123456789-") ) != _tcslen( str ) )
			return FALSE;
	}

	return TRUE;
}

// search A/D assign point.
BOOL CExeConf::GetADAssignPoint(int x, int y, int *arg, int *xp, int *yp)
{
	register int	i;

	// for each input point, 
	for (i = 0; i < NumInput; i++) {
		// skip if disable. 
		if (inputPoint[i].gx < 0 || inputPoint[i].gy < 0)		continue;

		// if clicked point is in neighbor of A/D AssignPoint, 
		if (x >= inputPoint[i].gx - (TcTapSize/2) && x <= inputPoint[i].gx + (TcTapSize/2) &&
			y >= inputPoint[i].gy - (TcTapSize/2) && y <= inputPoint[i].gy + (TcTapSize/2)) {
			// set argument. 
			*arg	= i;
			// set center position. 
			*xp		= inputPoint[i].gx,		*yp		= inputPoint[i].gy;
			// return OK. 
			return	TRUE;
		}
	}
	return	FALSE;
}

