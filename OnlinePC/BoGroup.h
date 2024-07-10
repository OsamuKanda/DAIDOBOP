// ConfigIf.h : Configuration Interface
//
#pragma once
#include "Config.h"


///////////////////////////////////////////////////////////////////////////////
//#define	TCEditNum	32
//#define	TCEditNum	6
#define	TCEditNum	12


// group data
typedef struct
{
	int cnt;							// Number of combination no
	int column;							// Number of column
	int	no[36];							// combination no
}GrInfo;

// âEç∂ÇÃíËã`
typedef enum
{
	idR		= 0,						// âE
	idL		= 1							// ç∂
}NmLR;

// CBoGroupIf class
class CBoGroup
{
public:
	CBoGroup(){};
	~CBoGroup(){};

// member data
public:
	GrInfo		m_stGr[TCEditNum];
	int			m_nGrMax;
	int			m_nChkCombiPtn[36][2];
	int			m_iTCLR[TCEditNum][2];
	
// member function
public:
	void CreateGroup(CConfig &cnf);
//	BOOL IsGroup(int upper, int lower);

private:
	void SetCombiPtn(int n1,int n2,int n3);
	BOOL CheckCombiPtn(int chk1, int chk2);
};

