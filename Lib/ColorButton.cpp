// ==============================================================================================================================================
// 
//  ColorButtonクラス ( CColorButton )	
//	※ MFCベースCButtonクラスの派生クラスです。
//  Version      :  2.0
//  FileName     :	ColorButton.cpp
//
//	History      :
//               :  Date       Ver.			Auther      Revision Item 
//				 : ----------- ---------	----------  ----------------------------- 
//				 : 2003/12/19  1.3.12.19	K.Ohnishi   新規
//				 : 2004/10/15  2.0			K.Ohnishi	ソース整理
// ==============================================================================================================================================

#include <afxwin.h>
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
	m_clrText		= RGB( 0, 0, 0 );
	m_dwOldBkColor	= m_clrBkgnd = ::GetSysColor(COLOR_BTNFACE);
	m_dwBlinkColor1	= RGB( 0, 0, 0 );
	m_dwBlinkColor2 = RGB( 0, 0, 0 );
	m_bBlinkFlag	= FALSE;
	m_uTimerID		= 0;
	m_rcMargin.SetRect( -1, -1, 0, 0 );
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton メッセージ ハンドラ

// ============================================================================
//     関数名: OnTimer
//       内容: タイマ処理
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorButton::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerID )	
	{
		if( m_bBlinkFlag )
		{
			//	点滅色1
			SetBackColor( m_dwBlinkColor1 );
			m_bBlinkFlag = FALSE;
		}
		else
		{
			//	点滅色2
			SetBackColor( m_dwBlinkColor2 );
			m_bBlinkFlag = TRUE;
		}
	}
	CButton::OnTimer(nIDEvent);
}
// ============================================================================
//     関数名: OnDestroy
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorButton::OnDestroy()
{
	if( m_uTimerID != 0 )
		//	タイマ停止
		KillTimer( m_uTimerID );
	CButton::OnDestroy();
}
// ============================================================================
//     関数名: DrawItem
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// DrawItemStruct構造体からFromHandleを取得
    CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
    // デバイスコンテキストを保存
    dc->SaveDC();
    // 再描画する領域を取得
    CRect rc = lpDrawItemStruct->rcItem;
    // 指定された文字の色を設定する(m_clrText)
    dc->SetTextColor( m_clrText );
    // 描画動作が全体の再描画もしくは、
    // 選択状態が変化した場合
    if( lpDrawItemStruct->itemAction &
                  ( ODA_DRAWENTIRE | ODA_SELECT ))
    {
        // DrawFrameControl()メソッドのフラグを設定
        // コントロールの境界線の大きさ、かつ、
        // 押されていないボタン
        UINT flg = DFCS_ADJUSTRECT | DFCS_BUTTONPUSH;
        // コントロールの状態が選択された状態の場合
        if( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            // フラグに押されたボタンのフラグを追加
            flg |= DFCS_PUSHED;
        }
        // コントロールを再描画する範囲を決定
        dc->DrawFrameControl( rc, DFC_BUTTON, flg );
        // 背景色のブラシを作成
        CBrush brush( m_clrBkgnd );
		//	ボタン全体を再描画
        rc.DeflateRect( m_rcMargin );
        // クリックされている状態の場合
        if( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            // 座標を右下にずらす
            rc.OffsetRect( 1, 1 );
        }
        // 指定したブラシで指定範囲を塗りつぶし
        dc->FillRect( rc, &brush );
        // 座標を一時的に覚える
        m_innerRc = rc;
        // 指定されたボタンのテキストを描画する
        CRect rrc( rc );
        CString strtxt;
        // 作成時に指定されたボタンの表面文字の取得
        GetWindowText( strtxt );
        if( strtxt.IsEmpty() != TRUE )
        {
            // バックカラーは変更なし
            dc->SetBkMode( TRANSPARENT );
            // 文字を書く範囲を作成
            CRect drc( rc );
            // 文字を書く
            dc->DrawText( strtxt, &drc, DT_SINGLELINE | DT_CALCRECT );
            // 文字の座標を計算
            CPoint topleft( rc.left + ( rc.Width() - drc.Width()) /2, rc.top + ( rc.Height() - drc.Height())/2 );

            UINT flg = 0;
            // ボタンが非使用の場合
            if( lpDrawItemStruct->itemState & ODS_DISABLED )
            {
                flg |= DSS_DISABLED;
            }
            // 文字を再描画
            dc->DrawState( topleft, CSize(0,0),
                     strtxt, 
                     flg, TRUE, strtxt.GetLength(),
                     (CBrush *)NULL );
        }
    }
    // フォーカスがきたとき
    // もしくはフォーカスがあるときは、
    // 描画する
    if( (lpDrawItemStruct->itemState & ODS_FOCUS) ||
        (lpDrawItemStruct->itemAction & ODA_FOCUS) )
    {
        CRect frc = m_innerRc;
        // システムサイズをシステムから取得し、
        // 描画する座標を計算
        frc.DeflateRect( 
               ::GetSystemMetrics( SM_CXBORDER ), 
               ::GetSystemMetrics( SM_CYBORDER ) );
        // フォーカスの四角枠を描画する
        dc->DrawFocusRect( frc );
    }
    // デバイスコンテキストを元に戻す
    dc->RestoreDC( -1 );
}

