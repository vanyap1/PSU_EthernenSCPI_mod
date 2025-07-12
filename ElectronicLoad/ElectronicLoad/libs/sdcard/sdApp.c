
/*
 * sdApp.c
 *
 * Created: 18.11.2024 0:18:32
 *  Author: Ivan Prints
 */ 
#include "sdApp.h"
#include <stdio.h>

static FATFS FatFs; // File system object
static FIL Fil;     // File object
static bool file_opened = false; // ???? ?????????? ?????

bool sd_init(void) {
	if (f_mount(&FatFs, "", 0) == FR_OK) {
		printf("SD card mounted successfully.\r\n");
		return true;
		} else {
		printf("SD card mount failed.\r\n");
		return false;
	}
}


bool sd_write_line(const char *filename, const char *buffer, uint32_t length) {
	FIL file;
	UINT bw;

	if (f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE) == FR_OK) {
		if (f_write(&file, buffer, length, &bw) == FR_OK && bw == length) {
			f_close(&file);
			return true;
		}
		f_close(&file);
	}
	printf("Failed to write to file: %s\r\n", filename);
	return false;
}


int sd_read_file_chunk(const char *filename, uint8_t *buffer, uint32_t length, uint32_t *bytes_read) {
	if (!file_opened) {
		if (f_open(&Fil, filename, FA_READ) == FR_OK) {
			file_opened = true;
			printf("File opened: %s\r\n", filename);
			} else {
			printf("Failed to open file: %s\r\n", filename);
			return -1; 
		}
	}

	UINT br;
	if (f_read(&Fil, buffer, length, &br) == FR_OK) {
		*bytes_read = br;
		if (br == 0) { 
			sd_close_file();
			printf("End of file reached: %s\r\n", filename);
		}
		return 0; 
		} else {
		printf("Failed to read file: %s\r\n", filename);
		sd_close_file();
		return -1; 
	}
}

void sd_close_file(void) {
	if (file_opened) {
		f_close(&Fil);
		file_opened = false;
		printf("File closed.\r\n");
	}
}

bool sd_list_files(const char *path) {
	DIR dir;
	FILINFO fno;

	if (f_opendir(&dir, path) == FR_OK) {
		printf("Listing files in directory: %s\r\n", path);
		while (true) {
			if (f_readdir(&dir, &fno) != FR_OK || fno.fname[0] == 0) break; // End of directory
			if (fno.fattrib & AM_DIR) {
				printf("[DIR]  %s\r\n", fno.fname);
				} else {
				printf("[FILE] %s (%lu bytes)\r\n", fno.fname, fno.fsize);
			}
		}
		f_closedir(&dir);
		return true;
		} else {
		printf("Failed to open directory: %s\r\n", path);
		return false;
	}
}

bool sd_print_file(const char *filename) {
	uint8_t buffer[512];
	UINT br;

	if (f_open(&Fil, filename, FA_READ) == FR_OK) {
		printf("Reading file: %s\r\n", filename);
		while (true) {
			if (f_read(&Fil, buffer, sizeof(buffer), &br) != FR_OK || br == 0) break;
			for (UINT i = 0; i < br; i++) {
				putchar(buffer[i]);
			}
		}
		f_close(&Fil);
		return true;
		} else {
		printf("Failed to open file: %s\r\n", filename);
		return false;
	}
}

