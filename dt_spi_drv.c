// =============================================================================
//	Copyright (C) 2009-2012, Heartland Data Co, Ltd.  All Rights Reserved.
//	
//	Title:	SPI Driver
//	
// =============================================================================
// =============================================================================
// 	Please customize the code for your environment.
// =============================================================================

//------------------------------------------------------------------------------
//	Desc:	Header for Port Controll
//------------------------------------------------------------------------------
#include "System_setting.h"

//------------------------------------------------------------------------------
//  Macro:	DT_UINT
//	Desc:	Please change Test Point argument type for DT10 Project setting.
//------------------------------------------------------------------------------
#define	DT_UINT unsigned int

// =============================================================================
// 	Don't change the code from here as possible.
// =============================================================================

//==============================================================================
//	Desc:	Test Point ID
//==============================================================================
#define		DT_VARIABLE_BIT			0x02
#define		DT_EVTTRG_BIT			0x08
#define		DT_VARIABLE_FAST_BIT	0x01
#define		DT_EVTTRG_FAST_BIT		0x02


//==============================================================================
//	Macro:	portInit
//	Desc:	Initialize Port Macro
//==============================================================================
static int init = 0;
#define	portInit()													\
	if( init == 0 ){												\
		PF_EEV1_1 = 1; PF_EEV1_2 = 1; PF_EEV1_3 = 1;				\
		init = 1;													\
	}

