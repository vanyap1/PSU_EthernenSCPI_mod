
/*
 * sdApp.h
 *
 * Created: 18.11.2024 0:18:18
 *  Author: Ivan Prints
 */ 
#ifndef SD_APP_H
#define SD_APP_H

#include "ff.h"
#include <stdint.h>
#include <stdbool.h>


bool sd_init(void);

bool sd_write_line(const char *filename, const char *buffer, uint32_t length);

int sd_read_file_chunk(const char *filename, uint8_t *buffer, uint32_t length, uint32_t *bytes_read);

void sd_close_file(void);

bool sd_list_files(const char *path);

bool sd_print_file(const char *filename);

#endif 

