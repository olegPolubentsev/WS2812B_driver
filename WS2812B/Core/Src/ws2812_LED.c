/*
 * ws2812_LED.c
 *
 *  Created on: 18 февр. 2021 г.
 *      Author: olegp
 */
//#include <malloc.h>
#include "ws2812_LED.h"

#define log_0 17
#define log_1 41


//==========================================================================
//#define quantity_led 100 //количесто диодов
//==========================================================================
//#define RESET_DELAY 50





//=======================================================================================================================
void WS2812_Init(uint16_t size, TIM_HandleTypeDef* h_tim, uint32_t chanel )  //записать в подготавливаемый массив цвето указанного диода
{

	for (int i = 0; i<RESET_DELAY; i++)
			    {
		myWS.buf[i] = 0;
			    }
		myWS.buf[RESET_DELAY+myWS.quantity_led*24] = 0;
		myWS.buf[RESET_DELAY+myWS.quantity_led*24+1] = 0;
		myWS.buf[RESET_DELAY+myWS.quantity_led*24+2] = 0;

}
//=======================================================================================================================
HAL_StatusTypeDef WS2812_Update(void)  //обновить ленту
{
	return HAL_TIM_PWM_Start_DMA (myWS.pointer_tim, myWS.tim_chanel, (uint32_t*)&myWS.buf[0], (RESET_DELAY+(myWS.quantity_led*24))*2+3);
}
//=======================================================================================================================
void WS2812_set_Prepared_Value(uint16_t pixel, uint16_t R, uint16_t G, uint16_t B)  //записать в подготавливаемый массив цвето указанного диода
{
	check_value_position(&pixel);
	check_value_colors(&R,&G,&B);
	myWS.array_prepared_values[myWS.quantity_led*pixel+0] = R;
	myWS.array_prepared_values[myWS.quantity_led*pixel+1] = G;
	myWS.array_prepared_values[myWS.quantity_led*pixel+2] = B;
}
//=======================================================================================================================
void WS2812_Reproduce_Prepared_Erray(void) //вывести заготовленый массив в буффер ДМА
{
	for (int i = 0; i < myWS.quantity_led ; i++)
		{
			WS2812_setColor_Pixel(i, myWS.array_prepared_values[myWS.quantity_led*i+0], myWS.array_prepared_values[myWS.quantity_led*i+1], myWS.array_prepared_values[myWS.quantity_led*i+2]);
		}
}
//=======================================================================================================================
void WS2812_Clear_Prepared_Erray(void) //очистить весь подготовленный массив
{
	for (int i = 0; i < myWS.quantity_led ; i++)
	{
		myWS.array_prepared_values[myWS.quantity_led*i+0] = 0;
		myWS.array_prepared_values[myWS.quantity_led*i+1] = 0;
		myWS.array_prepared_values[myWS.quantity_led*i+2] = 0;
	}


}
//=======================================================================================================================
void WS2812_setColor_All_Pixel(uint16_t R, uint16_t G, uint16_t B) //установить все светодиоды в указанный цвет
{
	//check_value_colors(&R,&G,&B);
	for (int i = 0; i < myWS.quantity_led; i++)
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
				myWS.buf[RESET_DELAY + pixel*24 + 8 + i] = log_1;
			}
			else
			{
				myWS.buf[RESET_DELAY + pixel*24 + 8 + i] = log_0;
			}

		}
	//GREEN -----------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if (bit_set(G,(7-i)) == 1)
			{
				myWS.buf[RESET_DELAY + pixel*24 + 0 + i] = log_1;
			}
			else
			{
				myWS.buf[RESET_DELAY + pixel*24 + 0 + i] = log_0;
			}

		}
	//BLUE -----------------------------------------------------
		for(int i=0; i<8; i++)
		{
			if (bit_set(B,(7-i)) == 1)
			{
				myWS.buf[RESET_DELAY + pixel*24 + 16 + i] = log_1;
			}
			else
			{
				myWS.buf[RESET_DELAY + pixel*24 + 16 + i] = log_0;
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

//=======================================================================================================================
void WS2812_Clear_buf(void)
{
	for (int i = RESET_DELAY; i<myWS.quantity_led*24+RESET_DELAY; i++)
	  	{
		myWS.buf[i] = log_0;
	    }

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
	if (*pixel > myWS.quantity_led-1) *pixel = myWS.quantity_led-1; else if (*pixel<0) *pixel = 0;
}
//=======================================================================================================================



