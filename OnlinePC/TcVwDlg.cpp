// TcVwDlg.cpp : TC View Screen source file
//
#include "stdafx.h"					// MFC�w�b�_
#include "OnlinePC.h"
#include "TcVwDlg.h"				// �N���X�w�b�_

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// definition
static TcName			*tcnm		= &theApp.m_stTcNm;


// �f�v���b�V�����\���L��
//#define	DISP_DEP

//----------------
// �萔
//----------------

//#define SermoHigh		370
//#define SermoWidth	280
#define FixTapColumn	3
#define LooseTapColumn	3
#define BTapColumn		3
#define DTapColumn		3
//#define SermoHigh		(370*2)
//#define SermoWidth		480
#define SermoHigh		(500*2)
#define SermoWidth		640
#define ATapNum			8
#define LooseTapColumn	3
#define N1TapColumn		3
#define DTapNum			6

#define TapRaw			3
#define SermoSize		40
#define FixWidth		( SermoSize * (FixTapColumn + 1) ) 
#define LooseWidth		( SermoSize * (LooseTapColumn + 1) ) 
#define BWidth			( SermoSize * (BTapColumn + 1) + 3 ) 
#define DWidth			( SermoSize * (DTapColumn + 1) + 3 )
#define FixHigh			( SermoSize * (TapRaw + 1) + 25 )
#define	SpaceWidth		10
#define LooseHigh		FixHigh
#define BHigh			FixHigh
#define DHigh			FixHigh
#define BaseX			30
#define BaseY			50
#define FixBaseX		(BBaseX+BWidth+SpaceWidth)//150
#define FixBaseY		50
#define BBaseX			30
#define BBaseY			130
#define LooseBaseX		(BBaseX+BWidth+SpaceWidth)//150
#define LooseBaseY		210
#define DBaseX			(FixBaseX+FixWidth+SpaceWidth)
#define DBaseY			130
#define DepOffset		350

#define PalleteColumn	4
#define PalleteRaw		4
#define PalleteBaseX	10
#define PalleteBaseY	35
#define PalleteWidth	PalleteColumn * SermoSize + 2
#define PalleteHigh		PalleteRaw * SermoSize + 2

// reference data
static CConfig			&cnf		= theApp.m_cnfBoexe;
static CConfig			&cnfD		= theApp.m_cnfBoexeD;
static COLORREF			&msgBase	= theApp.m_col.clMsgBase;
static COLORREF			&moldColor	= theApp.m_col.clMoldColor;
static COLORREF			Black		= RGB(0,0,0);
static COLORREF			White		= RGB(255,255,255);
static COLORREF			*colorTone	= theApp.m_col.clTones;

// TC�ʒu�L���G���A
CPoint	g_pt[NumTCTap];

/////////////////////////////////////////////////////////////////////////////
// CTcVwDlg�N���X

//-----------------------------------------------------------------------------
// ���O	: CTcVwDlg
// ���e	: �\�z�i�R���X�g���N�^�j
//		: �P�DAFX_DATA_INIT�̃R�[�h��MFC�����������Ɏg�p����̂ŐG��Ȃ��B
//		: �Q�D�����o�ϐ��̏������������ōs���B
// ����	: pParent	... �e��ʃ|�C���^
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
CTcVwDlg::CTcVwDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcVwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcVwDlg)
	//}}AFX_DATA_INIT

	// �����o�̏�����
	m_bFirst = FALSE;

	// create dialog.
	Create(CTcVwDlg::IDD, pParent);				
}

//-----------------------------------------------------------------------------
// ���O	: DoDataExchange
// ���e	: ��ʃf�[�^�̌����ƗL�����`�F�b�N
//		: MFC�������ŃR�[�h���ς���ׁA�قƂ�ǐG��Ȃ��B
// ����	: �c
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
void CTcVwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcVwDlg)
	//}}AFX_DATA_MAP
}

//-----------------------------------------------------------------------------
// ���O	: ���b�Z�[�W�}�b�v
// ���e	: ���b�Z�[�W�n���h���̓o�^
//		: MFC�������ŃR�[�h���ς���ׁA�قƂ�ǐG��Ȃ��B
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTcVwDlg, CDialog)
	//{{AFX_MSG_MAP(CTcVwDlg)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcVwDlg�N���X ���b�Z�[�W �n���h��

