// ==============================================================================================================================================
// 
//  ColorComboBox�N���X ( CColorComboBox )	
//	�� MFC�x�[�XCComboBox�N���X�̔h���N���X�ł��B
//  Version      :  1.0
//  FileName     :	ColorComboBox.cpp
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/10/19  1.0			K.Tanno		�V�K(CColorListBox�����j
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
	//	������
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
// CColorComboBox ���b�Z�[�W �n���h��

// ============================================================================
//     �֐���: DrawItem
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if ((int)lpDIS->itemID < 0)
		return;

	int		nMargin	= _TEXT_MARGIN;
	CDC		*pDC	= CDC::FromHandle(lpDIS->hDC);

    // �f�o�C�X�R���e�L�X�g��ۑ�
    pDC->SaveDC();
	//	���X�g�A�C�e�����擾
	LItem	*pItem		= Item((int)lpDIS->itemID);		
	if( !pItem )
		return;
	COLORREF bg			= pItem->cBg;
	COLORREF fg			= pItem->cFg;
    // �R���g���[���̏�Ԃ��I�����ꂽ��Ԃ̏ꍇ
    if( (lpDIS->itemState & ODS_SELECTED) && m_bHilight == TRUE  )
	{
		//	�n�C���C�g�F
		bg = ::GetSysColor( COLOR_HIGHLIGHT );
		fg = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
	}

	CBrush brush(bg);
	// �w�i�F�`��
	pDC->FillRect(&lpDIS->rcItem, &brush);
	//	����
	UINT nFormat = DT_SINGLELINE | pItem->uFlag;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(fg);
	CString	 sItemText;

	CRect rc(lpDIS->rcItem);
	rc.DeflateRect(nMargin, nMargin);

	GetLBText((int)lpDIS->itemID, sItemText);
	//	�e�L�X�g�`��
	pDC->DrawText(sItemText, -1, &rc, nFormat);

	//	�t�H�[�J�X����
//	if((lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS))
//	{
//		CRect rc(lpDIS->rcItem);
//		for(int i = 0; i < nMargin; i++)
//		{
//			pDC->DrawFocusRect(rc);
//		}
//	}
    // �f�o�C�X�R���e�L�X�g�����ɖ߂�
    pDC->RestoreDC( -1 );

}

// ============================================================================
//     �֐���: MeasureItem
//       ���e: 
//		 ����: 
//     �߂�l: 			
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
//     �֐���: OnDestroy
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorComboBox::OnDestroy()
{
	ResetContent();
	CComboBox::OnDestroy();
}

