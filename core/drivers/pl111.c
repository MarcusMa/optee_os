/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Bare-metal fireworks example for Cortex-A9 VE FVP model */

/* Screen driver, derived from brot.c */

/* Copyright (C) ARM Limited, 2011-2013. All rights reserved. */

#include <drivers/pl111.h>
#include <platform_config.h>

// Configure LCD for VE platforms
void init_lcd_ve(void)
{
	// VE System Register 32-bit word offsets
	const int VE_SYS_CFG_DATA = (0xA0/4);
	const int VE_SYS_CFG_CTRL = (0xA4/4);

	volatile unsigned int*  ve_sysreg = (unsigned int*) 0x1C010000;  // CS3 + 0x010000

	// Set CLCD clock
	//   SYS_CFG_DATA sets oscillator rate value as 5.4MHz
	//   SYS_CFG_CTRL( start=1 | write=1 | function=1 | site=0 | position=0 | device=1 )
	ve_sysreg[ VE_SYS_CFG_DATA ] = 5400000;
	ve_sysreg[ VE_SYS_CFG_CTRL ] = 0x80000000 | (1<<30) | (1<<20) | (0<<16) | (0<<12) | (1<<0);

	// Set DVI mux for correct MMB
	//   SYS_CFG_CTRL( start=1 | write=1 | function=7 | site=0 | position=0 | device=0 )
	ve_sysreg[ VE_SYS_CFG_DATA ] = 0;  // VE_DVI_MUX
	ve_sysreg[ VE_SYS_CFG_CTRL ] = 0x80000000 | (1<<30) | (7<<20) | (0<<16) | (0<<12) | (0<<0);
}


// Configure PL111 CLCD
void init_pl111(vaddr_t base, unsigned int width, unsigned int height,
		paddr_t frame_base)
{
	volatile unsigned int* clcd  = (unsigned int *)base;

	// PL111 register offsets (32-bit words)
	const int PL111_TIM0 = (0x00/4);
	const int PL111_TIM1 = (0x04/4);
	const int PL111_TIM2 = (0x08/4);
	const int PL111_TIM3 = (0x0C/4);
	const int PL111_UBAS = (0x10/4);
	const int PL111_LBAS = (0x14/4);
	const int PL111_CNTL = (0x18/4);
	const int PL111_IENB = (0x1C/4);

	// Timing number for an 8.4" LCD screen for use on a VGA screen
	unsigned int TIM0_VAL = ( (((width/16)-1)<<2) | (63<<8) | (31<<16) | (63<<8) );
	unsigned int TIM1_VAL = ( (height - 1) | (24<<10) | (11<<16) | (9<<24) );
	unsigned int TIM2_VAL = ( (0x7<<11) | ((width - 1)<<16) | (1<<26) );

	// Program the CLCD controller registers and start the CLCD
	clcd[ PL111_TIM0 ] = TIM0_VAL;
	clcd[ PL111_TIM1 ] = TIM1_VAL;
	clcd[ PL111_TIM2 ] = TIM2_VAL;
	clcd[ PL111_TIM3 ] = 0;
	clcd[ PL111_UBAS ] = frame_base;
	clcd[ PL111_LBAS ] = 0;
	clcd[ PL111_IENB ] = 0;

	// Set the control register: 16BPP 5:6:5, Power OFF
	// clcd[ PL111_CNTL ] = (1<<0) | (6<<1) | (1<<5);
	// 24bpp
	clcd[ PL111_CNTL ] = (1<<0) | (5<<1) | (1<<5);

	// Power ON
	clcd[ PL111_CNTL ] |= (1<<11);
}
