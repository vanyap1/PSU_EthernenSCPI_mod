
/*
* http_parser.c
*
* Created: 09.06.2023 13:02:46
*  Author: princ
*/
#include "http_parser.h"


bool extractValue(const char* requestString, const char* key, int* value) {
	const char* keyStart = strstr(requestString, key);
	if (keyStart == NULL) {
		return false;
	}
	const char* valueStart = keyStart + strlen(key) + 1;
	const char* valueEnd = strchr(valueStart, '&');
	if (valueEnd == NULL) {
		valueEnd = strchr(valueStart, ' ');
		if (valueEnd == NULL) {
			return false;
		}
		
	}
	char valueString[10];
	strncpy(valueString, valueStart, valueEnd - valueStart);
	valueString[valueEnd - valueStart] = '\0';
	*value = atoi(valueString);
	return true;
}
bool extractString(const char* requestString, const char* key, char value[]){
	const char* keyStart = strstr(requestString, key);
	if (keyStart == NULL) {
		//printf("Err. KEY %s cant'be find in string '%s'\r\n" , key, requestString);
		return false;
	}

	const char* valueStart = keyStart + strlen(key) + 1;
	const char* valueEnd = strchr(valueStart, '&');
	if (valueEnd == NULL) {
		valueEnd = strchr(valueStart, ' ');
		if (valueEnd == NULL) {
			//printf("Err. KEY incorrect\r\n");
			return false;
		}
		
	}
	strncpy(value, valueStart, valueEnd - valueStart);
	value[valueEnd - valueStart] = '\0';
	return true;
}
