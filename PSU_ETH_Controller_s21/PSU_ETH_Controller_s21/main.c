//#include <atmel_start.h>
#include <string.h>
#include "hw_driver.h"
#include "socket.h"
#include "http_parser.h"


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
	
	for(uint8_t HTTP_SOCKET = 3; HTTP_SOCKET <= 7; HTTP_SOCKET++){
		if(getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED){
			uint8_t rIP[4];
			getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
				
			uint16_t res_size = getSn_RX_RSR(HTTP_SOCKET);
			if (res_size > sizeof(TCP_RX_BUF)){
				res_size = sizeof(TCP_RX_BUF);
				}
				
			memset(TCP_RX_BUF, 0, sizeof(DATA_BUFF_SIZE));
			sprintf(http_ansver ,"<p><span style=\"color: #00ff00;\"><strong>data</strong></span></p>\n\r");
			send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));    //Uncomment for TCP
			recv(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, res_size);
						
			//if(res_size != 0){ // Actual for telnet connection
			//send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
			//}
			//uint8_t result;
			//result = socket(UdpTxSockNum, Sn_MR_UDP, UdpTxPort, SF_IO_NONBLOCK);
			//result = sendto(UdpTxSockNum, TCP_RX_BUF, res_size, UdpDestAddress, UdpTxPort);
						
			sprintf(http_ansver, "SOCKET NUM: %d;<br>RTC: %02d:%02d:%02d; \nRead bytes: %d<br>" , HTTP_SOCKET, 12, 11, 25,res_size);
			send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));	//Uncomment for TCP
			sprintf(http_ansver ,"IP:%d.%d.%d.%d<br>", rIP[0],rIP[1],rIP[2],rIP[3]);
			send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));	//Uncomment for TCP
					
			//send(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, strlen(TCP_RX_BUF));		//Uncomment for TCP
						
			char *get_request = strtok(TCP_RX_BUF, "\r");
			//if(strstr(get_request, "GET") != NULL && strstr(get_request, "favicon") == NULL){
			//uint16_t index = position - get_request;
			//printf("IP:%d.%d.%d.%d\r\n", rIP[0],rIP[1],rIP[2],rIP[3]);
			//printf("%s\n\r",get_request);
			//
			//int value;
			//if(extractValue(get_request,"value1", &value)){
			//printf("value1 = %d\n\r", value);
			//}
			//
			//if(extractValue(get_request,"value2", &value)){
				//printf("value2 = %d\n\r", value);
				//}
				//char valueStr[20];
			//if(extractString(get_request,"time", valueStr)){
				//printf("time = %s\n\r", valueStr);
				//sscanf(valueStr, "%hhu:%hhu:%hhu", &sys_rtc.hour, &sys_rtc.minute, &sys_rtc.second);
				//rtc_set(&sys_rtc);
				//}
			//if(extractString(get_request,"help", valueStr)){
				//send(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, strlen(TCP_RX_BUF));
				//}
			//sprintf(http_ansver ,"HTTP/1.0 200 OK\n\r");
			//send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
			//}
				
			disconnect(HTTP_SOCKET);			//Uncomment for TCP
			close(HTTP_SOCKET);
			}
					
			if(getSn_SR(HTTP_SOCKET) == SOCK_CLOSE_WAIT){
				disconnect(HTTP_SOCKET);
				//close(HTTP_SOCKET);
			}
					
			if(getSn_SR(HTTP_SOCKET) == SOCK_CLOSED){
				socket(HTTP_SOCKET, Sn_MR_TCP, socketPort[HTTP_SOCKET], 0);
				listen(HTTP_SOCKET);				
				}
			}
		}
}