// ============================================================================
//     �֐���: PreSubclassWindow
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorComboBox::PreSubclassWindow() 
{
	// �R���{�{�b�N�X�̃X�^�C�����擾
    DWORD style = GetStyle();

    if( ( style & CBS_OWNERDRAWVARIABLE ) != CBS_OWNERDRAWVARIABLE )
    {
		//	�I�[�i�[�`��L��
        ModifyStyle( 0, CBS_OWNERDRAWVARIABLE );	
    }
    if( ( style & CBS_HASSTRINGS ) != CBS_HASSTRINGS )
    {
		//	������\���L��
		ModifyStyle( 0, CBS_HASSTRINGS );
    }
	CComboBox::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColorComboBox �ǉ��֐�

// ============================================================================
//     �֐���: SetBackColor
//       ���e: �w�i�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorComboBox::SetBackColor(int nIndex, COLORREF clrBackColor, BOOL bRedraw)
{
	//	�R���{�����擾
	LItem *plbi = Item(nIndex);
	if(!plbi)
		return FALSE;
	//	�w�i�F�X�V
	plbi->cBg = clrBackColor;
	if(bRedraw)
		//	�ĕ`��
		RedrawWindow();
	return TRUE;
}
// ============================================================================
//     �֐���: SetTextColor
//       ���e: �e�L�X�g�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorComboBox::SetTextColor(int nIndex, COLORREF clrTextColor, BOOL bRedraw)
{
	//	�R���{�����擾
	LItem *plbi = Item(nIndex);
	if(!plbi)
		return FALSE;
	//	�w�i�F�X�V
	plbi->cFg = clrTextColor;
	if(bRedraw)
		//	�ĕ`��
		RedrawWindow();
	return TRUE;
}
// ============================================================================
//     �֐���: SetFontInfo
//       ���e: �t�H���g�쐬
//		 ����: int			�t�H���g����
//			 : BOOL			�t�H���g����	TRUE - �{�[���h	FALSE - �ʏ�  
//			 : BOOL			�t�H���g�Α�	TRUE - �{�[���h FALSE - �ʏ�
//			 : LPCTSTR		�t�H���g��	
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorComboBox::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
{
	int		nWeight;

	//	�I�u�W�F�N�g�폜
	m_Font.DeleteObject();
	//	����
	if(bBold)
		nWeight = FW_BOLD;
	else
		nWeight = FW_NORMAL;
	//	�t�H���g�쐬
	if(!m_Font.CreateFont( nSize,					// �t�H���g�̍����i�O�̎��́A�����I�ɒ��߂����j
						0,							// �t�H���g�̕��i�O�̎��́A�����I�ɒ��߂����j
						0,							// �X�Ε����ƃf�B�X�v���C�ʂ� x ���Ƃ̊p�x 
						0,							// �����̊���� x ���Ƃ̊p�x
						nWeight,					// �����̑���(�W��)
						bItalic,					// �C�^���b�N
						FALSE,						// ����
						FALSE,						// ��������
						DEFAULT_CHARSET,			// �����Z�b�g
						OUT_DEFAULT_PRECIS,			// �o�͐��x
						CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
						ANTIALIASED_QUALITY,		// �o�͕i��
						DEFAULT_PITCH | FF_SWISS,	// �s�b�`�ƃt�@�~��
						pFontName					// �^�C�v�t�F�C�X��
						) ) 
		return FALSE;
	//	�t�H���g�ݒ�
	SetFont(&m_Font);		
	return TRUE;
}


// ============================================================================
//     �֐���: AddItem
//       ���e: �ŏI�s�֒ǉ�
//		 ����: LPCTSTR		�e�L�X�g
//			 : COLORREF		�w�i�F
//			 : COLORREF		�e�L�X�g�F
//			 : UINT			�����w��
//			 : DWORD		�f�[�^�l
//     �߂�l: int			�ǉ������C���f�b�N�X�l	CB_ERR:���s
// ============================================================================
int CColorComboBox::AddItem(LPCTSTR lpszItem,
						   COLORREF clrBackColor,
						   COLORREF clrTextColor,
						   UINT uFlag,
						   DWORD dwData)
{
	// �ŏI�s�֒ǉ�
	return InsertItem(GetCount(), lpszItem, clrBackColor, clrTextColor, uFlag, dwData);
}


// ============================================================================
//     �֐���: InsertItem
//       ���e: �w��s�֒ǉ�
//		 ����: int			�C���f�b�N�X
//			 : LPCTSTR		�e�L�X�g
//			 : COLORREF		�w�i�F
//			 : COLORREF		�e�L�X�g�F
//			 : UINT			�����w��
//			 : DWORD		�f�[�^�l
//     �߂�l: int			�ǉ������C���f�b�N�X�l	CB_ERR:���s
// ============================================================================
int CColorComboBox::InsertItem(int nIndex,
							  LPCTSTR lpszItem,							  
							  COLORREF clrBackColor,
							  COLORREF clrTextColor,
							  UINT uFlag,
							  DWORD dwData)
{
	int	nRet;
	//	�s��
	if(nIndex < 0 || nIndex > GetCount())
		return CB_ERR;
	//	�s�ǉ�
	if( CB_ERR == ( nRet = InsertString(nIndex, lpszItem) ) )
		//	���s
		return CB_ERR;

//	SetTopIndex( nRet );
	LItem *lbi = new LItem;
	//	�쐬���s
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
//     �֐���: DeleteString
//       ���e: �w��s�폜
//		 ����: int		�C���f�b�N�X�l
//     �߂�l: BOOL		TRUE:����	FALSE:���s
// ============================================================================
BOOL CColorComboBox::DeleteString(int nIndex)
{
	int i = Ix(nIndex);

	if(i == -1)
		return FALSE;
//	int nCurIdx = GetTopIndex();
	//	���X�g���擾
	LItem *plbi = (LItem*)GetItemData(i);
	//	�w��s�폜
	if( CComboBox::DeleteString(i) == CB_ERR || plbi == NULL)
		return FALSE;
	//	���X�g���폜
	delete plbi;
//	SetTopIndex( nCurIdx );
	return TRUE;
}

// ============================================================================
//     �֐���: ResetContent
//       ���e: �S�s�폜
//		 ����: 
//     �߂�l: 
// ============================================================================
void CColorComboBox::ResetContent()
{
	while( GetCount() )
		DeleteString(0);
}

// ============================================================================
//     �֐���: EnableHilight
//       ���e: �I���s�n�C���C�g�\���@�L��/����
//		 ����: BOOL		TRUE:�L��	FALSE:����		
//     �߂�l: 
// ============================================================================
void CColorComboBox::EnableHilight(BOOL nEnable)
{
	m_bHilight = nEnable;
}


// ============================================================================
//     �֐���: Ix
//       ���e: �C���f�b�N�X�`�F�b�N
//		 ����: int	-1 - �I���C���f�b�N�X�擾
//     �߂�l: �C���f�b�N�X
// ============================================================================
int CColorComboBox::Ix(int ix)
{
	if(GetCount() == 0)					// ���ږ���
		return -1;

	if(ix == -1)						// �I���C���f�b�N�X�擾
		return GetCurSel();
	else if(ix < 0 || ix >= GetCount())	// �����C���f�b�N�X
		return -1;
	else
		return ix;
}

// ============================================================================
//     �֐���: Item
//       ���e: �w��C���f�b�N�X���A�h���X�擾
//		 ����: int
//			 : 
//     �߂�l: 
// ============================================================================
CColorComboBox::LItem* CColorComboBox::Item(int ix)
{
	int i = Ix(ix);

	if(i == -1)
		return NULL;

	return (LItem *)GetItemData(i);
}

