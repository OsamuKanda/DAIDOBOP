// ==============================================================================================================================================
// 
//  ColorEdit�N���X ( CColorEdit )	
//	�� MFC�x�[�XCEdit�N���X�̔h���N���X�ł��B
//  Version      :  1.0
//  FileName     :	ColorEdit.cpp
//
//	History      :
//               :  Date       Ver.			Auther		Revision Item 
//				 : ----------- ---------	----------	----------------------------- 
//				 : 2004/06/01  1.0			K.Ohnishi	�V�K
//				 : 2004/10/15  2.0			K.Ohnishi	�\�[�X����
// ==============================================================================================================================================

//	�֐��ꗗ ====================================================
//	1.	SetTextColor	-	�e�L�X�g�F��ݒ肵�܂��B
//	2.	SetBackColor	-	�w�i�F��ݒ肵�܂��B
//	3.	SetFontInfo		-	�t�H���g��ݒ肵�܂��B
//	4.	Blink			-	�_�ŏ��������܂��B
//	=============================================================

#include <afxwin.h>
#include "ColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorEdit

CColorEdit::CColorEdit()
{
	//	������
	m_clrText	= RGB(0, 0, 0);
	m_clrBkgnd	= RGB( 255, 255, 255 );
	m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
	m_uTimerID		= 0;
	m_bBlinkFlag	= FALSE;
	m_clrBlinkColor1 = ::GetSysColor( COLOR_BTNFACE );
	m_clrBlinkColor2 = ::GetSysColor( COLOR_BTNFACE );
}

CColorEdit::~CColorEdit()
{

}


BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
	//{{AFX_MSG_MAP(CColorEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorEdit ���b�Z�[�W �n���h��

// ============================================================================
//     �֐���: CtlColor
//       ���e: �`�揈��
//		 ����: 
//     �߂�l: �w�i�u���V			
// ============================================================================
HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// �e�L�X�g
	pDC->SetTextColor( m_clrText );					
	// �e�L�X�g�̔w�i
	pDC->SetBkColor( m_clrBkgnd );					
	// �R���g���[���̔w�i
	return m_brBkgnd;	
}
// ============================================================================
//     �֐���: OnTimer
//       ���e: �^�C�}����
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorEdit::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerID )	
	{
		if( m_bBlinkFlag )
		{
			//	�_�ŐF1
			SetBackColor( m_clrBlinkColor1, TRUE );
			m_bBlinkFlag = FALSE;
		}
		else
		{
			//	�_�ŐF2
			SetBackColor( m_clrBlinkColor2, TRUE );
			m_bBlinkFlag = TRUE;
		}
	}
	CEdit::OnTimer(nIDEvent);
}
// ============================================================================
//     �֐���: OnDestroy
//       ���e: 
//		 ����: 
//     �߂�l: 			
// ============================================================================
void CColorEdit::OnDestroy()
{
	if( m_uTimerID != 0 )
		KillTimer( m_uTimerID );
	CEdit::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CColorEdit �ǉ��֐�

// ============================================================================
//     �֐���: SetBackColor
//       ���e: �w�i�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
COLORREF CColorEdit::SetBackColor(COLORREF clrBackColor, BOOL bRedraw)
{
	COLORREF color = m_clrBkgnd;

	m_clrBkgnd = clrBackColor;
	if (bRedraw)
	{
		m_brBkgnd.DeleteObject();
		//	�u���V�쐬
		m_brBkgnd.CreateSolidBrush(m_clrBkgnd);
		//	�ĕ`��
		Invalidate();								
	}
	//	�ύX����O�̔w�i�F��Ԃ�
	return color;									
}

// ============================================================================
//     �֐���: SetTextColor
//       ���e: �e�L�X�g�F�ݒ�
//		 ����: COLORREF		�J���[�R�[�h	��FRGB(255,255,255)
//			 : BOOL			�ĕ`��			TRUE - ����	FALSE - ���Ȃ�  
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
COLORREF CColorEdit::SetTextColor(COLORREF clrTextColor, BOOL bRedraw)
{
	COLORREF color = m_clrText;

	m_clrText = clrTextColor;
	if (bRedraw)
		//	�ĕ`��
		Invalidate();								
	//	�ύX����O�̑O�i�F��Ԃ�
	return color;									
}
// ============================================================================
//     �֐���: SetFontInfo
//       ���e: �t�H���g�쐬
//		 ����: int			�t�H���g����
//			 : BOOL			�t�H���g����	TRUE - �{�[���h	FALSE - �ʏ�  
//			 : BOOL			�t�H���g�Α�	TRUE - �{�[���h FALSE - �ʏ�
//			 : LPCTSTR		�t�H���g��	<ColorStatic.h��#define��`���Ă���>
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorEdit::SetFontInfo(int nSize, BOOL bBold, BOOL bItalic, LPCTSTR pFontName )
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
//     �֐���: Blink
//       ���e: �e�L�X�g�_�ŏ���
//		 ����: UINT			�_�ŊԊu(msec�w��)	1000msec = 1sec  
//			 : COLORREF		�_�ŐF�P	��jRGB(255,255,255) - ��
//			 : COLORREF		�_�ŐF�Q	��jRGB(255,0,0) - ��
//     �߂�l: BOOL			TRUE:����	FALSE�F���s   
// ============================================================================
BOOL CColorEdit::BlinkOn(UINT uTime, COLORREF clrColor1, COLORREF clrColor2)
{
	//	�w��F����
	if( clrColor1 < RGB(0,0,0) || clrColor1 > RGB(255,255,255) )
		return FALSE;
	if( clrColor2 < RGB(0,0,0) || clrColor2 > RGB(255,255,255) )
		return FALSE;

	m_clrBlinkColor1 = clrColor1;
	m_clrBlinkColor2 = clrColor2;
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
void CColorEdit::BlinkOff()
{
	//	�^�C�}��~
	KillTimer( m_uTimerID );
	//	�w�i�F�߂�
	SetBackColor( m_dwOldBkColor, TRUE );
}



