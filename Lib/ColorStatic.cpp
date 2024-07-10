// ==============================================================================================================================================
// 
//  ColorStaticクラス ( CColorStatic )	
//	※ MFCベースCStaticクラスの派生クラスです。
//  Version      :  2.0
//  FileName     :	ColorStatic.cpp
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2003/10/16  1.0			K.Ohnishi	新規
//				 : 2004/10/15  2.0			K.Ohnishi	ソース整理
// ==============================================================================================================================================

#include <afxwin.h>
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic
CColorStatic::CColorStatic()
{
	//	初期化
	m_clrText		= RGB(0, 0, 0);
	m_clrBkgnd		= ::GetSysColor( COLOR_BTNFACE );
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	m_uTimerID		= 0;
	m_bBlinkFlag	= FALSE;
	m_clrBlinkColor1 = ::GetSysColor( COLOR_BTNFACE );
	m_clrBlinkColor2 = ::GetSysColor( COLOR_BTNFACE );
}

CColorStatic::~CColorStatic()
{

}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic メッセージ ハンドラ

// ============================================================================
//     関数名: CtlColor
//       内容: 描画処理
//		 引数: 
//     戻り値: 背景ブラシ			
// ============================================================================
HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// テキスト
	pDC->SetTextColor( m_clrText );					
	// テキストの背景
	pDC->SetBkColor( m_clrBkgnd );					
	// コントロールの背景
	return m_brBkgnd;	
}

// ============================================================================
//     関数名: OnTimer
//       内容: タイマ処理
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorStatic::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerID )	
	{
		if( m_bBlinkFlag )
		{
			//	点滅色1
			SetBackColor( m_clrBlinkColor1, TRUE );
			m_bBlinkFlag = FALSE;
		}
		else
		{
			//	点滅色2
			SetBackColor( m_clrBlinkColor2, TRUE );
			m_bBlinkFlag = TRUE;
		}
	}
	CStatic::OnTimer(nIDEvent);
}

// ============================================================================
//     関数名: OnDestroy
//       内容: 
//		 引数: 
//     戻り値: BOOL			
// ============================================================================
void CColorStatic::OnDestroy()
{
	if( m_uTimerID != 0 )
		KillTimer( m_uTimerID );
	CStatic::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CColorStatic 追加関数

// ============================================================================
//     関数名: SetBackColor
//       内容: 背景色設定
//		 引数: COLORREF		カラーコード	例：RGB(255,255,255)
//			 : BOOL			再描画			TRUE - する	FALSE - しない  
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
COLORREF CColorStatic::SetBackColor(COLORREF clrBackColor, BOOL bRedraw)
{
	COLORREF color = m_clrBkgnd;

	m_clrBkgnd = clrBackColor;
	m_clrOldBkColor = m_clrBkgnd;
	if (bRedraw)
	{
		m_brBkgnd.DeleteObject();
		//	ブラシ作成
		m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
		//	再描画
		Invalidate();								
	}
	//	変更する前の背景色を返す
	return color;									
}
// ============================================================================
//     関数名: SetTextColor
//       内容: テキスト色設定
//		 引数: COLORREF		カラーコード	例：RGB(255,255,255)
//			 : BOOL			再描画			TRUE - する	FALSE - しない  
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
COLORREF CColorStatic::SetTextColor(COLORREF clrTextColor, BOOL bRedraw)
{
	COLORREF color = m_clrText;

	m_clrText = clrTextColor;
	if (bRedraw)
		//	再描画
		Invalidate();								
	//	変更する前の前景色を返す
	return color;									
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
BOOL CColorStatic::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
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
BOOL CColorStatic::BlinkOn(UINT uTime, COLORREF dwColor1, COLORREF dwColor2)
{
	//	指定色判定
	if( dwColor1 < RGB(0,0,0) || dwColor1 > RGB(255,255,255) )
		return FALSE;
	if( dwColor2 < RGB(0,0,0) || dwColor2 > RGB(255,255,255) )
		return FALSE;

	m_clrBlinkColor1 = dwColor1;
	m_clrBlinkColor2 = dwColor2;
	//	現在背景色保持
	m_clrOldBkColor = m_clrBkgnd;
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
void CColorStatic::BlinkOff()
{
	//	タイマ停止
	KillTimer( m_uTimerID );
	//	背景色戻す
	SetBackColor( m_clrOldBkColor, TRUE );
}