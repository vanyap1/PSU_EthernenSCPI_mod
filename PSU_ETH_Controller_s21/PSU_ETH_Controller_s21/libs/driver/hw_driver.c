#include "hw_driver.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>
#include <hpl_adc_base.h>
#include <hpl_rtc_base.h>
#include <string.h>

struct spi_m_sync_descriptor ETH_SPI;
struct spi_m_sync_descriptor EXT_SPI;
struct i2c_m_sync_desc EXT_I2C;

struct pwm_descriptor PWM_0;
struct pwm_descriptor PWM_1;

struct usart_sync_descriptor DEBUG_SERIAL;
struct timer_descriptor TIMER_IRQ;

static struct timer_task TIMER_task1, TIMER_task2;

struct io_descriptor *eth_spi;
struct io_descriptor *ext_spi;
struct io_descriptor *ext_i2c;
struct io_descriptor *debug_serial;

uint8_t adcConversionRequest = 0;

static void TIMER_0_task1_cb(const struct timer_task *const timer_task);
static void TIMER_0_task2_cb(const struct timer_task *const timer_task);
static void ETH_Handler(void);
static void TIMER_IRQ_init(void);

void mcu_init(void)
{
	init_mcu();
	gpio_init();
	ADC_init();
	
	ETH_SPI_init();
	EXT_SPI_init();
	EXT_I2C_init();
	//PWM_init();
	DEBUG_Serial_init();
	
	ext_irq_register(ETH_INT, ETH_Handler);
	
	TIMER_IRQ_init();
	EXT_IRG_init();
}

static void ETH_Handler(void){

}

void buzer(uint16_t soundTime){
	gpio_set_pin_level(BZ, true);
	delay_ms(soundTime);
	gpio_set_pin_level(BZ, false);
}

