/*****************************************************************************
* | File      	:   OLED_1in5_rgb.h
* | Author      :   Waveshare team
* | Function    :   1.5inch OLED Module Drive function
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2020-08-17
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __OLED_1IN5_RGB_H
#define __OLED_1IN5_RGB_H		

#include "hw_driver.h"


/********************************************************************************
function:	
		Define the full screen height length of the display
********************************************************************************/


#define OLED_1in5_RGB_WIDTH  128//OLED width
#define OLED_1in5_RGB_HEIGHT 128 //OLED height


#define OLED_CS_0	    gpio_set_pin_level(LCD_CS, 0)
#define OLED_CS_1	    gpio_set_pin_level(LCD_CS, 1)

#define OLED_RST_0	  gpio_set_pin_level(LCD_RST, 0)
#define OLED_RST_1	  gpio_set_pin_level(LCD_RST, 1)

#define OLED_DC_0	    gpio_set_pin_level(LCD_DC ,0)
#define OLED_DC_1	    gpio_set_pin_level(LCD_DC ,1)



void OLED_Reset(void);
void OLED_WriteReg(uint8_t Reg);
void OLED_WriteData(uint8_t Data);
void OLED_InitReg(void);
void OLED_1in5_rgb_Init(void);
void OLED_1in5_rgb_Clear(uint16_t Color);
void OLED_1in5_rgb_Display(const uint16_t *Image);
void OLED_1in5_rgb_Display_Part(const uint16_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void OLED_1in5_DrawPaint(uint16_t x, uint16_t y, uint16_t Color);
void OLED_1in5_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
static void OLED_1in5_SendData_16Bit(uint16_t Data);
uint16_t convertColor(uint16_t color);

#endif  
	 
