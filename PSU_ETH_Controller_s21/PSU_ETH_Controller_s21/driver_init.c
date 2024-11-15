/*
* Code generated from Atmel Start.
*
* This file will be overwritten when reconfiguring your Atmel Start project.
* Please copy examples or other code you want to keep to a separate file
* to avoid losing it when reconfiguring.
*/

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

#include <hpl_adc_base.h>
#include <hpl_rtc_base.h>

struct spi_m_sync_descriptor SPI_0;
struct spi_m_sync_descriptor SPI_1;
struct timer_descriptor      TIMER_0;

struct adc_sync_descriptor ADC_0;

struct i2c_m_sync_desc I2C_0;

struct usart_sync_descriptor USART_0;

struct pwm_descriptor PWM_0;

struct pwm_descriptor PWM_1;

void ADC_0_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA02, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA02, PINMUX_PA02B_ADC_AIN0);

	// Disable digital pin circuitry
	gpio_set_pin_direction(PA03, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PA03, PINMUX_PA03B_ADC_AIN1);
}

void ADC_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, ADC);
	_gclk_enable_channel(ADC_GCLK_ID, CONF_GCLK_ADC_SRC);
}

void ADC_0_init(void)
{
	ADC_0_CLOCK_init();
	ADC_0_PORT_init();
	adc_sync_init(&ADC_0, ADC, (void *)NULL);
}

void EXTERNAL_IRQ_0_init(void)
{
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	// Set pin direction to input
	gpio_set_pin_direction(ETH_INT, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(ETH_INT,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(ETH_INT, PINMUX_PB08A_EIC_EXTINT8);

	ext_irq_init();
}

void SPI_0_PORT_init(void)
{

	gpio_set_pin_level(MOSI,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(MOSI, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(MOSI, PINMUX_PA04D_SERCOM0_PAD0);

	gpio_set_pin_level(SCK,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SCK, PINMUX_PA05D_SERCOM0_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(MISO, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(MISO,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(MISO, PINMUX_PA06D_SERCOM0_PAD2);
}

void SPI_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM0);
	_gclk_enable_channel(SERCOM0_GCLK_ID_CORE, CONF_GCLK_SERCOM0_CORE_SRC);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_init(&SPI_0, SERCOM0);
	SPI_0_PORT_init();
}

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(PA12,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(PA12, PINMUX_PA12C_SERCOM2_PAD0);

	gpio_set_pin_pull_mode(PA13,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(PA13, PINMUX_PA13C_SERCOM2_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM2);
	_gclk_enable_channel(SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC);
	_gclk_enable_channel(SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM2);
	I2C_0_PORT_init();
}

void SPI_1_PORT_init(void)
{

	gpio_set_pin_level(PA16,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(PA16, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA16, PINMUX_PA16D_SERCOM3_PAD0);

	gpio_set_pin_level(PA17,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(PA17, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PA17, PINMUX_PA17D_SERCOM3_PAD1);

	// Set pin direction to input
	gpio_set_pin_direction(PA18, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(PA18,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(PA18, PINMUX_PA18D_SERCOM3_PAD2);
}

void SPI_1_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
}

void SPI_1_init(void)
{
	SPI_1_CLOCK_init();
	spi_m_sync_init(&SPI_1, SERCOM3);
	SPI_1_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PB12, PINMUX_PB12C_SERCOM4_PAD0);

	gpio_set_pin_function(PB14, PINMUX_PB14C_SERCOM4_PAD2);
}

void USART_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM4);
	_gclk_enable_channel(SERCOM4_GCLK_ID_CORE, CONF_GCLK_SERCOM4_CORE_SRC);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM4, (void *)NULL);
	USART_0_PORT_init();
}

/**
* \brief Timer initialization function
*
* Enables Timer peripheral, clocks and initializes Timer driver
*/
static void TIMER_0_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
	timer_init(&TIMER_0, RTC, _rtc_get_timer());
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PB17, PINMUX_PB17E_TC6_WO1);
}

void PWM_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC6);
	_gclk_enable_channel(TC6_GCLK_ID, CONF_GCLK_TC6_SRC);
}

void PWM_0_init(void)
{
	PWM_0_CLOCK_init();
	PWM_0_PORT_init();
	pwm_init(&PWM_0, TC6, _tc_get_pwm());
}

void PWM_1_PORT_init(void)
{

	gpio_set_pin_function(PA21, PINMUX_PA21E_TC7_WO1);
}

void PWM_1_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, TC7);
	_gclk_enable_channel(TC7_GCLK_ID, CONF_GCLK_TC7_SRC);
}

void PWM_1_init(void)
{
	PWM_1_CLOCK_init();
	PWM_1_PORT_init();
	pwm_init(&PWM_1, TC7, _tc_get_pwm());
}

void system_init(void)
{
	init_mcu();

	gpio_set_pin_level(PSU_REM, false);
	gpio_set_pin_direction(PSU_REM, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PSU_REM, GPIO_PIN_FUNCTION_OFF);
	
	gpio_set_pin_level(PSU_OUT_EN, false);
	gpio_set_pin_direction(PSU_OUT_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_function(PSU_OUT_EN, GPIO_PIN_FUNCTION_OFF);
	// GPIO on PA07

	gpio_set_pin_level(ETH_CS,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);

	// Set pin direction to output
	gpio_set_pin_direction(ETH_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ETH_CS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA08

	gpio_set_pin_level(O2,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(O2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(O2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA09

	gpio_set_pin_level(O3,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(O3, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(O3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA10

	gpio_set_pin_level(O1,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(O1, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(O1, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA11

	// Set pin direction to input
	gpio_set_pin_direction(IN3, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(IN3,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(IN3, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15

	gpio_set_pin_level(CS2,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	true);

	// Set pin direction to output
	gpio_set_pin_direction(CS2, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(CS2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA23

	gpio_set_pin_level(GLD,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(GLD, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(GLD, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB09

	gpio_set_pin_level(SPI_RST,
	// <y> Initial level
	// <id> pad_initial_level
	// <false"> Low
	// <true"> High
	false);

	// Set pin direction to output
	gpio_set_pin_direction(SPI_RST, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SPI_RST, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB10

	// Set pin direction to input
	gpio_set_pin_direction(IN2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(IN2,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(IN2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB11

	// Set pin direction to input
	gpio_set_pin_direction(IN1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(IN1,
	// <y> Pull configuration
	// <id> pad_pull_config
	// <GPIO_PULL_OFF"> Off
	// <GPIO_PULL_UP"> Pull-up
	// <GPIO_PULL_DOWN"> Pull-down
	GPIO_PULL_OFF);

	gpio_set_pin_function(IN1, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();
	EXTERNAL_IRQ_0_init();

	SPI_0_init();

	I2C_0_init();

	SPI_1_init();

	USART_0_init();

	TIMER_0_init();

	//PWM_0_init();

	//PWM_1_init();
}
