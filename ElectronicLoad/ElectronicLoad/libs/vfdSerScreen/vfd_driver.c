/*
 * vfd_driver.c
 *
 * Created: 23.04.2023 16:53:37
 *  Author: Ivan Prints
 */ 
#include "hw_driver.h"
#include "vfd_driver.h"

#include <stdio.h>
#include <stdlib.h>



void vfd_data(uint8_t c)
{
	
}


void vfd_init()
{
	vfd_string(1, 1, "                    ");
	vfd_string(2, 1, "                    ");
	//vfd_clear();
		
}

void vfd_set_cursor(uint8_t line, uint8_t position){
	
	uint8_t vfdData[] = {VFD_CMD, MOVE_CURSOR, position, line, 0x0};
	SerialWrite(vfdData);
	delay_ms(5);
	
}

void vfd_set_brightness(uint8_t br){
	
	uint8_t vfdData[] = {VFD_CMD, SET_BRIGHTNESS, br, 0x0};
	SerialWrite(vfdData);
	delay_ms(3);
}

void vfd_set_underline_marker(uint8_t pos, uint8_t value){
	pos = (pos >= 20) ? 20 : pos;
	value = (value >= 1) ? 1 : value;
	uint8_t vfdData[] = {VFD_CMD, SET_ANNUNICATOR, value, pos, 0x0};
	SerialWrite(vfdData);
	delay_ms(3);
	
}

void vfd_clear(){
	uint8_t vfdData[] = {VFD_CMD, CLEAR_SCREEN, 0x0};
	SerialWrite(vfdData);
	delay_ms(3);
}

void vfd_string(uint8_t row, uint8_t column, uint8_t *c){
	vfd_set_cursor(row, column);
	SerialWrite(c);
	SerialWrite("\0");
	delay_ms(3);

}