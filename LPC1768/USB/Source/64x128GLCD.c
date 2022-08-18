/***********************************************************************/
/*                                                                     */
/*  FILE        :64128E.c                                              */
/*  DATE        :Tue, Sep 18, 2007                                     */
/*  DESCRIPTION :main program file.                                    */
/*  CPU GROUP   :1B                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.8).     */
/*                                                                     */
/***********************************************************************/
#include "utils.h"
#include "LPC17xx.h"
#include "64x128GLCD.h"

#define	HIGH	1
#define	LOW		0

#define LCD_BUSY_FLAG	LCD_D7

void glcd_StrobeEnable(void)
{
//	LCD_CTRL_SET |= LCDEN;
	delay(1);	
//	LCD_CTRL_CLR |= LCDEN;
}

/* writes particular command byte to lcd */
void LCD_WRITE_CMD(unsigned char cmd)
{
	LCD_CTRL_CLR = LCDRS;
//	__asm("NOP");
	delay(1);

	LCD_CTRL_CLR = LCDCS1B;
//	__asm("NOP");
	delay(1);
	
	LCD_CTRL_CLR = LCDRW;
//	__asm("NOP");
	delay(1);
	
    LCD_DATA_CLR = LCD_DATA_MASK;
    LCD_DATA_SET = cmd << 0;
//	__asm("NOP");
	delay(1);
	
	LCD_CTRL_SET = LCDRW;
//	__asm("NOP");
	delay(1);
		
	LCD_CTRL_SET = LCDCS1B;
//	__asm("NOP");
	delay(1);

	LCD_CTRL_SET = LCDRS;
//	__asm("NOP");
	delay(1);

//  delay(10);
#if 0
	LCD_DATA_DIR &= ~(LCD_BUSY_FLAG);
    while(LPC_GPIO2->FIOPIN & LCD_BUSY_FLAG);		/* wait for busy flag to become low */	
	LCD_DATA_DIR |= LCD_DATA_MASK;	
//	asm("NOP");
	delay(1);	
#endif	
}

/* writes particular data byte to lcd */
void LCD_WRITE_DATA(unsigned char data)
{
 	LCD_CTRL_SET = LCDRS;
//	__asm("NOP");
	delay(1);
	
 	LCD_CTRL_CLR = LCDCS1B;
//	__asm("NOP");
	delay(1);
	
 	LCD_CTRL_CLR = LCDRW;
//	__asm("NOP");
	delay(1);
        
    LCD_DATA_CLR = LCD_DATA_MASK;
    LCD_DATA_SET = data << 0;
//	__asm("NOP");
//	delay(100*5);
	delay(1);	
 	
	LCD_CTRL_SET = LCDRW;
//	__asm("NOP");
	delay(1);
		
	LCD_CTRL_SET = LCDCS1B;
//	__asm("NOP");
	delay(1);

 	LCD_CTRL_CLR = LCDRS;	
//	__asm("NOP");
	delay(1);

//	  delay(10);
#if 0
 LCD_DATA_DIR &= ~(LCD_BUSY_FLAG);
    while(LPC_GPIO2->FIOPIN & LCD_BUSY_FLAG);		/* wait for busy flag to become low */	
 LCD_DATA_DIR |= LCD_DATA_MASK;  
// asm("NOP");
 delay(1); 	
#endif 
 }

uint8_t LCD_READ_DATA(void)
{
	uint8_t rd = 0;

 	LCD_CTRL_SET |= LCDRS;
//	__asm("NOP");
	delay(1);
	
 	LCD_CTRL_CLR |= LCDCS1B;
//	__asm("NOP");
	delay(1);
	
 	LCD_CTRL_SET |= LCDRW;
//	__asm("NOP");
	delay(1);
        
	LCD_DATA_DIR &= ~(LCD_DATA_MASK);
//	rd = (FIO0PIN & LCD_DATA_MASK) >> 15;
	LCD_DATA_DIR |= LCD_DATA_MASK;	
//				asm("NOP");
	delay(100*5);
	
 	LCD_CTRL_CLR |= LCDRW;
//	asm("NOP");
	delay(1);
		
 	LCD_CTRL_SET |= LCDCS1B;
//	asm("NOP");
	delay(1);

 	LCD_CTRL_CLR |= LCDRS;	
//	asm("NOP");	
	delay(1);

	return rd;
 }

void LCD_INIT(void)
{	
 //	LCD_CTRL_CLR |= LCDRST;
//    __asm("NOP");
//	delay(1);
 
 //	LCD_CTRL_SET |= LCDRST;
//	__asm("NOP");
//	delay(1);
 
	LCD_WRITE_CMD(0xA0);		// ADC select	
//	__asm("NOP");
	delay(1);
	
	LCD_WRITE_CMD(0xC8);  	// SHL Normal
//	__asm("NOP");
	delay(1);
	
	LCD_WRITE_CMD(0xA3);		// LCD bias
//	__asm("NOP");
	delay(1);
 
	LCD_WRITE_CMD(0x2F);		// Power control
//	__asm("NOP");
	delay(1);
 
 	LCD_WRITE_CMD(0x22);		// reg resistor select
//	__asm("NOP");
	delay(1);
  
	LCD_WRITE_CMD(0x40);		// Initial display line 40
//	__asm("NOP");
	delay(1);
 
	LCD_WRITE_CMD(0xA4);		// Normal display
//	__asm("NOP");
	delay(1);
 
	LCD_WRITE_CMD(0xA6);		// REverce display a7
//	__asm("NOP");
	delay(1);
  
	LCD_WRITE_CMD(0x81);		// Ref vg select mode
//	__asm("NOP");
	delay(1);
	
	LCD_WRITE_CMD(0x3f);		// Ref vg reg select
//	__asm("NOP");
	delay(1);

	LCD_WRITE_CMD(0xAF);		// Display ON
//	__asm("NOP"); 
	delay(1);
	
	LCD_WRITE_CMD(0XB0);		// Set page address
//	__asm("NOP");
	delay(1);
 
	LCD_WRITE_CMD(0x10);		// Set coloumn addr  MSB 
//	__asm("NOP");	 
	delay(1);

	LCD_WRITE_CMD(0x00);		// Set coloumn addr LSB
//	__asm("NOP");
	delay(1);
}
 
/* set the cursor to perticular page and coloumn */
void LCD_SET_PAGE(unsigned char page,unsigned char col)
{
	unsigned char msb,lsb;
	msb		=	(((col & 0xF0) >> 4)| 0x10); //ABTH
	lsb		=	(col & 0x0F);
	LCD_WRITE_CMD(page);
	delay(1);
	LCD_WRITE_CMD(msb);
	delay(1);
	LCD_WRITE_CMD(lsb);
	delay(1);
}

/* clears entire lcd or clears all page*/
void LCD_CLEAR(void)
{
	unsigned char i = 0,j = 0;
	
	LCD_SET_PAGE(PAGE1,COL1);		// initial page, col
		
	for(i = 0; i < 8; i ++)
	{
          for(j = 0; j < 128; j ++)
            LCD_WRITE_DATA(0x00);
          LCD_SET_PAGE(PAGE1 + i + 1 , COL1);	// increment page
	}
	
	LCD_SET_PAGE(PAGE1,COL1);		// initial page,col
	    
}