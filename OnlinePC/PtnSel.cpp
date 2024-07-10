// PtnSel.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "PtnSel.h"

///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	*cnf;

// CPtnSel dialog

IMPLEMENT_DYNAMIC(CPtnSel, CDialog)
CPtnSel::CPtnSel(CWnd* pParent /*=NULL*/)
	: CDialog(CPtnSel::IDD, pParent)
{
	m_nPtnNo = -1;
	m_nMoldSize = 0;
	m_bHide = FALSE;
}

CPtnSel::~CPtnSel()
{
}

void CPtnSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editMdSize);
	DDX_Control(pDX, IDC_STATIC1, m_textSG[0]);
	DDX_Control(pDX, IDC_STATIC2, m_textSG[1]);
	DDX_Control(pDX, IDC_STATIC3, m_textSG[2]);
	DDX_Control(pDX, IDC_STATIC4, m_textSG[3]);
	DDX_Control(pDX, IDC_STATIC5, m_textSG[4]);
	DDX_Control(pDX, IDC_STATIC6, m_textSG[5]);
	DDX_Control(pDX, IDC_STATIC7, m_textSG[6]);
	DDX_Control(pDX, IDC_STATIC8, m_textSG[7]);
	DDX_Control(pDX, IDC_STATIC9, m_textSG[8]);
	DDX_Control(pDX, IDC_RADIO1, m_rdoChk[0]);
	DDX_Control(pDX, IDC_RADIO2, m_rdoChk[1]);
	DDX_Control(pDX, IDC_RADIO3, m_rdoChk[2]);
	DDX_Control(pDX, IDC_RADIO4, m_rdoChk[3]);
	DDX_Control(pDX, IDC_RADIO5, m_rdoChk[4]);
	DDX_Control(pDX, IDC_RADIO6, m_rdoChk[5]);
	DDX_Control(pDX, IDC_RADIO7, m_rdoChk[6]);
	DDX_Control(pDX, IDC_RADIO8, m_rdoChk[7]);
	DDX_Control(pDX, IDC_RADIO9, m_rdoChk[8]);
	DDX_Control(pDX, IDC_STATIC21, m_text1);
	DDX_Control(pDX, IDC_STATIC22, m_text2);
}


BEGIN_MESSAGE_MAP(CPtnSel, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnClickedRadio9)
	ON_BN_CLICKED(IDOK, OnClickedOk)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CPtnSel message handler
BOOL CPtnSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	BoSysInfo	stBsInfo;
	CString		str;
	int			i;

	// if hide flag is on, 
	if( m_bHide )
	{
		// mold size display off.
		m_editMdSize.ShowWindow(SW_HIDE);
		m_text1.ShowWindow(SW_HIDE);
		m_text2.ShowWindow(SW_HIDE);
	}
	else
	{
		// display mold size.
		str.Format(_T("%d"), m_nMoldSize);
		m_editMdSize.SetWindowText(str);
	}

	// check default pattern.
	if( m_nPtnNo >= 0 && m_nPtnNo < NumSteelKind )
		m_rdoChk[m_nPtnNo].SetCheck(BST_CHECKED);

	// for each steel kind,
	for( i = 0; i < NumSteelKind; i++ )
	{
		// Bosys information is read.
		if(FALSE == cnf->ReadBosysInfo(i, &stBsInfo))
		{
			AfxMessageBox(_T("B.O.パターン情報を読み込めません"), MB_ICONEXCLAMATION | MB_OK);
			break;
		}
		m_textSG[i].SetWindowText(stBsInfo.szComment);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CPtnSel::OnClickedRadio1(){ m_nPtnNo = 0; }
void CPtnSel::OnClickedRadio2(){ m_nPtnNo = 1; }
void CPtnSel::OnClickedRadio3(){ m_nPtnNo = 2; }
void CPtnSel::OnClickedRadio4(){ m_nPtnNo = 3; }
void CPtnSel::OnClickedRadio5(){ m_nPtnNo = 4; }
void CPtnSel::OnClickedRadio6(){ m_nPtnNo = 5; }
void CPtnSel::OnClickedRadio7(){ m_nPtnNo = 6; }
void CPtnSel::OnClickedRadio8(){ m_nPtnNo = 7; }
void CPtnSel::OnClickedRadio9(){ m_nPtnNo = 8; }

void CPtnSel::OnClickedOk()
{
	BoSysInfo	stBsInfo;
	CString		str;
//	int			nMdSize;

	//if( !m_bHide )
	//{
	//	// get mold size
	//	m_editMdSize.GetWindowText(str);
	//	nMdSize = _tstoi(str);
	//	if( nMdSize < 900 || nMdSize > 1650 )
	//	{
	//		AfxMessageBox(_T("モールド幅が不正です"), MB_ICONEXCLAMATION | MB_OK);
	//		return;
	//	}
	//	m_nMoldSize = nMdSize;
	//}

	// Bosys information is read.
	if( m_nPtnNo < 0 || FALSE == cnf->ReadBosysInfo(m_nPtnNo, &stBsInfo))
	{
		AfxMessageBox(_T("システム情報の読込に失敗しました"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	if( m_nPtnNo < 0 || _tcslen(stBsInfo.szComment) <= 0 )
	{
		AfxMessageBox(_T("このB.O.パターンは選択できません"));
		return;	
	}

	OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// CPtnSel member function

void CPtnSel::SetParam(CConfig &config, int pno, int nMoldSize)
{
	cnf = &config;
	m_nPtnNo = pno;
	m_nMoldSize = nMoldSize;
}

void CPtnSel::GetParam(int *pno, int *pMoldSize)
{
	*pno = m_nPtnNo;
	*pMoldSize = m_nMoldSize;
}

void CPtnSel::SetParam(CConfig &config, int pno)
{
	cnf = &config;
	m_nPtnNo = pno;
	m_bHide = TRUE;
}

void CPtnSel::GetParam(int *pno)
{
	*pno = m_nPtnNo;
}



