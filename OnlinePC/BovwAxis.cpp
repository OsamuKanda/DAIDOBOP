// BovwAxis.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BovwAxis.h"
#include ".\bovwaxis.h"


// CBovwAxis ダイアログ

IMPLEMENT_DYNAMIC(CBovwAxis, CDialog)
CBovwAxis::CBovwAxis(CWnd* pParent /*=NULL*/)
	: CDialog(CBovwAxis::IDD, pParent)
{
}

CBovwAxis::~CBovwAxis()
{
}

void CBovwAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editTempScale);
	DDX_Control(pDX, IDC_EDIT25, m_editTempZero);
}


BEGIN_MESSAGE_MAP(CBovwAxis, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBovwAxis message handler

// pushed OK button.
void CBovwAxis::OnBnClickedOk()
{
	if( UpdateAxisSetting() )
		OnOK();
}

// pushed Cancel button.
void CBovwAxis::OnBnClickedCancel()
{
	OnCancel();
}

// initialize dialog.
BOOL CBovwAxis::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupTrendAxisDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////////////////
// CBoexeAxis user function

// set up trend graph pens.
void CBovwAxis::SetupTrendAxisDlg(void)
{
	CString	str;

	// set the temperature axis text.
	str.Format("%lf", m_stTempAxis.scale);
	m_editTempScale.SetWindowText(str);

	str.Format("%lf", m_stTempAxis.zero);
	m_editTempZero.SetWindowText(str);
}

// update chart axis setting.
BOOL CBovwAxis::UpdateAxisSetting(void)
{
	ChartAxis	tempAxis;
	CString		str;

	// store temperature axis.
	m_editTempScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &tempAxis.scale);
	if (tempAxis.scale == 0.0) {
		// error message display
		AfxMessageBox(IDS_TRD_AXIS07, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_editTempZero.GetWindowText(str);
	_stscanf(str, _T("%lf"), &tempAxis.zero);

	m_stTempAxis	= tempAxis;

	return TRUE;
}

// read axis file.
void CBovwAxis::GetAxis(ChartAxis &axis)
{
	axis = m_stTempAxis;
}

// write axis file.
void CBovwAxis::SetAxis(ChartAxis &axis)
{
	m_stTempAxis = axis;
}

