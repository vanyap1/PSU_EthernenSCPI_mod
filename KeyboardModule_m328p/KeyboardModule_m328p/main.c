#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "uart_hal.h"
#include "gpio_driver.h"

#define SEND_RELEASE_EVENT 0  // 1 - ????????? "E" ??? ???????????, 0 - ??????????
#define LONG_PRESS_THRESHOLD 500  // ?? ??? ??????? ??????????
#define REPEAT_PERIOD 100          // ?????? ??? ??????? ??????? ??????????

static FILE mystdout = FDEV_SETUP_STREAM((void *)uart_send_byte, NULL, _FDEV_SETUP_WRITE);

// ??????
typedef struct {
	gpio* pin;
	char symbol;
} Button;

gpio debugLed = {(uint8_t *)&PORTB , PORTB5};
gpio btnU     = {(uint8_t *)&PORTD , PORTD4};
gpio btnD     = {(uint8_t *)&PORTD , PORTD3};
gpio btnR     = {(uint8_t *)&PORTD , PORTD6};
gpio btnL     = {(uint8_t *)&PORTD , PORTD5};
gpio btnSET   = {(uint8_t *)&PORTD , PORTD2};

Button buttons[] = {
	{ &btnSET, 'S' },
	{ &btnU,   'U' },
	{ &btnD,   'D' },
	{ &btnR,   'R' },
	{ &btnL,   'L' },
};

#define BUTTON_COUNT (sizeof(buttons)/sizeof(buttons[0]))

int main(void)
{
	gpio_set_pin_direction(&debugLed, PORT_DIR_OUT);
	gpio_set_pin_level(&debugLed, false);

	for (int i = 0; i < BUTTON_COUNT; i++) {
		gpio_set_pin_direction(buttons[i].pin, PORT_DIR_OUT);
		gpio_set_pin_level(buttons[i].pin, true); // ????????
	}

	uart_init(19200, 0);
	stdout = &mystdout;
	sei();

	int8_t activeBtn = -1;
	uint16_t pressTime = 0;
	uint16_t repeatTime = 0;
	bool isLongPress = false;
	bool wasPressed = false;

	while (1) {
		int8_t pressed = -1;

		for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
			if (!gpio_get_pin_level(buttons[i].pin)) {
				pressed = i;
				break;
			}
		}

		if (pressed != -1) {
			if (pressed != activeBtn) {
				// ???? ?????? ?????????
				activeBtn = pressed;
				pressTime = 0;
				repeatTime = 0;
				isLongPress = false;
				wasPressed = true;
				} else {
				// ?????? ???????????
				if (!isLongPress && pressTime >= LONG_PRESS_THRESHOLD) {
					isLongPress = true;
				}

				if (isLongPress) {
					if (repeatTime == 0) {
						gpio_set_pin_level(&debugLed, true);
						printf("%c\n\r", buttons[pressed].symbol);  // ??????
						gpio_set_pin_level(&debugLed, false);
					}
					repeatTime += 10;
					if (repeatTime >= REPEAT_PERIOD)
					repeatTime = 0;
					} else {
					pressTime += 10;
				}
			}
			} else {
			// ?????? ?????????
			if (activeBtn != -1) {
				gpio_set_pin_level(&debugLed, true);

				if (!isLongPress && wasPressed) {
					printf("%c\n\r", buttons[activeBtn].symbol + 32);  // ???? ??????
				}

				#if SEND_RELEASE_EVENT
				printf("E\n\r");
				#endif
				gpio_set_pin_level(&debugLed, false);
			}

			activeBtn = -1;
			pressTime = 0;
			repeatTime = 0;
			isLongPress = false;
			wasPressed = false;
		}

		_delay_ms(10);
	}
}
