#pragma once
#include "afxwin.h"
#include "NnStruct.h"

// CNnTable ダイアログ
class CNnTable : public CDialog
{
	DECLARE_DYNAMIC(CNnTable)

public:
	CNnTable(CWnd* pParent = NULL);
	virtual ~CNnTable();

// member data
public:
	int	xOffset; 
	int	yOffset; 
	int	remarkWidth; 
	int	rowHeight;
	int	m_nAsnNet;

// user function
public:
	void DrawNNTable(CDC &dc);
	BOOL GetNeuralNetEntry(int x, int y, int *id);

// ダイアログ データ
	enum { IDD = IDD_NNTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy(); 

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_frameNN;
	afx_msg void OnClickedClose();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
