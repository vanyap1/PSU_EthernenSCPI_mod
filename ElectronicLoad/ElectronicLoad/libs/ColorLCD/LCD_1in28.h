/*****************************************************************************
* | File      	:   LCD_1inch28.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master 
*                and enhance portability
*----------------
* |	This version:   V1.0
* | Date        :   2020-12-06
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __LCD_1IN28_H
#define __LCD_1IN28_H	
	
#include "hw_driver.h"
#include <stdint.h>
#include <stdlib.h>		//itoa()
#include <stdio.h>


#define LCD_1IN28_HEIGHT 240
#define LCD_1IN28_WIDTH 240


#define HORIZONTAL 0
#define VERTICAL   1

#define LCD_1IN28_SetBacklight(Value) DEV_SetBacklight(Value) 

#define LCD_1IN28_CS_0	    gpio_set_pin_level(LCD_CS, 0) 
#define LCD_1IN28_CS_1	    gpio_set_pin_level(LCD_CS, 1)
	                        
#define LCD_1IN28_RST_0	  gpio_set_pin_level(LCD_RST, 0)
#define LCD_1IN28_RST_1	  gpio_set_pin_level(LCD_RST, 1)
	                        
#define LCD_1IN28_DC_0	    gpio_set_pin_level(LCD_DC ,0)
#define LCD_1IN28_DC_1	    gpio_set_pin_level(LCD_DC ,1)  

typedef struct{
	uint16_t WIDTH;
	uint16_t HEIGHT;
	uint8_t SCAN_DIR;
}LCD_1IN28_ATTRIBUTES;
extern LCD_1IN28_ATTRIBUTES LCD_1IN28;

/********************************************************************************
function:	
			Macro definition variable name
********************************************************************************/
void LCD_1IN28_Init(uint8_t Scan_dir);
void LCD_1IN28_Clear(uint16_t Color);
void LCD_1IN28_Display(uint16_t *Image);
void LCD_1IN28_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image);
void LCD_1IN28_DisplayPoint(uint16_t X, uint16_t Y, uint16_t Color);

void LCD_1IN28_DrawPaint(uint16_t x, uint16_t y, uint16_t Color);
void LCD_1IN28_SetBackLight(uint16_t Value);
#endif
