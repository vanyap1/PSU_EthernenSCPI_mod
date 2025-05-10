/*****************************************************************************
* | File      	:   LCD_1inch28.c
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
#include "LCD_1in28.h"


#include <stdlib.h>		//itoa()
#include <stdio.h>


#define _delay_ms(x) delay_ms((uint32_t)(x))

LCD_1IN28_ATTRIBUTES LCD_1IN28;


/******************************************************************************
function :	Hardware reset
parameter:
******************************************************************************/
static void LCD_1IN28_Reset(void)
{
    LCD_1IN28_RST_1;
	_delay_ms(100);
    LCD_1IN28_RST_0;
    _delay_ms(100);
    LCD_1IN28_RST_1;
    _delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void LCD_1IN28_SendCommand(uint8_t Reg)
{
    LCD_1IN28_DC_0;
    LCD_1IN28_CS_0;
    EXT_SPI_WriteBuff(&Reg,1);
    //LCD_1IN28_CS_1;
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_1IN28_SendData_8Bit(uint8_t Data)
{
    LCD_1IN28_DC_1;
    LCD_1IN28_CS_0;
    EXT_SPI_WriteBuff(&Data,1);
    LCD_1IN28_CS_1;
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void LCD_1IN28_SendData_16Bit(uint16_t Data)
{
    LCD_1IN28_DC_1;
    LCD_1IN28_CS_0;
	EXT_SPI_write_Byte(Data >>8);
	EXT_SPI_write_Byte(Data & 0xff);
	//EXT_SPI_WriteBuff(&Data,2);
    LCD_1IN28_CS_1;
}

/******************************************************************************
function :	Initialize the lcd register
parameter:
******************************************************************************/
static void LCD_1IN28_InitReg(void)
{
	
	LCD_1IN28_SendCommand(0xFE);	//6.4.8. Inter Register Enable1(FEh)			 
	LCD_1IN28_SendCommand(0xEF);	//6.4.9. Inter Register Enable2(EFh)

	LCD_1IN28_SendCommand(0x84);			
	LCD_1IN28_SendData_8Bit(0x40); 
	
	LCD_1IN28_SendCommand(0xB6);	//6.3.3. Display Function Control (B6h)
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x20);

	LCD_1IN28_SendCommand(0x36);	//6.2.18. Memory Access Control(36h)
	LCD_1IN28_SendData_8Bit(0x08);	//Set as vertical screen

	LCD_1IN28_SendCommand(0x3A);	//6.2.22. COLMOD: Pixel Format Set (3Ah)		
	LCD_1IN28_SendData_8Bit(0x05); 

	LCD_1IN28_SendCommand(0x90);			
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08); 

	LCD_1IN28_SendCommand(0xC3);	//6.4.4. Power C ontrol 2 ( C3 h)			
	LCD_1IN28_SendData_8Bit(0x13);
	LCD_1IN28_SendCommand(0xC4);	//6.4.5. Power C ontrol 3 ( C4 h)		
	LCD_1IN28_SendData_8Bit(0x13);

	LCD_1IN28_SendCommand(0xC9);	//6.4.6. Power C ontrol 4 ( C9 h)		
	LCD_1IN28_SendData_8Bit(0x22);

	LCD_1IN28_SendCommand(0xF0);   //6.4.10. SET_GAMMA1 (F0h)
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x09);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x08);
	LCD_1IN28_SendData_8Bit(0x26);
 	LCD_1IN28_SendData_8Bit(0x2A);

 	LCD_1IN28_SendCommand(0xF1);    //6.4.11. SET_GAMMA2 (F1h)
 	LCD_1IN28_SendData_8Bit(0x43);
 	LCD_1IN28_SendData_8Bit(0x70);
 	LCD_1IN28_SendData_8Bit(0x72);
 	LCD_1IN28_SendData_8Bit(0x36);
 	LCD_1IN28_SendData_8Bit(0x37);  
 	LCD_1IN28_SendData_8Bit(0x6F);


 	LCD_1IN28_SendCommand(0xF2);   //6.4.12. SET_GAMMA3 (F2h)
 	LCD_1IN28_SendData_8Bit(0x45);
 	LCD_1IN28_SendData_8Bit(0x09);
 	LCD_1IN28_SendData_8Bit(0x08);
 	LCD_1IN28_SendData_8Bit(0x08);
 	LCD_1IN28_SendData_8Bit(0x26);
 	LCD_1IN28_SendData_8Bit(0x2A);

 	LCD_1IN28_SendCommand(0xF3);   //6.4.13. SET_GAMMA4 (F3h)
 	LCD_1IN28_SendData_8Bit(0x43);
 	LCD_1IN28_SendData_8Bit(0x70);
 	LCD_1IN28_SendData_8Bit(0x72);
 	LCD_1IN28_SendData_8Bit(0x36);
 	LCD_1IN28_SendData_8Bit(0x37); 
 	LCD_1IN28_SendData_8Bit(0x6F);

	LCD_1IN28_SendCommand(0xE8);	//6.4.1. Frame Rate (E8h)
	LCD_1IN28_SendData_8Bit(0x34);

	
	LCD_1IN28_SendCommand(0x62);			
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x0D);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xED);
	LCD_1IN28_SendData_8Bit(0x70); 
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x0F);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xEF);
	LCD_1IN28_SendData_8Bit(0x70); 
	LCD_1IN28_SendData_8Bit(0x70);

	LCD_1IN28_SendCommand(0x63);			
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x11);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x70); 
	LCD_1IN28_SendData_8Bit(0x70);
	LCD_1IN28_SendData_8Bit(0x18);
	LCD_1IN28_SendData_8Bit(0x13);
	LCD_1IN28_SendData_8Bit(0x71);
	LCD_1IN28_SendData_8Bit(0xF3);
	LCD_1IN28_SendData_8Bit(0x70); 
	LCD_1IN28_SendData_8Bit(0x70);
	
	LCD_1IN28_SendCommand(0x64);			
	LCD_1IN28_SendData_8Bit(0x28);
	LCD_1IN28_SendData_8Bit(0x29);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x01);
	LCD_1IN28_SendData_8Bit(0xF1);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x07);
	
	LCD_1IN28_SendCommand(0x66);			
	LCD_1IN28_SendData_8Bit(0x3C);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0xCD);
	LCD_1IN28_SendData_8Bit(0x67);
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x45);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	
	LCD_1IN28_SendCommand(0x67);			
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x3C);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x00);
	LCD_1IN28_SendData_8Bit(0x01);
	LCD_1IN28_SendData_8Bit(0x54);
	LCD_1IN28_SendData_8Bit(0x10);
	LCD_1IN28_SendData_8Bit(0x32);
	LCD_1IN28_SendData_8Bit(0x98);

	LCD_1IN28_SendCommand(0x74);			
	LCD_1IN28_SendData_8Bit(0x10);	
	LCD_1IN28_SendData_8Bit(0x85);	
	LCD_1IN28_SendData_8Bit(0x80);
	LCD_1IN28_SendData_8Bit(0x00); 
	LCD_1IN28_SendData_8Bit(0x00); 
	LCD_1IN28_SendData_8Bit(0x4E);
	LCD_1IN28_SendData_8Bit(0x00);					
	
	
  LCD_1IN28_SendCommand(0x98);			
	LCD_1IN28_SendData_8Bit(0x3e);
	LCD_1IN28_SendData_8Bit(0x07);
	
	
	LCD_1IN28_SendCommand(0x35);	//6.2.17. Tearing Effect Line ON (35h)
	LCD_1IN28_SendCommand(0x21);	//6.2.8. Display Inversion ON (21h)

	LCD_1IN28_SendCommand(0x11);	//6.2.4. Sleep Out Mode (11h)
	_delay_ms(120);
	LCD_1IN28_SendCommand(0x29);	//6.2.10. Display ON (29h)
	_delay_ms(20);
}

