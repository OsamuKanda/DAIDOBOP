// MsgWnd.cpp : インプリメンテーション ファイル
//
#include "../include/MsgWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ファイルタイムの加算
FILETIME FT_Add(FILETIME ft1, FILETIME ft2);
// ？日分のデータ取得
FILETIME FT_Day(INT nCount);
// ファイルタイムの比較
int FT_Comp(FILETIME ft1, FILETIME ft2);

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd

CMsgWnd::CMsgWnd()
{
	int	i;

	m_dcWin = NULL;
	m_bmpWin = NULL;
	m_nNumRow = NumRow;

	// メッセージバッファポインタの初期化
	m_msgHeadLine	= m_msgTailLine	= m_msgDispLine	= 0;
	m_msgNumLine	= 0;
	m_msgColumn		= 0;

	// メッセージバッファの初期化
	for (i = 0; i < MsgMaxRow; i++){
		*m_msgBuf[i]	= '\0';
		m_msgCol[i] = RGB(0,0,0);
	}

	// 表示用フォントの作成
	m_cFont.CreateFont(	-9, 0, 0, 0, 0, 0, 0, 0,
						SHIFTJIS_CHARSET, 0, 0, 0, 0, "FixedSys");

	//-----------------
	// ログ関係初期化
	//-----------------
	memset(m_szLogPath, 0, sizeof(m_szLogPath));
	strcpy(m_szLogPath, "..\\Log");
	m_nDays = 0;
	m_bLog = false;
}

CMsgWnd::~CMsgWnd()
{
	// 終了時にログを保存
	SaveLogFile();
	// フォントの削除
	m_cFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMsgWnd, CWnd)
	//{{AFX_MSG_MAP(CMsgWnd)
	ON_MESSAGE( WM_APPENDMSG, OnAppendMsg )
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMsgWnd メッセージ ハンドラ

int CMsgWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// スクロールバーのレンジ設定
	SetScrollRange(SB_VERT, 0, MsgMaxRow - m_nNumRow - 1, FALSE);

	return 0;
}

void CMsgWnd::PostNcDestroy() 
{
	if( m_bmpWin != NULL )
		delete m_bmpWin;
	if( m_dcWin != NULL )
		delete m_dcWin;

	CWnd::PostNcDestroy();
}

void CMsgWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch(nSBCode) 
	{
		case SB_LINEUP:
			// 上スクロールキーが押された時
			ScrolldwnMsgWin();
			break;
		case SB_LINEDOWN:
			// 下スクロールキーが押された時
			ScrollupMsgWin();
			break;
		case SB_PAGEUP:
		{
			// 上ページアップが押された時
			for (int i = 0; i < 16; i++)
				ScrolldwnMsgWin();
			break;
		}
		case SB_PAGEDOWN:
		{
			// 下ページアップが押された時
			for (int i = 0; i < 16; i++)
				ScrollupMsgWin();
			break;
		}
	}

	// スクロールバー位置をセット
	SetScrollPos();	

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMsgWnd::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CRect	rc;
	GetClientRect(rc);

	if(m_dcWin == NULL)
	{
		m_dcWin = new CDC;
		m_dcWin->CreateCompatibleDC(&dc);
		m_bmpWin = new CBitmap;
		m_bmpWin->CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		m_dcWin->SelectObject(m_bmpWin);
	}
	
	// メッセージウィンドウの描画
	DrawMsgWindow(m_dcWin);

	// 画面貼り付け
	dc.BitBlt(0,0,rc.Width(),rc.Height(),m_dcWin,0,0,SRCCOPY);
}

