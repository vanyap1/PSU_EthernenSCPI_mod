/**
 * \file
 *
 * \brief SAM TC
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <hpl_pwm.h>
#include <hpl_tc_config.h>
#include <hpl_timer.h>
#include <utils.h>
#include <utils_assert.h>
#include <hpl_tc_base.h>

#ifndef CONF_TC0_ENABLE
#define CONF_TC0_ENABLE 0
#endif
#ifndef CONF_TC1_ENABLE
#define CONF_TC1_ENABLE 0
#endif
#ifndef CONF_TC2_ENABLE
#define CONF_TC2_ENABLE 0
#endif
#ifndef CONF_TC3_ENABLE
#define CONF_TC3_ENABLE 0
#endif
#ifndef CONF_TC4_ENABLE
#define CONF_TC4_ENABLE 0
#endif
#ifndef CONF_TC5_ENABLE
#define CONF_TC5_ENABLE 0
#endif
#ifndef CONF_TC6_ENABLE
#define CONF_TC6_ENABLE 0
#endif
#ifndef CONF_TC7_ENABLE
#define CONF_TC7_ENABLE 0
#endif

/**
 * \brief TC IRQ base index
 */
#define TC_IRQ_BASE_INDEX ((uint8_t)TC0_IRQn)

/**
 * \brief TC base address
 */
#define TC_HW_BASE_ADDR ((uint32_t)TC0)

/**
 * \brief TC number offset
 */
#define TC_NUMBER_OFFSET 0

/**
 * \brief Macro is used to fill usart configuration structure based on its
 * number
 *
 * \param[in] n The number of structures
 */
