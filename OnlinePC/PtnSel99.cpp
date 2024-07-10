// PtnSel99.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "PtnSel99.h"

///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	*cnf;


// CPtnSel99 ダイアログ
IMPLEMENT_DYNAMIC(CPtnSel99, CDialog)

CPtnSel99::CPtnSel99(CWnd* pParent /*=NULL*/)
	: CDialog(CPtnSel99::IDD, pParent)
{
	m_nPtnNo = -1;
}

CPtnSel99::~CPtnSel99()
{
}

void CPtnSel99::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listGC);
}


BEGIN_MESSAGE_MAP(CPtnSel99, CDialog)
	ON_BN_CLICKED(IDOK, &CPtnSel99::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPtnSel99::OnBnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CPtnSel99 メッセージ ハンドラ

// ダイアログの初期化
BOOL CPtnSel99::OnInitDialog()
{
	CDialog::OnInitDialog();

	BoSysInfo	stBsInfo;
	CString		str;

	// 幅１、高さ24のイメージを持つ
	m_image.Create(1, 24, ILC_COLOR, 0, 0);

	// 高さ調整用のイメージリストを設定する
	m_listGC.SetImageList(&m_image, LVSIL_STATE);
	CHeaderCtrl	*ctl = m_listGC.GetHeaderCtrl();
	if( ctl != NULL ) ctl->SetImageList(&m_image);

	// set list style.
	m_listGC.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	CFont	newFont, *font;
	LOGFONT	lf;
	double	dR, dG, dB;
	int		i;

	// change font
	font = m_listGC.GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -14;
	lf.lfWidth = 0;
	newFont.CreateFontIndirect(&lf);
	m_listGC.SetFont(&newFont);

	// setup list
	m_listGC.DeleteAllItems();
	m_listGC.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listGC.InsertColumn(1, _T("No"), LVCFMT_CENTER, 45);
	m_listGC.InsertColumn(2, _T("表示コメント"), LVCFMT_LEFT, 270);
	for( i = 0; i < NumSteelKind; i++ )
	{
		m_listGC.InsertItem(i, _T(""));
		str.Format(_T("%d"), i+1);
		m_listGC.SetItemText(i, 1, str);

		// Bosys information is read.
		if(cnf->ReadBosysInfo(i, &stBsInfo))
		{
			str = stBsInfo.szComment;
			m_listGC.SetItemText(i, 2, str);
		}
	}

	// 現在の設定値を選択
	m_listGC.SetItemState(m_nPtnNo, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CPtnSel99::OnBnClickedOk()
{
	BoSysInfo	stBsInfo;
	int			sel;

	sel = GetCurSel(m_listGC);

	if( sel == -1 )
	{
		AfxMessageBox(_T("項目を選択して下さい"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	
	// Bosys information is read.
	if( sel < 0 || FALSE == cnf->ReadBosysInfo(sel, &stBsInfo))
	{
		AfxMessageBox(_T("システム情報の読込に失敗しました"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	if( sel < 0 || _tcslen(stBsInfo.szComment) <= 0 )
	{
		AfxMessageBox(_T("このB.O.パターンは選択できません"));
		return;	
	}

	m_nPtnNo = sel;

	OnOK();
}

void CPtnSel99::OnBnClickedCancel()
{
	OnCancel();
}

void CPtnSel99::SetParam(CConfig &config, int pno)
{
	cnf = &config;
	m_nPtnNo = pno;
}

void CPtnSel99::GetParam(int *pno)
{
	*pno = m_nPtnNo;
}

int CPtnSel99::GetCurSel(CListCtrl& list)
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return -1;
	else
	{
		return list.GetNextSelectedItem(pos);
	}
}


