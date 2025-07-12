/*
 * vfd_driver.h
 *
 * Created: 23.04.2023 16:53:58
 *  Author: Ivan Prints
 */ 

#include <string.h>#include <stdio.h>
#include <stdint.h>


#ifndef VFD_DRIVER_H
#define VFD_DRIVER_H

#define SCREEN_BAUDRATE		19200

#define VFD_CMD				0x1f
#define MOVE_CURSOR			0x24
#define SET_BRIGHTNESS		0x58		
#define SET_ANNUNICATOR		0x23
#define CLEAR_SCREEN		0x0c


typedef struct {
	char first_line[21];
	char second_line[21];
} display;


//extern void parseString(const char* input, display* output);






void spiwrite(uint8_t c);
void vfd_command(uint8_t c);
void vfd_data(uint8_t c);
void vfd_init();
void vfd_string(uint8_t row, uint8_t column, uint8_t *c);
void vfd_set_cursor(uint8_t line, uint8_t position);
void vfd_set_brightness(uint8_t br);
void vfd_set_underline_marker(uint8_t pos, uint8_t value);
#endif  // VFD_DRIVER_H