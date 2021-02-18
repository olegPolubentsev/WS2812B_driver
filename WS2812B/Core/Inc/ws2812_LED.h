/*
 * ws2812_LED.h
 *
 *  Created on: 17 февр. 2021 г.
 *      Author: olegplb
 */

#ifndef INC_WS2812_LED_H_
#define INC_WS2812_LED_H_

#define log_0 17
#define log_1 41
//==========================================================================
#define quantity_led 100 //количесто диодов
//==========================================================================
#define RESET_DELAY 40


uint32_t buf[RESET_DELAY+(quantity_led*24)+3] = {0};
uint8_t array_prepared_values[3][quantity_led];

//**************
void WS2812_set_Prepared_Value(uint16_t, uint16_t, uint16_t, uint16_t);
void WS2812_Reproduce_Prepared_Erray(void);
void WS2812_Clear_Prepared_Erray(void);
void WS2812_setColor_All_Pixel(uint16_t, uint16_t, uint16_t);
void WS2812_setColor_Pixel(uint16_t, uint16_t, uint16_t, uint16_t);
//**************

uint8_t array_prepared_values[3][quantity_led] = {0};

//=======================================================================================================================
void WS2812_Ini()  //записать в подготавливаемый массив цвето указанного диода
{
	//HAL_TIM_PWM_Start_DMA (&htim3, TIM_CHANNEL_1, (uint32_t*)&buf, (RESET_DELAY+(quantity_led*24))*2+4);
}
//=======================================================================================================================
void WS2812_set_Prepared_Value(uint16_t pixel, uint16_t R, uint16_t G, uint16_t B)  //записать в подготавливаемый массив цвето указанного диода
{
	check_value_position(&pixel);
	check_value_colors(&R,&G,&B);
	array_prepared_values[0][pixel] = R;
	array_prepared_values[1][pixel] = G;
	array_prepared_values[2][pixel] = B;
}
//=======================================================================================================================
void WS2812_Reproduce_Prepared_Erray(void) //вывести заготовленый массив в буффер ДМА
{
	for (int i = 0; i < quantity_led ; i++)
		{
			WS2812_setColor_Pixel(i, array_prepared_values[0][i], array_prepared_values[1][i], array_prepared_values[2][i]);
		}
}
//=======================================================================================================================
void WS2812_Clear_Prepared_Erray(void) //очистить весь подготовленный массив
{
	for (int i = 0; i < quantity_led ; i++)
	{
		array_prepared_values[0][i] = 0;
		array_prepared_values[1][i] = 0;
		array_prepared_values[2][i] = 0;
	}


}
//=======================================================================================================================
void WS2812_setColor_All_Pixel(uint16_t R, uint16_t G, uint16_t B) //установить все светодиоды в указанный цвет
{
	check_value_colors(&R,&G,&B);
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

