/**
  ******************************************************************************
  * @file    menu.c
  * @author  AW
  * @version V1.0
  * @date    12-Jan-2021
  * @brief   Simple LCD menu example.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "components.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static const char MENU_LED_TEXT[2][4] = {"OFF", "ON "};

/* Public variables ----------------------------------------------------------*/
MenuItem_TypeDef* menu_item;

extern int32_t enc_cnt;
extern _Bool enc_inc, enc_dec;

extern uint32_t adc_voltage_mV[ADC_CHANNEL_NUMBER];

/** MENU LED CODE BEGIN *****************************************************************************************************/

void menu_led_fcn(MenuItem_TypeDef* hmenuitem, LED_HandleTypeDef* hled, const char led_name[3])
{
  if(enc_inc) LED_On(hled);        // If counter has increased - turn LED on
  else if(enc_dec) LED_Off(hled);  // If counter has decreased - turn LED off
  hmenuitem->display_strlen = sprintf(hmenuitem->display_str, "LED %s: %s", led_name, MENU_LED_TEXT[LED_Check(hled)]);
}

/* LED Red #1: on-board red LED (LD3) */
void menu_ledr1_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledr1, "R1"); }
MenuItem_TypeDef menu_ledr1 = {"LED R1: OFF", 11, &menu_ledr2, &menu_ain2, NULL, NULL, menu_ledr1_fcn};

/* LED Red #2: breadboard red LED (LD3EX) */
void menu_ledr2_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledr2, "R2"); }
MenuItem_TypeDef menu_ledr2 = {"LED R2: OFF", 11, &menu_ledg1, &menu_ledr1, NULL, NULL, menu_ledr2_fcn};

/* LED Green #1: on-board green LED (LD1) */
void menu_ledg1_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledg1, "G1"); }
MenuItem_TypeDef menu_ledg1 = {"LED G1: OFF", 11, &menu_ledg2, &menu_ledr2, NULL, NULL, menu_ledg1_fcn};

/* LED Green #2: breadboard green LED (LD1EX) */
void menu_ledg2_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledg2, "G2"); }
MenuItem_TypeDef menu_ledg2 = {"LED G2: OFF", 11, &menu_ledb1, &menu_ledg1, NULL, NULL, menu_ledg2_fcn};

/* LED Blue #1: on-board blue LED (LD2) */
void menu_ledb1_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledb1, "B1"); }
MenuItem_TypeDef menu_ledb1 = {"LED B1: OFF", 11, &menu_ledb2, &menu_ledg2, NULL, NULL, menu_ledb1_fcn};

/* LED Blue #2: breadboard blue LED (LD2EX) */
void menu_ledb2_fcn(MenuItem_TypeDef* hmenuitem){ menu_led_fcn(hmenuitem, &hledb2, "B2"); }
MenuItem_TypeDef menu_ledb2 = {"LED B2: OFF", 11, &menu_ledrgb_r, &menu_ledb1, NULL, NULL, menu_ledb2_fcn};

/** MENU LED CODE END *******************************************************************************************************/

/** MENU LED RGB CODE BEGIN *************************************************************************************************/
void menu_ledrgb_fcn(MenuItem_TypeDef* hmenuitem, LED_RGB_HandleTypeDef* hledrgb, LED_Channel ch, char c)
{
  if(enc_inc || enc_dec)
	LED_RGB_SetDuty(hledrgb, ch, (float)(enc_cnt));
  hmenuitem->display_strlen = sprintf(hmenuitem->display_str, "RGB (%c): %03d [%%]", c, (int)LED_RGB_GetDuty(hledrgb, ch));
}

/* LED ARGB red channel */
void menu_ledrgb_r_fcn(MenuItem_TypeDef* hmenuitem){ menu_ledrgb_fcn(hmenuitem, &hledrgb1, LED_CHANNEL_R, 'R'); }
MenuItem_TypeDef menu_ledrgb_r = {"RGB (R): 000 [%%]", 14, &menu_ledrgb_g, &menu_ledb2,    NULL, NULL, menu_ledrgb_r_fcn};

/* LED ARGB green channel */
void menu_ledrgb_g_fcn(MenuItem_TypeDef* hmenuitem){ menu_ledrgb_fcn(hmenuitem, &hledrgb1, LED_CHANNEL_G, 'G'); }
MenuItem_TypeDef menu_ledrgb_g = {"RGB (G): 000 [%%]", 14, &menu_ledrgb_b, &menu_ledrgb_r, NULL, NULL, menu_ledrgb_g_fcn};

/* LED ARGB blue channel */
void menu_ledrgb_b_fcn(MenuItem_TypeDef* hmenuitem){ menu_ledrgb_fcn(hmenuitem, &hledrgb1, LED_CHANNEL_B, 'B'); }
MenuItem_TypeDef menu_ledrgb_b = {"RGB (B): 000 [%%]", 14, &menu_ain1,     &menu_ledrgb_g, NULL, NULL, menu_ledrgb_b_fcn};

/** MENU LED RGB CODE END ***************************************************************************************************/

/** MENU ANALOG INPUTS CODE BEGIN *******************************************************************************************/
void menu_ain_fcn(MenuItem_TypeDef* hmenuitem, char n, int ain)
{
  hmenuitem->display_strlen = sprintf(hmenuitem->display_str, "POT%c: %04d [mV]", n, ain);
}

/* Analog input #1: potentiometer #1 */
void menu_ain1_fcn(MenuItem_TypeDef* hmenuitem){ menu_ain_fcn(hmenuitem, '1', (int)adc_voltage_mV[ADC_POT1]); }
MenuItem_TypeDef menu_ain1 = {"POT1: 0000 [mV]", 15, &menu_ain2,  &menu_ledrgb_b,    NULL, NULL, menu_ain1_fcn};

/* Analog input #2: potentiometer #2 */
void menu_ain2_fcn(MenuItem_TypeDef* hmenuitem){ menu_ain_fcn(hmenuitem, '2', (int)adc_voltage_mV[ADC_POT2]); }
MenuItem_TypeDef menu_ain2 = {"POT2: 0000 [mV]", 15, &menu_ledr1, &menu_ain1, NULL, NULL, menu_ain2_fcn};

/** MENU ANALOG INPUTS CODE END *********************************************************************************************/

/** MENU DIMMER CODE BEGIN **************************************************************************************************/
// TODO
/** MENU DIMMER CODE END ****************************************************************************************************/

/** MENU LIGHT SENSORS CODE BEGIN *******************************************************************************************/
// TODO
/** MENU LIGHT SENSORS CODE END *********************************************************************************************/

/** MENU TEMPERATURE SENSORS CODE BEGIN **************************************************************************************/
// TODO
/** MENU TEMPERATURE SENSORS CODE END ****************************************************************************************/

/** MENU HEATER CODE BEGIN ***************************************************************************************************/
// TODO
/** MENU HEATER CODE END *****************************************************************************************************/

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/

/* Public function -----------------------------------------------------------*/
