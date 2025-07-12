
/*
 * netHandler.h
 *
 * Created: 29.12.2024 12:50:03
 *  Author: Ivan Prints
 */ 

#ifndef  _NETHANDLER_H_
#define  _NETHANDLER_H_
extern float amp, volt;
extern float ampDMM, voltDMM;
extern float watt;


uint8_t netInit(void);

void netHandler(void);


#endif
