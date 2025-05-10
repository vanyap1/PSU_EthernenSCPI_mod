/*****************************************************************************
* | File      	:   OLED_1in5_rgb.c
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
#include "OLED_1in5_rgb.h"
#include "stdio.h"

/*******************************************************************************
function:
            Hardware reset
*******************************************************************************/
void OLED_Reset(void)
{
    OLED_RST_1;
    delay_ms(100);
    OLED_RST_0;
    delay_ms(100);
    OLED_RST_1;
    delay_ms(100);
}

/*******************************************************************************
function:
            Write register address and data
*******************************************************************************/
void OLED_WriteReg(uint8_t Reg)
{

    OLED_DC_0;
    OLED_CS_0;
    EXT_SPI_WriteBuff(&Reg, 1);
    //OLED_CS_1;
}

void OLED_WriteData(uint8_t Data)
{   
    OLED_DC_1;
    OLED_CS_0;
    EXT_SPI_WriteBuff(&Data, 1);
    OLED_CS_1;
}

/*******************************************************************************
function:
        Common register initialization
*******************************************************************************/
void OLED_InitReg(void)
{
	
   OLED_WriteReg(0xfd);                               //set command lock
   OLED_WriteData(0x12);
   OLED_WriteReg(0xfd);                               //set command lock
   OLED_WriteData(0xb1);
   OLED_WriteReg(0xae);                               //Set Sleep mode ON/OFF (AEh / AFh);

   OLED_WriteReg(0xb3);                               //Set Front Clock Divider / Oscillator Frequency
   OLED_WriteData(0xf1);


   OLED_WriteReg(0xca);                               //Set Multiplex Ratio
   OLED_WriteData(0x7f);                                  //127


   OLED_WriteReg(0xa0);                               //Set Re-map & Dual COM Line Mode
   OLED_WriteData(0x74);                                  //h74

   OLED_WriteReg(0x15);                               //Set Column Address
   OLED_WriteData(0x00);                                  //127
   OLED_WriteData(0x7f);

   OLED_WriteReg(0x75);                               //Set Row Address
   OLED_WriteData(0x00);                                  //127
   OLED_WriteData(0x7f);


   OLED_WriteReg(0xa1);                               //Set Display Start Line 96
   //#if Rows = 127
   OLED_WriteData(0x00);
   //#else
   //OLED_WriteData(0x60);                                  //96
   //#endif

   OLED_WriteReg(0xa2);                              //Set Display Offset
   OLED_WriteData(0x00);

   OLED_WriteReg(0xb5);                               //Set GPIO
   OLED_WriteData(0x00);


   OLED_WriteReg(0xab);                               //Set Function selection
   OLED_WriteData(0x01);


   OLED_WriteReg(0xb1);                               //Set Phase Length
   OLED_WriteData(0x32);


   OLED_WriteReg(0xbe);                               //Set VCOMH Voltage
   OLED_WriteData(0x05);

   OLED_WriteReg(0xa6);                               //Set Entire Display OFF

   OLED_WriteReg(0xc1);                               //Set Contrast Current for Color A,B,C
   OLED_WriteData(0x88);                                  //c8
   OLED_WriteData(0xa0);                                  //80  70
   OLED_WriteData(0x88);                                  //c8

   OLED_WriteReg(0xc7);                               //Master Contrast Current Control
   OLED_WriteData(0x0f);

   OLED_WriteReg(0xb8);                               //Gamma Grey Scale                   //
   OLED_WriteData(0x02);
   OLED_WriteData(0x03);
   OLED_WriteData(0x04);
   OLED_WriteData(0x05);
   OLED_WriteData(0x06);
   OLED_WriteData(0x07);
   OLED_WriteData(0x08);
   OLED_WriteData(0x09);
   OLED_WriteData(0x0a);
   OLED_WriteData(0x0b);
   OLED_WriteData(0x0c);
   OLED_WriteData(0x0d);
   OLED_WriteData(0x0e);
   OLED_WriteData(0x0f);
   OLED_WriteData(0x10);
   OLED_WriteData(0x11);
   OLED_WriteData(0x12);
   OLED_WriteData(0x13);
   OLED_WriteData(0x15);
   OLED_WriteData(0x17);
   OLED_WriteData(0x19);
   OLED_WriteData(0x1b);
   OLED_WriteData(0x1d);
   OLED_WriteData(0x1f);
   OLED_WriteData(0x21);
   OLED_WriteData(0x23);
   OLED_WriteData(0x25);
   OLED_WriteData(0x27);
   OLED_WriteData(0x2a);
   OLED_WriteData(0x2d);
   OLED_WriteData(0x30);
   OLED_WriteData(0x33);
   OLED_WriteData(0x36);
   OLED_WriteData(0x39);
   OLED_WriteData(0x3c);
   OLED_WriteData(0x3f);
   OLED_WriteData(0x42);
   OLED_WriteData(0x45);
   OLED_WriteData(0x48);
   OLED_WriteData(0x4c);
   OLED_WriteData(0x50);
   OLED_WriteData(0x54);
   OLED_WriteData(0x58);
   OLED_WriteData(0x5c);
   OLED_WriteData(0x60);
   OLED_WriteData(0x64);
   OLED_WriteData(0x68);
   OLED_WriteData(0x6c);
   OLED_WriteData(0x70);
   OLED_WriteData(0x74);
   OLED_WriteData(0x78);
   OLED_WriteData(0x7d);
   OLED_WriteData(0x82);
   OLED_WriteData(0x87);
   OLED_WriteData(0x8c);
   OLED_WriteData(0x91);
   OLED_WriteData(0x96);
   OLED_WriteData(0x9b);
   OLED_WriteData(0xa0);
   OLED_WriteData(0xa5);
   OLED_WriteData(0xaa);
   OLED_WriteData(0xaf);
   OLED_WriteData(0xb4);


   OLED_WriteReg(0xb4);                               // Set Segment Low Voltage                    //
   OLED_WriteData(0xa0);
   OLED_WriteData(0xb5);
   OLED_WriteData(0x55);

   OLED_WriteReg(0xb6);                               //Set Second Pre-charge period
   OLED_WriteData(0x01);

   OLED_WriteReg(0xaf);
}

