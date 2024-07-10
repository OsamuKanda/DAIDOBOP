// MsgWnd.h : ヘッダー ファイル
//
#if !defined(AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_)
#define AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd ウィンドウ

// 定数
#define	RowH			INT(18)						// １行の高さ	
#define	ColumnW			FLOAT(8.2)					// １文字の幅	
#define	NumRow			INT(40)						// 行数	
#define	MsgColumn		INT(125)					// １行の文字数	
#define	ScrollBarWidth	INT(18)						// スクロールバーの幅	
#define	MsgWidth		INT(MsgColumn * ColumnW)	// 表示エリアの最大幅	
#define	MsgHeight		INT(RowH * m_nNumRow)		// 表示可能エリアの幅	
#define	MsgXOffset		INT(0)						// 表示エリアの高さ	
#define	MsgYOffset		INT(0)						// 表示エリアの位置Ｘ	
#define	MsgMaxRow		INT(500)					// 保存最大行数	
#define	MsgPlaneWidth	INT(MsgWidth - ScrollBarWidth)
#define MSGWINDOW		LPSTR("MSGWINDOW")
#define WM_APPENDMSG	UINT(WM_APP + 0x00)
#define	MSGWINDOWCOLOR	COLORREF(RGB(255,255,200))

// クラス定義
class CMsgWnd : public CWnd
{
// コンストラクション
public:
	CMsgWnd();

// アトリビュート
public:
	CDC			*m_dcWin;
	CBitmap		*m_bmpWin;
	int			m_nNumRow;
	CFont		m_cFont;
	char		m_msgBuf[MsgMaxRow][MsgColumn + 1];		// メッセージ保存エリア	
	COLORREF	m_msgCol[MsgMaxRow];					// メッセージ色エリア	
	int			m_msgHeadLine;							// メッセージ先頭行
	int			m_msgTailLine;							// メッセージ最終行
	int			m_msgNumLine;							// 現在のメッセージ行
	int			m_msgColumn;							// 現在のメッセージカラム
	int			m_msgDispLine;							// 現在のメッセージ表示行

	// ログ関係
	char		m_szLogPath[MAX_PATH];					// ログ保存パス
	int			m_nDays;								// ログ保存日数
	bool		m_bLog;									// ログ保存有無

// オペレーション
public:
	virtual void Append(LPSTR fmt, ...);				// メッセージをウィンドウに追加
	virtual void Append(COLORREF col, LPSTR fmt, ...);

	// ログ関係
	virtual BOOL SetLogPath(const char *pszPath);		// コンフィグ情報のパスを設定
	virtual const char *GetLogPath(void);				// コンフィグ情報のパスを取得
	virtual void SetLogDays(int day);					// ログ保存日数設定
	virtual int GetLogDays(void);						// ログ保存日数取得
	virtual void SetLog(bool bFlg);						// ログ保存フラグ設定
	virtual bool GetLog(void);							// ログ保存フラグ取得
	virtual void ForceSave(void);						// ウィンドウ終了時の強制書き込み
	virtual void ResetContent(void);					// メッセージのリセット

private:
	void AddMessage(LPSTR, COLORREF);
	void DispMsgLine(void);
	void DrawMsgWindow(CDC *);
	void DrawMsgLine(CDC *, int, LPSTR, COLORREF);
	void SetScrollPos(void);
	void ScrolldwnMsgWin(void);
	void ScrollupMsgWin(void);

	// ログ関係
	void SaveLogFile(void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CMsgWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMsgWnd();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMsgWnd)
	afx_msg LRESULT OnAppendMsg(WPARAM,LPARAM);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_)
