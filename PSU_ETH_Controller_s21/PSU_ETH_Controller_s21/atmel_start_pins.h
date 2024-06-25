/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD20 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define PA02 GPIO(GPIO_PORTA, 2)
#define PA03 GPIO(GPIO_PORTA, 3)
#define MOSI GPIO(GPIO_PORTA, 4)
#define SCK GPIO(GPIO_PORTA, 5)
#define MISO GPIO(GPIO_PORTA, 6)
#define ETH_CS GPIO(GPIO_PORTA, 7)
#define O2 GPIO(GPIO_PORTA, 8)
#define O3 GPIO(GPIO_PORTA, 9)
#define O1 GPIO(GPIO_PORTA, 10)
#define IN3 GPIO(GPIO_PORTA, 11)
#define PA12 GPIO(GPIO_PORTA, 12)
#define PA13 GPIO(GPIO_PORTA, 13)
#define CS2 GPIO(GPIO_PORTA, 15)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define PA18 GPIO(GPIO_PORTA, 18)
#define PA21 GPIO(GPIO_PORTA, 21)
#define GLD GPIO(GPIO_PORTA, 23)
#define ETH_INT GPIO(GPIO_PORTB, 8)
#define SPI_RST GPIO(GPIO_PORTB, 9)
#define IN2 GPIO(GPIO_PORTB, 10)
#define IN1 GPIO(GPIO_PORTB, 11)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB14 GPIO(GPIO_PORTB, 14)
#define PB17 GPIO(GPIO_PORTB, 17)

#endif // ATMEL_START_PINS_H_INCLUDED
