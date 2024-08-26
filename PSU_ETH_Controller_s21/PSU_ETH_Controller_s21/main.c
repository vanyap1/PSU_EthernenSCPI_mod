#include <string.h>
#include "hw_driver.h"
#include "socket.h"
#include "http_parser.h"
#include "indexPage.h"
#include "ADS1x1x.h"

#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

//char debugSerialBuffer[256];

FATFS FatFs;										/* FatFs work area needed for each volume */
FIL Fil;											/* File object needed for each open file */


wiz_NetInfo netInfo = {
	.mac  = {0x20, 0xcf, 0xF0, 0x82, 0x76, 0x00},	// Mac address
	.ip   = {192, 168, 1, 99},						// IP address
	.sn   = {255, 255, 255, 0},						// Subnet mask
	.dns =  {8,8,8,8},								// DNS address (google dns)
	.gw   = {192, 168, 1, 1},						// Gateway address
.dhcp = NETINFO_STATIC};						//Static IP configuration

uint16_t socketPort[8]		= {80, 23, 23, 80, 80, 80, 80, 80};
uint8_t  UdpDestAddress[4]	= {192,168,1,255};
uint16_t UdpTxPort			= 3000;
uint8_t	 UdpTxSockNum		= 0;
uint16_t UdpRxPort			= 3001;
uint8_t	 UdpRxSockNum		= 1;


float amp = 0.5, volt = 3.3;
float ampDMM = 2.15, voltDMM = 14.4;
uint8_t outState = 2;
uint8_t remoteCtrl = 0;
uint8_t psuErr = 0;
int16_t adcVal[2];
ADS1x1x_config_t my_adc;

uint8_t rx_tx_buff_sizes[]={2,2,2,2,2,2,2,2};

