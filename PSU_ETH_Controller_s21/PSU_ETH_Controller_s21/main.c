//#include <atmel_start.h>
#include <string.h>
#include "hw_driver.h"
#include "socket.h"
#include "http_parser.h"
#include "indexPage.h"

#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>



uint8_t *testBuffer 	= "Wiznet Says Hi!\n\r";
char debugSerialBuffer[256];

wiz_NetInfo netInfo = { .mac  = {0x20, 0xcf, 0xF0, 0x82, 0x76, 0x00}, // Mac address
.ip   = {192, 168, 1, 99},         // IP address
.sn   = {255, 255, 255, 0},         // Subnet mask
.dns =  {8,8,8,8},			  // DNS address (google dns)
.gw   = {192, 168, 1, 1}, // Gateway address
.dhcp = NETINFO_STATIC};    //Static IP configuration

uint16_t socketPort[8] = {80, 23, 23, 80, 8080, 8080, 8080, 5000};
uint8_t  UdpDestAddress[4]		= { 192,168,1,255 };
uint16_t UdpTxPort			= 3000;
uint8_t	 UdpTxSockNum			= 0;
uint16_t UdpRxPort			= 3001;
uint8_t	 UdpRxSockNum			= 1;

float amp = 0, volt = 0;






uint8_t rx_tx_buff_sizes[]={2,2,2,2,2,2,2,2};
	
char http_ansver[128]="\0";	

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	//atmel_start_init();

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
		//
		
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
	//delay_ms(250);
	
	
	
	
	if(getSn_SR(UdpRxSockNum) == SOCK_CLOSED){
		socket(UdpRxSockNum, Sn_MR_UDP, UdpRxPort, SF_MULTI_ENABLE);
		listen(UdpRxSockNum);
	}
	
	if(getSn_IR(UdpRxSockNum)){ // & Sn_CR_RECV
		uint16_t udp_size = getSn_RX_RSR(UdpRxSockNum);
		if (udp_size > 0) {
			uint8_t ip[4];
			uint16_t port;
			if (udp_size > TCP_RX_BUF) udp_size = TCP_RX_BUF;
			memset(TCP_RX_BUF, 0, sizeof(TCP_RX_BUF));
			uint16_t ret = recvfrom(UdpRxSockNum, (uint8_t*)TCP_RX_BUF, udp_size, ip, &port);
			uint8_t result = 0;
			result = socket(UdpTxSockNum, Sn_MR_UDP, UdpTxPort, SF_IO_NONBLOCK);
			result = sendto(UdpTxSockNum, TCP_RX_BUF, udp_size, ip, UdpTxPort);
			setSn_IR(UdpRxSockNum, 0x1F);	
		}
	}
	
	
	for(uint8_t HTTP_SOCKET = 3; HTTP_SOCKET <= 7; HTTP_SOCKET++) {
		if (getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED) {
			uint8_t rIP[4];
			getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
			uint16_t res_size = getSn_RX_RSR(HTTP_SOCKET);
			if (res_size > sizeof(TCP_RX_BUF)) {
				res_size = sizeof(TCP_RX_BUF);
			}

			memset(TCP_RX_BUF, 0, sizeof(TCP_RX_BUF));
			recv(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, res_size);

			if (strstr((char*)TCP_RX_BUF, "GET / ") != NULL) {
				//send(HTTP_SOCKET, (uint8_t*)html_page, strlen(html_page));
				size_t total_length = strlen(psu_page);
				size_t sent_length = 0;

				while (sent_length < total_length) {
					size_t chunk_size = total_length - sent_length > 1024 ? 1024 : total_length - sent_length;
					send(HTTP_SOCKET, (uint8_t*)(psu_page + sent_length), chunk_size);
					delay_ms(20);
					sent_length += chunk_size;
				}
				
				
				
				
				} else if (strstr((char*)TCP_RX_BUF, "GET /set_vals") != NULL) {
				char *query_string = strstr((char*)TCP_RX_BUF, "GET /set_vals") + strlen("GET /set_vals?");
				// Обробка параметрів запиту
				
				sscanf(query_string, "amp=%f&volt=%f", &amp, &volt);
				// Тут додайте код для встановлення значень ампер, вольт та ват
				send(HTTP_SOCKET, (uint8_t*)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}", 65);
				} else if (strstr((char*)TCP_RX_BUF, "GET /control") != NULL) {
				char *query_string = strstr((char*)TCP_RX_BUF, "GET /control") + strlen("GET /control?");
				char device[10], action[10];
				
				sscanf(query_string, "device=%[^&]&action=%s", device, action);

				if (strcmp(device, "fan") == 0) {
					if (strcmp(action, "on") == 0) {
							gpio_set_pin_level(O2, true);
							gpio_set_pin_level(O3, true);
						} else if (strcmp(action, "off") == 0) {
							gpio_set_pin_level(O2, false);
							gpio_set_pin_level(O3, false);
					}
					} else if (strcmp(device, "psu") == 0) {
					if (strcmp(action, "on") == 0) {
						// Код для включення блоку живлення
						} else if (strcmp(action, "off") == 0) {
						// Код для виключення блоку живлення
					}
				}
				send(HTTP_SOCKET, (uint8_t*)"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"success\":true}", 65);
				} else if (strstr((char*)TCP_RX_BUF, "GET /get_vals") != NULL) {
				
				float watt = amp * volt;

				char json_response[128];
				int len = snprintf(json_response, sizeof(json_response),
				"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
				"{\"amp\":%.2f,\"volt\":%.2f,\"watt\":%.2f}", amp, volt, watt);

				send(HTTP_SOCKET, (uint8_t*)json_response, strlen(json_response));
				
				}

			disconnect(HTTP_SOCKET);
			close(HTTP_SOCKET);
		}

		if (getSn_SR(HTTP_SOCKET) == SOCK_CLOSE_WAIT) {
			disconnect(HTTP_SOCKET);
		}

		if (getSn_SR(HTTP_SOCKET) == SOCK_CLOSED) {
			socket(HTTP_SOCKET, Sn_MR_TCP, socketPort[HTTP_SOCKET], 0);
			listen(HTTP_SOCKET);
		}
	}
	
	
	
	
	}
}
