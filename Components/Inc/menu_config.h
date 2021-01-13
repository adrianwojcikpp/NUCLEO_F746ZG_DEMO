/**
  ******************************************************************************
  * @file    menu_config.h
  * @author  AW
  * @version V1.0
  * @date    12-Jan-2021
  * @brief   Simple LCD menu example.
  *
  ******************************************************************************
  */
#ifndef INC_MENU_CONFIG_H_
#define INC_MENU_CONFIG_H_

/* Config --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "menu.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern MenuItem_TypeDef* menu_item;

/* LED Red #1: on-board red LED (LD3) */
extern MenuItem_TypeDef menu_ledr1;

/* LED Red #2: breadboard red LED (LD3EX) */
extern MenuItem_TypeDef menu_ledr2;

/* LED Green #1: on-board green LED (LD1) */
extern MenuItem_TypeDef menu_ledg1;

/* LED Green #2: breadboard green LED (LD1EX) */
extern MenuItem_TypeDef menu_ledg2;

/* LED Blue #1: on-board blue LED (LD2) */
extern MenuItem_TypeDef menu_ledb1;

/* LED Blue #2: breadboard blue LED (LD2EX) */
extern MenuItem_TypeDef menu_ledb2;

/* LED ARGB red channel */
extern MenuItem_TypeDef menu_ledrgb_r;

/* LED ARGB green channel */
extern MenuItem_TypeDef menu_ledrgb_g;

/* LED ARGB blue channel */
extern MenuItem_TypeDef menu_ledrgb_b;

/* Analog input #1: potentiometer #1 */
extern MenuItem_TypeDef menu_ain1;

/* Analog input #2: potentiometer #2 */
extern MenuItem_TypeDef menu_ain2;

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_MENU_CONFIG_H_ */
