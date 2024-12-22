#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hw_driver.h"
#include "socket.h"
#include "http_parser.h"
#include "indexPage.h"
#include "ADS1x1x.h"

#include "stdint.h"
#include "stdbool.h"

#include "sdApp.h"
//#include "ff.h"
//FATFS FatFs;										/* FatFs work area needed for each volume */
//FIL Fil;
//char debugSerialBuffer[256];

											/* File object needed for each open file */
#define SD_BUFFER_SIZE 512

wiz_NetInfo netInfo = {
	.mac  = {0x20, 0xcf, 0xF0, 0x82, 0x46, 0x00},	// Mac address
	.ip   = {192, 168, 1, 99},						// IP address
	.sn   = {255, 255, 255, 0},						// Subnet mask
	.dns =  {8,8,8,8},								// DNS address (google dns)
	.gw   = {192, 168, 1, 1},						// Gateway address
.dhcp = NETINFO_STATIC};						//Static IP configuration

uint16_t socketPort[8]		= {80, 23, 23, 80, 80, 80, 80, 80};
uint8_t  UdpDestAddress[4]	= {192,168,1,255};
uint16_t UdpTxPort			= 33000;
uint8_t	 UdpTxSockNum		= 0;
uint16_t UdpRxPort			= 33001;
uint8_t	 UdpRxSockNum		= 1;
uint8_t result;

float amp = 0.5, volt = 3.3;
float ampDMM = 2.15, voltDMM = 14.4;
uint8_t outState = 2;
uint8_t remoteCtrl = 0;
uint8_t psuErr = 0;
int16_t adcVal[2];
ADS1x1x_config_t my_adc;

uint8_t rx_tx_buff_sizes[]={2,2,2,2,2,2,2,2};
char UdpAnsver[128]="\0";
uint8_t debugSerialBuffer[192];
uint8_t uartRxBuff[192];

//MAX5822 DAC finctional
#define MAX5822ADDR		0x38
#define MAX5822_EXT_REG 0xf0
#define MAX5822_EXT_VAL 0x0c

#define SET_DAC_INIT \
{ \
	uint8_t tmpData[] = {MAX5822_EXT_REG, MAX5822_EXT_VAL}; \
	I2C_write_batch(MAX5822ADDR, (uint8_t *)&tmpData, sizeof(tmpData)); \
}

#define SET_DAC_CURRENT(value)  \
{ \
	double scaledValue = ((value) * ((3685.0 - 500.0) / 5.0)+500); \
	uint16_t intScaledValue = (uint16_t)(scaledValue + 0.5); \
	uint8_t tmpData1[] = {(uint8_t)(((intScaledValue)>>8)&0x0f) | (1 << 4), (uint8_t)((intScaledValue)&0xff)}; \
	I2C_write_batch(MAX5822ADDR, (uint8_t *)&tmpData1, sizeof(tmpData1));\
}





#define SET_DAC_VOLTAGE(value)  \
{ \
	double scaledValue = ((value) * ((4000.0 - 10.0) / 62.65)+6); \
	uint16_t intScaledValue = (uint16_t)(scaledValue + 0.5); \
	uint8_t tmpData1[] = {(uint8_t)(((intScaledValue)>>8)&0x0f), (uint8_t)((intScaledValue)&0xff)}; \
	I2C_write_batch(MAX5822ADDR, (uint8_t *)&tmpData1, sizeof(tmpData1)); \
}


