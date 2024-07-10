// ==============================================================================================================================================
// 
//  ColorComboBoxクラス ( CColorComboBox )	
//	※ MFCベースCComboBoxクラスの派生クラスです。
//  Version      :  1.0
//  FileName     :	ColorComboBox.cpp
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/10/19  1.0			K.Tanno		新規(CColorListBox改造）
// ==============================================================================================================================================

#include <afxwin.h>
#include "ColorComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox
CColorComboBox::CColorComboBox()
{
	//	初期化
	m_iItemHeight		= 0;
	m_bHilight			= FALSE;
}

CColorComboBox::~CColorComboBox()
{

}

BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
	//{{AFX_MSG_MAP(CColorComboBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CColorComboBox メッセージ ハンドラ

// ============================================================================
//     関数名: DrawItem
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((int)lpDIS->itemID < 0)
		return;

	int		nMargin	= _TEXT_MARGIN;
	CDC		*pDC	= CDC::FromHandle(lpDIS->hDC);

    // デバイスコンテキストを保存
    pDC->SaveDC();
	//	リストアイテム情報取得
	LItem	*pItem		= Item((int)lpDIS->itemID);		
	if( !pItem )
		return;
	COLORREF bg			= pItem->cBg;
	COLORREF fg			= pItem->cFg;
    // コントロールの状態が選択された状態の場合
    if( (lpDIS->itemState & ODS_SELECTED) && m_bHilight == TRUE  )
	{
		//	ハイライト色
		bg = ::GetSysColor( COLOR_HIGHLIGHT );
		fg = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
	}

	CBrush brush(bg);
	// 背景色描画
	pDC->FillRect(&lpDIS->rcItem, &brush);
	//	書式
	UINT nFormat = DT_SINGLELINE | pItem->uFlag;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(fg);
	CString	 sItemText;

	CRect rc(lpDIS->rcItem);
	rc.DeflateRect(nMargin, nMargin);

	GetLBText((int)lpDIS->itemID, sItemText);
	//	テキスト描画
	pDC->DrawText(sItemText, -1, &rc, nFormat);

	//	フォーカス処理
//	if((lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS))
//	{
//		CRect rc(lpDIS->rcItem);
//		for(int i = 0; i < nMargin; i++)
//		{
//			pDC->DrawFocusRect(rc);
//		}
//	}
    // デバイスコンテキストを元に戻す
    pDC->RestoreDC( -1 );

}

// ============================================================================
//     関数名: MeasureItem
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	if(m_iItemHeight == 0)
	{
		CClientDC dc(this);
		CRect r;
		dc.DrawText("M", -1, &r, DT_CALCRECT);
		lpMIS->itemHeight = r.Height();
	}
	else
		lpMIS->itemHeight = m_iItemHeight;

}

// ============================================================================
//     関数名: OnDestroy
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorComboBox::OnDestroy()
{
	ResetContent();
	CComboBox::OnDestroy();
}

