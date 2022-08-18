/**************************************************************************/
/*! 
    @file     ST7783.h
    @author   K. Townsend (microBuilder.eu)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
/*#ifndef __ST7783_H__
#define __ST7783_H__*/

/* Pin connections on Hplus Ex board 
P2.0 - P2.7 --> data port
CS ->P0.4, 
RS->p0.5, 
WR->P0.6, 
RD->p0.7 
RST-> p0.8 */

//#include "projectconfig.h"

#include "lcd.h"
#include "lpc17xx_gpio.h"

//ABTH start
//CHG
#define ST7783_CS_PORT         0     // CS (LCD Pin 7)
#define ST7783_CS_PIN          4
#define ST7783_CD_PORT         0     // CS/RS (LCD Pin 8)
#define ST7783_CD_PIN          5
#define ST7783_WR_PORT         0     // WR (LCD Pin 9)
#define ST7783_WR_PIN          6
#define ST7783_RD_PORT         0     // RD (LCD Pin 10)
#define ST7783_RD_PIN          7

//CHG
#define ST7783_CS_CD_PINS        0x30    // 4 + 5
#define ST7783_RD_WR_PINS        0xC0    // 6 + 7
#define ST7783_WR_CS_PINS        0x50    // 6 + 4
#define ST7783_CD_RD_WR_PINS     0xD0    // 4 + 6 + 7
#define ST7783_CS_CD_RD_WR_PINS  0xF0    // 4 + 5 + 6 + 7


// Backlight and Reset pins
#define ST7783_RES_PORT        0    // LCD Reset  (LCD Pin 31)
#define ST7783_RES_PIN         8

// as of now don't know which pin it is - so not connected
/*#define ST7783_BL_PORT         2     // Backlight Enable (LCD Pin 16)
#define ST7783_BL_PIN          9*/

// Data pins 
// Note: data pins must be consecutive and on the same port
#define ST7783_DATA_PORT       2     // 8-Pin Data Port
#define ST7783_DATA_PIN1       0
#define ST7783_DATA_PIN2       1
#define ST7783_DATA_PIN3       2
#define ST7783_DATA_PIN4       3
#define ST7783_DATA_PIN5       4
#define ST7783_DATA_PIN6       5
#define ST7783_DATA_PIN7       6
#define ST7783_DATA_PIN8       7
#define ST7783_DATA_MASK       0x000000FF
#define ST7783_DATA_OFFSET     0     // Offset = PIN1

// These registers allow fast single operation clear+set of bits (see section 8.5.1 of LPC1343 UM)
#define ST7783_GPIO2DATA_DATA        (*((uint32_t *) (LPC_GPIO2_BASE + (ST7783_DATA_MASK << 2))))
#define ST7783_GPIO1DATA_WR          (*((uint32_t *) (LPC_GPIO0_BASE + ((1 << ST7783_WR_PIN) << 2))))
#define ST7783_GPIO1DATA_CD          (*((uint32_t *) (LPC_GPIO0_BASE + ((1 << ST7783_CD_PIN) << 2))))
#define ST7783_GPIO1DATA_CS          (*((uint32_t *) (LPC_GPIO0_BASE + ((1 << ST7783_CS_PIN) << 2))))
#define ST7783_GPIO1DATA_RD          (*((uint32_t *) (LPC_GPIO0_BASE + ((1 << ST7783_RD_PIN) << 2))))
#define ST7783_GPIO3DATA_RES         (*((uint32_t *) (LPC_GPIO0_BASE + ((1 << ST7783_RES_PIN) << 2))))
#define ST7783_GPIO1DATA_CS_CD       (*((uint32_t *) (LPC_GPIO0_BASE + ((ST7783_CS_CD_PINS) << 2))))
#define ST7783_GPIO1DATA_RD_WR       (*((uint32_t *) (LPC_GPIO0_BASE + ((ST7783_RD_WR_PINS) << 2))))
#define ST7783_GPIO1DATA_WR_CS       (*((uint32_t *) (LPC_GPIO0_BASE + ((ST7783_WR_CS_PINS) << 2))))
#define ST7783_GPIO1DATA_CD_RD_WR    (*((uint32_t *) (LPC_GPIO0_BASE + ((ST7783_CD_RD_WR_PINS) << 2))))
#define ST7783_GPIO1DATA_CS_CD_RD_WR (*((uint32_t *) (LPC_GPIO0_BASE + ((ST7783_CS_CD_RD_WR_PINS) << 2))))

// Macros to set data bus direction to input/output
#define ST7783_GPIO2DATA_SETINPUT  LPC_GPIO2->FIODIR &= ~ST7783_DATA_MASK
#define ST7783_GPIO2DATA_SETOUTPUT LPC_GPIO2->FIODIR |= ST7783_DATA_MASK
//ABTH end
																			
// Macros for control line state
#define CLR_CD          ST7783_GPIO1DATA_CD = (0)
#define SET_CD          ST7783_GPIO1DATA_CD = (1 << ST7783_CD_PIN)
#define CLR_CS          ST7783_GPIO1DATA_CS = (0)
#define SET_CS          ST7783_GPIO1DATA_CS = (1 << ST7783_CS_PIN)
#define CLR_WR          ST7783_GPIO1DATA_WR = (0)
#define SET_WR          ST7783_GPIO1DATA_WR = (1 << ST7783_WR_PIN)
#define CLR_RD          ST7783_GPIO1DATA_RD = (0)
#define SET_RD          ST7783_GPIO1DATA_RD = (1 << ST7783_RD_PIN)
#define CLR_RESET       ST7783_GPIO3DATA_RES = (0)
#define SET_RESET       ST7783_GPIO3DATA_RES = (1 << ST7783_RES_PIN)
/*#define SET_CD          ST7783_GPIO1DATA_CD = (0)
#define CLR_CD          ST7783_GPIO1DATA_CD = (1 << ST7783_CD_PIN)
#define SET_CS          ST7783_GPIO1DATA_CS = (0)
#define CLR_CS          ST7783_GPIO1DATA_CS = (1 << ST7783_CS_PIN)
#define SET_WR          ST7783_GPIO1DATA_WR = (0)
#define CLR_WR          ST7783_GPIO1DATA_WR = (1 << ST7783_WR_PIN)
#define SET_RD          ST7783_GPIO1DATA_RD = (0)
#define CLR_RD          ST7783_GPIO1DATA_RD = (1 << ST7783_RD_PIN)
#define CLR_RESET       ST7783_GPIO3DATA_RES = (0)
#define SET_RESET       ST7783_GPIO3DATA_RES = (1 << ST7783_RES_PIN)*/

// These 'combined' macros are defined to improve code performance by
// reducing the number of instructions in heavily used functions
#define CLR_CS_CD           ST7783_GPIO1DATA_CS_CD = (0);
#define SET_RD_WR           ST7783_GPIO1DATA_RD_WR = (ST7783_RD_WR_PINS);
#define SET_WR_CS           ST7783_GPIO1DATA_WR_CS = (ST7783_WR_CS_PINS);
#define SET_CD_RD_WR        ST7783_GPIO1DATA_CD_RD_WR = (ST7783_CD_RD_WR_PINS);
#define CLR_CS_CD_SET_RD_WR ST7783_GPIO1DATA_CS_CD_RD_WR = (ST7783_RD_WR_PINS);
#define CLR_CS_SET_CD_RD_WR ST7783_GPIO1DATA_CS_CD_RD_WR = (ST7783_CD_RD_WR_PINS);

//#endif
