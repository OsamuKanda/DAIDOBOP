// PenSetting.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "PenSetting.h"
#include <tchar.h>

// pen back color
COLORREF	colBack[PEN_COL_NUM] = {
				RGB(255,0,0),	
				RGB(127,0,0),	
				RGB(0,255,0),	
				RGB(0,127,0),	
				RGB(0,0,255),	
				RGB(0,0,127),	
				RGB(255,0,255), 
				RGB(127,0,127), 
				RGB(255,255,0), 
				RGB(127,127,0), 
				RGB(0,255,255), 
				RGB(0,127,127)
};

// CPenSetting dialog
IMPLEMENT_DYNAMIC(CPenSetting, CDialog)
CPenSetting::CPenSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CPenSetting::IDD, pParent)
{
	memset(&m_stInfo, 0, sizeof(m_stInfo));
}

CPenSetting::~CPenSetting()
{
}

void CPenSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbCol[0]);
	DDX_Control(pDX, IDC_COMBO2, m_cmbCol[1]);
	DDX_Control(pDX, IDC_COMBO4, m_cmbCol[2]);
	DDX_Control(pDX, IDC_COMBO6, m_cmbCol[3]);
	DDX_Control(pDX, IDC_COMBO7, m_cmbCol[4]);
	DDX_Control(pDX, IDC_COMBO8, m_cmbCol[5]);
	DDX_Control(pDX, IDC_COMBO9, m_cmbCol[6]);
	DDX_Control(pDX, IDC_COMBO20, m_cmbCombi);
	DDX_Control(pDX, IDC_EDIT31, m_editMax[0]);
	DDX_Control(pDX, IDC_EDIT32, m_editMax[1]);
	DDX_Control(pDX, IDC_EDIT34, m_editMax[2]);
	DDX_Control(pDX, IDC_EDIT36, m_editMax[3]);
	DDX_Control(pDX, IDC_EDIT37, m_editMax[4]);
	DDX_Control(pDX, IDC_EDIT41, m_editMin[0]);
	DDX_Control(pDX, IDC_EDIT42, m_editMin[1]);
	DDX_Control(pDX, IDC_EDIT44, m_editMin[2]);
	DDX_Control(pDX, IDC_EDIT46, m_editMin[3]);
	DDX_Control(pDX, IDC_EDIT47, m_editMin[4]);
	DDX_Control(pDX, IDC_CHECK1, m_btnPen[0]);
	DDX_Control(pDX, IDC_CHECK2, m_btnPen[1]);
	DDX_Control(pDX, IDC_CHECK4, m_btnPen[2]);
	DDX_Control(pDX, IDC_CHECK6, m_btnPen[3]);
	DDX_Control(pDX, IDC_CHECK7, m_btnPen[4]);
}


BEGIN_MESSAGE_MAP(CPenSetting, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CPenSetting message handler

BOOL CPenSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	str;
	int		i, j, sel;

	for( i = 0; i < PEN_NUM; i++ )
	{
		m_btnPen[i].SetCheck(m_stInfo.bEnbPen[i]);
	}

	for( i = 0; i < PEN_SET_NUM; i++ )
	{
		for( j = 0, sel = 0; j < PEN_COL_NUM; j++ )
		{
			m_cmbCol[i].InsertItem(j, _T(""), colBack[j], 0);
			if( m_stInfo.colPen[i] == colBack[j] )
				sel = j;
		}
		m_cmbCol[i].SetCurSel(sel);
	}

	for( i = 0; i < PEN_COMBI_NUM; i++ )
	{
		str.Format(_T("%d"), i+1);
		m_cmbCombi.AddString(str);
	}
	m_cmbCombi.SetCurSel(m_stInfo.nCombiNo);

	for( i = 0; i < PEN_NUM; i++ )
	{
		str.Format(m_stInfo.szFmt[i], m_stInfo.dMax[i]);
		m_editMax[i].SetWindowText(str);
		str.Format(m_stInfo.szFmt[i], m_stInfo.dMin[i]);
		m_editMin[i].SetWindowText(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPenSetting::OnClickedOk()
{
	CString	str;
	int		i, sel;

	for( i = 0; i < PEN_NUM; i++ )
	{
		m_stInfo.bEnbPen[i] = m_btnPen[i].GetCheck();
	}

	for( i = 0; i < PEN_SET_NUM; i++ )
	{
		sel = m_cmbCol[i].GetCurSel();
		if( sel >= 0 )
			m_stInfo.colPen[i] = colBack[sel];
	}

	sel = m_cmbCombi.GetCurSel();
	if( sel >= 0 )
		m_stInfo.nCombiNo = sel;

	for( i = 0; i < PEN_NUM; i++ )
	{
		m_editMax[i].GetWindowText(str);
		m_stInfo.dMax[i] = _tstof(str.GetBuffer(10));
		str.ReleaseBuffer();
		m_editMin[i].GetWindowText(str);
		m_stInfo.dMin[i] = _tstof(str.GetBuffer(10));
		str.ReleaseBuffer();
	}

	OnOK();
}

void CPenSetting::SetParam(StPenSetInfo *pInfo)
{
	m_stInfo = *pInfo;
}

void CPenSetting::GetParam(StPenSetInfo *pInfo)
{
	 *pInfo = m_stInfo;
}

