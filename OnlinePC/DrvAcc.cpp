// DrvAcc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "DrvAcc.h"
#include "../lib/DrvChk.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CBolog	&lg = theApp.m_log;


// CDrvAcc dialog
IMPLEMENT_DYNAMIC(CDrvAcc, CDialog)
CDrvAcc::CDrvAcc(CWnd* pParent /*=NULL*/)
	: CDialog(CDrvAcc::IDD, pParent)
{
	memset(m_trendDir, 0, sizeof(m_trendDir));
	m_numTrendFile	= 0;
	m_bAcc			= FALSE;
	m_isLocal		= TRUE;
}

CDrvAcc::~CDrvAcc()
{
}

void CDrvAcc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_cmb1);
	DDX_Control(pDX, IDC_RADIO1, m_rdo1);
	DDX_Control(pDX, IDC_RADIO2, m_rdo2);
	DDX_Control(pDX, IDC_DRV_LOAD, m_btn1);
	DDX_Control(pDX, IDC_DRV_SAVE, m_btn2);
}

BEGIN_MESSAGE_MAP(CDrvAcc, CDialog)
	ON_BN_CLICKED(IDC_DRV_LOAD, OnBnClickedToLocal)
	ON_BN_CLICKED(IDC_DRV_SAVE, OnBnClickedToDrive)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnLvnItemchangedList1)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CDrvAcc メッセージ ハンドラ

// initialize dialog.
BOOL CDrvAcc::OnInitDialog()
{
	CDialog::OnInitDialog();

	StDrvInfo	di[DRV_NUM_MAX];
	int			i, nNum;

	// 初期はローカル
	m_isLocal		= TRUE;

	// set list style.
	m_list.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.DeleteAllItems();
	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_list.InsertColumn(1, _T("開始時刻"), LVCFMT_LEFT, 365);
	m_list.InsertColumn(2, _T("B.O.ﾊﾟﾀｰﾝNo"), LVCFMT_CENTER, 110);
	m_list.InsertColumn(3, _T("データコメント"), LVCFMT_LEFT, 280);

	// get drive name.
	nNum = CDrvChk::EnumDrive(di, sizeof(di));

	// initialize drive combobox.
	for( i = 0; i < nNum && i < DRV_NUM_MAX; i++ )
	{
		// except local fixed drive
		if( !((strstr(di[i].szDrv, _T("C:")) != NULL && di[i].nmType == DRIVE_FIXED) ||
			  (strstr(di[i].szDrv, _T("D:")) != NULL && di[i].nmType == DRIVE_FIXED)) )
		{
			*(_tcsstr(di[i].szDrv, _T("\\"))) = _T('\0');
			m_cmb1.AddString(di[i].szDrv);
		}
	}
	m_cmb1.SetCurSel(0);

	// select local radio button.
	m_rdo1.SetCheck(BST_CHECKED);

	// set trend data list
	SetTrendDataFromDrv(lg.LogRootDir, 0);

 	// disable 'to local' button.
	m_btn1.EnableWindow(FALSE);

	// enable 'to drive' button.
	m_btn2.EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// pushed 'To Local' button.
void CDrvAcc::OnBnClickedToLocal()
{
	if( m_bAcc )
		return;

	// access flg is on.
	m_bAcc = TRUE;

	// copy the selection file from the drive to the local.
	CopySelectFile(0);

	// access flg is off.
	m_bAcc = FALSE;
}

// pushed 'To Drive' button.
void CDrvAcc::OnBnClickedToDrive()
{
	if( m_bAcc )
		return;

	// access flg is on.
	m_bAcc = TRUE;

	// copy the selection file from the local to the drive.
	CopySelectFile(1);

	// access flg is off.
	m_bAcc = FALSE;
}

// pushed close button.
void CDrvAcc::OnBnClickedCancel()
{
	if( m_bAcc )
		return;

	OnCancel();
}

// selection change drive combobox
void CDrvAcc::OnCbnSelchangeCombo1()
{
	if( m_bAcc )
		return;

	// if 'drive' button's check is on,
	if( BST_CHECKED == m_rdo2.GetCheck() )
	{
		OnBnClickedRadio2();
	}
}

// pushed 'local' radio button
void CDrvAcc::OnBnClickedRadio1()
{
 	if( m_bAcc )
		return;
	
	// ローカル
	m_isLocal = TRUE;

	// set trend data list
	SetTrendDataFromDrv(lg.LogRootDir, 0);

	// disable 'to local' button.
	m_btn1.EnableWindow(FALSE);

	// enable 'to drive' button.
	m_btn2.EnableWindow(TRUE);
}

// pushed 'drive' radio button.
void CDrvAcc::OnBnClickedRadio2()
{
	_TCHAR	szDrv[MAX_PATH];
	int		sel;

	if( m_bAcc )
		return;

	// get selection drive name.
	sel = m_cmb1.GetCurSel();
	if( sel == -1 )
		return;
	m_cmb1.GetLBText(sel, szDrv);

	// ドライブ
	m_isLocal = FALSE;

    // set trend data list
	SetTrendDataFromDrv(szDrv, 0);

	// enable 'to local' button.
	m_btn1.EnableWindow(TRUE);

	// disable 'to drive' button.
	m_btn2.EnableWindow(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// member function.

// get list current selection.
int CDrvAcc::GetListCurSel(CListCtrl &list)
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return -1;
	else
	{
		return list.GetNextSelectedItem(pos);
	}
}

// set trand data from drive.
void CDrvAcc::SetTrendDataFromDrv(LPCTSTR lpszDir, int nHead)
{
	TrendDirectory	*p;
	int				i;

	// get trend file entry in the trend file directory.
	m_numTrendFile = lg.GetTrendDirectoryEx(lpszDir, m_trendDir);

	// reset trend file list.
	m_list.SetRedraw(FALSE);
	m_list.DeleteAllItems();

	// for each entry,
	for (i = 0, p = m_trendDir; i < m_numTrendFile; i++, p++) 
	{
		// append trend file entry.
		m_list.InsertItem(i, p->name);
		m_list.SetItemText(i, 1, lg.MakeupTrendEntStringEx(lpszDir, p));
		if( i >= nHead && i < (nHead + 20) )
			SetTrendEntryInfo(i, lpszDir, p->name);
	}

	m_list.SetRedraw(TRUE);
}

// set trand data from drive.
void CDrvAcc::SetTrendDataFromDrv2(LPCTSTR lpszDir, int nPos)
{
	TrendDirectory	*p;
	_TCHAR			szDrv[MAX_PATH];
	int				i, sel;

	// ドライブ側なら
	if( !m_isLocal )
	{
		// ドライブ側の名前を使用する
		sel = m_cmb1.GetCurSel();
		if( sel != -1 )
		{
			m_cmb1.GetLBText(sel, szDrv);

			// for each entry,
			for (i = nPos, p = m_trendDir + nPos; i < m_numTrendFile; i++, p++) 
			{
				if( i >= nPos && i < (nPos + 20) )
					SetTrendEntryInfo(i, szDrv, p->name);
				else
					break;
			}
		}
	}
	// ローカル側なら
	else
	{
		// for each entry,
		for (i = nPos, p = m_trendDir + nPos; i < m_numTrendFile; i++, p++) 
		{
			if( i >= nPos && i < (nPos + 20) )
				SetTrendEntryInfo(i, lpszDir, p->name);
			else
				break;
		}
	}
}

// set entry information.
void CDrvAcc::SetTrendEntryInfo(int sel, LPCTSTR lpszDir, LPCTSTR lpszName)
{
	CString			str;
	DataFileHeader	m_header;
	_TCHAR			dirName[MAX_PATH];
	FILE			*fp;

	// sel no check
	if (sel < 0) return;

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lpszDir ), _T("\\") ), lpszName );

	// open and read the trend file header.
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
	{
		m_list.SetItemText(sel, 2, _T(""));
		m_list.SetItemText(sel, 3, _T(""));
		return;
	}

	// read header info.
	fread(&m_header, sizeof(m_header), 1, fp);

	// close the trend file.
	fclose(fp);

	// B.O.パターンNoを表示
	if( m_header.header.version >= 801 && m_header.header.nPtnNo >= 0 && m_header.header.nPtnNo < NumSteelKind )
	{
		str.Format(_T("%d"), m_header.header.nPtnNo+1);
		m_list.SetItemText(sel, 2, str);
	}

	// display remark of this entry.
	m_list.SetItemText(sel, 3, m_header.header.remark);
}

