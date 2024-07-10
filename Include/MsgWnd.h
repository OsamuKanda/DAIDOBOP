// MsgWnd.h : �w�b�_�[ �t�@�C��
//
#if !defined(AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_)
#define AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������

/////////////////////////////////////////////////////////////////////////////
// CMsgWnd �E�B���h�E

// �萔
#define	RowH			INT(18)						// �P�s�̍���	
#define	ColumnW			FLOAT(8.2)					// �P�����̕�	
#define	NumRow			INT(40)						// �s��	
#define	MsgColumn		INT(125)					// �P�s�̕�����	
#define	ScrollBarWidth	INT(18)						// �X�N���[���o�[�̕�	
#define	MsgWidth		INT(MsgColumn * ColumnW)	// �\���G���A�̍ő啝	
#define	MsgHeight		INT(RowH * m_nNumRow)		// �\���\�G���A�̕�	
#define	MsgXOffset		INT(0)						// �\���G���A�̍���	
#define	MsgYOffset		INT(0)						// �\���G���A�̈ʒu�w	
#define	MsgMaxRow		INT(500)					// �ۑ��ő�s��	
#define	MsgPlaneWidth	INT(MsgWidth - ScrollBarWidth)
#define MSGWINDOW		LPSTR("MSGWINDOW")
#define WM_APPENDMSG	UINT(WM_APP + 0x00)
#define	MSGWINDOWCOLOR	COLORREF(RGB(255,255,200))

// �N���X��`
class CMsgWnd : public CWnd
{
// �R���X�g���N�V����
public:
	CMsgWnd();

// �A�g���r���[�g
public:
	CDC			*m_dcWin;
	CBitmap		*m_bmpWin;
	int			m_nNumRow;
	CFont		m_cFont;
	char		m_msgBuf[MsgMaxRow][MsgColumn + 1];		// ���b�Z�[�W�ۑ��G���A	
	COLORREF	m_msgCol[MsgMaxRow];					// ���b�Z�[�W�F�G���A	
	int			m_msgHeadLine;							// ���b�Z�[�W�擪�s
	int			m_msgTailLine;							// ���b�Z�[�W�ŏI�s
	int			m_msgNumLine;							// ���݂̃��b�Z�[�W�s
	int			m_msgColumn;							// ���݂̃��b�Z�[�W�J����
	int			m_msgDispLine;							// ���݂̃��b�Z�[�W�\���s

	// ���O�֌W
	char		m_szLogPath[MAX_PATH];					// ���O�ۑ��p�X
	int			m_nDays;								// ���O�ۑ�����
	bool		m_bLog;									// ���O�ۑ��L��

// �I�y���[�V����
public:
	virtual void Append(LPSTR fmt, ...);				// ���b�Z�[�W���E�B���h�E�ɒǉ�
	virtual void Append(COLORREF col, LPSTR fmt, ...);

	// ���O�֌W
	virtual BOOL SetLogPath(const char *pszPath);		// �R���t�B�O���̃p�X��ݒ�
	virtual const char *GetLogPath(void);				// �R���t�B�O���̃p�X���擾
	virtual void SetLogDays(int day);					// ���O�ۑ������ݒ�
	virtual int GetLogDays(void);						// ���O�ۑ������擾
	virtual void SetLog(bool bFlg);						// ���O�ۑ��t���O�ݒ�
	virtual bool GetLog(void);							// ���O�ۑ��t���O�擾
	virtual void ForceSave(void);						// �E�B���h�E�I�����̋�����������
	virtual void ResetContent(void);					// ���b�Z�[�W�̃��Z�b�g

private:
	void AddMessage(LPSTR, COLORREF);
	void DispMsgLine(void);
	void DrawMsgWindow(CDC *);
	void DrawMsgLine(CDC *, int, LPSTR, COLORREF);
	void SetScrollPos(void);
	void ScrolldwnMsgWin(void);
	void ScrollupMsgWin(void);

	// ���O�֌W
	void SaveLogFile(void);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CMsgWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMsgWnd();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MSGWND_H__9A59D843_41EB_4D12_BC5A_E2330D1C4D2C__INCLUDED_)