void gpio_init(void){
	
	gpio_set_pin_level(PSU_REM, false);
	gpio_set_pin_direction(PSU_REM, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PSU_REM, GPIO_PIN_FUNCTION_OFF);
	
	gpio_set_pin_level(PSU_OUT_EN, false);
	gpio_set_pin_direction(PSU_OUT_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PSU_OUT_EN, GPIO_PIN_FUNCTION_OFF);
	
	
	// GPIO on PA07
	gpio_set_pin_level(ETH_CS, true);
	gpio_set_pin_direction(ETH_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(ETH_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA08
	gpio_set_pin_level(O2, false);
	gpio_set_pin_direction(O2, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(O2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA09
	gpio_set_pin_level(O3, false);
	gpio_set_pin_direction(O3, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(O3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA10
	gpio_set_pin_level(O1, false);
	gpio_set_pin_direction(O1, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(O1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA11
	gpio_set_pin_direction(IN3, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IN3,	GPIO_PULL_OFF);
	gpio_set_pin_function(IN3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15
	gpio_set_pin_level(CS2, true);
	gpio_set_pin_direction(CS2, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(CS2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA23
	gpio_set_pin_level(GLD, false);
	gpio_set_pin_direction(GLD, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(GLD, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB09
	gpio_set_pin_level(SPI_RST, true);
	gpio_set_pin_direction(SPI_RST, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(SPI_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB10
	gpio_set_pin_direction(IN2, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IN2, GPIO_PULL_OFF);
	gpio_set_pin_function(IN2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB11
	gpio_set_pin_direction(IN1, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IN1, GPIO_PULL_OFF);
	gpio_set_pin_function(IN1, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PA22
	gpio_set_pin_level(DLDA, false);
	gpio_set_pin_direction(DLDA, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(DLDA, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PA24
	gpio_set_pin_level(DLDC, false);
	gpio_set_pin_direction(DLDC, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(DLDC, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PA25
	gpio_set_pin_level(BZ, false);
	gpio_set_pin_direction(BZ, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(BZ, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PB04
	gpio_set_pin_direction(IP_B2, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IP_B2, GPIO_PULL_OFF);
	gpio_set_pin_function(IP_B2, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PB05
	gpio_set_pin_direction(IP_B0, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IP_B0, GPIO_PULL_OFF);
	gpio_set_pin_function(IP_B0, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PB06
	gpio_set_pin_direction(IP_B3, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IP_B3, GPIO_PULL_OFF);
	gpio_set_pin_function(IP_B3, GPIO_PIN_FUNCTION_OFF);
	
	// GPIO on PB07
	gpio_set_pin_direction(IP_B1, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IP_B1, GPIO_PULL_OFF);
	gpio_set_pin_function(IP_B1, GPIO_PIN_FUNCTION_OFF);
	
	
}


void EXT_IRG_init(void){
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);
	
	gpio_set_pin_direction(ETH_INT, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(ETH_INT, GPIO_PULL_OFF);
	gpio_set_pin_function(ETH_INT, PINMUX_PB08A_EIC_EXTINT8);
	ext_irq_init();
}


void ETH_SPI_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
	spi_m_sync_init(&ETH_SPI, SERCOM0);
	
	gpio_set_pin_level(MOSI, false);
	gpio_set_pin_direction(MOSI, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(MOSI, PINMUX_PA04D_SERCOM0_PAD0);
	
	gpio_set_pin_level(SCK, false);
	gpio_set_pin_direction(SCK, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(SCK, PINMUX_PA05D_SERCOM0_PAD1);

	gpio_set_pin_direction(MISO, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(MISO, GPIO_PULL_OFF);
	gpio_set_pin_function(MISO, PINMUX_PA06D_SERCOM0_PAD2);
	
	spi_m_sync_init(&ETH_SPI, SERCOM0);
	spi_m_sync_get_io_descriptor(&ETH_SPI, &eth_spi);
	spi_m_sync_enable(&ETH_SPI);
	
	// GPIO on PA19 SDCARD ENABLE
	gpio_set_pin_level(SD_CS, true);
	gpio_set_pin_direction(SD_CS, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(SD_CS, GPIO_PIN_FUNCTION_OFF);
	
}

void ETH_SPI_Select(bool state){
	gpio_set_pin_level(ETH_CS, state);
}


void ETH_SPI_ReadBuff(uint8_t* buff, uint16_t len){
	io_read(eth_spi,buff,len);
}

void ETH_SPI_WriteBuff(uint8_t* buff, uint16_t len){
	io_write(eth_spi,buff,len);
}


void W5500_Select(void){
	ETH_SPI_Select(false);
}

void W5500_Unselect(void){
	ETH_SPI_Select(true);
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len){
	io_read(eth_spi,buff,len);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len){
	io_write(eth_spi,buff,len);
}

uint8_t W5500_ReadByte(void){
	uint8_t data_byte = 0;
	io_read(eth_spi,&data_byte,1);
	return data_byte;
}
void W5500_Write_byte(uint8_t byte){
	io_write(eth_spi, &byte, 1);
}



void EXT_SPI_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
	
	spi_m_sync_init(&EXT_SPI, SERCOM3);
	
	gpio_set_pin_level(PA16, false);
	gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA16, PINMUX_PA16D_SERCOM3_PAD0);

	gpio_set_pin_level(PA17, false);
	gpio_set_pin_direction(PA17, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PA17, PINMUX_PA17D_SERCOM3_PAD1);

	gpio_set_pin_direction(PA18, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(PA18, GPIO_PULL_OFF);
	gpio_set_pin_function(PA18, PINMUX_PA18D_SERCOM3_PAD2);
	
	spi_m_sync_init(&EXT_SPI, SERCOM3);
	spi_m_sync_get_io_descriptor(&EXT_SPI, &ext_spi);
	spi_m_sync_enable(&EXT_SPI);
	
}

void EXT_SPI_Select(bool state){
	gpio_set_pin_level(ETH_CS, state);
}


void EXT_SPI_ReadBuff(uint8_t* buff, uint16_t len){
	io_read(ext_spi,buff,len);
}

void EXT_SPI_WriteBuff(uint8_t* buff, uint16_t len){
	io_write(ext_spi,buff,len);
}

void EXT_SPI_write_Byte(uint8_t arg){
	//uint8_t *addr_ptr = &arg;
	io_write(ext_spi, &arg, 1);
}

uint8_t EXT_SPI_read_Byte(){
	uint8_t data_byte = 0;
	io_read(ext_spi,&data_byte,1);
	return data_byte;
}



void EXT_I2C_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM2);
	_gclk_enable_channel(SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC);
	_gclk_enable_channel(SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC);
	
	i2c_m_sync_init(&EXT_I2C, SERCOM2);
	
	gpio_set_pin_pull_mode(PA12, GPIO_PULL_OFF);
	gpio_set_pin_function(PA12, PINMUX_PA12C_SERCOM2_PAD0);
	
	gpio_set_pin_pull_mode(PA13, GPIO_PULL_OFF);
	gpio_set_pin_function(PA13, PINMUX_PA13C_SERCOM2_PAD1);
	
	i2c_m_sync_get_io_descriptor(&EXT_I2C, &ext_i2c);
	i2c_m_sync_enable(&EXT_I2C);
	
}

bool I2C_write_batch(uint8_t addres, uint8_t *data, uint8_t data_len){
	i2c_m_sync_set_slaveaddr(&EXT_I2C, addres, I2C_M_SEVEN);
	//i2c_m_sync_cmd_write(&EXT_I2C, 0x00, data, data_len);
	return (io_write(ext_i2c, (uint8_t *)data, data_len) >= 0) ? true : false;
}

bool I2C_read_batch(uint8_t addres, uint8_t reg ,uint8_t *data, uint8_t data_len){
	i2c_m_sync_set_slaveaddr(&EXT_I2C, addres, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&EXT_I2C, reg, data, data_len);
	//return (io_read(ext_i2c, (uint8_t *)data, data_len) >= 0) ? true : false;
	return true;
}
bool I2C_read_batch_addr(uint8_t addres, uint8_t reg, uint8_t *data, uint8_t data_len){
	i2c_m_sync_set_slaveaddr(&EXT_I2C, addres, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&EXT_I2C, reg, data, data_len);
	return true;
}

void PWM_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, TC6);
	_gclk_enable_channel(TC6_GCLK_ID, CONF_GCLK_TC6_SRC);
	gpio_set_pin_function(PB17, PINMUX_PB17E_TC6_WO1);
	pwm_init(&PWM_0, TC6, _tc_get_pwm());
	
	_pm_enable_bus_clock(PM_BUS_APBC, TC7);
	_gclk_enable_channel(TC7_GCLK_ID, CONF_GCLK_TC7_SRC);
	gpio_set_pin_function(PA21, PINMUX_PA21E_TC7_WO1);
	pwm_init(&PWM_1, TC7, _tc_get_pwm());
	
	pwm_set_parameters(&PWM_0, 5000, 0);
	pwm_set_parameters(&PWM_1, 5000, 0);
	
	pwm_enable(&PWM_0);
	pwm_enable(&PWM_1);
}

void PWM_write(uint8_t ch, uint16_t val){
	if(ch == 0){
		pwm_set_parameters(&PWM_0, 5000, val);
		}else{
		pwm_set_parameters(&PWM_1, 5000, val);
	}
}

void ADC_init(){

}



void DEBUG_Serial_init(void){
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
	
	usart_sync_init(&DEBUG_SERIAL, SERCOM4, (void *)NULL);

	gpio_set_pin_function(PB12, PINMUX_PB12C_SERCOM4_PAD0);
	gpio_set_pin_function(PB14, PINMUX_PB14C_SERCOM4_PAD2);
	
	usart_sync_get_io_descriptor(&DEBUG_SERIAL, &debug_serial);
	//usart_sync_register_callback(&DEBUG_SERIAL, USART_ASYNC_RXC_CB, usart_rx_cb);
	usart_sync_enable(&DEBUG_SERIAL);
}

void SerialWrite(uint8_t* buff){
	io_write(debug_serial, buff, strlen((char *)buff));
	//io_write(debug_serial, (uint8_t *)"Hello World!", 12);
}

uint8_t GetIpSwitch(void){
	uint8_t res = 0;
	res |= (gpio_get_pin_level(IP_B0) << 0);
	res |= (gpio_get_pin_level(IP_B1) << 1);
	res |= (gpio_get_pin_level(IP_B2) << 2);
	res |= (gpio_get_pin_level(IP_B3) << 3);
	
	return res;
}

static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	adcConversionRequest = 1;
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
	gpio_toggle_pin_level(GLD);
	
}

uint8_t adcRequest(void){
	uint8_t retVal = adcConversionRequest;
	adcConversionRequest = 0;
	return retVal;
}


static void TIMER_IRQ_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
	timer_init(&TIMER_IRQ, RTC, _rtc_get_timer());
	
	TIMER_task1.interval = 32768/4;
	TIMER_task1.cb       = TIMER_0_task1_cb;
	TIMER_task1.mode     = TIMER_TASK_REPEAT;
	
	TIMER_task2.interval = 32768/2;
	TIMER_task2.cb       = TIMER_0_task2_cb;
	TIMER_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_IRQ, &TIMER_task1);
	timer_add_task(&TIMER_IRQ, &TIMER_task2);
	timer_start(&TIMER_IRQ);
}