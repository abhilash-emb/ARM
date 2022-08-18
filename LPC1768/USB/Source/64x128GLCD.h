#ifndef  _64x128CLCD_H_
#define  _64x128CLCD_H_

#include "type.h"

#define LCD_DATA_DIR	   LPC_GPIO2->FIODIR
#define LCD_DATA_SET	   LPC_GPIO2->FIOSET
#define LCD_DATA_CLR	   LPC_GPIO2->FIOCLR

#define LCD_CTRL_DIR	   LPC_GPIO0->FIODIR
#define LCD_CTRL_SET       LPC_GPIO0->FIOSET
#define LCD_CTRL_CLR       LPC_GPIO0->FIOCLR

#define LCD_CTRL_CS_DIR	   LPC_GPIO2->FIODIR
#define LCD_CTRL_CS_SET    LPC_GPIO2->FIOSET
#define LCD_CTRL_CS_CLR    LPC_GPIO2->FIOCLR

#define LCD_D0 (1<<0)			   //glcd 11 pin  ,tx1 ,p2.0
#define LCD_D1 (1<<1)			   //glcd 10 pin,rx1,p2.1
#define LCD_D2 (1<<2)			  //glcd 9 pin	,pwm1.3,p2.2
#define LCD_D3 (1<<3)			  //glcd 8 pin	,pwm1.4,p2.3
#define LCD_D4 (1<<4)			  //glcd 7 pin ,pwm1.5,p2.4
#define LCD_D5 (1<<5)			  //glcd 6 pin ,pwm1.6,p2.5
#define LCD_D6 (1<<6)			 //glcd 5 pin  ,p2.6
#define LCD_D7 (1<<7)			 //glcd 4 pin ,p2.7

#define LCDRW	 (1<<19)			//glcd 13 pin,p0.19
#define LCDRS	 (1<<20)			//14 pin,p0.20
#define LCDCS1B  (1<<8)		   //16 pin ,gnd
//#define LCDRST   (1<<8)		   //15 pin,p0.8
//#define LCDEN    (1<<9)		   //12 pin 


#define LCD_DATA_MASK (LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)
#define LCD_CTRL_MASK (LCDRW | LCDRS)//  | LCDEN )

#define PAGE1	0xB0
#define COL1	0
#define COL2	1
#define COL3	2
#define COL4	4
#define COL5  8
#define COL6  16
#define COL7	32
#define COL8	64




#define		HIGH	1		
#define		LOW		0

void LCD_INIT(void);
void LCD_WRITE_CMD(unsigned char cmd);
uint8_t LCD_READ_CMD(void);
void LCD_WRITE_DATA(unsigned char data);
void LCD_SET_PAGE(unsigned char page,unsigned char col);
void LCD_CLEAR(void);

#endif //_64x128CLCD_H_

