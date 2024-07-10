// TcVwDlg.h : TC View Screen header file
//
#if !defined(_TCVIEWDLG_H_)
#define _SPEANADLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TC View Screen

//----------------
// �萔
//----------------


//----------------
// �N���X��`
//----------------
class CTcVwDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CTcVwDlg(CWnd* pParent = NULL);			// �W���̃R���X�g���N�^

// �����o�ϐ��i�O������J�j
protected:
	CFont		m_fontPointInfo;
	CDC			m_mdc;
	CBitmap		m_bitmap;
	BOOL		m_bFirst;

// ���\�b�h�i�O�����J�j
public:
	void SetData(int ch, double data1, double data2);			
												// setting data
	void createTcViewFrame(CDC &dc);			// create TC view frame
	void drawSermo(CDC &dc);					// draw sermo view

// �_�C�A���O �f�[�^
public:
	//{{AFX_DATA(CTcVwDlg)
	enum { IDD = IDD_TC_VIEW_DLG };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTcVwDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTcVwDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(_SPEANADLG_H_)
