/**
  ******************************************************************************
  * @file    disp.h
  * @author  AW
  * @version V1.0
  * @date    4-Nov-2020
  * @brief   Simple 4 x 8 segment LED display driver library.
  *
  ******************************************************************************
  */
#ifndef INC_DISP_H_
#define INC_DISP_H_

/* Config --------------------------------------------------------------------*/
#define DISP_SEG_NO 7  //! Without decimal point segment
#define DISP_DIG_NO 4

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Typedef -------------------------------------------------------------------*/
#define DISP_PortType GPIO_TypeDef*
#define DISP_PinType uint16_t

typedef enum {
  DISP_DP_1 = 0, DISP_DP_2, DISP_DP_3, DISP_DP_4, DISP_NONE
} DISP_DP_TypeDef;

typedef struct {
  // Display GPIO ports and pins
  DISP_PortType DigitPorts[DISP_DIG_NO];
  DISP_PinType  DigitPins[DISP_DIG_NO];
  DISP_PortType SegmentPorts[DISP_SEG_NO];
  DISP_PinType  SegmentPins[DISP_SEG_NO];
  DISP_PortType DecimalPointPort;
  DISP_PinType  DecimalPointPin;
  // Display data
  uint8_t DataArray[DISP_DIG_NO];
  DISP_DP_TypeDef  DecimalPoint;
} DISP_HandleTypeDef;

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
void DISP_EnableDecimalPoint(DISP_HandleTypeDef* hdisp, DISP_DP_TypeDef dp);
void DISP_printDecUInt(DISP_HandleTypeDef* hdisp, uint16_t dec);
void DISP_ROUTINE(DISP_HandleTypeDef* hdisp);


#endif /* INC_DISP_H_ */