/********************************************************************************
function:	Set the resolution and scanning method of the screen
parameter:
		Scan_dir:   Scan direction
********************************************************************************/
static void LCD_1IN28_SetAttributes(uint8_t Scan_dir)
{
    //Get the screen scan direction
    LCD_1IN28.SCAN_DIR = Scan_dir;
    uint8_t MemoryAccessReg = 0x08;

    //Get GRAM and LCD width and height
    if(Scan_dir == HORIZONTAL) {
        LCD_1IN28.HEIGHT	= LCD_1IN28_HEIGHT;
        LCD_1IN28.WIDTH   = LCD_1IN28_WIDTH;
        MemoryAccessReg = 0XC8;
    } else {
        LCD_1IN28.HEIGHT	= LCD_1IN28_WIDTH;
        LCD_1IN28.WIDTH   = LCD_1IN28_HEIGHT;
        MemoryAccessReg = 0X68;
    }

    // Set the read / write scan direction of the frame memory
    LCD_1IN28_SendCommand(0x36); //MX, MY, RGB mode
    LCD_1IN28_SendData_8Bit(MemoryAccessReg);	//0x08 set RGB
}

/********************************************************************************
function :	Initialize the lcd
parameter:
********************************************************************************/
void LCD_1IN28_Init(uint8_t Scan_dir)
{

    //Hardware reset
    LCD_1IN28_Reset();

    //Set the resolution and scanning method of the screen
    LCD_1IN28_SetAttributes(Scan_dir);
    
    //Set the initialization register
    LCD_1IN28_InitReg();
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
		Xstart 	:   X direction Start coordinates
		Ystart  :   Y direction Start coordinates
		Xend    :   X direction end coordinates
		Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_1IN28_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    //set the X coordinates
    LCD_1IN28_SendCommand(0x2A);
    LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Xstart);
	  LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Xend);

    //set the Y coordinates
    LCD_1IN28_SendCommand(0x2B);
    LCD_1IN28_SendData_8Bit(0x00);
	  LCD_1IN28_SendData_8Bit(Ystart);
	  LCD_1IN28_SendData_8Bit(0x00);
    LCD_1IN28_SendData_8Bit(Yend);

    LCD_1IN28_SendCommand(0X2C);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void LCD_1IN28_Clear(uint16_t Color)
{
	uint16_t i;
	LCD_1IN28_SetWindows(0, 0, LCD_1IN28_WIDTH - 1, LCD_1IN28_HEIGHT - 1);
	LCD_1IN28_DC_1;
	uint16_t tmpData[LCD_1IN28_HEIGHT];
	for (i = 0; i < LCD_1IN28_HEIGHT; i++) {
		tmpData[i] = Color;
	}
	for (i = 0; i < LCD_1IN28_WIDTH; i++) {
		EXT_SPI_WriteBuff((uint8_t*)tmpData, LCD_1IN28_HEIGHT * 2);
	}
	LCD_1IN28_DC_0;
}


