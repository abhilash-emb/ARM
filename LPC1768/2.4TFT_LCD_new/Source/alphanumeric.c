/**************************************************************************/
/*! 
    @file     alphanumeric.c
    @author   K. Townsend (microBuilder.eu)

    @brief    Shows an alpha-numeric input dialogue

    @section Example

    @code 
    #include "drivers/lcd/tft/dialogues/alphanumeric.h"

    // Print results from an alpha-numeric dialogue
    char* results = alphaShowDialogue();
    printf("%s\r\n", results);

    @endcode

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
#include <string.h>

#include "alphanumeric.h"

#include "LPC17xx.h"
//#include "core/systick/systick.h"
#include "lcd.h"
#include "drawing.h"
//#include "drivers/lcd/tft/touchscreen.h"
#include "dejavusans9.h"
#include "utils.h"

/* This kind of messes with your head, but defining the pixel locations
   this way allows the calculator example to be rendered on another
   TFT LCD with a different screen resolution or orientation without 
   having to rewrite all the individual pixel-level code                  */

#define ALPHA_BTN_SPACING   (5)
#define ALPHA_BTN_WIDTH     ((lcdGetWidth() - (ALPHA_BTN_SPACING * 6)) / 5)
#define ALPHA_KEYPAD_TOP    ((lcdGetHeight() / 6) + (ALPHA_BTN_SPACING * 2))
#define ALPHA_BTN_HEIGHT    (((lcdGetHeight() - ALPHA_KEYPAD_TOP) - (ALPHA_BTN_SPACING * 7)) / 6)
// #define ALPHA_BTN_WIDTH     ((240 - (ALPHA_BTN_SPACING * 6)) / 5)
// #define ALPHA_KEYPAD_TOP    ((320 / 6) + (ALPHA_BTN_SPACING * 2))
// #define ALPHA_BTN_HEIGHT    (((320 - ALPHA_KEYPAD_TOP) - (ALPHA_BTN_SPACING * 7)) / 6)

/* X/Y positions for buttons */
#define ALPHA_ROW1_TOP      (ALPHA_KEYPAD_TOP + ALPHA_BTN_SPACING)
#define ALPHA_ROW2_TOP      (ALPHA_KEYPAD_TOP + ALPHA_BTN_HEIGHT + (ALPHA_BTN_SPACING * 2))
#define ALPHA_ROW3_TOP      (ALPHA_KEYPAD_TOP + ((ALPHA_BTN_HEIGHT * 2) + ((ALPHA_BTN_SPACING) *  3)))
#define ALPHA_ROW4_TOP      (ALPHA_KEYPAD_TOP + ((ALPHA_BTN_HEIGHT * 3) + ((ALPHA_BTN_SPACING) *  4)))
#define ALPHA_ROW5_TOP      (ALPHA_KEYPAD_TOP + ((ALPHA_BTN_HEIGHT * 4) + ((ALPHA_BTN_SPACING) *  5)))
#define ALPHA_ROW6_TOP      (ALPHA_KEYPAD_TOP + ((ALPHA_BTN_HEIGHT * 5) + ((ALPHA_BTN_SPACING) *  6)))
#define ALPHA_COL1_LEFT     (ALPHA_BTN_SPACING)
#define ALPHA_COL2_LEFT     ((ALPHA_BTN_SPACING * 2) + ALPHA_BTN_WIDTH)
#define ALPHA_COL3_LEFT     ((ALPHA_BTN_SPACING * 3) + (ALPHA_BTN_WIDTH * 2))
#define ALPHA_COL4_LEFT     ((ALPHA_BTN_SPACING * 4) + (ALPHA_BTN_WIDTH * 3))
#define ALPHA_COL5_LEFT     ((ALPHA_BTN_SPACING * 5) + (ALPHA_BTN_WIDTH * 4))

/* Control which 'page' is currently shown on the keypad */
static uint8_t alphaPage = 0;

/* Keeps track of the string contents */
static uint8_t alphaString[80];
//static uint8_t *alphaString_ptr;

/* For quick retrieval of button X/Y locqtions */
uint32_t alphaBtnX[5], alphaBtnY[6];

/* Array showing which characters should be displayed on each alphaPage */
/* You can rearrange the keypad by modifying the array contents below   */
/* --------------------    --------------------   --------------------   --------------------
    A   B   C   D BACK      a   b   c   d BACK      .   ,   :   ; BACK     7   8   9     BACK
    E   F   G   H   I       e   f   g   h   i       '   "   (   )          4   5   6            
    J   K   L   M   N       j   k   l   m   n       ?   !   {   }          7   8   9            
    O   P   Q   R   S       o   p   q   r   s       #   &   @   ~          .   0  SPC           
    T   U   V   W SHFT      t   u   v   w SHFT      %   =   /   \  SHFT                  SHFT   
    X   Y   Z  SPC OK       x   y   z  SPC OK       +   -   _  SPC  OK                    OK      
   --------------------    --------------------   --------------------   -------------------- */
