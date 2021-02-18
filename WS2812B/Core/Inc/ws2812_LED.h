/*
 * ws2812_LED.h
 *
 *  Created on: 17 февр. 2021 г.
 *      Author: olegplb
 */
#ifndef INC_WS2812_LED_H_
#define INC_WS2812_LED_H_
#include "main.h"

extern uint32_t *buf;
extern uint16_t quantity_led;
//**************
void WS2812_set_Prepared_Value(__uint16_t pixel, __uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_Reproduce_Prepared_Erray(void);
void WS2812_Clear_Prepared_Erray(void);
void WS2812_setColor_All_Pixel(__uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_setColor_Pixel(__uint16_t pixel, __uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_Init(__uint16_t size, TIM_HandleTypeDef*, __uint32_t);
HAL_StatusTypeDef WS2812_Update(void);
void WS2812_Clear_buf(void);
//**************
#endif /* INC_WS2812_LED_H_ */