LRESULT CMsgWnd::OnAppendMsg(WPARAM wParam, LPARAM lParam)
{
	// メッセージを追加し表示する
	AddMessage((LPSTR)lParam, (COLORREF)wParam);
	// スクロールバー位置を設定する
	SetScrollPos();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// ユーザー関数

void CMsgWnd::Append(LPSTR fmt, ...)
{
	_TCHAR		buf[256];
	va_list		pvar;

	// メッセージをバッファにセットする
	va_start(pvar, fmt);
	_vstprintf(buf, fmt, pvar);
	va_end(pvar);

	// 改行をバッファの最後にセットする。
	_tcscat(buf, _T("\n"));

	// バッファの内容を画面に表示する
	SendMessage(WM_APPENDMSG, (WPARAM)-1, (DWORD)buf);
	Sleep(1);
}

void CMsgWnd::Append(COLORREF col, LPSTR fmt, ...)
{
	_TCHAR		buf[256];
	va_list		pvar;

	// メッセージをバッファにセットする
	va_start(pvar, fmt);
	_vstprintf(buf, fmt, pvar);
	va_end(pvar);

	// 改行をバッファの最後にセットする。
	_tcscat(buf, _T("\n"));

	// バッファの内容を画面に表示する
	SendMessage(WM_APPENDMSG, (WPARAM)col, (DWORD)buf);
	Sleep(1);
}

void CMsgWnd::AddMessage(LPSTR msg, COLORREF col)
{
	char		*p;

	// set color
	m_msgCol[m_msgTailLine] = col;

	// 書込みバッファポインタを取得
	p	= m_msgBuf[m_msgTailLine] + m_msgColumn;
	// メッセージの終わりまで
	for ( ; *msg; msg++) {
		// 改行コードでない場合
		if (*msg != '\n') {
			// メッセージをバッファに追加する
			*p++	= *msg;
			// 文字数をカウント
			m_msgColumn++;
		}
		// 改行か表示文字数の限界なら
		if (*msg == '\n' || m_msgColumn == MsgColumn) {
			// 現在のバッファを閉じる
			*p	= '\0';
			// バッファの内容を現在行に表示する
			DispMsgLine();
			// メッセージ行数をインクリメントする
			m_msgNumLine++;
			// 行ラインが最終に達したら
			if (++m_msgTailLine == MsgMaxRow) 
			{
				SaveLogFile();
				// 先頭に戻す
				m_msgTailLine	= 0;
			}
			// メッセージバッファが満タンになったら
			if (m_msgNumLine >= MsgMaxRow) {
				// メッセージ行数を最大値に丸める
				m_msgNumLine	= MsgMaxRow;
				// ヘッダ行が最大行数の場合、ヘッダラインを初期化する
				if (++m_msgHeadLine == MsgMaxRow)
				{
					m_msgHeadLine	= 0;
				}
			}
			// 文字数を初期化
			m_msgColumn	= 0;

			// set color
			m_msgCol[m_msgTailLine] = col;

			// 
			p	= m_msgBuf[m_msgTailLine] + m_msgColumn;
		}
	}
	/* if the last character is not a new line code, */
	if (msg[-1] != '\n') {
		/* terminate current line. */
		*p = '\0';
		/* display a current line. */
		DispMsgLine();
	}
}

void CMsgWnd::DispMsgLine(void)
{
	int		n;

	// 表示行数を計算
	n	= (m_msgTailLine + MsgMaxRow - m_msgDispLine) % MsgMaxRow;

	// if the displaying space is left
	if (m_msgNumLine < MsgMaxRow && n < m_nNumRow) 
	{
	}
	// if the displaying position is the tail of screen.
	else if (n == m_nNumRow) 
	{
		// increase the display point.
		if (++m_msgDispLine == MsgMaxRow)
			m_msgDispLine	= 0;
	}
	// if the displaying space was circled up,
	else if (m_msgNumLine == MsgMaxRow && n == 0) 
	{
		// set up the displaying point.
		m_msgDispLine	= (m_msgTailLine - m_nNumRow + MsgMaxRow + 1) % MsgMaxRow;
	}
}

/*====================================================*/
/* draw the message window.
*/
void CMsgWnd::DrawMsgWindow(CDC *pDC)
{
	int		i, n;
	CRect	rc;

	// 背景消去
	GetClientRect(rc);
	pDC->FillSolidRect(rc, MSGWINDOWCOLOR);

	/* for each message line,												  */
	for (n = 0, i = m_msgDispLine; i != m_msgTailLine && n < m_nNumRow; n++) {
		/* draw a message line.												  */
		DrawMsgLine(pDC, RowH * n, m_msgBuf[i], m_msgCol[i]);
		/* increase the displaying pointer.									  */
		if (++i == MsgMaxRow)		i	= 0;
	}
}

/*====================================================*/
/* draw a message line.
*/
void CMsgWnd::DrawMsgLine(CDC *pDC, int y, char *msg, COLORREF col)
{
//	CFont		*pOldFont;
	COLORREF	bkCol;

	pDC->FillSolidRect(0, y, MsgPlaneWidth, RowH, MSGWINDOWCOLOR);
//	pOldFont = pDC->SelectObject(&m_cFont);
	pDC->SetBkMode(TRANSPARENT);
	if( col != -1 )	bkCol = pDC->SetTextColor(col);
	pDC->TextOut(0, y, msg, strlen(msg));
	if( col != -1 )	pDC->SetTextColor(bkCol);
//	pDC->SelectObject(pOldFont);
}

void CMsgWnd::SetScrollPos(void)
{
	int		pos;

	/* calculate the scroll bar position.									  */
	pos	= (m_msgDispLine - m_msgHeadLine + MsgMaxRow) % MsgMaxRow;

	/* set the scroll bar position.											  */
	CWnd::SetScrollPos(SB_VERT, pos, TRUE);

	// 再描画
	Invalidate(FALSE);
}

// スクロールDown時の処理
void CMsgWnd::ScrolldwnMsgWin(void)
{
	/* if the display pointer is the head of buffer, do nothing.			  */
	if (m_msgDispLine == m_msgHeadLine)
		return;

	/* increase the display point.											  */
	if (--m_msgDispLine < 0)
		m_msgDispLine	= MsgMaxRow - 1;
}

// スクロールUp時の処理
void CMsgWnd::ScrollupMsgWin(void)
{
	int		n;

	/* if the displayinh head line is the tail line in the buffer,			  */
	n	= (m_msgTailLine - m_msgDispLine + MsgMaxRow) % MsgMaxRow;
	if (n <= m_nNumRow) {
		/* do nothing.														  */
		return;
	}

	/* increase the display point.											  */
	if (++m_msgDispLine == MsgMaxRow)
		m_msgDispLine	= 0;
}

// コンフィグ情報のパスを設定
BOOL CMsgWnd::SetLogPath(const char *pszPath)
{
	int	len, max;

	// 異常アドレスチェック
	if( pszPath == NULL )
		return FALSE;

	// パス長チェック
	max = MAX_PATH-1;
	len = strlen(m_szLogPath);
	if( len > max )
		return FALSE;

	// パスを更新
	strncpy(m_szLogPath, pszPath, max);

	return TRUE;
}

// コンフィグ情報のパスを取得
const char *CMsgWnd::GetLogPath(void)
{
	return m_szLogPath;
}

// ログ保存日数設定
void CMsgWnd::SetLogDays(int day)
{
	m_nDays = day;
}

// ログ保存日数取得
int CMsgWnd::GetLogDays(void)
{
	return m_nDays;
}

// ログをファイルに保存（古いファイルは削除）
void CMsgWnd::SaveLogFile(void)
{
	// ログ無効なら抜ける
	if( !m_bLog )
		return;

	FILE	*streem = NULL;
	char	buf[MAX_PATH];
	int		i;

	// ファイル名の作成
	HANDLE			fp;
	WIN32_FIND_DATA	fd; 

	try
	{
		//-----------------------------
		// 過去のいらないファイルを削除
		//-----------------------------
		sprintf(buf, "%s\\*.*", m_szLogPath);
		fp = FindFirstFile(buf, &fd);

		FILETIME ft;
		SYSTEMTIME st;

		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &ft);

		if(INVALID_HANDLE_VALUE != fp)
		{
			for(BOOL f = TRUE;f == TRUE;)
			{
				if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					if( FT_Comp(FT_Add( fd.ftLastWriteTime, FT_Day(m_nDays)), ft) < 0)
					{
						sprintf(buf, "%s\\%s", m_szLogPath, fd.cFileName);
						DeleteFile(buf);
					}
				}
				f = FindNextFile(fp, &fd);
			}
		}
		FindClose(fp);

		//-----------------------------
		// 新たなファイルを作成
		//-----------------------------
		sprintf(buf, "%s\\%04d%02d%02d.log", m_szLogPath, st.wYear, st.wMonth, st.wDay);

		// ファイルのオープン
		if(NULL == (streem = fopen(buf, "a+t")))
		{
			return;
		}

		for(i = 0;i != m_msgTailLine;i++)
		{
			if(i >= MsgMaxRow)
				i = 0;

			if(EOF == fputs(m_msgBuf[i], streem))
				break;

			if(EOF == fputs("\n", streem))
				break;
		}
		// ファイルを閉じる
		fclose(streem);
		streem = NULL;
	}
	catch(...)
	{
		// ファイルを閉じる
		if(streem != NULL)
			fclose(streem);
	}
}

