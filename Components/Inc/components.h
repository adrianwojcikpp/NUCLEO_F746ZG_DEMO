/**
  ******************************************************************************
  * @file    components.h
  * @author  AW 
  * @version V1.0
  * @date     
  * @brief   Components drivers and configuration files.
  *
  ******************************************************************************
  */
#ifndef INC_COMPONENTS_H_
#define INC_COMPONENTS_H_

/* Config --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/

//! LED
#include "led.h"
#include "led_config.h"

//! Push-buttons
#include "btn.h"
#include "btn_config.h"

//! Rotary encoder
#include "encoder.h"
#include "encoder_config.h"

//! Dimmer board
#include "dimmer.h"
#include "dimmer_config.h"

//! LED PWM
#include "led_pwm.h"
#include "led_pwm_config.h"

//! LED RGB
#include "led_rgb.h"
#include "led_rgb_config.h"

//! 7-Segment LED display
#include "disp.h"
#include "disp_config.h"

//! HD44780 LCD
#include "lcd.h"
#include "lcd_config.h"

//! HEATER PWM
#include "heater_pwm.h"
#include "heater_pwm_config.h"

//! BH1750 Digital light sensor
#include "bh1750.h"
#include "bh1750_config.h"

//! BMP280 Digital pressure and temperature sensor
//! https://github.com/BoschSensortec/BMP280_driver
#include "bmp280.h"
#include "bmp280_defs.h"
#include "bmp280_config.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

//! ADC ----------------------------------------------------------------------
#define ADC_CHANNEL_NUMBER 2
#define ADC_POT1 0 // Rank 1
#define ADC_POT2 1 // Rank 2

/* Macro ---------------------------------------------------------------------*/

//! COMMON -------------------------------------------------------------------
/**
 * @brief Linear transformation of 'x' from <amin, amax> to <bmin, bmax>.
 * @param[in] x Input variable 
 * @param[in] amin Minimum of input range
 * @param[in] amax Maximum of input range
 * @param[in] bmin Minimum of output range
 * @param[in] bmax Maximum of output range
 * @return Scaled output variable in <bmin, bmax> range
 */
#define LINEAR_TRANSFORM(x,amin,amax,bmin,bmax) (((x-amin)/(amax-amin))*(bmax-bmin)+bmin)

//! ENCODER ------------------------------------------------------------------
#ifdef ENC_HARDWARE_COUNTER

/**
 * @brief Encoder counter to triac firing angle of lamp controller.
 * @param[in] henc  Encoder handler
 * @param[in] hlamp Lamp handler
 * @return Triac firing angle in degrees
 */
#define ENC2ANG(henc,hlamp) LINEAR_TRANSFORM((float)(ENC_GetCounter((henc))), \
                                              (float)((henc)->CounterMin),  \
                                              (float)((henc)->CounterMax),  \
                                              (hlamp)->TriacFiringAngleMin, \
                                              (hlamp)->TriacFiringAngleMax)

#else

/**
 * @brief Encoder counter to triac firing angle of lamp controller.
 * @param[in] henc  Encoder handler
 * @param[in] hlamp Lamp handler
 * @return Triac firing angle in degrees
 */
#define ENC2ANG(henc,hlamp) LINEAR_TRANSFORM((float)((henc)->Counter),      \
                                              (float)((henc)->CounterMin),  \
                                              (float)((henc)->CounterMax),  \
                                              (hlamp)->TriacFiringAngleMin, \
                                              (hlamp)->TriacFiringAngleMax)

#endif

//! ADC ----------------------------------------------------------------------
#define ADC_BIT_RES      12    // [bits]
#define ADC_REG_MAX      (float)((1ul << ADC_BIT_RES) - 1)
#define ADC_VOLTAGE_MAX  3.3f  // [V]

/**
 * @brief ADC data register to voltage in millivolts.
 * @param[in] reg  Data register
 * @return Input voltage in millivolts
 */
#define ADC_REG2VOLTAGE(reg) (uint32_t)(1000*LINEAR_TRANSFORM((float)reg,  \
                                                     0.0, ADC_REG_MAX,     \
                                                     0.0, ADC_VOLTAGE_MAX))

//! DAC ----------------------------------------------------------------------
#define DAC_BIT_RES      12    // [bits]
#define DAC_REG_MAX      (float)((1ul << DAC_BIT_RES) - 1)
#define DAC_VOLTAGE_MAX  3.3f  // [V]

/**
 * @brief DAC voltage in volts to data register.
 * @param[in] vol  Output voltage in volts
 * @return Data register
 */
#define DAC_VOLTAGE2REG(vol) (uint32_t)(LINEAR_TRANSFORM((float)vol, \
                                               0.0, DAC_VOLTAGE_MAX, \
                                               0.0, DAC_REG_MAX      ))

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_COMPONENTS_H_ */
