#include "LPC17xx.h"
#include "utils.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lcd.h"
//#include "st7783.h"
#include "alphanumeric.h"
#include "drawing.h"
#include "dejavusans9.h"
#include <string.h>

/*#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library*/

void init_peripherals(void);
void init_UART(void);
void print_str(const char *);

UART_CFG_Type uart_cfg;

int main()
{
	uint16_t c_id;
	char s[10] = {0};
	
	//SystemInit();
	
	GPIO_SetDir(1,BIT29,1);
	GPIO_SetValue(1,BIT29);
	
	
	init_peripherals();
	
	init_UART();
	
	/*LPC_GPIO1->FIODIR    =  BIT29;				// P1.29 = Output 
	LPC_GPIO1->FIOSET 	 =  BIT29;			// Turn-ON LED*/
	/*delay_ms(1000);
	LPC_GPIO1->FIOCLR 	 =  BIT29;			// Turn-OFF LED*/
	/*GPIO_SetDir(1,BIT29,1);
	GPIO_SetValue(1,BIT29);
	//delay_ms(2000);*/
	print_str("Start");
	
	lcdInit();
	
	c_id = lcdGetControllerID();
	u_itoa(c_id,s);
	print_str(s);
	
	lcdTest();
	/*st7783Delay(10);
	st7783Delay(10);
	st7783Delay(10);*/
	
	drawFill(COLOR_WHITE);
	
	drawString(5, 30, COLOR_BLACK, &dejaVuSans9ptFontInfo, "DejaVu Sans 9");
	
	print_str("\nIn While");
	
	//delay_ms(1000);
	GPIO_ClearValue(1,BIT29);
	
	while(1)
	{
		
		
	}
	
	//return 0;
}


void init_peripherals()
{
	//UART 0 Pin configuration
	PINSEL_CFG_Type PinCfg;
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	
	/*LPC_GPIO1->FIODIR    =  BIT29;				// P1.29 = Output 
	LPC_GPIO1->FIOCLR 	 =  BIT29;				// Turn-OFF LED*/
	
}

void init_UART()
{
	uart_cfg.Baud_rate 	= 115200;
	uart_cfg.Databits 	= UART_DATABIT_8;
	uart_cfg.Parity 		= UART_PARITY_NONE;
	uart_cfg.Stopbits		= UART_STOPBIT_1;
	
	UART_Init(LPC_UART0,&uart_cfg);
	UART_TxCmd(LPC_UART0, ENABLE);
}	

void print_str(const char *ip_str)
{
	UART_Send(LPC_UART0,(uint8_t*)ip_str,strlen(ip_str),BLOCKING);
}
