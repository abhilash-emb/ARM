#include "LPC17xx.h"
#include "hardware.h"
#include "64x128GLCD.h"

/**
*******************************************************************
	Function Name : init_ports()

	Description   :

	Input         : Void

	Output        : None

	Note          :
********************************************************************
*/
void init_ports(void)						
{
  LCD_DATA_DIR |= LCD_DATA_MASK;
  LCD_CTRL_DIR |= LCD_CTRL_MASK;
  LCD_CTRL_CS_DIR |= LCDCS1B;
}


/**
*****************************************************************************
	Function Name : Hardwareinit()

	Description   :

	Input         : Void 

	Output        : None

	Note          :
******************************************************************************
*/
void  HardwareInit(void)
{  
  init_ports();
}




