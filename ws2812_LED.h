/*
 * ws2812_LED.h
 *
 *  Created on: 17 февр. 2021 г.
 *      Author: olegplb
 */
#ifndef INC_WS2812_LED_H_
#define INC_WS2812_LED_H_

#define RESET_DELAY 50
#include "main.h"


typedef struct {
uint32_t *buf;
uint8_t  array_prepared_values[300];
uint16_t quantity_led; //количесто диодов
uint32_t pointer_tim;
uint32_t tim_chanel;
} WS2812;

///WS2812 myWS;
#define WS_SIZE_BUF(_SIZE_) RESET_DELAY+(_SIZE_*24)*2+3
#define WS_ARRAY_CR_(_NAME_,_SIZE_) _NAME_##WS[WS_SIZE_BUF(_SIZE_)]
#define WS_INIT_(_NAME_,_SIZE_,_P_TIM_,_CHANEL_TIM_) WS_ARRAY_CR_(_NAME_,_SIZE_); WS2812 _NAME_ = {.buf = _NAME_##WS, .quantity_led = _SIZE_, .pointer_tim = _P_TIM_, .tim_chanel = _CHANEL_TIM_};

//#define WS_SIZE_BUF(__NUMB_LED_) .....
//#define STRUCT_INIT(_NAME_,_SIZE_)  typedef struct { uint32_t buf[_SIZE_]; uint16_t buf_size; uint32_t pointer_tim; uint32_t tim_chanel; }WS2812;
//#define WS2812_INIT(_NAME_,_SIZE_)  {STRUCT_INIT(_NAME_,_SIZE_) WS2812 _NAME_}
//WS2812_INIT(myWS, 10);
//WS2812.buf_size = RESET_DELAY+(_SIZE_*24)*2+3;
//#define INI(_SIZE_) WS2812_INIT(myWS, _SIZE_);
//		Wssss _NAME_ = {.buf = _NAME_ ## buf,}
//WS2812 my_ws2812 = {.buf = _NAME_ ## buf,}
//**************
void WS2812_set_Prepared_Value(WS2812 *WS, __uint16_t pixel, __uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_Reproduce_Prepared_Erray(WS2812 *WS);
void WS2812_Clear_Prepared_Erray(WS2812 *WS);
void WS2812_setColor_All_Pixel(WS2812 *WS, __uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_setColor_Pixel(WS2812 *WS, __uint16_t pixel, __uint16_t R, __uint16_t G, __uint16_t B);
void WS2812_Init(WS2812 *WS, __uint16_t size, TIM_HandleTypeDef*, __uint32_t);
HAL_StatusTypeDef WS2812_Update(WS2812 *WS);
void WS2812_Clear_buf(WS2812 *WS);
//**************
#endif /* INC_WS2812_LED_H_ */
