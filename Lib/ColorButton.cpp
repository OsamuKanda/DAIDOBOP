// ==============================================================================================================================================
// 
//  ColorButton�N���X ( CColorButton )	
//	�� MFC�x�[�XCButton�N���X�̔h���N���X�ł��B
//  Version      :  2.0
//  FileName     :	ColorButton.cpp
//
//	History      :
//               :  Date       Ver.			Auther      Revision Item 
//				 : ----------- ---------	----------  ----------------------------- 
//				 : 2003/12/19  1.3.12.19	K.Ohnishi   �V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
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
// CColorButton ���b�Z�[�W �n���h��

// ============================================================================
//     �֐���: OnTimer
//       ���e: �^�C�}����
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorButton::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerID )	
	{
		if( m_bBlinkFlag )
		{
			//	�_�ŐF1
			SetBackColor( m_dwBlinkColor1 );
			m_bBlinkFlag = FALSE;
		}
		else
		{
			//	�_�ŐF2
			SetBackColor( m_dwBlinkColor2 );
			m_bBlinkFlag = TRUE;
		}
	}
	CButton::OnTimer(nIDEvent);
}
// ============================================================================
//     �֐���: OnDestroy
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorButton::OnDestroy()
{
	if( m_uTimerID != 0 )
		//	�^�C�}��~
		KillTimer( m_uTimerID );
	CButton::OnDestroy();
}
// ============================================================================
//     �֐���: DrawItem
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// DrawItemStruct�\���̂���FromHandle���擾
    CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
    // �f�o�C�X�R���e�L�X�g��ۑ�
    dc->SaveDC();
    // �ĕ`�悷��̈���擾
    CRect rc = lpDrawItemStruct->rcItem;
    // �w�肳�ꂽ�����̐F��ݒ肷��(m_clrText)
    dc->SetTextColor( m_clrText );
    // �`�擮�삪�S�̂̍ĕ`��������́A
    // �I����Ԃ��ω������ꍇ
    if( lpDrawItemStruct->itemAction &
                  ( ODA_DRAWENTIRE | ODA_SELECT ))
    {
        // DrawFrameControl()���\�b�h�̃t���O��ݒ�
        // �R���g���[���̋��E���̑傫���A���A
        // ������Ă��Ȃ��{�^��
        UINT flg = DFCS_ADJUSTRECT | DFCS_BUTTONPUSH;
        // �R���g���[���̏�Ԃ��I�����ꂽ��Ԃ̏ꍇ
        if( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            // �t���O�ɉ����ꂽ�{�^���̃t���O��ǉ�
            flg |= DFCS_PUSHED;
        }
        // �R���g���[�����ĕ`�悷��͈͂�����
        dc->DrawFrameControl( rc, DFC_BUTTON, flg );
        // �w�i�F�̃u���V���쐬
        CBrush brush( m_clrBkgnd );
		//	�{�^���S�̂��ĕ`��
        rc.DeflateRect( m_rcMargin );
        // �N���b�N����Ă����Ԃ̏ꍇ
        if( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            // ���W���E���ɂ��炷
            rc.OffsetRect( 1, 1 );
        }
        // �w�肵���u���V�Ŏw��͈͂�h��Ԃ�
        dc->FillRect( rc, &brush );
        // ���W���ꎞ�I�Ɋo����
        m_innerRc = rc;
        // �w�肳�ꂽ�{�^���̃e�L�X�g��`�悷��
        CRect rrc( rc );
        CString strtxt;
        // �쐬���Ɏw�肳�ꂽ�{�^���̕\�ʕ����̎擾
        GetWindowText( strtxt );
        if( strtxt.IsEmpty() != TRUE )
        {
            // �o�b�N�J���[�͕ύX�Ȃ�
            dc->SetBkMode( TRANSPARENT );
            // �����������͈͂��쐬
            CRect drc( rc );
            // ����������
            dc->DrawText( strtxt, &drc, DT_SINGLELINE | DT_CALCRECT );
            // �����̍��W���v�Z
            CPoint topleft( rc.left + ( rc.Width() - drc.Width()) /2, rc.top + ( rc.Height() - drc.Height())/2 );

            UINT flg = 0;
            // �{�^������g�p�̏ꍇ
            if( lpDrawItemStruct->itemState & ODS_DISABLED )
            {
                flg |= DSS_DISABLED;
            }
            // �������ĕ`��
            dc->DrawState( topleft, CSize(0,0),
                     strtxt, 
                     flg, TRUE, strtxt.GetLength(),
                     (CBrush *)NULL );
        }
    }
    // �t�H�[�J�X�������Ƃ�
    // �������̓t�H�[�J�X������Ƃ��́A
    // �`�悷��
    if( (lpDrawItemStruct->itemState & ODS_FOCUS) ||
        (lpDrawItemStruct->itemAction & ODA_FOCUS) )
    {
        CRect frc = m_innerRc;
        // �V�X�e���T�C�Y���V�X�e������擾���A
        // �`�悷����W���v�Z
        frc.DeflateRect( 
               ::GetSystemMetrics( SM_CXBORDER ), 
               ::GetSystemMetrics( SM_CYBORDER ) );
        // �t�H�[�J�X�̎l�p�g��`�悷��
        dc->DrawFocusRect( frc );
    }
    // �f�o�C�X�R���e�L�X�g�����ɖ߂�
    dc->RestoreDC( -1 );
}