char alphaKeys[4][6][5] =  {  {  { 'A', 'B', 'C', 'D', '<' },
                                 { 'E', 'F', 'G', 'H', 'I' },
                                 { 'J', 'K', 'L', 'M', 'N' },
                                 { 'O', 'P', 'Q', 'R', 'S' },
                                 { 'T', 'U', 'V', 'W', '*' },
                                 { 'X', 'Y', 'Z', ' ', '>' }   },

                              {  { 'a', 'b', 'c', 'd', '<' }, 
                                 { 'e', 'f', 'g', 'h', 'i' },
                                 { 'j', 'k', 'l', 'm', 'n' },
                                 { 'o', 'p', 'q', 'r', 's' },
                                 { 't', 'u', 'v', 'w', '*' },
                                 { 'x', 'y', 'z', ' ', '>' }  },

                              {  { '.', ',', ':', ';', '<' }, 
                                 { '\'', '\"', '(', ')', ' ' },
                                 { '?', '!', '{', '}', ' ' },
                                 { '#', '&', '@', '~', ' ' },
                                 { '%', '=', '/', '\\', '*' },
                                 { '+', '-', '_', ' ', '>' }  },

                              {  { '7', '8', '9', ' ', '<' }, 
                                 { '4', '5', '6', ' ', ' ' },
                                 { '1', '2', '3', ' ', ' ' },
                                 { '.', '0', ' ', ' ', ' ' },
                                 { ' ', ' ', ' ', ' ', '*'},
                                 { ' ', ' ', ' ', ' ', '>' }  }  };

/**************************************************************************/
/*! 
    @brief  Renders the UI
*/
/**************************************************************************/
void alphaRenderButton(uint8_t alphaPage, uint8_t col, uint8_t row, bool selected)
{
  char c = alphaKeys[alphaPage][row][col];
  char key[2];
  key[0] = alphaKeys[alphaPage][row][col];
  key[1] = '\0';
  // Handle special characters
  switch (c)
  {
    case '<':
      // Backspace
      drawButton (alphaBtnX[col], alphaBtnY[row], ALPHA_BTN_WIDTH, ALPHA_BTN_HEIGHT, &dejaVuSans9ptFontInfo, 7, "<", selected); 
      break;
    case '*':
      // Page Shift
      drawButton (alphaBtnX[col], alphaBtnY[row], ALPHA_BTN_WIDTH, ALPHA_BTN_HEIGHT, &dejaVuSans9ptFontInfo, 7, "^", selected); 
      break;
    case '>':
      // OK
      drawButton (alphaBtnX[col], alphaBtnY[row], ALPHA_BTN_WIDTH, ALPHA_BTN_HEIGHT, &dejaVuSans9ptFontInfo, 7, "OK", selected); 
      break;
    default:
      // Standard character
      drawButton (alphaBtnX[col], alphaBtnY[row], ALPHA_BTN_WIDTH, ALPHA_BTN_HEIGHT, &dejaVuSans9ptFontInfo, 7, key, selected); 
      break;
  }
}

/**************************************************************************/
/*! 
    @brief  Renders the UI
*/
/**************************************************************************/
void alphaRefreshScreen(void)
{
  uint8_t x, y;

  /* Draw keypad */
  for (y = 0; y < 6; y++)
  {
    for (x = 0; x < 5; x++)
    {
      alphaRenderButton(alphaPage, x, y, FALSE);
    }
  }

  /* Render Text */
  drawRectangleFilled(0, 0, lcdGetWidth() - 1, ALPHA_KEYPAD_TOP - (ALPHA_BTN_SPACING * 2), 0xFFFF);
  drawString(ALPHA_BTN_SPACING, ALPHA_BTN_SPACING, COLOR_BLACK, &dejaVuSans9ptFontInfo, (char *)&alphaString);
}

