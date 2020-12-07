/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "components.h"
#include <stdio.h>
#include <string.h>

//#include "disp_garbage.c" // TODO: napisz tą bibliteke po ludzku

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	LED_R1, LED_R2, LED_G1, LED_G2, LED_B1, LED_B2,  // LEDs
	LED_RGB_R, LED_RGB_G, LED_RGB_B,                 // LED RGB
	POT1, POT2,                                      // Potentiometers (ADC)
	LAMP,                                            // Dimmer board (lamp controller)
	LIGHT_SENSOR_1, LIGHT_SENSOR_2,                  // BH1750 light sensors
	TEMP_SENSOR_1, TEMP_SENSOR_2,                    // BMP280 temperature sensors
	HEATER                                           //
} MenuItem_TypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
MenuItem_TypeDef menu_item = LED_R1;

// Rotary encoder
static int32_t enc_cnt = 0;

// Lamp controller
static float triac_firing_ang = 90;

// ADC conversion results
uint16_t adc_measurement[ADC_CHANNEL_NUMBER] = {0, 0};   // ADC register value
uint32_t adc_voltage_mV[ADC_CHANNEL_NUMBER] = {0, 0};

// DAC data
uint16_t sine_wave[] = {
#include "sine_data.csv"
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LED_Handler(LED_HandleTypeDef* led, uint8_t led_color, uint8_t led_n, char* lcd_buffer, uint8_t* lcd_text_len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Push buttons handling */
  if(GPIO_Pin == hbtn1.Pin)
	 menu_item = (menu_item < HEATER) ? (menu_item + 1) : (LED_R1);
  else if(GPIO_Pin == hbtn2.Pin)
	 menu_item = (menu_item > LED_R1) ? (menu_item - 1) : (HEATER);

  /* Dimmer (LAMP) handling */
  else if(GPIO_Pin == hlamp1.SYNC_Pin)
	LAMP_StartTimer(&hlamp1, triac_firing_ang);
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Dimmer (LAMP) handling */
  if(htim->Instance == hlamp1.Timer->Instance)
    LAMP_TriacFiring(&hlamp1);
  else if(htim->Instance == TIM7)
	DISP_ROUTINE(&hdisp1);
}

/**
  * @brief  Regular conversion complete callback in non blocking mode
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1)
  {
    adc_voltage_mV[ADC_POT1] = ADC_REG2VOLTAGE(adc_measurement[ADC_POT1]);
    adc_voltage_mV[ADC_POT2] = ADC_REG2VOLTAGE(adc_measurement[ADC_POT2]);
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char lcd_buffer[LCD_LINE_BUF_LEN];
  uint8_t lcd_text_len;

  struct bmp280_uncomp_data bmp280_1_data;
  struct bmp280_uncomp_data bmp280_2_data;
  int32_t temp;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_SPI4_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim6);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_wave, 100, DAC_ALIGN_12B_R) ;

  LCD_Init(&hlcd1);
  LED_RGB_Init(&hledrgb1);
  HEATER_PWM_Init(&hheaterpwm1);
  ENC_Init(&henc1);
  BH1750_Init(&hbh1750_1);
  BH1750_Init(&hbh1750_2);
  BMP280_Init(&bmp280_1);
  BMP280_Init(&bmp280_2);

  HAL_TIM_Base_Start_IT(&htim7);
  DISP_EnableDecimalPoint(&hdisp1, DISP_DP_4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/* Multiple channels | Independent mode | Direct memory access mode */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_measurement, ADC_CHANNEL_NUMBER);

	switch(menu_item)
	{
	/* LEDR1: On-board red LED *****************************************/
	case LED_R1:
	  LED_Handler(&hledr1, 'R', 1, lcd_buffer, &lcd_text_len);
      break;

    /* LEDR2: External (breadboard) red LED ***************************/
	case LED_R2:
	  LED_Handler(&hledr2, 'R', 2, lcd_buffer, &lcd_text_len);
      break;

    /* LEDG1: On-board green LED **************************************/
	case LED_G1:
	  LED_Handler(&hledg1, 'G', 1, lcd_buffer, &lcd_text_len);
      break;

    /* LEDG2: External (breadboard) green LED *************************/
	case LED_G2:
	  LED_Handler(&hledg2, 'G', 2, lcd_buffer, &lcd_text_len);
      break;

    /* LEDB1: On-board blue LED ***************************************/
	case LED_B1:
	  LED_Handler(&hledb1, 'B', 1, lcd_buffer, &lcd_text_len);
      break;

    /* LEDB2: External (breadboard) blue LED *************************/
	case LED_B2:
	  LED_Handler(&hledb2, 'B', 2, lcd_buffer, &lcd_text_len);
	  break;

	/* LED RGB - channel red PWM control *****************************/
	case LED_RGB_R:
	  lcd_text_len = sprintf(lcd_buffer, "RGB (R): %03d [%%]", (int)(ENC_GetCounter(&henc1)));
	  LED_RGB_SetDuty(&hledrgb1, LED_CHANNEL_R, (float)(ENC_GetCounter(&henc1)));
      break;

    /* LED RGB - channel green PWM control ***************************/
	case LED_RGB_G:
	  lcd_text_len = sprintf(lcd_buffer, "RGB (G): %03d [%%]", (int)(ENC_GetCounter(&henc1)));
	  LED_RGB_SetDuty(&hledrgb1, LED_CHANNEL_G, (float)(ENC_GetCounter(&henc1)));
      break;

    /* LED RGB - channel blue PWM control ****************************/
	case LED_RGB_B:
	  lcd_text_len = sprintf(lcd_buffer, "RGB (B): %03d [%%]", (int)(ENC_GetCounter(&henc1)));
	  LED_RGB_SetDuty(&hledrgb1, LED_CHANNEL_B, (float)(ENC_GetCounter(&henc1)));
      break;

    /* Potentiometer #1 (ADC channel of rank 1) *********************/
	case POT1:
	  lcd_text_len = sprintf(lcd_buffer, "POT1: %04d [mV]", (int)adc_voltage_mV[ADC_POT1]);
	  break;

	/* Potentiometer #2 (ADC channel of rank 2) *********************/
	case POT2:
	  lcd_text_len = sprintf(lcd_buffer, "POT2: %04d [mV]", (int)adc_voltage_mV[ADC_POT2]);
	  break;

	/* Dimmer (lamp controller) ************************************/
	case LAMP:
	  triac_firing_ang = ENC2ANG(&henc1, &hlamp1);
	  lcd_text_len = sprintf(lcd_buffer, "LAMP: %03d [%%]", (int)(100 - ENC_GetCounter(&henc1)));
	  break;

	/* Digital light sensor (BH1750) #1 ****************************/
	case LIGHT_SENSOR_1:
	  lcd_text_len = sprintf(lcd_buffer, "L1: %05d [LX]", (int)(BH1750_ReadLux(&hbh1750_1)));
	  break;

	/* Digital light sensor (BH1750) #2 ****************************/
	case LIGHT_SENSOR_2:
	  lcd_text_len = sprintf(lcd_buffer, "L2: %05d [LX]", (int)(BH1750_ReadLux(&hbh1750_2)));
	  break;

	/* Digital temperature and pressure sensor (BMP280) #1 ********/
	case TEMP_SENSOR_1:
	  bmp280_get_uncomp_data(&bmp280_1_data, &bmp280_1);
	  bmp280_get_comp_temp_32bit(&temp, bmp280_1_data.uncomp_temp, &bmp280_1);
	  lcd_text_len = sprintf(lcd_buffer, "TEMP1: %04d [*C]", (int)temp);
	  HAL_Delay(150);
	  break;

	/* Digital temperature and pressure sensor (BMP280) #2 ********/
	case TEMP_SENSOR_2:
	  bmp280_get_uncomp_data(&bmp280_2_data, &bmp280_2);
	  bmp280_get_comp_temp_32bit(&temp, bmp280_2_data.uncomp_temp, &bmp280_2);
	  lcd_text_len = sprintf(lcd_buffer, "TEMP2: %04d [*C]", (int)temp);
      HAL_Delay(150);
	  break;

	/* PWM-controller resistor as heater **************************/
	case HEATER:
	  HEATER_PWM_SetDuty(&hheaterpwm1, (float)(ENC_GetCounter(&henc1)));
	  lcd_text_len = sprintf(lcd_buffer, "HEATER: %d [%%]", (int)(ENC_GetCounter(&henc1)));
	  break;

	default: break;
	}

	enc_cnt = ENC_GetCounter(&henc1);

	/* LCD menu  **************************************************/
	memset(lcd_buffer + lcd_text_len, ' ', (LCD_LINE_BUF_LEN-1) - lcd_text_len);
	LCD_SetCursor(&hlcd1, 0, 0);
	LCD_printStr(&hlcd1, lcd_buffer);
	LCD_SetCursor(&hlcd1, 1, 0);
	LCD_printf(&hlcd1, "ENC: %03d", ENC_GetCounter(&henc1));

	/* DISP *******************************************************/
	DISP_printDecUInt(&hdisp1, (int)adc_voltage_mV[ADC_POT2]);

	HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LED_Handler(LED_HandleTypeDef* led, uint8_t led_color, uint8_t led_n, char* lcd_buffer, uint8_t* lcd_text_len)
{
  if(enc_cnt < ENC_GetCounter(&henc1))
    LED_On(led);
  else if(enc_cnt > ENC_GetCounter(&henc1))
    LED_Off(led);

  if(LED_Check(led))
    *lcd_text_len = sprintf(lcd_buffer, "LED %c%d ON ", led_color, led_n);
  else
	*lcd_text_len = sprintf(lcd_buffer, "LED %c%d OFF", led_color, led_n);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