// ============================================================================
//     関数名: PreSubclassWindow
//       内容: 
//		 引数: 
//     戻り値: 			
// ============================================================================
void CColorComboBox::PreSubclassWindow() 
{
	// コンボボックスのスタイルを取得
    DWORD style = GetStyle();

    if( ( style & CBS_OWNERDRAWVARIABLE ) != CBS_OWNERDRAWVARIABLE )
    {
		//	オーナー描画有効
        ModifyStyle( 0, CBS_OWNERDRAWVARIABLE );	
    }
    if( ( style & CBS_HASSTRINGS ) != CBS_HASSTRINGS )
    {
		//	文字列表示有効
		ModifyStyle( 0, CBS_HASSTRINGS );
    }
	CComboBox::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox 追加関数

// ============================================================================
//     関数名: SetBackColor
//       内容: 背景色設定
//		 引数: COLORREF		カラーコード	例：RGB(255,255,255)
//			 : BOOL			再描画			TRUE - する	FALSE - しない  
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
BOOL CColorComboBox::SetBackColor(int nIndex, COLORREF clrBackColor, BOOL bRedraw)
{
	//	コンボ情報を取得
	LItem *plbi = Item(nIndex);
	if(!plbi)
		return FALSE;
	//	背景色更新
	plbi->cBg = clrBackColor;
	if(bRedraw)
		//	再描画
		RedrawWindow();
	return TRUE;
}
// ============================================================================
//     関数名: SetTextColor
//       内容: テキスト色設定
//		 引数: COLORREF		カラーコード	例：RGB(255,255,255)
//			 : BOOL			再描画			TRUE - する	FALSE - しない  
//     戻り値: BOOL			TRUE:成功	FALSE：失敗   
// ============================================================================
BOOL CColorComboBox::SetTextColor(int nIndex, COLORREF clrTextColor, BOOL bRedraw)
{
	//	コンボ情報を取得
	LItem *plbi = Item(nIndex);
	if(!plbi)
		return FALSE;
	//	背景色更新
	plbi->cFg = clrTextColor;
	if(bRedraw)
		//	再描画
		RedrawWindow();
	return TRUE;
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
BOOL CColorComboBox::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
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
//     関数名: AddItem
//       内容: 最終行へ追加
//		 引数: LPCTSTR		テキスト
//			 : COLORREF		背景色
//			 : COLORREF		テキスト色
//			 : UINT			書式指定
//			 : DWORD		データ値
//     戻り値: int			追加したインデックス値	CB_ERR:失敗
// ============================================================================
int CColorComboBox::AddItem(LPCTSTR lpszItem,
						   COLORREF clrBackColor,
						   COLORREF clrTextColor,
						   UINT uFlag,
						   DWORD dwData)
{
	// 最終行へ追加
	return InsertItem(GetCount(), lpszItem, clrBackColor, clrTextColor, uFlag, dwData);
}


// ============================================================================
//     関数名: InsertItem
//       内容: 指定行へ追加
//		 引数: int			インデックス
//			 : LPCTSTR		テキスト
//			 : COLORREF		背景色
//			 : COLORREF		テキスト色
//			 : UINT			書式指定
//			 : DWORD		データ値
//     戻り値: int			追加したインデックス値	CB_ERR:失敗
// ============================================================================
int CColorComboBox::InsertItem(int nIndex,
							  LPCTSTR lpszItem,							  
							  COLORREF clrBackColor,
							  COLORREF clrTextColor,
							  UINT uFlag,
							  DWORD dwData)
{
	int	nRet;
	//	行数
	if(nIndex < 0 || nIndex > GetCount())
		return CB_ERR;
	//	行追加
	if( CB_ERR == ( nRet = InsertString(nIndex, lpszItem) ) )
		//	失敗
		return CB_ERR;

//	SetTopIndex( nRet );
	LItem *lbi = new LItem;
	//	作成失敗
	if( !lbi )
		return CB_ERR;
	lbi->cBg	= clrBackColor;
	lbi->cFg	= clrTextColor;
	lbi->uFlag  = uFlag;
	lbi->dwData = dwData;

	if( CB_ERR == SetItemData(nIndex, reinterpret_cast<DWORD>(lbi)) )
		return CB_ERR;
	return nRet;
}

// ============================================================================
//     関数名: DeleteString
//       内容: 指定行削除
//		 引数: int		インデックス値
//     戻り値: BOOL		TRUE:成功	FALSE:失敗
// ============================================================================
BOOL CColorComboBox::DeleteString(int nIndex)
{
	int i = Ix(nIndex);

	if(i == -1)
		return FALSE;
//	int nCurIdx = GetTopIndex();
	//	リスト情報取得
	LItem *plbi = (LItem*)GetItemData(i);
	//	指定行削除
	if( CComboBox::DeleteString(i) == CB_ERR || plbi == NULL)
		return FALSE;
	//	リスト情報削除
	delete plbi;
//	SetTopIndex( nCurIdx );
	return TRUE;
}

// ============================================================================
//     関数名: ResetContent
//       内容: 全行削除
//		 引数: 
//     戻り値: 
// ============================================================================
void CColorComboBox::ResetContent()
{
	while( GetCount() )
		DeleteString(0);
}

// ============================================================================
//     関数名: EnableHilight
//       内容: 選択行ハイライト表示　有効/無効
//		 引数: BOOL		TRUE:有効	FALSE:無効		
//     戻り値: 
// ============================================================================
void CColorComboBox::EnableHilight(BOOL nEnable)
{
	m_bHilight = nEnable;
}


// ============================================================================
//     関数名: Ix
//       内容: インデックスチェック
//		 引数: int	-1 - 選択インデックス取得
//     戻り値: インデックス
// ============================================================================
int CColorComboBox::Ix(int ix)
{
	if(GetCount() == 0)					// 項目無し
		return -1;

	if(ix == -1)						// 選択インデックス取得
		return GetCurSel();
	else if(ix < 0 || ix >= GetCount())	// 無効インデックス
		return -1;
	else
		return ix;
}

// ============================================================================
//     関数名: Item
//       内容: 指定インデックス情報アドレス取得
//		 引数: int
//			 : 
//     戻り値: 
// ============================================================================
CColorComboBox::LItem* CColorComboBox::Item(int ix)
{
	int i = Ix(ix);

	if(i == -1)
		return NULL;

	return (LItem *)GetItemData(i);
}