int main(void)
{
	mcu_init();
	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_Write_byte);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	wizphy_reset();
	delay_ms(100);
	wizchip_init(rx_tx_buff_sizes,rx_tx_buff_sizes);
	netInfo.ip[3] = 90+GetIpSwitch();
	wizchip_setnetinfo(&netInfo);
	ctlnetwork(CN_SET_NETINFO, (void*) &netInfo);
	
	gpio_set_pin_level(DLDA, true);
	gpio_set_pin_level(O2, true);
	gpio_set_pin_level(O3, true);
	
	setIMR(0xff);
	setSIMR(0xff);
	setINTLEVEL(1);
	for(uint8_t HTTP_SOCKET = 0; HTTP_SOCKET <= 7; HTTP_SOCKET++){
		//setINTLEVEL(500);
		//setSn_IMR(HTTP_SOCKET, 0x04);
		setSn_IMR(HTTP_SOCKET, 0x04);
		//setSn_IR(HTTP_SOCKET, 0x1F);
	}
	//ADS1x1x_init(&my_adc,ADS1115,ADS1x1x_I2C_ADDRESS_ADDR_TO_GND,MUX_SINGLE_0,PGA_4096);
	//ADS1x1x_set_threshold_hi(&my_adc, 0xFFFF);
	//ADS1x1x_set_threshold_lo(&my_adc, 0x0000);
	//ADS1x1x_set_comparator_queue(&my_adc,COMPARATOR_QUEUE_1);
	//ADS1x1x_set_data_rate(&my_adc,DATA_RATE_ADS111x_860);
	//ADS1x1x_set_mode(&my_adc,MODE_CONTINUOUS);
	psuErr = 1;
	buzer(10);
	
	uint8_t buffer[SD_BUFFER_SIZE];
	uint32_t bytes_read;
	if (sd_init()) {
		
		sd_list_files("/");
		sd_print_file("test.txt");
		
		const char *line = "Example line to append to the file.\r\n";
		if (sd_write_line("test.txt", line, strlen(line))) {
			printf("Line written successfully.\r\n");
		}
		
		while (true) {
			int result = sd_read_file_chunk("config.txt", buffer, SD_BUFFER_SIZE, &bytes_read);
			if (result == 0 && bytes_read > 0) {
				printf("Read %lu bytes: %.*s\r\n", bytes_read, bytes_read, buffer);
				} else {
				break;
			}
		}
		sd_close_file();
	}
	
	
	uint8_t addrM = 0x38;
	uint16_t dacValue[2];
	//SET_DAC_CURRENT(0);
	//SET_DAC_VOLTAGE(0);
	//SET_DAC_INIT;
	printf("\r\nHello, SAMD21!\r\n");
	


	while (1) {
		//gpio_toggle_pin_level(DLDC);
		//gpio_toggle_pin_level(DLDA);
		
		//gpio_toggle_pin_level(BZ);
		//PWM_write(1, 3500);
		
		//EXT_SPI_WriteBuff(testBuffer, strlen((char *)testBuffer));
		//sprintf(debugSerialBuffer, "IpSW= %d; %d%d%d\r\n", GetIpSwitch(), gpio_get_pin_level(IN1),gpio_get_pin_level(IN2),gpio_get_pin_level(IN3));
		//SerialWrite(debugSerialBuffer);
		//SerialWrite(testBuffer);
		
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
			sprintf(debugSerialBuffer, "BUFF LEN=%d", rxBytesGet());
			//SerialWrite(debugSerialBuffer);
			
			
			
			/*	ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_0);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(10);
			adcVal[0] = ADS1x1x_read(&my_adc);
			
			ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_1);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(10);
			adcVal[1] = ADS1x1x_read(&my_adc);
			voltDMM = adcVal[0] * 0.002561492;// * 0.0025496554;
			ampDMM = adcVal[1] - 3691;
			ampDMM = ( ampDMM < 0) ?  0 : (ampDMM  * 0.000203718);
			*/
		}
		
		
		if(getSn_SR(UdpRxSockNum) == SOCK_CLOSED){
			socket(UdpRxSockNum, Sn_MR_UDP, UdpRxPort, SF_MULTI_ENABLE);
			listen(UdpRxSockNum);
		}
		
		if(getSn_IR(UdpRxSockNum)){ // & Sn_CR_RECV
			uint16_t udp_size = getSn_RX_RSR(UdpRxSockNum);
			if (udp_size > 0) {
				uint8_t ip[4];
				uint16_t port;
				if (udp_size > DATA_BUFF_SIZE) udp_size = DATA_BUFF_SIZE;
				memset(TCP_RX_BUF, 0, sizeof(TCP_RX_BUF));
				uint16_t ret = recvfrom(UdpRxSockNum, (uint8_t*)TCP_RX_BUF, udp_size, ip, &port);
				// UDP Datagram - TCP_RX_BUF
				uint8_t okFlg = 0;
				char state[4];
				if (sscanf(TCP_RX_BUF, "OUTP:STAT %3s", state) == 1) {
					if (strcasecmp(state, "OFF") == 0) {
						printf("Output is OFF\r\n");
						sprintf(UdpAnsver, "ok");
						okFlg = 1;
						} else if (strcasecmp(state, "ON") == 0) {
						printf("Output is ON\r\n");
						sprintf(UdpAnsver, "ok");
						okFlg = 1;
					}
					
				}
				
				char command[10];
				if (sscanf(TCP_RX_BUF, "MEAS:%[^?]", command) == 1) {
					if (strcasecmp(command, "VOLT") == 0) {
						printf("Measure Voltage command\r\n");
						sprintf(UdpAnsver, "48.031");
						okFlg = 1;
						} else if (strcasecmp(command, "CURR") == 0) {
						printf("Measure Current command\r\n");
						sprintf(UdpAnsver, "0.121");
						okFlg = 1;
					}
					
				}
				
				
				
				if(strcasecmp(TCP_RX_BUF, "*RST") == 0){
					sprintf(UdpAnsver, "ok");
					}else{
					if(okFlg == 0){
						sprintf(UdpAnsver, "err");
					}
				}
				result = socket(UdpTxSockNum, Sn_MR_UDP, UdpTxPort, SF_IO_NONBLOCK);
				result = sendto(UdpTxSockNum, (uint8_t*)UdpAnsver, strlen(UdpAnsver), UdpDestAddress, UdpTxPort);
				//Old code
				//socket(UdpTxSockNum, Sn_MR_UDP, UdpTxPort, SF_IO_NONBLOCK);
				//sendto(UdpTxSockNum, (uint8_t *)TCP_RX_BUF, udp_size, ip, UdpTxPort);
				//setSn_IR(UdpRxSockNum, 0x1F);
			}
		}
		
		for (uint8_t HTTP_SOCKET = 5; HTTP_SOCKET <= 7; HTTP_SOCKET++) {
			if (getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED) {
				uint8_t rIP[4];
				getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);

				uint16_t res_size = getSn_RX_RSR(HTTP_SOCKET);
				if (res_size > 0) {
					//printf("New Connection \r\n");
					//printf("Read bytes: %d, SOCK: %d \r\n", res_size, HTTP_SOCKET);
					
					if (res_size > sizeof(TCP_RX_BUF)) {
						res_size = sizeof(TCP_RX_BUF);
					}
					
					
					memset(TCP_RX_BUF, 0, sizeof(TCP_RX_BUF));
					recv(HTTP_SOCKET, (uint8_t *)TCP_RX_BUF, res_size);


					
					if (strstr((char *)TCP_RX_BUF, "GET / ") != NULL) {
						size_t total_length = strlen(psu_page);
						size_t sent_length = 0;
						printf("index.html send\r\n");
						while (sent_length < total_length) {
							size_t chunk_size = (total_length - sent_length > 2048) ? 2048 : total_length - sent_length;
							send(HTTP_SOCKET, (uint8_t *)(psu_page + sent_length), chunk_size);
							//uint8_t msCount = 0;
							while (getSn_TX_FSR(HTTP_SOCKET) != getSn_TxMAX(HTTP_SOCKET)) {
								delay_ms(1);
								//msCount++;
							}
							//printf("TX Time = %d\r\n", msCount);
							sent_length += chunk_size;
						}
						
						} else if (strstr((char *)TCP_RX_BUF, "GET /favicon.ico") != NULL) {
						printf("favicon.ico send\r\n");
						send(HTTP_SOCKET, (uint8_t *)"HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\r\n\n", 47);
						send(HTTP_SOCKET, (uint8_t *)favicon_ico, sizeof(favicon_ico));

						} else if (strstr((char *)TCP_RX_BUF, "GET /get_vals") != NULL) {
						float watt = ampDMM * voltDMM;
						char json_response[256];
						snprintf(json_response, sizeof(json_response),
						"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\r\n\n"
						"{\"amp\":%.3f,\"volt\":%.3f,\"watt\":%.3f,\"ampDMM\":%.3f,\"voltDMM\":%.3f,"
						"\"outState\":%d,\"rem\":%d,\"err\":%d,\"in0\":%d,\"in1\":%d,\"in2\":%d}",
						amp, volt, watt, ampDMM, voltDMM, outState, remoteCtrl, psuErr, 0, 1, 1);
						send(HTTP_SOCKET, (uint8_t *)json_response, strlen(json_response));
						
						} else if (strstr((char *)TCP_RX_BUF, "GET /set_vals") != NULL) {
						char *query_string = strstr((char *)TCP_RX_BUF, "GET /set_vals") + strlen("GET /set_vals?");
						sscanf(query_string, "amp=%f&volt=%f", &amp, &volt);
						printf("%f; %f\r\n", amp, volt);
						
						SET_DAC_VOLTAGE(volt);
						SET_DAC_CURRENT(amp);
						remoteCtrl = 1;
						send(HTTP_SOCKET, (uint8_t *)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\r\n\n{\"success\":true}", 67);
						
						} else if (strstr((char *)TCP_RX_BUF, "GET /control") != NULL) {
						char *query_string = strstr((char *)TCP_RX_BUF, "GET /control") + strlen("GET /control?");
						char device[10], action[10];
						
						sscanf(query_string, "device=%[^&]&action=%s", device, action);
						printf("%s; %s\r\n", device, action);
						
						if (strcmp(device, "fan") == 0) {
							buzer(3);
							gpio_set_pin_level(O2, strcmp(action, "on") == 0);
							gpio_set_pin_level(O3, strcmp(action, "on") == 0);
							} else if (strcmp(device, "rem") == 0) {
							buzer(3);
							remoteCtrl = (strcmp(action, "on") == 0);
							gpio_set_pin_level(O1, remoteCtrl);
							} else if (strcmp(device, "psu") == 0) {
							buzer(3);
							outState = (strcmp(action, "on") == 0) ? 1 : 0;
							remoteCtrl = 1;
						}
						send(HTTP_SOCKET, (uint8_t *)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\r\n\n{\"success\":true}", 65);
					}
					disconnect(HTTP_SOCKET);
					
					while (getSn_TX_FSR(HTTP_SOCKET) != getSn_TxMAX(HTTP_SOCKET)) {
						delay_ms(1);
					}
					close(HTTP_SOCKET);
				}
			}
			if(outState == 1){
				gpio_set_pin_level(PSU_OUT_EN, true);
				}else{
				gpio_set_pin_level(PSU_OUT_EN, false);
				
			}
			if(remoteCtrl == 1){
				gpio_set_pin_level(PSU_REM, true);
				}else{
				gpio_set_pin_level(PSU_REM, false);
				
			}
			//if (getSn_SR(HTTP_SOCKET) == SOCK_CLOSE_WAIT) {
			//disconnect(HTTP_SOCKET);
			//}

			if (getSn_SR(HTTP_SOCKET) == SOCK_CLOSED) {
				socket(HTTP_SOCKET, Sn_MR_TCP, socketPort[HTTP_SOCKET], 0);
				listen(HTTP_SOCKET);
			}
		}
	}
}