/**************************************************************************/
/*! 
    @brief  Processes the supplied touch data
*/
/**************************************************************************/
/*char alphaHandleTouchEvent(void)
{
  tsTouchData_t data;
  char result = '\0';
  uint8_t row, col;
  int32_t tsError = -1;

  // Blocking delay until a valie touch event occurs
  while (tsError)
  {
    tsError = tsWaitForEvent(&data, 0);
  }

  // Attempt to convert touch data to char
  if ((data.y < ALPHA_ROW1_TOP) || (data.y > ALPHA_ROW6_TOP + ALPHA_BTN_HEIGHT))
  {
    return result;
  }

  // Get column
  if ((data.x > alphaBtnX[0]) && (data.x < alphaBtnX[0] + ALPHA_BTN_WIDTH))
    col = 0;
  else if ((data.x > alphaBtnX[1]) && (data.x < alphaBtnX[1] + ALPHA_BTN_WIDTH))
    col = 1;
  else if ((data.x > alphaBtnX[2]) && (data.x < alphaBtnX[2] + ALPHA_BTN_WIDTH))
    col = 2;
  else if ((data.x > alphaBtnX[3]) && (data.x < alphaBtnX[3] + ALPHA_BTN_WIDTH))
    col = 3;
  else if ((data.x > ALPHA_COL5_LEFT) && (data.x < ALPHA_COL5_LEFT + ALPHA_BTN_WIDTH))
    col = 4;
  else
    return result;

  // Get row
  if ((data.y > ALPHA_ROW1_TOP) && (data.y < ALPHA_ROW1_TOP + ALPHA_BTN_HEIGHT))
    row = 0;
  else if ((data.y > ALPHA_ROW2_TOP) && (data.y < ALPHA_ROW2_TOP + ALPHA_BTN_HEIGHT))
    row = 1;
  else if ((data.y > ALPHA_ROW3_TOP) && (data.y < ALPHA_ROW3_TOP + ALPHA_BTN_HEIGHT))
    row = 2;
  else if ((data.y > ALPHA_ROW4_TOP) && (data.y < ALPHA_ROW4_TOP + ALPHA_BTN_HEIGHT))
    row = 3;
  else if ((data.y > ALPHA_ROW5_TOP) && (data.y < ALPHA_ROW5_TOP + ALPHA_BTN_HEIGHT))
    row = 4;
  else if ((data.y > ALPHA_ROW6_TOP) && (data.y < ALPHA_ROW6_TOP + ALPHA_BTN_HEIGHT))
    row = 5;
  else
    return result;

  // Match found ... update button and process the results
  alphaRenderButton(alphaPage, col, row, TRUE);
  result = alphaKeys[alphaPage][row][col];
  
  if (result == '<')
  {
    // Trim character if backspace was pressed
    if (alphaString_ptr > alphaString)
    {
      alphaString_ptr--;
      *alphaString_ptr = '\0';
    }
  }
  else if (result == '*')
  {
    // Switch page if the shift button was pressed
    alphaPage++;
    if (alphaPage > 3)
    {
      alphaPage = 0;
    }
    // Update the UI
    alphaRefreshScreen();
  }
  else if (result == '>')
  {
    // OK button
    delay_ms(CFG_TFTLCD_TS_KEYPADDELAY);
    return '>';
  }
  else
  {
    // Add text to string buffer
    *alphaString_ptr++ = result;
  }

  // Brief delay
  delay_ms(CFG_TFTLCD_TS_KEYPADDELAY);

  // Return button to deselected state
  alphaRefreshScreen();
  return result;
}*/

/**************************************************************************/
/*! 
    @brief  Displays the dialogue box and waits for valid user input

    @section Example

    @code 
    #include "drivers/lcd/tft/dialogues/alphanumeric.h"

    // Print results from an alpha-numeric dialogue
    char* results = alphaShowDialogue();
    printf("%s\r\n", results);

    @endcode
*/
/**************************************************************************/
char* alphaShowDialogue(void)
{
  //char result;

  /* These need to be instantiated here since the width and height of 
     the lcd is retrieved dynamically depending on screen orientation */
  alphaBtnX[0] = ALPHA_COL1_LEFT;
  alphaBtnX[1] = ALPHA_COL2_LEFT;
  alphaBtnX[2] = ALPHA_COL3_LEFT;
  alphaBtnX[3] = ALPHA_COL4_LEFT;
  alphaBtnX[4] = ALPHA_COL5_LEFT;
  alphaBtnY[0] = ALPHA_ROW1_TOP;
  alphaBtnY[1] = ALPHA_ROW2_TOP;
  alphaBtnY[2] = ALPHA_ROW3_TOP;
  alphaBtnY[3] = ALPHA_ROW4_TOP;
  alphaBtnY[4] = ALPHA_ROW5_TOP;
  alphaBtnY[5] = ALPHA_ROW6_TOP;

  /* Initialise the string buffer */
  memset(&alphaString[0], 0, sizeof(alphaString));
  //alphaString_ptr = alphaString;
  alphaPage = 0;

  /* Draw the background and render the buttons */
  drawFill(COLOR_WHITE);
  drawRectangleFilled(0, ALPHA_KEYPAD_TOP - ALPHA_BTN_SPACING, lcdGetWidth() - 1, lcdGetHeight() - 1, COLOR_DARKGRAY);
  drawLine(0, (ALPHA_KEYPAD_TOP - ALPHA_BTN_SPACING) + 1, lcdGetWidth() - 1, (ALPHA_KEYPAD_TOP - ALPHA_BTN_SPACING) + 1, COLOR_LIGHTGRAY);
  alphaRefreshScreen();

  /* Capture results until the 'OK' button is pressed */
  while(1)
  {
    /*result = alphaHandleTouchEvent();
    if (result == '>') return (char *)&alphaString;*/
  }
}