//-----------------------------------------------------------------------------
// ���O	: OnOK�AOnCancel�AOnClose
// ���e	: ���[�h���X��ʒ�^�R�[�h
//		: OnOK		... ���^�[���L�[�ŉ�ʂ�����̂𖳌��ɂ���
//		: OnCancel	... �d�r�b�L�[�ŉ�ʂ�����̂𖳌��ɂ���
//		: OnClose	... �~�{�^���AAlt+F4�ŉ�ʂ�����̂𖳌��ɂ���
// ����	: �Ȃ�
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
void CTcVwDlg::OnOK() {}
void CTcVwDlg::OnCancel() {}
void CTcVwDlg::OnClose() 
{
	// ��ʔ�\��
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
// ���O	: PostNcDestroy
// ���e	: ��ʂ��j�����ꂽ��ɌĂ΂��B�����I�ɔj�����Ȃ��ꍇ�́A
//		: �e���j�����ꂽ���Ɏ����Ŕj�������B
//		: �P�D���������I�u�W�F�N�g���̍폜
//		: �Q�D�{�N���X��new�Ő�������Ă���ׁAdelete�Ŕj������B
//		:	�@���݂ɔj������͉̂�ʂł͂Ȃ��N���X�̎��́i�C���X�^���X�j
// ����	: �Ȃ�
// �ߒl	: �Ȃ�
//-----------------------------------------------------------------------------
void CTcVwDlg::PostNcDestroy() 
{
	// �C���X�^���X�J��
	delete this;
}

//-----------------------------------------------------------------------------
// ���O	: OnInitDialog
// ���e	: ��ʂ��j�����ꂽ��ɌĂ΂��B�����I�ɔj�����Ȃ��ꍇ�́A
//		: �e���j�����ꂽ���Ɏ����Ŕj�������B
//		: �P�D�{�N���X��new�Ő�������Ă���ׁAdelete�Ŕj������B
//		:	�@���݂ɔj������͉̂�ʂł͂Ȃ��N���X�̎��́i�C���X�^���X�j
// ����	: �Ȃ�
// �ߒl	: ����FTRUE�A�ُ�FFALSE
//-----------------------------------------------------------------------------
BOOL CTcVwDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TC�\���ʒu�̐ݒ�
	g_pt[0]		= CPoint(DBaseX + 0 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[1]		= CPoint(DBaseX + 0 * SermoSize,	DBaseY + 1 * SermoSize);
	g_pt[2]		= CPoint(DBaseX + 1 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[3]		= CPoint(DBaseX + 1 * SermoSize,	DBaseY + 1 * SermoSize);
	g_pt[4]		= CPoint(DBaseX + 2 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[5]		= CPoint(DBaseX + 2 * SermoSize,	DBaseY + 1 * SermoSize);

	g_pt[6]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[7]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[8]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 2 * SermoSize);
	g_pt[9]		= CPoint(FixBaseX + 1 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[10]	= CPoint(FixBaseX + 1 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[11]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[12]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[13]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 2 * SermoSize);

	g_pt[14]	= CPoint(BBaseX + 0 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[15]	= CPoint(BBaseX + 0 * SermoSize,	BBaseY + 1 * SermoSize);
	g_pt[16]	= CPoint(BBaseX + 1 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[17]	= CPoint(BBaseX + 1 * SermoSize,	BBaseY + 1 * SermoSize);
	g_pt[18]	= CPoint(BBaseX + 2 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[19]	= CPoint(BBaseX + 2 * SermoSize,	BBaseY + 1 * SermoSize);

	g_pt[20]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[21]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[22]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 2 * SermoSize);
	g_pt[23]	= CPoint(LooseBaseX + 1 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[24]	= CPoint(LooseBaseX + 1 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[25]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[26]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[27]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 2 * SermoSize);

	return TRUE;
}

void CTcVwDlg::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	CRect	rect;

	if( !m_bFirst )
	{
		createTcViewFrame(dc);
		m_bFirst = TRUE;
	}
	
	CBitmap	*old;
	old = (CBitmap *)m_mdc.SelectObject(&m_bitmap);

	drawSermo(m_mdc);

	CRect	rc;
	GetClientRect(&rc);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&m_mdc,0,0,SRCCOPY);
	m_mdc.SelectObject(old);
}

///////////////////////////////////////////////////////////////////////////////
// ���[�U�[�ǉ��֐�

// Tc View�\���t���[���𐶐�����B
void CTcVwDlg::createTcViewFrame(CDC &dc)
{
	CRect	rc;
	// clear memory plane.
	GetClientRect(rc);

	// create font for Point Info
	m_fontPointInfo.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS,	_T("MS Shell Dlg") );

	// create bitmap
	m_mdc.CreateCompatibleDC(&dc);
	m_bitmap.CreateCompatibleBitmap(&dc, SermoWidth, SermoHigh);
}

// 
void CTcVwDlg::drawSermo(CDC& dc)
{
	int	getTempColor(int);
	SharedMemory *pShMem = GetSharedMemory();
	int	x, y, ch, nNumber;
	int	i;
	CFont *pFont;
	CRect rc;

	// erase all entry for BO.
	boxFillSolid(dc, msgBase, 0, 0, SermoWidth+50, SermoHigh+50);
#ifdef DISP_DEP
	PrintText(dc, 0, RGB(0,0,0), 10, 10, "�a�D�n�D��");
#endif
	boxFillSolid(dc, moldColor, BBaseX - 15, BBaseY - 30, BWidth, BHigh);
	PrintText(dc, 0, RGB(0,0,0), BBaseX + (BWidth / 2) - 20, BBaseY - 24, "B");
	boxFillSolid(dc, moldColor, FixBaseX - 15, FixBaseY - 30, FixWidth, FixHigh);
	PrintText(dc, 0, RGB(0,0,0), FixBaseX + (FixWidth / 2) - 20, FixBaseY - 24, "C");
	boxFillSolid(dc, moldColor, DBaseX - 15, DBaseY - 30, DWidth, DHigh);
	PrintText(dc, 0, RGB(0,0,0), DBaseX + (DWidth / 2) - 20, DBaseY - 24, "D");
	boxFillSolid(dc, moldColor, LooseBaseX - 15, LooseBaseY -30, LooseWidth, LooseHigh);
	PrintText(dc, 0, RGB(0,0,0), LooseBaseX + (LooseWidth / 2) - 20, LooseBaseY -30 + 5, "A");

#ifdef DISP_DEP
	// erase all entry for DEP.
	boxFillSolid(dc, msgBase, 0, DepOffset, SermoWidth+50, SermoHigh+50);
	PrintText(dc, 0, RGB(0,0,0), 10, DepOffset + 10, "�f�v���b�V������");
	boxFillSolid(dc, moldColor, BBaseX - 15, DepOffset + BBaseY - 30, BWidth, BHigh);
	PrintText(dc, 0, RGB(0,0,0), BBaseX + (BWidth / 2) - 20, DepOffset + BBaseY - 24, "B");
	boxFillSolid(dc, moldColor, FixBaseX - 15, DepOffset + FixBaseY - 30, FixWidth, FixHigh);
	PrintText(dc, 0, RGB(0,0,0), FixBaseX + (FixWidth / 2) - 20, DepOffset + FixBaseY - 24, "C");
	boxFillSolid(dc, moldColor, DBaseX - 15, DepOffset + DBaseY - 30, DWidth, DHigh);
	PrintText(dc, 0, RGB(0,0,0), DBaseX + (DWidth / 2) - 20, DepOffset + DBaseY - 24, "D");
	boxFillSolid(dc, moldColor, LooseBaseX - 15, DepOffset + LooseBaseY -30, LooseWidth, LooseHigh);
	PrintText(dc, 0, RGB(0,0,0), LooseBaseX + (LooseWidth / 2) - 20, DepOffset + LooseBaseY -30 + 5, "A");
#endif

	nNumber = 1;

	// B.O.���@
	pFont = (CFont *)dc.SelectObject(&m_fontPointInfo);
	for(i = 0;i < NumTCTap;i++)
	{
		x = g_pt[nNumber-1].x;
		y = g_pt[nNumber-1].y;
		ch = cnf.inputPoint[nNumber - 1].adCh;
		if( ch > -1 )
			boxFillSolid(dc, colorTone[ch], x, y, SermoSize, SermoSize);
		else
			boxFillSolid(dc, White, x, y, SermoSize, SermoSize);
		box(dc, PS_SOLID, 1, Black, R2_COPYPEN, x, y, SermoSize, SermoSize);
//		PrintText(dc, &m_fontPointInfo, Black, x + 3, y + 6, "%s", tcnm->name[nNumber-1]);
		rc.left		= x;
		rc.top		= y;
		rc.right	= x + SermoSize;
		rc.bottom	= y + SermoSize;
		dc.DrawText(tcnm->name[nNumber-1], TCNM_MAX_LEN, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		nNumber++;
	}
	dc.SelectObject(pFont);

#ifdef DISP_DEP
	nNumber = 1;

	// ����گ��ݑ��@
	for(i = 0;i < NumTCTap;i++)
	{
		x = g_pt[nNumber-1].x;
		y = g_pt[nNumber-1].y + DepOffset;
		ch = cnfD.inputPoint[nNumber - 1].adCh;
		if( ch > -1 )
			boxFillSolid(dc, colorTone[ch], x, y, SermoSize, SermoSize);
		else
			boxFillSolid(dc, White, x, y, SermoSize, SermoSize);
		box(dc, PS_SOLID, 1, Black, R2_COPYPEN, x, y, SermoSize, SermoSize);
		PrintText(dc, 0, Black, x + 3, y + 6, "#%2d", nNumber);
		nNumber++;
	}
#endif
}