// ログ保存フラグ設定
void CMsgWnd::SetLog(bool bFlg)
{
	m_bLog = bFlg;
}

// ログ保存フラグ取得
bool CMsgWnd::GetLog(void)
{
	return m_bLog;
}

// ウィンドウ終了時の強制書き込み
void CMsgWnd::ForceSave(void)
{
	// 終了時にログを保存
	SaveLogFile();
	// 先頭に戻す
	m_msgTailLine	= 0;
}

// メッセージリストのリセット
void CMsgWnd::ResetContent(void)
{
	// メッセージバッファポインタの初期化
	m_msgHeadLine	= m_msgTailLine	= m_msgDispLine	= 0;
	m_msgNumLine	= 0;
	m_msgColumn		= 0;
	SetScrollPos();
}

// ファイルタイムの加算
FILETIME FT_Add(FILETIME ft1, FILETIME ft2)
{
	__int64	i1, i2, i3;
	FILETIME ft3;

	i1 = (__int64)( ( (__int64)ft1.dwHighDateTime << 32 ) | ft1.dwLowDateTime );
	i2 = (__int64)( ( (__int64)ft2.dwHighDateTime << 32 ) | ft2.dwLowDateTime );
	i3 = i1 + i2;

	ft3.dwLowDateTime = (DWORD)i3;
	ft3.dwHighDateTime = (DWORD)( (__int64)i3 >> 32 );

	return (FILETIME)ft3;
}

// ？日分のデータ取得
FILETIME FT_Day(INT nCount)
{
	FILETIME FT_Add(FILETIME, FILETIME);
	FILETIME ft;
	FILETIME Val = {0,0};

	// １日のデータをセット
	ft.dwLowDateTime = 0x2a69c000;
	ft.dwHighDateTime = 0xc9;

	for(int i = 0;i < nCount;i++)
		Val = FT_Add(Val, ft);

	return (FILETIME)Val;
}

// ファイルタイムの比較
int FT_Comp(FILETIME ft1, FILETIME ft2)
{
	if(ft1.dwHighDateTime > ft2.dwHighDateTime)
		return 1;
	else 
		if(ft1.dwHighDateTime < ft2.dwHighDateTime)
			return -1;

	if(ft1.dwLowDateTime > ft2.dwLowDateTime)
		return 1;
	else 
		if(ft1.dwLowDateTime < ft2.dwLowDateTime)
			return -1;

	return 0;	
}