//------------------------------------------------------------------------------
//	Func: _TP_BusOut
//	Desc: Called by Test Point
//------------------------------------------------------------------------------
void _TP_BusOut( DT_UINT addr, DT_UINT dat )
{
	DT_UINT	i, s;

	portInit();
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	for( i = 0x8000; i != 0; i >>= 1 ){
		PF_EEV1_3 = (dat & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	if( addr != 0 ){
		s = addr >> 1;
		for( i = 1; s != 0; s >>= 1, i <<= 1 );
		for( ; i != 0; i >>= 1 ){
			PF_EEV1_3 = (addr & i) ? 1:0;	/* DATA */
			PF_EEV1_1 = 0;					/* CLK LOW */
			PF_EEV1_1 = 1;					/* CLK HI */
		}
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

//------------------------------------------------------------------------------
//	Func: _TP_MemoryOutput
//	Desc: Called by Variable Test Point
//------------------------------------------------------------------------------
void _TP_MemoryOutput( DT_UINT addr, DT_UINT dat, void *value, DT_UINT size )
{
	DT_UINT	i, s;
	unsigned char *p = (unsigned char *)value;

	portInit();
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	for( i = 0x8000; i != 0; i >>= 1 ){
		PF_EEV1_3 = (dat & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	if( addr != 0 ){
		s = addr >> 1;
		for( i = 1; s != 0; s >>= 1, i <<= 1 );
		for( ; i != 0; i >>= 1 ){
			PF_EEV1_3 = (addr & i) ? 1:0;	/* DATA */
			PF_EEV1_1 = 0;					/* CLK LOW */
			PF_EEV1_1 = 1;					/* CLK HI */
		}
	}
	PF_EEV1_2 = 1;	/* CS HI */

	PF_EEV1_2 = 0;	/* CS LOW */
	if( size >= 256 ) size = 256;
	for( i = 0x80; i != 0; i >>= 1 ){
		PF_EEV1_3 = (size & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	for( ; size != 0; --size, ++p ){
		for( i = 0x80; i != 0; i >>= 1 ){
			PF_EEV1_3 = (*p & i) ? 1:0;	/* DATA */
			PF_EEV1_1 = 0;					/* CLK LOW */
			PF_EEV1_1 = 1;					/* CLK HI */
		}
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

//------------------------------------------------------------------------------
//	Func: _TP_EventTrigger
//	Desc: Called by Event Trigger Test Point
//------------------------------------------------------------------------------
void _TP_EventTrigger( DT_UINT addr, DT_UINT dat )
{
	DT_UINT	i, s;

	portInit();
	dat = ((unsigned short)(dat&0x0FFF))<<4;
	addr |= DT_EVTTRG_BIT;
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	for( i = 0x8000; i != 0; i >>= 1 ){
		PF_EEV1_3 = (dat & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	if( addr != 0 ){
		s = addr >> 1;
		for( i = 1; s != 0; s >>= 1, i <<= 1 );
		for( ; i != 0; i >>= 1 ){
			PF_EEV1_3 = (addr & i) ? 1:0;	/* DATA */
			PF_EEV1_1 = 0;					/* CLK LOW */
			PF_EEV1_1 = 1;					/* CLK HI */
		}
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

//------------------------------------------------------------------------------
//	Func: _TP_BusOutFast
//	Desc: Called by Fast Test Point
//------------------------------------------------------------------------------
void _TP_BusOutFast( DT_UINT step, DT_UINT bit )
{
	DT_UINT	i;

	portInit();
	step = step << 2;
	bit = bit + 2;
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	for( i = 1 << (bit-1); i != 0; i >>= 1 ){
		PF_EEV1_3 = (step & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

//------------------------------------------------------------------------------
//	Func: _TP_MemoryOutputFast
//	Desc: Called by Variable Fast Test Point
//------------------------------------------------------------------------------
void _TP_MemoryOutputFast( DT_UINT step, DT_UINT bit, void *value, DT_UINT size )
{
	DT_UINT	i;
	unsigned char *p = (unsigned char *)value;

	portInit();
	step = (step << 2) + DT_VARIABLE_FAST_BIT;
	bit = bit + 2;
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	for( i = 1 << (bit-1); i != 0; i >>= 1 ){
		PF_EEV1_3 = (step & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	PF_EEV1_2 = 1;	/* CS HI */

	PF_EEV1_2 = 0;	/* CS LOW */
	if( size >= 256 ) size = 256;
	for( i = 0x80; i != 0; i >>= 1 ){
		PF_EEV1_3 = (size & i) ? 1:0;	/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	for( ; size != 0; --size, ++p ){
		for( i = 0x80; i != 0; i >>= 1 ){
			PF_EEV1_3 = (*p & i) ? 1:0;	/* DATA */
			PF_EEV1_1 = 0;					/* CLK LOW */
			PF_EEV1_1 = 1;					/* CLK HI */
		}
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

//------------------------------------------------------------------------------
//	Func: _TP_EventTriggerFast
//	Desc: Called by Event Trigger Fast Test Point
//------------------------------------------------------------------------------
void _TP_EventTriggerFast( DT_UINT dat )
{
	DT_UINT	i;

	portInit();
//	enterCritical();		/* Disable Interrupt */
	PF_EEV1_1 = 1;	/* CLK HI */
	PF_EEV1_2 = 0;	/* CS LOW */
	PF_EEV1_3 = (DT_EVTTRG_FAST_BIT & 0x2) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;								/* CLK LOW */
	PF_EEV1_1 = 1;								/* CLK HI */
	PF_EEV1_3 = (DT_EVTTRG_FAST_BIT & 0x1) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;								/* CLK LOW */
	PF_EEV1_1 = 1;								/* CLK HI */
	PF_EEV1_2 = 1;	/* CS HI */

	PF_EEV1_2 = 0;	/* CS LOW */
	PF_EEV1_3 = (dat & 0x80) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x40) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x20) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x10) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x08) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x04) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x02) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x01) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x8000) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x4000) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x2000) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x1000) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x0800) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x0400) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x0200) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	PF_EEV1_3 = (dat & 0x0100) ? 1:0;	/* DATA */
	PF_EEV1_1 = 0;					/* CLK LOW */
	PF_EEV1_1 = 1;					/* CLK HI */
	for( i = 0x8000; i != 0; i >>= 1 ){
		PF_EEV1_3 = 0;					/* DATA */
		PF_EEV1_1 = 0;					/* CLK LOW */
		PF_EEV1_1 = 1;					/* CLK HI */
	}
	PF_EEV1_2 = 1;	/* CS HI */
//	exitCritical();		/* Enable Interrupt */
}

// =============================================================================