int main(void)
{
	mcu_init();
	reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
	reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_Write_byte);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
	wizphy_reset();
	delay_ms(100);
	wizchip_init(rx_tx_buff_sizes,rx_tx_buff_sizes);
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
		
		ADS1x1x_init(&my_adc,ADS1115,ADS1x1x_I2C_ADDRESS_ADDR_TO_GND,MUX_SINGLE_0,PGA_4096);
		ADS1x1x_set_threshold_hi(&my_adc, 0xFFFF);
		ADS1x1x_set_threshold_lo(&my_adc, 0x0000);
		ADS1x1x_set_comparator_queue(&my_adc,COMPARATOR_QUEUE_1);
		ADS1x1x_set_data_rate(&my_adc,DATA_RATE_ADS111x_860);
		ADS1x1x_set_mode(&my_adc,MODE_CONTINUOUS);
	}
	buzer(10);
	
	UINT bw;
	FRESULT fr;
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	fr = f_open(&Fil, "abc.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */
	if (fr == FR_OK) {
		f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */
	}


	while (1) {
		//gpio_toggle_pin_level(DLDC);
		//gpio_toggle_pin_level(DLDA);
		
		//gpio_toggle_pin_level(BZ);
		//PWM_write(1, 3500);
		
		//EXT_SPI_WriteBuff(testBuffer, strlen((char *)testBuffer));
		//sprintf(debugSerialBuffer, "IpSW= %d; %d%d%d\n\r", GetIpSwitch(), gpio_get_pin_level(IN1),gpio_get_pin_level(IN2),gpio_get_pin_level(IN3));
		//SerialWrite(debugSerialBuffer);
		//SerialWrite(testBuffer);
		
		gpio_set_pin_level(DLDA, !gpio_get_pin_level(ETH_INT));
		
		if(adcRequest() == 1){
			ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_0);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(2);
			adcVal[0] = ADS1x1x_read(&my_adc);
			
			ADS1x1x_set_multiplexer(&my_adc,MUX_SINGLE_1);
			ADS1x1x_start_conversion(&my_adc);
			delay_ms(2);
			adcVal[1] = ADS1x1x_read(&my_adc);
			voltDMM = adcVal[0] * 0.002335539;
			ampDMM = adcVal[1];
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
				recvfrom(UdpRxSockNum, (uint8_t*)TCP_RX_BUF, udp_size, ip, &port);
				socket(UdpTxSockNum, Sn_MR_UDP, UdpTxPort, SF_IO_NONBLOCK);
				sendto(UdpTxSockNum, (uint8_t *)TCP_RX_BUF, udp_size, ip, UdpTxPort);
				setSn_IR(UdpRxSockNum, 0x1F);
			}
		}
		
		for(uint8_t HTTP_SOCKET = 5; HTTP_SOCKET <= 7; HTTP_SOCKET++) {
			if (getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED) {
				uint8_t rIP[4];
				getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
				uint16_t res_size = getSn_RX_RSR(HTTP_SOCKET);
				if (res_size > sizeof(TCP_RX_BUF)) {
					res_size = sizeof(TCP_RX_BUF);
				}

				memset(TCP_RX_BUF, 0, sizeof(TCP_RX_BUF));
				recv(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, res_size);
				
				//main page dowload to client handler
				if (strstr((char*)TCP_RX_BUF, "GET / ") != NULL) {
					size_t total_length = strlen(psu_page);
					size_t sent_length = 0;

					while (sent_length < total_length) {
						size_t chunk_size = total_length - sent_length > 1024 ? 1024 : total_length - sent_length;
						send(HTTP_SOCKET, (uint8_t*)(psu_page + sent_length), chunk_size);
						delay_ms(20);
						sent_length += chunk_size;
					}
					
					//favicon handler
					} else if (strstr((char*)TCP_RX_BUF, "GET /favicon.ico") != NULL) {
					send(HTTP_SOCKET, (uint8_t *)"HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\n\r\n", 47);
					send(HTTP_SOCKET, (uint8_t *)favicon_ico, sizeof(favicon_ico));
					
					//Set nev values
					} else if (strstr((char*)TCP_RX_BUF, "GET /set_vals") != NULL) {
					char *query_string = strstr((char*)TCP_RX_BUF, "GET /set_vals") + strlen("GET /set_vals?");
					buzer(10);
					sscanf(query_string, "amp=%f&volt=%f", &amp, &volt);
					remoteCtrl = 1;
					send(HTTP_SOCKET, (uint8_t*)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}", 67);
					
					//Update values by java-script on the main page
					} else if (strstr((char*)TCP_RX_BUF, "GET /get_vals") != NULL) {
					float watt = ampDMM * voltDMM;

					char json_response[256];
					snprintf(json_response, sizeof(json_response),
					"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
					"{\"amp\":%.2f,\"volt\":%.2f,\"watt\":%.2f,\"ampDMM\":%.2f,\"voltDMM\":%.2f,\"outState\":%d ,\"rem\":%d,\"err\":%d,\"in0\":%d,\"in1\":%d,\"in2\":%d }", amp, volt, watt, ampDMM, voltDMM, outState, remoteCtrl, psuErr, 0, 1, 1);  //outState = (0 - disable, 1 - enable, 2 - error)

					send(HTTP_SOCKET, (uint8_t*)json_response, strlen(json_response));
					
					//GPIO control by javascript or other sources
					} else if (strstr((char*)TCP_RX_BUF, "GET /control") != NULL) {
					char *query_string = strstr((char*)TCP_RX_BUF, "GET /control") + strlen("GET /control?");
					char device[10], action[10];
					sscanf(query_string, "device=%[^&]&action=%s", device, action);

					if (strcmp(device, "fan") == 0) {
						buzer(10);
						if (strcmp(action, "on") == 0) {
							gpio_set_pin_level(O2, true);
							gpio_set_pin_level(O3, true);
							} else if (strcmp(action, "off") == 0) {
							gpio_set_pin_level(O2, false);
							gpio_set_pin_level(O3, false);
						}
						
						} else if (strcmp(device, "rem") == 0) {
						buzer(10);
						if (strcmp(action, "on") == 0) {
							remoteCtrl = 1;
							} else if (strcmp(action, "off") == 0) {
							remoteCtrl = 0;
						}
						gpio_set_pin_level(O1, remoteCtrl);
						
						} else if (strcmp(device, "psu") == 0) {
						buzer(10);
						if (strcmp(action, "on") == 0) {
							outState = 1;
							} else if (strcmp(action, "off") == 0) {
							outState = 0;
						}
						remoteCtrl = 1;
					}
					send(HTTP_SOCKET, (uint8_t*)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}", 65);
				}

				disconnect(HTTP_SOCKET);
				close(HTTP_SOCKET);
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
