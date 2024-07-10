// BoColor.h : BO Color
//
#pragma once

#include "../include/Bodef.h"

// CBoColor class
class CBoColor
{
public:
	CBoColor(){};
	~CBoColor(){};

// member data
public:
	COLORREF	clMsgBase;
	COLORREF	clMoldColor;
	COLORREF	clTones[NumInput];
	
// member function
public:
	BOOL Initialize(void);
};