// copy selection file.(0:drive->local, 1:local->drive)
void CDrvAcc::CopySelectFile(int nWay)
{
	CString	strFile, strSrc, strDst;
	_TCHAR	szName[MAX_PATH];
	_TCHAR	szDrv[MAX_PATH];
	int		sel, ret, cnt;

	// get selection drive name.
	sel = m_cmb1.GetCurSel();
	if( sel == -1 )
		return;
	m_cmb1.GetLBText(sel, szDrv);

	// for each list.
	cnt = 0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	while(pos != NULL)
	{
		// get selection file name.
		sel = m_list.GetNextSelectedItem(pos);
		m_list.GetItemText(sel, 0, szName, sizeof(szName));

		// ※パスの終端には\0が２つ必要
		// make file path.
		if( nWay == 0 )
		{
			strFile.Format(_T("%s\\%s\1"), szDrv, szName);
			strSrc += strFile;
			strFile.Format(_T("%s\\%s\1"), lg.LogRootDir, szName);
			strDst += strFile;
		}
		else
		{
			strFile.Format(_T("%s\\%s\1"), lg.LogRootDir, szName);
			strSrc += strFile;
			strFile.Format(_T("%s\\%s\1"), szDrv, szName);
			strDst += strFile;
		}
		cnt++;
	}

	// １つ以上のファイルがあるなら\1を\0に変換
	if( cnt > 0 )
	{
		// 最後の\0を追加
		strSrc += _T('\1');	
		strDst += _T('\1');	

		// \1を\0に置き換え
		strSrc.Replace(_T('\1'), _T('\0'));
		strDst.Replace(_T('\1'), _T('\0'));
	}
	// if file number is zero, function is ended.
	else
		return;

	// copy files.
	ret = CDrvChk::CopyFile(m_hWnd, strDst, strSrc);
	if( ret != 0 )
	{
		// get error message.
		LPVOID lpMsgBuf;
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, ret, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR) &lpMsgBuf, 0, NULL);

		// disp error message
		AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);
	
		// free buffer.
		LocalFree(lpMsgBuf);
	}
	else
	{
		// disp ok message.
//		AfxMessageBox(_T("copy ok !"));
	}
}

void CDrvAcc::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( nSBCode == SB_ENDSCROLL )
	{
		// set trend data list
		SetTrendDataFromDrv2(lg.LogRootDir, nPos);
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDrvAcc::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	SetTrendDataFromDrv2(lg.LogRootDir, pNMLV->iItem);

	*pResult = 0;
}
