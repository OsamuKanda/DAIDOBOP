// BoexeAxis.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BoexeAxis.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnfBoexe;


// CBoexeAxis dialog
IMPLEMENT_DYNAMIC(CBoexeAxis, CDialog)
CBoexeAxis::CBoexeAxis(CWnd* pParent /*=NULL*/)
	: CDialog(CBoexeAxis::IDD, pParent)
{
}

CBoexeAxis::~CBoexeAxis()
{
}

void CBoexeAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbTmScale);
	DDX_Control(pDX, IDC_EDIT1, m_editTempScale);
	DDX_Control(pDX, IDC_EDIT25, m_editTempZero);
	DDX_Control(pDX, IDC_EDIT26, m_editLevelScale);
	DDX_Control(pDX, IDC_EDIT27, m_editLevelZero);
	DDX_Control(pDX, IDC_EDIT28, m_editVcScale);
	DDX_Control(pDX, IDC_EDIT29, m_editVcZero);
	DDX_Control(pDX, IDC_EDIT30, m_editSNScale);
	DDX_Control(pDX, IDC_EDIT31, m_editSNZero);
	DDX_Control(pDX, IDC_EDIT32, m_editWidthScale);
	DDX_Control(pDX, IDC_EDIT33, m_editWidthZero);
	DDX_Control(pDX, IDC_STATIC1, m_textTmScale);
}

BEGIN_MESSAGE_MAP(CBoexeAxis, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBoexeAxis message handler

BOOL CBoexeAxis::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup trend axis
	SetupTrendAxisDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CBoexeAxis::OnClickedOk()
{
	if( UpdateAxisSetting() )
		OnOK();
}

void CBoexeAxis::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CBoexeAxis member function

// set up trend graph pens.
void CBoexeAxis::SetupTrendAxisDlg(void)
{
	CString	str;
	m_textTmScale.ShowWindow(SW_HIDE);
	m_cmbTmScale.ShowWindow(SW_HIDE);

	// set the temperature axis text.
	str.Format("%lf", cnf.m_stTempAxis.scale);
	m_editTempScale.SetWindowText(str);

	str.Format("%lf", cnf.m_stTempAxis.zero);
	m_editTempZero.SetWindowText(str);

	// set the meniscas level axis text.
	str.Format("%lf", cnf.m_stLevelAxis.scale);
	m_editLevelScale.SetWindowText(str);

	str.Format("%lf", cnf.m_stLevelAxis.zero);
	m_editLevelZero.SetWindowText(str);

	// set the casting speed axis text.
	str.Format("%lf", cnf.m_stVcAxis.scale);
	m_editVcScale.SetWindowText(str);

	str.Format("%lf", cnf.m_stVcAxis.zero);
	m_editVcZero.SetWindowText(str);

	// set the SN axis text.
	str.Format("%lf", cnf.m_stSNAxis.scale);
	m_editSNScale.SetWindowText(str);

	str.Format("%lf", cnf.m_stSNAxis.zero);
	m_editSNZero.SetWindowText(str);

	// set the width axis text.
//	str.Format("%lf", cnf.m_stWidthAxis.scale);
//	m_editWidthScale.SetWindowText(str);

//	str.Format("%lf", cnf.m_stWidthAxis.zero);
//	m_editWidthZero.SetWindowText(str);
}

// update chart axis setting.
BOOL CBoexeAxis::UpdateAxisSetting(void)
{
	ChartAxis	timeAxis;
	ChartAxis	tempAxis;
	ChartAxis	levelAxis;
	ChartAxis	vcAxis;
	ChartAxis	snAxis;
//	ChartAxis	widthAxis;
	CString		str;
//	double		v;
//	long		sel;

/*
	sel = m_cmbTmScale.GetCurSel();
	m_cmbTmScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &v);
	if      (v <=  0.3)	timeAxis.scale =  0.25;
	else if (v <=  0.6)	timeAxis.scale =  0.5;
	else if (v <=  1.5)	timeAxis.scale =  1.0;
	else if (v <=  2.5)	timeAxis.scale =  2.0;
	else if (v <=  6.0)	timeAxis.scale =  5.0;
	else if (v <= 15.0)	timeAxis.scale = 10.0;
	else if (v <= 25.0)	timeAxis.scale = 20.0;
	timeAxis.zero = 0;
*/
	// store time axis.
	timeAxis.scale	= 0.25;
	timeAxis.zero	= 0;

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

	// store level axis.
	m_editLevelScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &levelAxis.scale);
	if (levelAxis.scale == 0.0) {
		// error message display
		AfxMessageBox(IDS_TRD_AXIS08, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_editLevelZero.GetWindowText(str);
	_stscanf(str, _T("%lf"), &levelAxis.zero);

	// store Vc axis.
	m_editVcScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &vcAxis.scale);
	if (vcAxis.scale == 0.0) {
		// error message display
		AfxMessageBox(IDS_TRD_AXIS09, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_editVcZero.GetWindowText(str);
	_stscanf(str, _T("%lf"), &vcAxis.zero);

	// store sn axis.
	m_editSNScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &snAxis.scale);
	if (snAxis.scale == 0.0) {
		// error message display
		AfxMessageBox(IDS_TRD_AXIS10, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_editSNZero.GetWindowText(str);
	_stscanf(str, _T("%lf"), &snAxis.zero);

	// store temperature axis.
//	m_editWidthScale.GetWindowText(str);
//	_stscanf(str, _T("%lf"), &widthAxis.scale);
//	if (widthAxis.scale == 0.0) {
		// error message display
//		AfxMessageBox(IDS_TRD_AXIS11, MB_OK|MB_ICONEXCLAMATION);
//		return FALSE;
//	}
//	m_editWidthZero.GetWindowText(str);
//	_stscanf(str, _T("%lf"), &widthAxis.zero);

	cnf.m_stTimeAxis	= timeAxis;
	cnf.m_stTempAxis	= tempAxis;
	cnf.m_stLevelAxis	= levelAxis;
	cnf.m_stVcAxis		= vcAxis;
	cnf.m_stSNAxis		= snAxis;
//	cnf.m_stWidthAxis	= widthAxis;

	// write axis file.
	cnf.WriteAxisFile();

	return TRUE;
}