#define TC_CONFIGURATION(n)                                                                                            \
	{                                                                                                                  \
		(n),                                                                                                           \
		    TC_CTRLA_MODE(CONF_TC##n##_MODE) | TC_CTRLA_WAVEGEN(TC_CTRLA_WAVEGEN_MPWM_Val)                             \
		        | TC_CTRLA_PRESCALER(CONF_TC##n##_PRESCALER) | (CONF_TC##n##_RUNSTDBY << TC_CTRLA_RUNSTDBY_Pos)        \
		        | TC_CTRLA_PRESCSYNC(CONF_TC##n##_PRESCSYNC),                                                          \
		    (CONF_TC##n##_DBGRUN << TC_DBGCTRL_DBGRUN_Pos),                                                            \
		    (CONF_TC##n##_OVFEO << TC_EVCTRL_OVFEO_Pos) | (CONF_TC##n##_TCEI << TC_EVCTRL_TCEI_Pos)                    \
		        | (CONF_TC##n##_TCINV << TC_EVCTRL_TCINV_Pos) | (CONF_TC##n##_EVACT << TC_EVCTRL_EVACT_Pos)            \
		        | (CONF_TC##n##_MCEO0 << TC_EVCTRL_MCEO0_Pos) | (CONF_TC##n##_MCEO1 << TC_EVCTRL_MCEO1_Pos),           \
		    CONF_TC##n##_PER, CONF_TC##n##_CC0, CONF_TC##n##_CC1                                                       \
	}

/**
 * \brief TC configuration type
 */
struct tc_configuration {
	uint8_t                number;
	hri_tc_ctrla_reg_t     ctrl_a;
	hri_tc_dbgctrl_reg_t   dbg_ctrl;
	hri_tc_evctrl_reg_t    event_ctrl;
	hri_tccount8_per_reg_t per;
	hri_tccount32_cc_reg_t cc0;
	hri_tccount32_cc_reg_t cc1;
};

/**
 * \brief Array of TC configurations
 */
static struct tc_configuration _tcs[] = {
#if CONF_TC0_ENABLE == 1
    TC_CONFIGURATION(0),
#endif
#if CONF_TC1_ENABLE == 1
    TC_CONFIGURATION(1),
#endif
#if CONF_TC2_ENABLE == 1
    TC_CONFIGURATION(2),
#endif
#if CONF_TC3_ENABLE == 1
    TC_CONFIGURATION(3),
#endif
#if CONF_TC4_ENABLE == 1
    TC_CONFIGURATION(4),
#endif
#if CONF_TC5_ENABLE == 1
    TC_CONFIGURATION(5),
#endif
#if CONF_TC6_ENABLE == 1
    TC_CONFIGURATION(6),
#endif
#if CONF_TC7_ENABLE == 1
    TC_CONFIGURATION(7),
#endif
};

static struct _pwm_device *_tc6_dev = NULL;

static struct _pwm_device *_tc7_dev = NULL;

static int8_t         get_tc_index(const void *const hw);
static uint8_t        tc_get_hardware_index(const void *const hw);
static void           _tc_init_irq_param(const void *const hw, void *dev);
static inline uint8_t _get_hardware_offset(const void *const hw);
/**
 * \brief Initialize TC for PWM mode
 */
int32_t _pwm_init(struct _pwm_device *const device, void *const hw)
{
	int8_t i   = get_tc_index(hw);
	device->hw = hw;

	hri_tc_wait_for_sync(hw);
	if (hri_tc_get_CTRLA_reg(hw, TC_CTRLA_ENABLE)) {
		hri_tc_write_CTRLA_reg(hw, 0);
		hri_tc_wait_for_sync(hw);
	}
	hri_tc_write_CTRLA_reg(hw, TC_CTRLA_SWRST);
	hri_tc_wait_for_sync(hw);

	hri_tc_write_CTRLA_reg(hw, _tcs[i].ctrl_a);
	hri_tc_write_DBGCTRL_reg(hw, _tcs[i].dbg_ctrl);
	hri_tc_write_EVCTRL_reg(hw, _tcs[i].event_ctrl);

	if ((_tcs[i].ctrl_a & TC_CTRLA_MODE_Msk) == TC_CTRLA_MODE_COUNT32) {
		hri_tccount32_write_CC_reg(hw, 0, _tcs[i].cc0);
		hri_tccount32_write_CC_reg(hw, 1, _tcs[i].cc1);
	} else if ((_tcs[i].ctrl_a & TC_CTRLA_MODE_Msk) == TC_CTRLA_MODE_COUNT16) {
		hri_tccount16_write_CC_reg(hw, 0, (hri_tccount16_cc_reg_t)_tcs[i].cc0);
		hri_tccount16_write_CC_reg(hw, 1, (hri_tccount16_cc_reg_t)_tcs[i].cc1);
	} else {
		/* 8-bit resolution is not accepted by duty cycle control */
		return ERR_INVALID_DATA;
	}

	_tc_init_irq_param(hw, (void *)device);
	NVIC_DisableIRQ((IRQn_Type)((uint8_t)TC_IRQ_BASE_INDEX + tc_get_hardware_index(hw)));
	NVIC_ClearPendingIRQ((IRQn_Type)((uint8_t)TC_IRQ_BASE_INDEX + tc_get_hardware_index(hw)));
	NVIC_EnableIRQ((IRQn_Type)((uint8_t)TC_IRQ_BASE_INDEX + tc_get_hardware_index(hw)));

	return ERR_NONE;
}
/**
 * \brief De-initialize TC for PWM mode
 */
void _pwm_deinit(struct _pwm_device *const device)
{
	void *const hw = device->hw;

	NVIC_DisableIRQ((IRQn_Type)(TC_IRQ_BASE_INDEX + tc_get_hardware_index(hw)));

	hri_tc_clear_CTRLA_ENABLE_bit(hw);
	hri_tc_set_CTRLA_SWRST_bit(hw);
}
/**
 * \brief Start PWM
 */
void _pwm_enable(struct _pwm_device *const device)
{
	hri_tc_set_CTRLA_ENABLE_bit(device->hw);
}
/**
 * \brief Stop PWM
 */
void _pwm_disable(struct _pwm_device *const device)
{
	hri_tc_clear_CTRLA_ENABLE_bit(device->hw);
}
/**
 * \brief Set PWM parameter
 */
void _pwm_set_param(struct _pwm_device *const device, const pwm_period_t period, const pwm_period_t duty_cycle)
{
	int8_t      i  = get_tc_index(device->hw);
	void *const hw = device->hw;
	_tcs[i].cc0    = period;
	_tcs[i].cc1    = duty_cycle;
	if ((_tcs[i].ctrl_a & TC_CTRLA_MODE_Msk) == TC_CTRLA_MODE_COUNT32) {
		hri_tccount32_write_CC_reg(hw, 0, _tcs[i].cc0);
		hri_tccount32_write_CC_reg(hw, 1, _tcs[i].cc1);
	} else {
		hri_tccount16_write_CC_reg(hw, 0, _tcs[i].cc0);
		hri_tccount16_write_CC_reg(hw, 1, _tcs[i].cc1);
	}
}
/**
 * \brief Get pwm waveform period value
 */
pwm_period_t _pwm_get_period(const struct _pwm_device *const device)
{
	void *const hw = device->hw;
	int8_t      i  = get_tc_index(hw);

	if ((_tcs[i].ctrl_a & TC_CTRLA_MODE_Msk) == TC_CTRLA_MODE_COUNT32) {
		return (pwm_period_t)(hri_tccount32_read_CC_reg(hw, 0));
	} else {
		return (pwm_period_t)(hri_tccount16_read_CC_reg(hw, 0));
	}
}
/**
 * \brief Get pwm waveform duty cycle
 */
uint32_t _pwm_get_duty(const struct _pwm_device *const device)
{
	void *const hw = device->hw;
	int8_t      i  = get_tc_index(hw);
	uint32_t    per;
	uint32_t    duty_cycle;

	if ((_tcs[i].ctrl_a & TC_CTRLA_MODE_Msk) == TC_CTRLA_MODE_COUNT32) {
		per        = hri_tccount32_read_CC_reg(hw, 0);
		duty_cycle = hri_tccount32_read_CC_reg(hw, 1);
	} else {
		per        = hri_tccount16_read_CC_reg(hw, 0);
		duty_cycle = hri_tccount16_read_CC_reg(hw, 1);
	}

	return ((duty_cycle * 1000) / per);
}
/**
 * \brief Check if PWM is running
 */
bool _pwm_is_enabled(const struct _pwm_device *const device)
{
	return hri_tc_get_CTRLA_ENABLE_bit(device->hw);
}
/**
 * \brief Enable/disable PWM interrupt
 */
void _pwm_set_irq_state(struct _pwm_device *const device, const enum _pwm_callback_type type, const bool disable)
{
	ASSERT(device);

	if (PWM_DEVICE_PERIOD_CB == type) {
		hri_tc_write_INTEN_OVF_bit(device->hw, disable);
	} else if (PWM_DEVICE_ERROR_CB == type) {
		hri_tc_write_INTEN_ERR_bit(device->hw, disable);
	}
}

/**
 * \brief Retrieve timer helper functions
 */
struct _timer_hpl_interface *_tc_get_timer(void)
{
	return NULL;
}

/**
 * \brief Retrieve pwm helper functions
 */
struct _pwm_hpl_interface *_tc_get_pwm(void)
{
	return NULL;
}
/**
 * \internal TC interrupt handler for PWM
 *
 * \param[in] instance TC instance number
 */
static void tc_pwm_interrupt_handler(struct _pwm_device *device)
{
	void *const hw = device->hw;

	if (hri_tc_get_interrupt_OVF_bit(hw)) {
		hri_tc_clear_interrupt_OVF_bit(hw);
		if (NULL != device->callback.pwm_period_cb) {
			device->callback.pwm_period_cb(device);
		}
	}
	if (hri_tc_get_INTEN_ERR_bit(hw)) {
		hri_tc_clear_interrupt_ERR_bit(hw);
		if (NULL != device->callback.pwm_error_cb) {
			device->callback.pwm_error_cb(device);
		}
	}
}

/**
 * \brief TC interrupt handler
 */
void TC6_Handler(void)
{
	tc_pwm_interrupt_handler(_tc6_dev);
}

/**
 * \brief TC interrupt handler
 */
void TC7_Handler(void)
{
	tc_pwm_interrupt_handler(_tc7_dev);
}

/**
 * \internal Retrieve TC hardware index
 *
 * \param[in] hw The pointer to hardware instance
 */
static uint8_t tc_get_hardware_index(const void *const hw)
{
#ifndef _UNIT_TEST_
	return ((uint32_t)hw - TC_HW_BASE_ADDR) >> 10;
#else
	return ((uint32_t)hw - TC_HW_BASE_ADDR) / sizeof(Tc);
#endif
}

/**
 * \internal Retrieve TC index
 *
 * \param[in] hw The pointer to hardware instance
 *
 * \return The index of TC configuration
 */
static int8_t get_tc_index(const void *const hw)
{
	uint8_t tc_offset = tc_get_hardware_index(hw) + TC_NUMBER_OFFSET;
	uint8_t i;

	for (i = 0; i < ARRAY_SIZE(_tcs); i++) {
		if (_tcs[i].number == tc_offset) {
			return i;
		}
	}

	ASSERT(false);
	return -1;
}

/**
 * \brief Init irq param with the given tc hardware instance
 */
static void _tc_init_irq_param(const void *const hw, void *dev)
{
	if (hw == TC6) {
		_tc6_dev = (struct _pwm_device *)dev;
	}
	if (hw == TC7) {
		_tc7_dev = (struct _pwm_device *)dev;
	}
}

static inline uint8_t _get_hardware_offset(const void *const hw)
{
	return (((uint32_t)hw - TC_HW_BASE_ADDR) >> 10) + TC_NUMBER_OFFSET;
}
