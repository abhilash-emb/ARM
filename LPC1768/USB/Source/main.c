//USB MS device with FAT32 & file read write operations

#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "GLCD_User.h"
#include "hardware.h"
#include "64x128GLCD.h"
#include "lpc17xx_pinsel.h"
#include "ff.h"
#include "usbhost_lpc17xx.h"
#include "usbhost_ms.h"
#include "diskio.h"
#include "lpc17xx_nvic.h"


#include <string.h>

#define USB			1
#define RET_OK 	0

void print_int(uint32_t ip);
void print_str(const char *);
void int_to_decimal(uint32_t tmp);

UART_CFG_Type uart_cfg;
//BYTE Buff[8*512] /* __attribute__ ((aligned (4)))*/;  /* Data buffer */
FATFS Fatfs[_DRIVES];		/* File system object for each logical drive */
BYTE buf_fr[4096] = {0};

//#define LED (1<<29)
/*void blink_led()
{
	LPC_GPIO1->FIOSET 	 =  BIT29;
	delay(150000);
	LPC_GPIO1->FIOCLR 	 =  BIT29;			// Turn-OFF LED
	delay(150000);
}*/

void GPIO_Init(void)
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
	
	// LED init
	LPC_GPIO1->FIODIR    =  BIT29;				// P1.29 = Output 
	LPC_GPIO1->FIOCLR 	 =  BIT29;				// Turn-OFF LED
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

int main()
{
	uint32_t  tmp,p2,br;
	FRESULT fret;
	FATFS *fs;
	FIL fobj;
	BYTE buf_fr[100] = {0};
	static const BYTE ft[] = {0, 12, 16, 32};
	long long usb_size;
	
/*	// DeInit NVIC and SCBNVIC
	NVIC_DeInit();
	NVIC_SCBDeInit();
	NVIC_SetPriorityGrouping(0x05);

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif
*/	
	SystemInit();
	
	GPIO_Init();
	
	init_UART();
		
  delay(100000);
	
	print_str("USB Host FAT32 FS Demo\n");
	print_str("Insert USB Pen Drive\n");
		
	if(RET_OK == disk_initialize(USB))
		print_str("USB Pen Drive Detected !\n");
	else
		print_str("**NO USB Pen Drive**\n");
	
	/*if(RET_OK == disk_ioctl(USB, GET_SECTOR_SIZE, &tmp))
		print_int(tmp);
	if(RET_OK == disk_ioctl(USB, GET_SECTOR_COUNT, &tmp))
		print_int(tmp);
	if(RET_OK == disk_ioctl(USB, GET_BLOCK_SIZE, &tmp))
		print_int(tmp);*/
	
	//experiment with working drive numbers
	if(FR_OK == f_mount(1,&Fatfs[0])) 
		if(FR_OK == f_getfree("1:", (DWORD*)&p2, &fs)) {
			/*print_int(p2);
			print_int(ft[fs->fs_type & 3]);
			print_int(fs->csize*512);
			print_int(fs->n_fats);
			print_int(fs->n_rootdir);
			print_int(fs->fsize);
			print_int(fs->n_fatent - 2);
			print_int(fs->fatbase);
			print_int(fs->dirbase);
			print_int(fs->database);*/
			print_str("Free Space:\t");
			usb_size = (long long)p2*fs->csize*512;
			tmp = (uint32_t)(usb_size/100000000);
			//print_int(((long long)(p2*fs->csize*512)/1000000000)*100);
			int_to_decimal(tmp);
			//print_int(tmp);
			print_str("GB\n");
		}
	
	/*if(FR_OK == f_open(&fobj, "1:abhilash.txt", FA_OPEN_EXISTING | FA_READ))
		print_str("File open successfully\n");
	
	while(1) {
		fret = f_read(&fobj, buf_fr, sizeof (buf_fr), &br);  // Read a chunk of source file
		if (fret || br == 0) break; //error or eof 
	}
	
	print_str((const char *)buf_fr);
		
	if(FR_OK == f_close(&fobj))
		print_str("\nFile closed successfully\n");*/
	
			
	print_str("e to Exit\n");
	
	f_mount(1,NULL);
	
	while(1) 
	{
			if('e' == UART_ReceiveByte(LPC_UART0)) break;
			//blink_led();
	}
	
	LCD_CLEAR(); 		
	UART_DeInit(LPC_UART0);
	
	return 0;
	
}

void print_int(uint32_t ip_int)
{
	char sp_dstr[16];
	u_itoa(ip_int,sp_dstr);
	strcat(sp_dstr,"\n");
	print_str(sp_dstr);
}

void print_str(const char *ip_str)
{
	UART_Send(LPC_UART0,(uint8_t*)ip_str,strlen(ip_str),BLOCKING);
}

void int_to_decimal(uint32_t ip_int)
{
	char sp_dstr[16];
	char *sp_ptr = sp_dstr;
	char dec1,dec2;
		
	u_itoa(ip_int,sp_dstr);
	while(*sp_ptr++);
	sp_ptr -= 2;
	dec1 = *sp_ptr;
	dec2 = *sp_ptr+1;
	*sp_ptr++ = '.';
	*sp_ptr++ = dec1;
	*sp_ptr++ = dec2;
	*sp_ptr = '\0';
	
	print_str(sp_dstr);
	
}

