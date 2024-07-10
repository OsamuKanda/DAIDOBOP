#pragma once
#include "Config.h"

///////////////////////////////////////////////////////////////////////////////
// definition
#define	WM_REDRAW	(WM_APP + 0)

typedef	struct {
	int	yPos[NumInput];
} SelDispData;


///////////////////////////////////////////////////////////////////////////////
// CSelGraph class
class CSelGraph : public CWnd
{
	DECLARE_DYNAMIC(CSelGraph)

public:
	CSelGraph();
	virtual ~CSelGraph();

// member data
public:
	CBitmap		m_bitmap;
	int			m_nCombiIdx;
	COLORREF	m_clBase;
	COLORREF	m_clLine;
	COLORREF	m_clRed;
	COLORREF	m_clGreen;

	HGLOBAL		m_hDispData;
	SelDispData	*m_pDispData;
	int			m_nNumDispData;

	CString		m_strTrendOutFilePath;

	//@
	int			m_nOffset;
	int			m_nLength;
	int			m_nShift;
	int			m_nArg1, m_nArg2;

// member function
protected:
	void SetupDispDataBuffer(void);
	void DestroyDispDataBuffer(void);
	void GraphPaint(CDC &dc);
	void ClearChart(CDC &dc);
	void DrawChart(CDC &dc, int upper, int lower);
	int	 ScaleY(int temp);

public:
	void RedrawChart(int nArg1, int nArg2);
	void LoadDispData(LPCTSTR pName, int offset, int length, float fFilterTemp, int shift);
	void DataShiftP(long& shift);
	void DataShiftM(long& shift);

//
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);
};

