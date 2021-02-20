/*
 * ws2812_LED.c
 *
 *  Created on: 18 февр. 2021 г.
 *      Author: olegp
 */

#include "ws2812_LED.h"
#include "main.h"

#define EXCEEDED_NUMBER_OF_ARRAY_ELEMENTS
#define NUMBER_OF_ELEMENTS_OF_THE_ARRAY_WRONG
//#define WS->log_0 17
//#define WS->log_1 41

//=======================================================================================================================
void WS2812_Init(WS2812 *WS)  //записать в подготавливаемый массив цвето указанного диода
{

	for (int i = 0; i<RESET_DELAY; i++)
			    {
		WS->buf[i] = 0;
			    }
		(WS->buf[RESET_DELAY+WS->quantity_led*24]) = 0;
		(WS->buf[RESET_DELAY+WS->quantity_led*24+1]) = 0;
		(WS->buf[RESET_DELAY+WS->quantity_led*24+2]) = 0;

}
//=======================================================================================================================
HAL_StatusTypeDef WS2812_Update(WS2812 *WS)  //обновить ленту
{
	return HAL_TIM_PWM_Start_DMA (WS->pointer_tim, WS->tim_chanel, WS->buf, (RESET_DELAY+(WS->quantity_led*24))*2+3);
}
//=======================================================================================================================
void WS2812_set_Prepared_Value(WS2812 *WS, uint16_t pixel, uint16_t R, uint16_t G, uint16_t B)  //записать в подготавливаемый массив цвето указанного диода
{
	check_value_position(WS,&pixel);
	WS->array_prepared_values[WS->quantity_led*pixel+0] = R;
	WS->array_prepared_values[WS->quantity_led*pixel+1] = G;
	WS->array_prepared_values[WS->quantity_led*pixel+2] = B;
}
//=======================================================================================================================
void WS2812_Reproduce_Prepared_Erray(WS2812 *WS) //вывести заготовленый массив в буффер ДМА
{
	for (int i = 0; i < WS->quantity_led ; i++)
		{
			WS2812_setColor_Pixel(WS, i, WS->array_prepared_values[WS->quantity_led*i+0], WS->array_prepared_values[WS->quantity_led*i+1], WS->array_prepared_values[WS->quantity_led*i+2]);
		}
}
//=======================================================================================================================
void WS2812_Clear_Prepared_Erray(WS2812 *WS) //очистить весь подготовленный массив
{
	for (int i = 0; i < WS->quantity_led ; i++)
	{
		WS->array_prepared_values[WS->quantity_led*i+0] = 0;
		WS->array_prepared_values[WS->quantity_led*i+1] = 0;
		WS->array_prepared_values[WS->quantity_led*i+2] = 0;
	}


}
//=======================================================================================================================
void WS2812_setColor_All_Pixel(WS2812 *WS, uint16_t R, uint16_t G, uint16_t B) //установить все светодиоды в указанный цвет
{

	for (int i = 0; i < WS->quantity_led; i++)
	{
		WS2812_setColor_Pixel(WS,i, (uint8_t)R, (uint8_t)G, (uint8_t)B);
	}
}

//=======================================================================================================================
void WS2812_setColor_Pixel(WS2812 *WS, uint16_t pixel, uint8_t R, uint8_t G, uint8_t B) //установить светодиод под указанным номером в указанный цвет
{
	check_value_position(WS, &pixel);
	//GGGGGGGG RRRRRRRR BBBBBBBB //  8+8+8
	//RED -------------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if ((R >> 7-i)&0x01) WS->buf[RESET_DELAY + pixel*24 + 8 + i] = WS->log_1;
			else WS->buf[RESET_DELAY + pixel*24 + 8 + i] = WS->log_0;
	//GREEN -----------------------------------------------------
			if ((G >> 7-i)&0x01) WS->buf[RESET_DELAY + pixel*24 + 0 + i] = WS->log_1;
			else WS->buf[RESET_DELAY + pixel*24 + 0 + i] = WS->log_0;
	//BLUE -----------------------------------------------------
			if ((B >> 7-i)&0x01) WS->buf[RESET_DELAY + pixel*24 + 16 + i] = WS->log_1;
			else WS->buf[RESET_DELAY + pixel*24 + 16 + i] = WS->log_0;
		}

}
//=======================================================================================================================
void WS2812_Clear_buf(WS2812 *WS)
{
	for (int i = RESET_DELAY; i<WS->quantity_led*24+RESET_DELAY; i++)
	  	{
		WS->buf[i] = WS->log_0;
	    }

}
void check_value_position(WS2812 *WS, uint16_t *pixel)
{
	if (*pixel > WS->quantity_led-1) {*pixel = WS->quantity_led-1; return EXCEEDED_NUMBER_OF_ARRAY_ELEMENTS;}
	else if (*pixel<0) {*pixel = 0; return NUMBER_OF_ELEMENTS_OF_THE_ARRAY_WRONG;}
}
//=======================================================================================================================



