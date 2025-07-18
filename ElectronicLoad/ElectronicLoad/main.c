#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hw_driver.h"
#include "socket.h"
#include "ADS1x1x.h"
#include "vfd_driver.h"

#include "stdint.h"
#include "stdbool.h"
#include "sdApp.h"
#include "netHandler.h"



int16_t adcVal[2];
ADS1x1x_config_t my_adc;


int main(void)
{
	mcu_init();
	
	
	gpio_set_pin_level(DLDA, true);
	gpio_set_pin_level(O2, true);
	gpio_set_pin_level(O3, true);
	
	netInit();
	ADS1x1x_init(&my_adc,ADS1115,ADS1x1x_I2C_ADDRESS_ADDR_TO_GND,MUX_SINGLE_0,PGA_4096);
	ADS1x1x_set_threshold_hi(&my_adc, 0xFFFF);
	ADS1x1x_set_threshold_lo(&my_adc, 0x0000);
	ADS1x1x_set_comparator_queue(&my_adc,COMPARATOR_QUEUE_1);
	ADS1x1x_set_data_rate(&my_adc,DATA_RATE_ADS111x_860);
	ADS1x1x_set_mode(&my_adc,MODE_CONTINUOUS);
	
	gpio_set_pin_level(REM, true);
	delay_ms(2000);
	vfd_init();
	buzer(10);
	vfd_set_underline_marker(20, true);
	
	sprintf(debugSerialBuffer, "  IP:192.168.1.%d   ", 90+GetIpSwitch());
	vfd_string(1, 1, debugSerialBuffer);
	vfd_string(2, 1, " DC Electronic Load ");
	delay_ms(3000);
	vfd_string(1, 1, "                    ");
	vfd_string(2, 1, "                    ");
	
	//uint8_t buffer[SD_BUFFER_SIZE];
	//uint32_t bytes_read;
	//if (sd_init()) {
		//
		//sd_list_files("/");
		//sd_print_file("test.txt");
		//
		//const char *line = "Example line to append to the file.\r\n";
		//if (sd_write_line("test.txt", line, strlen(line))) {
			//printf("Line written successfully.\r\n");
		//}
		//
		//while (true) {
			//int result = sd_read_file_chunk("config.txt", buffer, SD_BUFFER_SIZE, &bytes_read);
			//if (result == 0 && bytes_read > 0) {
				//printf("Read %lu bytes: %.*s\r\n", bytes_read, bytes_read, buffer);
				//} else {
				//break;
			//}
		//}
		//sd_close_file();
	//}
	
	
	uint8_t addrM = 0x38;
	uint16_t dacValue[2];
	//SET_DAC_CURRENT(0);
	//SET_DAC_VOLTAGE(0);
	//SET_DAC_INIT;
	//printf("Hello, SAMD21!\r\n");
	uint16_t debugCounter = 0;
	
	
	
	while (1) {
		//gpio_toggle_pin_level(DLDC);
		//gpio_toggle_pin_level(DLDA);
		
		//gpio_toggle_pin_level(BZ);
		//PWM_write(1, 3500);
		
		gpio_set_pin_level(DLDA, !gpio_get_pin_level(ETH_INT));
		
		
		//if(SerialReady(&uartRxBuff)){
		//	SerialWrite(&uartRxBuff);
		//}
		
		if (SerialReady(&uartRxBuff) == 1) {
			SerialWrite(&uartRxBuff);
			} else if (SerialReady(&uartRxBuff) == 2) {
			SerialWrite("Buffer Overflow");
		}
		
		if(adcRequest() == 1){
			//sprintf(debugSerialBuffer, "BUFF LEN=%d", rxBytesGet());
			
			
			
			ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_0);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(10);
			adcVal[0] = ADS1x1x_read(&my_adc);
			
			ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_1);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(10);
			adcVal[1] = ADS1x1x_read(&my_adc);
			voltDMM = abs(adcVal[0]) * 0.00126518;// * 0.0025496554;
			ampDMM = abs(adcVal[1]) - 28;
			
			ampDMM = ( ampDMM < 8) ?  0 : (ampDMM  * 0.001303780964);
			watt = ampDMM * voltDMM;
			
			
			sprintf(debugSerialBuffer, "%.2fV %.2fA %dW", voltDMM, ampDMM, (uint16_t)watt);
			vfd_string(1, 1, debugSerialBuffer);
			sprintf(debugSerialBuffer, "<   %.3f; %.3f   >", volt, amp);
			vfd_string(2, 1, debugSerialBuffer);
			
		}
		netHandler();
			
	}
	return true;
}
