// AdTable.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "AdTable.h"
#include "../include/graphic.h"

static TcName	*tcnm		= &theApp.m_stTcNm;


///////////////////////////////////////////////////////////////////////////////
// definition

#define	colWidth	18
#define	chWidth		25

#define	ClosePlace	40
#define	TableWidth	(2 * xOffset + 4 * (colWidth + chWidth + argWidth))
#define	TableHeight	(2 * yOffset + 17 * rowHeight + ClosePlace)

#define	CloseWidth	80
#define CloseHight	25
#define	xClosePos	(TableWidth - CloseWidth - 20)
#define yClosePos	(TableHeight - CloseHight - 30)

// reference data
static ADChannel	*adChannel	= theApp.m_cnf.adChannel;
static COLORREF		Black		= RGB(0,0,0);
static COLORREF		*ToneColor	= theApp.m_col.clTones;


///////////////////////////////////////////////////////////////////////////////
// CAdTable ダイアログ

IMPLEMENT_DYNAMIC(CAdTable, CDialog)
CAdTable::CAdTable(CWnd* pParent /*=NULL*/)
	: CDialog(CAdTable::IDD, pParent)
{
	Create(CAdTable::IDD, pParent);				
}

CAdTable::~CAdTable()
{
}

void CAdTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AD_FRAME, m_frameAD);
}


BEGIN_MESSAGE_MAP(CAdTable, CDialog)
	ON_BN_CLICKED(IDC_CLOSE, OnClickedClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CAdTable メッセージ ハンドラ

void CAdTable::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

void CAdTable::OnClickedClose()
{
	ShowWindow(SW_HIDE);
}

void CAdTable::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawADAssign(dc);
}

void CAdTable::DrawADAssign(CDC &dc)
{
	int		i;
	int		xOffset, yOffset, argWidth, rowHeight;
	CRect	rc;
	CBrush	brBox(Black);

	m_frameAD.GetClientRect(rc);
	m_frameAD.MapWindowPoints(this, rc);
	xOffset = rc.left;
	yOffset	= rc.top;
	argWidth = rc.Width()/4 - colWidth - chWidth;
	rowHeight = rc.Height()/(NumADChannelDisp / 4);

	// for each A/D channel,
	for (i = 0; i < NumADChannelDisp; i++) {

		// calculate geometry of this column.
		rc.left	= (i / (NumADChannelDisp / 4)) * (colWidth+chWidth+argWidth) + xOffset;
		rc.top	= rowHeight * (i % (NumADChannelDisp / 4)) + yOffset;
		rc.right = rc.left + colWidth;
		rc.bottom = rc.top + rowHeight;

		// draw input channel color.
		dc.FillSolidRect(rc, ToneColor[i]);
		dc.FrameRect(rc, &brBox);

		// draw input channel number.
		rc.left += colWidth;
		rc.right += chWidth;
		dc.FrameRect(rc, &brBox);
		PrintText(dc, NULL, Black, rc.left + 3, rc.top, "%2d", i + 1);

		// draw assigned argument.
		rc.left += chWidth;
		rc.right += argWidth;
		dc.FrameRect(rc, &brBox);
		PrintText(dc, NULL, Black, rc.left + 5, rc.top,
			GetADAssignedName(i));
	}
}

// set up the string that shows assigned argument of specified channel.
LPCTSTR CAdTable::GetADAssignedName(int ch)
{
	static _TCHAR	buf[64];
	CString			str;
	int				arg;

	// if the assigned argument is thermo-couple tap,
	if ((arg = adChannel[ch].arg) >= 0 && arg < NumTCTap) {
		// make up thermo-couple tap name.
		_stprintf(buf, _T("%s"), tcnm->name[arg]);
	}
	// if the assigned argument is meniscas level.
	else if (arg == SNID) {
		// make up meniscas SN name.
		str.LoadString(IDS_DEF_SN);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	// if the assigned argument is meniscas level.
	else if (arg == LevelID) {
		// make up meniscas level name.
		str.LoadString(IDS_DEF_LEVEL);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	// if the assigned argument is casting verocity. 
	else if (arg == VcID) {
		// make up casting verocity name. 
		str.LoadString(IDS_DEF_VC);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	//// if the assigned argument is mold wtdth.
	//else if (arg == WidthID) {
	//	// make up mold wtdth name. */
	//	str.LoadString(IDS_DEF_WIDTH);
	//	_stprintf(buf, _T("%s"), (LPCTSTR)str);
	//}
	//// if the assigned argument is mold wtdth.
	//else if (arg == WidthLID) {
	//	// make up mold wtdth name. */
	//	str.LoadString(IDS_DEF_WIDTH);
	//	str += _T("L");
	//	_stprintf(buf, _T("%s"), (LPCTSTR)str);
	//}
	//// if the assigned argument is mold wtdth.
	//else if (arg == WidthRID) {
	//	// make up mold wtdth name. */
	//	str.LoadString(IDS_DEF_WIDTH);
	//	str += _T("R");
	//	_stprintf(buf, _T("%s"), (LPCTSTR)str);
	//}
	// if the assigned argument is mold wtdth.
	else if (arg == LengthID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_LENGTH);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	//////////////////////////////////////////////////////////////////////////////
	// CSV保存のみの項目
	else if (arg == TdWeiID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_TDW);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	else if (arg == MdWtAID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_MDWTA);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	else if (arg == MdWtCID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_MDWTC);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	else if (arg == MdWtDID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_MDWTD);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	else if (arg == MdTmpDID) {
		// make up mold wtdth name. */
		str.LoadString(IDS_DEF_MDTMPD);
		_stprintf(buf, _T("%s"), (LPCTSTR)str);
	}
	// another,
	else {
		// make up nothing assigned name.
		_tcscpy(buf, _T(" - "));
	}

	return	buf;
}
