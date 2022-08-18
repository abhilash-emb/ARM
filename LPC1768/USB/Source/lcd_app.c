#include "utils.h"
#include "64x128GLCD.h"


#define MAX_THICKNESS 4

#define NUM_OF_ROWS 64
#define NUM_OF_COL 128
#define NUM_OF_PAGES 8

uint8_t frame_buf[NUM_OF_COL][NUM_OF_PAGES];

enum BRUSH_TYPE
{
	BRUSH_OFF,
	BRUSH_ON,
 	BRUSH_XOR
};

/**
*******************************************************************************
  Function Name : lcd_goto_xy()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void lcd_goto_xy(uint8_t x_address, uint8_t y_address)
{
  uint8_t page_num = x_address/NUM_OF_PAGES;

  LCD_SET_PAGE(PAGE1 + page_num, y_address);
}

/**
*******************************************************************************
  Function Name : put_pixel()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void put_pixel(uint16_t px, uint16_t py, uint8_t brush)
{
  uint8_t data = 0;
  uint16_t page_num = 0;
  
  if((px< NUM_OF_ROWS) && (py < NUM_OF_COL))
  {
    page_num = px / NUM_OF_PAGES;
    data = 1 << (px % 8);

    
    lcd_goto_xy(px,py);
    
    switch(brush)
    {
      case BRUSH_ON:
        frame_buf[py][page_num] |= data;
        break;
        
      case BRUSH_OFF:
        frame_buf[py][page_num] &= ~data;
        break;
        
      case BRUSH_XOR:
        frame_buf[py][page_num] ^= data;
        break;
        
      default:
        break;
    }
    LCD_WRITE_DATA(frame_buf[py][page_num]);
  }
}

/**
*******************************************************************************
  Function Name : lcd_draw_line()

  Description :	 

  Input :	

  Output :	none

  Note : 
*******************************************************************************
*/
void lcd_draw_line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t thickness)
{
	//ABTH
  int16_t dx,dy,dxabs,dyabs,i,px,py,sdx,sdy,x,y;
  uint16_t j = 0;
  
  if(thickness > MAX_THICKNESS)
  {
    thickness = MAX_THICKNESS;
  }
  dx = x2 - x1;
  dy = y2 - y1;
  
  sdx = SIGN(dx);
  sdy = SIGN(dy);
  
  dxabs = ABSOLUTE(dx);
  dyabs = ABSOLUTE(dy);
  
  x = 0; y = 0;
  
  px = x1;
  py = y1;
  
  if(dxabs >= dyabs)
  {
    for(i=0;i <=dxabs;i++)
    {
      y += dyabs;
      if(y >= dxabs)
      {
        y -= dxabs;
        py += sdy;
      }
      for(j = 0; j < thickness; j++)
      {
        put_pixel(px+j,py,BRUSH_ON);
      }
      px +=sdx;
    }
  }
  else
  {
    for(i=0; i<=dyabs;i++)
    {
      x += dxabs;
      if(x >= dyabs)
      {
        x -= dyabs;
        px += sdx;
      }
      for(j = 0; j < thickness; j++)
      {
      	put_pixel(px+j,py,BRUSH_ON);
      }
      py += sdy;
    }
  }
}

/**
*******************************************************************************
  Function Name : lcd_draw_circle()

  Description :	 

  Input :	

  Output :	none

  Note : Bresenham's Circle Algorithm 
         Taken from http://en.wikipedia.org/wiki/Bresenham's_line_algorithm 
*******************************************************************************
*/
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint16_t radius)
{
  int32_t f = 1 - radius;
  uint16_t ddF_x = 0;
  int32_t ddF_y = -2 * radius;
  uint16_t x = 0;
  uint16_t y = radius;
  
  put_pixel(x0, y0 + radius,BRUSH_ON);
  put_pixel(x0, y0 - radius,BRUSH_ON);
  put_pixel(x0 + radius, y0,BRUSH_ON);
  put_pixel(x0 - radius, y0,BRUSH_ON);
  
  while(x < y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;
    put_pixel(x0 + x, y0 + y,BRUSH_ON);
    put_pixel(x0 - x, y0 + y,BRUSH_ON);
    put_pixel(x0 + x, y0 - y,BRUSH_ON);
    put_pixel(x0 - x, y0 - y,BRUSH_ON);
    put_pixel(x0 + y, y0 + x,BRUSH_ON);
    put_pixel(x0 - y, y0 + x,BRUSH_ON);
    put_pixel(x0 + y, y0 - x,BRUSH_ON);
    put_pixel(x0 - y, y0 - x,BRUSH_ON);
  }
}