// ============================================================================
//     関数名: PreSubclassWindow
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorButton::PreSubclassWindow() 
{
	// ボタンのスタイルを取得
    DWORD style = GetStyle();

    if( ( style & BS_OWNERDRAW ) != BS_OWNERDRAW)
    {
		//	オーナー描画有効
        ModifyStyle( 0, BS_OWNERDRAW);	
    }

	CButton::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColorButton 追加関数

// ============================================================================
//     関数名: SetDrawMargin
//       内容: オーナー描画する領域を変更する
//		 引数: 
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
void CColorButton::SetDrawMargin(CRect& rect)
{
	m_rcMargin = rect;	

}

// ============================================================================
//     関数名: SetBackColor
//       内容: 背景色設定
//		 引数: COLORREF		色値
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
void CColorButton::SetBackColor(COLORREF color)
{
    m_clrBkgnd = color;
    this->RedrawWindow();
}
// ============================================================================
//     関数名: SetTextColor
//       内容: テキスト色設定
//		 引数: COLORREF		色値
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
void CColorButton::SetTextColor(COLORREF color)
{
    m_clrText = color;
    this->RedrawWindow();
}
// ============================================================================
//     関数名: SetFontInfo
//       内容: フォント作成
//		 引数: int			フォント高さ
//			 : BOOL			フォント太さ	TRUE - ボールド	FALSE - 通常  
//			 : BOOL			フォント斜体	TRUE - ボールド FALSE - 通常
//			 : LPCTSTR		フォント名	
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
BOOL CColorButton::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
{
	int		nWeight;

	//	オブジェクト削除
	m_Font.DeleteObject();
	//	太字
	if(bBold)
		nWeight = FW_BOLD;
	else
		nWeight = FW_NORMAL;
	//	フォント作成
	if(!m_Font.CreateFont( nSize,					// フォントの高さ（０の時は、自動的に調節される）
						0,							// フォントの幅（０の時は、自動的に調節される）
						0,							// 傾斜方向とディスプレイ面の x 軸との角度 
						0,							// 文字の基線と x 軸との角度
						nWeight,					// 文字の太さ(標準)
						bItalic,					// イタリック
						FALSE,						// 下線
						FALSE,						// 取り消し線
						DEFAULT_CHARSET,			// 文字セット
						OUT_DEFAULT_PRECIS,			// 出力精度
						CLIP_DEFAULT_PRECIS,		// クリッピング精度
						ANTIALIASED_QUALITY,		// 出力品質
						DEFAULT_PITCH | FF_SWISS,	// ピッチとファミリ
						pFontName					// タイプフェイス名
						) ) 
		return FALSE;
	//	フォント設定
	SetFont(&m_Font);		
	return TRUE;
}

// ============================================================================
//     関数名: BlinkOn
//       内容: テキスト点滅処理	開始
//		 引数: UINT			点滅間隔(msec指定)	1000msec = 1sec  
//			 : COLORREF		点滅色１	例）RGB(255,255,255) - 白
//			 : COLORREF		点滅色２	例）RGB(255,0,0) - 赤
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
BOOL CColorButton::BlinkOn(UINT uTime, COLORREF dwColor1, COLORREF dwColor2)
{
	//	指定色判定
	if( dwColor1 < RGB(0,0,0) || dwColor1 > RGB(255,255,255) )
		return FALSE;
	if( dwColor2 < RGB(0,0,0) || dwColor2 > RGB(255,255,255) )
		return FALSE;

	m_dwBlinkColor1 = dwColor1;
	m_dwBlinkColor2 = dwColor2;
	//	点滅中
	if( m_uTimerID > 0 )
		return TRUE;
	//	現在背景色保持
	m_dwOldBkColor = m_clrBkgnd;
	BlinkOff();
	//	タイマ開始
	if( 0 == (m_uTimerID = SetTimer( 1, uTime, NULL ) ) )		
		return FALSE;

	return TRUE;
}
// ============================================================================
//     関数名: BlinkOff
//       内容: テキスト点滅処理 停止
//		 引数: 
//     戻り値: 
// ============================================================================
void CColorButton::BlinkOff()
{
	//	タイマ停止
	KillTimer( m_uTimerID );
	m_uTimerID = 0;
	//	背景色戻す
	SetBackColor( m_dwOldBkColor );
}


