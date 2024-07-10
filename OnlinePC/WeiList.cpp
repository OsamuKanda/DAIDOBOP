// WeiList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "WeiList.h"

///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;


// CWeiList ダイアログ
IMPLEMENT_DYNAMIC(CWeiList, CDialog)

CWeiList::CWeiList(CWnd* pParent /*=NULL*/)
	: CDialog(CWeiList::IDD, pParent)
{
	CString		m_szConfPath = _T("");
	int			m_nNetIdx = -1;
	int			m_nWeiCntP = 0;
	int			m_nWeiCntC = 0;
	memset(m_stWeiInfoP, 0, sizeof(m_stWeiInfoP));
	memset(m_stWeiInfoC, 0, sizeof(m_stWeiInfoC));
}

CWeiList::~CWeiList()
{
}

void CWeiList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CWeiList, CDialog)
	ON_BN_CLICKED(IDC_WEI_SEL, &CWeiList::OnBnClickedWeiSel)
	ON_BN_CLICKED(IDC_WEI_DEL, &CWeiList::OnBnClickedWeiDel)
	ON_BN_CLICKED(IDCANCEL, &CWeiList::OnBnClickedCancel)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CWeiList メッセージ ハンドラ

BOOL CWeiList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set list style.
	m_list.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_list.InsertColumn(1, _T("No."), LVCFMT_CENTER, 40);
	m_list.InsertColumn(2, _T("更新日時"), LVCFMT_CENTER, 170);
	m_list.InsertColumn(3, _T("学習回数"), LVCFMT_CENTER, 83);

	// 学習結果リストを読み込む
	m_nWeiCntP = cnf.GetWeiListP(m_szConfPath, m_nNetIdx, m_stWeiInfoP);
	m_nWeiCntC = cnf.GetWeiListC(m_szConfPath, m_nNetIdx, m_stWeiInfoC);

	// リストに学習結果データ情報表示
	DispWeiList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// 選択
void CWeiList::OnBnClickedWeiSel()
{
	CString	str;
	int		nSel;

	// 選択されているなら
	nSel = GetListCurSel(m_list);
	if( nSel >= 0 && nSel < m_nWeiCntP && nSel < m_nWeiCntC && nSel < WEI_BAK_CNT )
	{
		m_szSavePathP = m_stWeiInfoP[nSel].path;
		m_szSavePathC = m_stWeiInfoC[nSel].path;
	}
	else
		AfxMessageBox(_T("学習結果データが選択されていません"));

	// 閉じる
	CDialog::OnOK();
}

// 削除
void CWeiList::OnBnClickedWeiDel()
{
	CString	str;
	int		nSel;

	nSel = GetListCurSel(m_list);
	if( nSel >= 0 && nSel < m_nWeiCntP && nSel < m_nWeiCntC && nSel < WEI_BAK_CNT )
	{
		if( IDYES == AfxMessageBox(str + _T("削除してもよろしいですか？"), MB_YESNO) )
		{
			_tremove(m_stWeiInfoP[nSel].path);
			_tremove(m_stWeiInfoC[nSel].path);
		}
	}
	else
		AfxMessageBox(_T("学習結果データが選択されていません"));

	// 学習結果リストを読み込む
	m_nWeiCntP = cnf.GetWeiListP(m_szConfPath, m_nNetIdx, m_stWeiInfoP);
	m_nWeiCntC = cnf.GetWeiListC(m_szConfPath, m_nNetIdx, m_stWeiInfoC);

	// リストに学習結果データ情報表示
	DispWeiList();
}

// 閉じる
void CWeiList::OnBnClickedCancel()
{

	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// メンバ変数

// 学習結果データ情報をリストに表示
void CWeiList::DispWeiList(void)
{
	CString	str;
	int		i;

	// 全て削除
	m_list.DeleteAllItems();

	// 項目追加
	for( i = 0; i < m_nWeiCntP; i++ )
	{
		str.Format(_T("%d"), i+1);
		m_list.InsertItem(i, _T(""));
		m_list.SetItemText(i, 1, str);
		m_list.SetItemText(i, 2, GetTimeStr(m_stWeiInfoP[i].updateTime));
		str.Format(_T("%d"), GetLarningTimes(m_stWeiInfoP[i].path));
		m_list.SetItemText(i, 3, str);
	}
}


// 学習回数を取得
int CWeiList::GetLarningTimes(LPTSTR path)
{
	NeuralNet	*net;
	int			times;

	// ニューラルネット情報読出し
	net = loadNeuralNet(path);
	if( net == NULL ) return 0;

	// 学習回数情報取得
	times = net->times;

	// ニューラルネット領域開放
	freeNeuralNet(net);

	return times;
}

// 日時表示文字列を生成
LPCTSTR CWeiList::GetTimeStr(time_t tm)
{
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	tp	= localtime(&tm);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

// get list current selection.
int CWeiList::GetListCurSel(CListCtrl &list)
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return -1;
	else
	{
		return list.GetNextSelectedItem(pos);
	}
}
