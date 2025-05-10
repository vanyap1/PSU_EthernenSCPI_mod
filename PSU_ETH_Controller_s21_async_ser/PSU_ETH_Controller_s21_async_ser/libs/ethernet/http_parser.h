
/*
 * http_parser.h
 *
 * Created: 09.06.2023 13:02:39
 *  Author: princ
 */ 

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



bool extractValue(const char* requestString, const char* key, int* value);
bool extractString(const char* requestString, const char* key, char value[]);