/*
 * ws2812_LED.h
 *
 *  Created on: 17 февр. 2021 г.
 *      Author: olegplb
 */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#ifndef INC_WS2812_LED_H_
#define INC_WS2812_LED_H_

#define log_0 17
#define log_1 41
//==========================================================================
//#define quantity_led 100 //количесто диодов
//==========================================================================
#define RESET_DELAY 50

uint32_t *buf;
//uint32_t buf[50+2*24+3];
uint16_t buf_size = 0;
uint8_t *array_prepared_values;
uint16_t quantity_led = 0; //количесто диодов

TIM_HandleTypeDef pointer_tim;
uint32_t pointer_tim_chanel;

//**************
void WS2812_set_Prepared_Value(uint16_t, uint16_t, uint16_t, uint16_t);
void WS2812_Reproduce_Prepared_Erray(void);
void WS2812_Clear_Prepared_Erray(void);
void WS2812_setColor_All_Pixel(uint16_t, uint16_t, uint16_t);
void WS2812_setColor_Pixel(uint16_t, uint16_t, uint16_t, uint16_t);
void WS2812_Init(uint16_t,TIM_HandleTypeDef*, uint32_t);
HAL_StatusTypeDef WS2812_Update(void);
//**************

//=======================================================================================================================
void WS2812_Init(uint16_t size, TIM_HandleTypeDef *h_tim, uint32_t chanel)  //записать в подготавливаемый массив цвето указанного диода
{
	buf = (uint32_t*)malloc((RESET_DELAY+(size*24)+3)* sizeof(uint32_t));
	quantity_led = size;
	array_prepared_values = (uint8_t*)malloc(size*3 * sizeof(uint8_t));

	pointer_tim = *h_tim;
	pointer_tim_chanel = chanel;

	buf_size = RESET_DELAY+(quantity_led*24)*2+4;

	delay_ws();
}
//=======================================================================================================================
HAL_StatusTypeDef WS2812_Update(void)  //обновить ленту
{
	return HAL_TIM_PWM_Start_DMA (&pointer_tim, pointer_tim_chanel, (uint32_t*)&buf, (RESET_DELAY+(quantity_led*24))*2+4);
	//HAL_TIM_PWM_Start_DMA (&pointer_tim, pointer_tim_chanel, (uint32_t*)&buf, (RESET_DELAY+(quantity_led*24))*2+4);
	//delay_ws();
	//return a;
}
//=======================================================================================================================
void WS2812_set_Prepared_Value(uint16_t pixel, uint16_t R, uint16_t G, uint16_t B)  //записать в подготавливаемый массив цвето указанного диода
{
	check_value_position(&pixel);
	check_value_colors(&R,&G,&B);
	array_prepared_values[quantity_led*pixel+0] = R;
	array_prepared_values[quantity_led*pixel+1] = G;
	array_prepared_values[quantity_led*pixel+2] = B;
}
//=======================================================================================================================
void WS2812_Reproduce_Prepared_Erray(void) //вывести заготовленый массив в буффер ДМА
{
	for (int i = 0; i < quantity_led ; i++)
		{
			WS2812_setColor_Pixel(i, array_prepared_values[quantity_led*i+0], array_prepared_values[quantity_led*i+1], array_prepared_values[quantity_led*i+2]);
		}
}
//=======================================================================================================================
void WS2812_Clear_Prepared_Erray(void) //очистить весь подготовленный массив
{
	for (int i = 0; i < quantity_led ; i++)
	{
		array_prepared_values[quantity_led*i+0] = 0;
	    array_prepared_values[quantity_led*i+1] = 0;
	    array_prepared_values[quantity_led*i+2] = 0;
	}


}
//=======================================================================================================================
void WS2812_setColor_All_Pixel(uint16_t R, uint16_t G, uint16_t B) //установить все светодиоды в указанный цвет
{
	//check_value_colors(&R,&G,&B);
	for (int i = 0; i < quantity_led; i++)
	{
		WS2812_setColor_Pixel(i, R, G, B);
	}
}

//=======================================================================================================================
void WS2812_setColor_Pixel(uint16_t pixel, uint16_t R, uint16_t G, uint16_t B) //установить светодиод под указанным номером в указанный цвет
{
	check_value_position(&pixel);
	check_value_colors(&R,&G,&B);

	//GGGGGGGG RRRRRRRR BBBBBBBB //  8+8+8
	//RED -----------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if (bit_set(R,(7-i)) == 1)
			{
				buf[RESET_DELAY + pixel*24 + 8 + i] = log_1;
			}
			else
			{
				buf[RESET_DELAY + pixel*24 + 8 + i] = log_0;
			}

		}
	//GREEN -----------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if (bit_set(G,(7-i)) == 1)
			{
				buf[RESET_DELAY + pixel*24 + 0 + i] = log_1;
			}
			else
			{
				buf[RESET_DELAY + pixel*24 + 0 + i] = log_0;
			}

		}
	//BLUE -----------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if (bit_set(B,(7-i)) == 1)
			{
				buf[RESET_DELAY + pixel*24 + 16 + i] = log_1;
			}
			else
			{
				buf[RESET_DELAY + pixel*24 + 16 + i] = log_0;
			}

		}

}
//=======================================================================================================================
int bit_set(int pixel_value, int pos)
{
	int value = 0;
    if (((pixel_value >> pos)&0x01) == 1 ) value = 1;
	return value;
}
//=======================================================================================================================
int delay_ws(void)
{
	for(unsigned long i=0; i<100000; i++);
}
//=======================================================================================================================
void reset_buf(void)
{
	for (int i = 0; i<RESET_DELAY; i++)
	    {
	  	  buf[i] = 0;
	    }
	for (int i = RESET_DELAY; i<quantity_led*24+RESET_DELAY; i++)
	  	{
		  buf[i] = log_0;
	    }
	buf[RESET_DELAY+quantity_led*24] = 0;
	buf[RESET_DELAY+quantity_led*24+1] = 0;
	buf[RESET_DELAY+quantity_led*24+2] = 0;
}
//=======================================================================================================================
void check_value_colors(uint16_t *R, uint16_t *G, uint16_t *B)
{
	if (*R>255) *R=255;
	else if (*R<0) *R=0;
		else if (*G>255) *G=255;
		else if (*G<0) *G=0;
			else if (*B>255) *B=255;
			else if (*B<0) *B=0;
}
void check_value_position(uint16_t *pixel)
{
	if (*pixel > quantity_led-1) *pixel = quantity_led-1; else if (*pixel<0) *pixel = 0;
}
//=======================================================================================================================
#endif /* INC_WS2812_LED_H_ */