/******************************************************************************
function :	Sends the image buffer in RAM to displays
parameter:
******************************************************************************/
void LCD_1IN28_Display(uint16_t *Image)
{
    uint16_t i,j;
    LCD_1IN28_SetWindows(0, 0, LCD_1IN28_WIDTH-1, LCD_1IN28_HEIGHT-1);
    LCD_1IN28_DC_1;
	//DEV_Digital_Write(DEV_DC_PIN, 1);
    for(i = 0; i < LCD_1IN28_WIDTH; i++){
      for(j = 0; j < LCD_1IN28_HEIGHT; j++){
        EXT_SPI_write_Byte((*Image+i*LCD_1IN28_WIDTH+j)>>8);
        EXT_SPI_write_Byte(*(Image+i*LCD_1IN28_WIDTH+j));
      }
    }
}

void LCD_1IN28_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image)
{
	
	LCD_1IN28_SetWindows(Xstart, Ystart, Xend - 1, Yend - 1);
	delay_ms(1);
	
	LCD_1IN28_DC_1;
	
	uint32_t pixelCount = (Xend - Xstart) * (Yend - Ystart);
	EXT_SPI_WriteBuff((uint8_t*)Image, pixelCount * 2);

	LCD_1IN28_DC_0;
}

/******************************************************************************
function: Draw a point
parameter	:
	    X	: 	Set the X coordinate
	    Y	:	Set the Y coordinate
	  Color :	Set the color
******************************************************************************/
void LCD_1IN28_DrawPaint(uint16_t x, uint16_t y, uint16_t Color)
{
	LCD_1IN28_SetWindows(x,y,x,y);
	LCD_1IN28_SendData_16Bit(Color); 	    
}

/*******************************************************************************
function:
	Setting backlight
parameter	:
	  value : Range 0~1000   Duty cycle is value/1000	
*******************************************************************************/
void LCD_1IN28_SetBackLight(uint16_t Value)
{
	//DEV_Set_PWM(Value);
}

