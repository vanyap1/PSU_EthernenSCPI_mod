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
uint8_t rx_tx_buff_sizes[]={2,2,2,2,2,2,2,2};
	
	

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
	while (1) {
		delay_ms(100);
		gpio_toggle_pin_level(GLD);
		gpio_toggle_pin_level(DLDC);
		gpio_toggle_pin_level(DLDA);
		
		//gpio_toggle_pin_level(BZ);
		PWM_write(1, 500);
		delay_ms(50);
		PWM_write(1, 3500);
		delay_ms(50);
		
		EXT_SPI_WriteBuff(testBuffer, strlen(testBuffer));
		sprintf(debugSerialBuffer, "IpSW= %d; %d%d%d\n\r", GetIpSwitch(), gpio_get_pin_level(IN1),gpio_get_pin_level(IN2),gpio_get_pin_level(IN3));
		SerialWrite(debugSerialBuffer);
		//SerialWrite(testBuffer);
	}
}
