// MyList.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "MyList.h"
#include ".\mylist.h"


// CMyList

IMPLEMENT_DYNAMIC(CMyList, CListCtrl)
CMyList::CMyList()
{
}

CMyList::~CMyList()
{
}


BEGIN_MESSAGE_MAP(CMyList, CListCtrl)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// CMyList メッセージ ハンドラ
void CMyList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd	*pWnd = GetParent();

	if( pWnd != NULL )
	{
		UINT pos = GetScrollPos(SB_VERT);
		pWnd->PostMessage(WM_VSCROLL, MAKEWPARAM(nSBCode, pos), (LPARAM)pScrollBar);
	}

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