/********************************************************************************
function:
            initialization
********************************************************************************/
void OLED_1in5_rgb_Init(void)
{
    //Hardware reset
    OLED_Reset();

    //Set the initialization register
    OLED_InitReg();
    delay_ms(200);

    //Turn on the OLED display
    OLED_WriteReg(0xAF);
}

/********************************************************************************
function:
            Clear screen
********************************************************************************/
void OLED_1in5_rgb_Clear(uint16_t Color)
{
    uint16_t i;

    OLED_WriteReg(0x15);
    OLED_WriteData(0);
    OLED_WriteData(127);
    OLED_WriteReg(0x75);
    OLED_WriteData(0);
    OLED_WriteData(127);
    // fill!
    OLED_WriteReg(0x5C);
	OLED_DC_1;
	OLED_CS_0;
    Color = convertColor(Color);
	uint16_t tmpData[OLED_1in5_RGB_HEIGHT];
    for (i = 0; i < OLED_1in5_RGB_HEIGHT; i++) {
	    tmpData[i] = Color;
	    //COLOR mode - nnnBBBBnRRRRGGGG
    }
    for (i = 0; i < OLED_1in5_RGB_WIDTH; i++) {
	    EXT_SPI_WriteBuff((uint8_t*)tmpData, OLED_1in5_RGB_HEIGHT * 2);
    }
    
    OLED_CS_1;
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_rgb_Display(const uint16_t *Image)
{
    uint16_t i, j, temp;

    OLED_WriteReg(0x15);
    OLED_WriteData(0);
    OLED_WriteData(127);
    OLED_WriteReg(0x75);
    OLED_WriteData(0);
    OLED_WriteData(127);
    // fill!
    OLED_WriteReg(0x5C);
	
    for(i=0; i<OLED_1in5_RGB_HEIGHT; i++)
        for(j=0; j<OLED_1in5_RGB_WIDTH*2; j++)
        {
            temp = Image[j + i*256];
            OLED_WriteData(temp);
			
        }
}

/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED_1in5_rgb_Display_Part(const uint16_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    uint16_t i, j, temp;

    OLED_WriteReg(0x15);
    OLED_WriteData(Xstart);
    OLED_WriteData(Xend-1);
    OLED_WriteReg(0x75);
    OLED_WriteData(Ystart);
    OLED_WriteData(Yend-1);
    // fill!
    OLED_WriteReg(0x5C);   
    
    for(i=0; i<Yend-Ystart; i++)
        for(j=0; j<(Xend-Xstart)*2; j++)
        {
            temp = Image[j + i*(Xend-Xstart)*2];
            OLED_WriteData(temp);
        }
}

void OLED_1in5_DrawPaint(uint16_t x, uint16_t y, uint16_t Color)
{
	OLED_1in5_SetWindows(x,y,x,y);
	OLED_1in5_SendData_16Bit(Color);
}


void OLED_1in5_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend){
	OLED_WriteReg(0x15);
	OLED_WriteData(Xstart);
	OLED_WriteData(Xend-1);
	OLED_WriteReg(0x75);
	OLED_WriteData(Ystart);
	OLED_WriteData(Yend-1);
}
void OLED_1in5_SendData_16Bit(uint16_t Data){
	OLED_WriteReg(0x5C);
	OLED_DC_1;
	OLED_CS_0;
	//EXT_SPI_WriteBuff(&Data, 2);
	EXT_SPI_write_Byte(Data >>8);
	EXT_SPI_write_Byte(Data & 0xff);
	OLED_CS_1;
	
}

uint16_t convertColor(uint16_t color) {
	uint8_t blue = (color & 0x001F) >> 1;  // b4..b1 (?????? 4 ????)
	uint8_t red = (color & 0xF800) >> 12; // r4..r1 (?????? 4 ????)
	uint8_t green = (color & 0x07E0) >> 7; // g5..g2 (?????? 4 ????)
	uint16_t newColor = (blue << 12) | (red << 8) | (green << 4);
	return newColor;
}