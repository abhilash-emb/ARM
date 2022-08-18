#include "64x128GLCD.h"
#include "hardware.h"

extern uint8_t data_table[];
extern unsigned char get_width_of_char(unsigned char charecter);
extern unsigned char get_data_from_data_table(unsigned char charecter, unsigned char col);

uint8_t is_valid_ascii(int8_t key)
{
  uint8_t retval = FALSE;

  if(key >= ' ' && key <= '~')
    retval = TRUE;

  return retval;
}

void lcd_display_char(char charecter)
{
  uint8_t width = 0, index = 0, data = 0;

  width = get_width_of_char(charecter);
  for(index = 0; index < width; index++)
  {
    data = get_data_from_data_table(charecter, index);
    LCD_WRITE_DATA(data);
  }
}

void lcd_display_string(const char *str)
{
  if(str == NULL)
    return;

  while(*str != (uint32_t)NULL)
  {
    lcd_display_char(*str);
    str++;
  }
}
