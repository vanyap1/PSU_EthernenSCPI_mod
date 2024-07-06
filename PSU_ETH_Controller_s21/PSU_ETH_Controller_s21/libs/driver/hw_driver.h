#ifndef HW_DRIVER_INCLUDED
#define HW_DRIVER_INCLUDED

#include "pin_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_sync.h>
#include <hal_ext_irq.h>

#include <hal_spi_m_sync.h>

#include <hal_i2c_m_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_usart_sync.h>
#include <hal_timer.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>

extern struct spi_m_sync_descriptor EXT_SPI;
extern struct spi_m_sync_descriptor ETH_SPI;
extern struct i2c_m_sync_desc EXT_I2C;
extern struct usart_sync_descriptor DEBUG_SERIAL;
extern struct timer_descriptor      TIMER_IRQ;

extern struct pwm_descriptor PWM_0;
extern struct pwm_descriptor PWM_1;



void mcu_init(void);
static void ETH_Handler(void);
void buzer(uint16_t soundTime);
void gpio_init(void);	
void EXT_IRG_init(void);

void ETH_SPI_init(void);
void ETH_SPI_Select(bool state);
void ETH_SPI_ReadBuff(uint8_t* buff, uint16_t len);
void ETH_SPI_WriteBuff(uint8_t* buff, uint16_t len);

void W5500_Select(void);
void W5500_Unselect(void);
void W5500_ReadBuff(uint8_t* buff, uint16_t len);
void W5500_WriteBuff(uint8_t* buff, uint16_t len);
uint8_t W5500_ReadByte(void);
void W5500_Write_byte(uint8_t byte);


void EXT_SPI_init(void);
void EXT_SPI_Select(bool state);
void EXT_SPI_ReadBuff(uint8_t* buff, uint16_t len);
void EXT_SPI_WriteBuff(uint8_t* buff, uint16_t len);

void EXT_I2C_init(void);
bool I2C_write_batch(uint8_t addres, uint8_t *data, uint8_t data_len);
bool I2C_read_batch(uint8_t addres ,uint8_t *data, uint8_t data_len);
bool I2C_read_batch_addr(uint8_t addres, uint8_t reg, uint8_t *data, uint8_t data_len);

void PWM_init(void);
void PWM_write(uint8_t ch, uint16_t val);

void ADC_init(void);

void DEBUG_Serial_init(void);
void SerialWrite(uint8_t* buff);

uint8_t GetIpSwitch(void);

static void TIMER_0_task1_cb(const struct timer_task *const timer_task);
static void TIMER_0_task2_cb(const struct timer_task *const timer_task);

static void TIMER_IRQ_init(void);

#endif	
	
	