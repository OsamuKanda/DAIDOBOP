// NnTable.cpp : 実装ファイル
//
#include "stdafx.h"
#include "OnlinePC.h"
#include "NnTable.h"
#include "../include/graphic.h"


///////////////////////////////////////////////////////////////////////////////
// definition

// The Neural Net Table Window definition
#define	colWidth		18
#define	idWidth			25
#define	ClosePlace		60
#define	TableWidth		(2 * xOffset + colWidth + idWidth + remarkWidth)
#define	TableHeight		(2 * yOffset + (NumNeuralNet) * rowHeight + ClosePlace)
#define	CloseWidth		80
#define CloseHight		25
#define	xClosePos		(TableWidth - CloseWidth - 20)
#define yClosePos		(TableHeight - CloseHight - 30)

// The color definition.
#define	Black			RGB(0,0,0)
#define	White			RGB(255,255,255)
#define	ColorTone		theApp.m_col.clTones

// The neural network definition.
#define	NeuralnetConf	theApp.m_cnf.neuralnetConf


///////////////////////////////////////////////////////////////////////////////
// CNnTable ダイアログ

IMPLEMENT_DYNAMIC(CNnTable, CDialog)
CNnTable::CNnTable(CWnd* pParent /*=NULL*/)
	: CDialog(CNnTable::IDD, pParent)
{
	Create(CNnTable::IDD, pParent);		

	xOffset		= 0; 
	yOffset		= 0; 
	remarkWidth = 0; 
	rowHeight	= 0;
	m_nAsnNet	= -1;
}

CNnTable::~CNnTable()
{
}

// exchange data
void CNnTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NN_FRAME, m_frameNN);
}

// message map
BEGIN_MESSAGE_MAP(CNnTable, CDialog)
	ON_BN_CLICKED(IDC_CLOSE, OnClickedClose)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CNnTable message handler
void CNnTable::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

void CNnTable::OnClickedClose()
{
	ShowWindow(SW_HIDE);
}

void CNnTable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// draw NN table
	DrawNNTable(dc);
}

void CNnTable::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (GetNeuralNetEntry(point.x, point.y, &m_nAsnNet) == TRUE) 
	{
		// pop up Neural Net Structure Dialog Box. 
		CNnStruct	dlg(m_nAsnNet, this);
		dlg.DoModal();
		m_nAsnNet = -1;
		Invalidate(TRUE);
	}
	else
		MessageBeep(-1);

	CDialog::OnLButtonDblClk(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// CNnTable user function

// draw Neural Net table.
void CNnTable::DrawNNTable(CDC &dc)
{
	int		i;
	CRect	rc;
	CBrush	brBox(Black);

	// base pos setting.
	m_frameNN.GetClientRect(rc);
	m_frameNN.MapWindowPoints(this, rc);
	xOffset = rc.left;
	yOffset	= rc.top;
	remarkWidth = rc.Width() - colWidth - idWidth;
	rowHeight = rc.Height()/NumNeuralNet;

	// for each neural net, 
	for (i = 0; i < NumNeuralNet; i++) {

		// calculate geometry of this column. 
		rc.left	= xOffset;
		rc.top	= rowHeight * i + yOffset;
		rc.right = rc.left + colWidth;
		rc.bottom = rc.top + rowHeight;

		// draw neural net color. 
		if (m_nAsnNet == i)
			dc.FillSolidRect(rc, White);
		else
			dc.FillSolidRect(rc, ColorTone[i*4]);
		dc.FrameRect(rc, &brBox);

		// draw neural net number. 
		rc.left += colWidth;
		rc.right += idWidth;
		dc.FrameRect(rc, &brBox);
		PrintText(dc, NULL, Black, rc.left + 3, rc.top, "%2d", i + 1);

		// draw neural net remark. 
		rc.left += idWidth;
		rc.right += remarkWidth;
		dc.FrameRect(rc, &brBox);
		PrintText(dc, NULL, Black, rc.left + 3, rc.top,
			NeuralnetConf[i].remark);
	}
}

// get table entry from mouse geometry.
BOOL CNnTable::GetNeuralNetEntry(int x, int y, int *id)
{
	if (x < xOffset || x >= TableWidth - xOffset ||
		y < yOffset || y >= TableHeight - yOffset)
		return	FALSE;

	y	-= yOffset;
	*id	= y / rowHeight;

	return	TRUE;
}