// ============================================================================
//     �֐���: PreSubclassWindow
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorButton::PreSubclassWindow() 
{
	// �{�^���̃X�^�C�����擾
    DWORD style = GetStyle();

    if( ( style & BS_OWNERDRAW ) != BS_OWNERDRAW)
    {
		//	�I�[�i�[�`��L��
        ModifyStyle( 0, BS_OWNERDRAW);	
    }

	CButton::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CColorButton �ǉ��֐�

// ============================================================================
//     �֐���: SetDrawMargin
//       ���e: �I�[�i�[�`�悷��̈��ύX����
//		 ����: 
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
void CColorButton::SetDrawMargin(CRect& rect)
{
	m_rcMargin = rect;	

}

// ============================================================================
//     �֐���: SetBackColor
//       ���e: �w�i�F�ݒ�
//		 ����: COLORREF		�F�l
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
void CColorButton::SetBackColor(COLORREF color)
{
    m_clrBkgnd = color;
    this->RedrawWindow();
}
// ============================================================================
//     �֐���: SetTextColor
//       ���e: �e�L�X�g�F�ݒ�
//		 ����: COLORREF		�F�l
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
void CColorButton::SetTextColor(COLORREF color)
{
    m_clrText = color;
    this->RedrawWindow();
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
BOOL CColorButton::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
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
//     �֐���: BlinkOn
//       ���e: �e�L�X�g�_�ŏ���	�J�n
//		 ����: UINT			�_�ŊԊu(msec�w��)	1000msec = 1sec  
//			 : COLORREF		�_�ŐF�P	��jRGB(255,255,255) - ��
//			 : COLORREF		�_�ŐF�Q	��jRGB(255,0,0) - ��
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorButton::BlinkOn(UINT uTime, COLORREF dwColor1, COLORREF dwColor2)
{
	//	�w��F����
	if( dwColor1 < RGB(0,0,0) || dwColor1 > RGB(255,255,255) )
		return FALSE;
	if( dwColor2 < RGB(0,0,0) || dwColor2 > RGB(255,255,255) )
		return FALSE;

	m_dwBlinkColor1 = dwColor1;
	m_dwBlinkColor2 = dwColor2;
	//	�_�Œ�
	if( m_uTimerID > 0 )
		return TRUE;
	//	���ݔw�i�F�ێ�
	m_dwOldBkColor = m_clrBkgnd;
	BlinkOff();
	//	�^�C�}�J�n
	if( 0 == (m_uTimerID = SetTimer( 1, uTime, NULL ) ) )		
		return FALSE;

	return TRUE;
}
// ============================================================================
//     �֐���: BlinkOff
//       ���e: �e�L�X�g�_�ŏ��� ��~
//		 ����: 
//     �߂�l: 
// ============================================================================
void CColorButton::BlinkOff()
{
	//	�^�C�}��~
	KillTimer( m_uTimerID );
	m_uTimerID = 0;
	//	�w�i�F�߂�
	SetBackColor( m_dwOldBkColor );
}


