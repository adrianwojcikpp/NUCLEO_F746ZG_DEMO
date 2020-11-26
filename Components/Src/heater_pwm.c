/**
  ******************************************************************************
  * @file    heater_pwm.c
  * @author  AW
  * @version V1.0
  * @date    15-Nov-2020
  * @brief   Simple PWM-controlled heater driver library
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "heater_pwm.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/

/* Public function -----------------------------------------------------------*/

/**
 * @brief PWM-controlled heater initialization.
 * @param[in] hheaterpwm Heater handler
 * @return None
 */
void HEATER_PWM_Init(HEATER_PWM_HandleTypeDef* hheaterpwm)
{
  HAL_TIM_PWM_Start(hheaterpwm->Timer, hheaterpwm->Channel);
}

/**
 * @brief Sets duty of PWM-controlled heater.
 * @param[in] hheaterpwm Heater handler
 * @param[in] duty       PWM duty in percents
 * @return None
 */
void HEATER_PWM_SetDuty(HEATER_PWM_HandleTypeDef* hheaterpwm, float duty)
{
  int COMPARE = (duty * (__HAL_TIM_GET_AUTORELOAD(hheaterpwm->Timer)+1)) / 100;
  __HAL_TIM_SET_COMPARE(hheaterpwm->Timer, hheaterpwm->Channel, COMPARE);
}
