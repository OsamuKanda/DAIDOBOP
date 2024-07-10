// Config.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//
#include "stdafx.h"
#include "BoColor.h"
#include "../include/graphic.h"

BOOL CBoColor::Initialize( void )
{
	// allocate message base window color (wheat). 
	clMsgBase	= RGB(245, 222, 179);

	//// allocate mold drawing color (goldenrod). 
	clMoldColor	= RGB(218, 165, 32);

	// allocate neural network drawing colors. 
//	allocColorTones(NumADChannelDisp, 150, 52, clTones);
	allocColorTones(NumADChannelDisp, 150, 15, clTones);

	return TRUE;
}

