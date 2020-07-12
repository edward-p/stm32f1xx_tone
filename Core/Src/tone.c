/*
 * tone.c
 *
 *  Created on: Jul 12, 2020
 *      Author: edward
 */

#include "tone.h"
#include "main.h"

TIM_HandleTypeDef htim1;
GPIO_TypeDef *__gpio;
uint16_t __pin;
long __toggle_count;

void tone(GPIO_TypeDef *gpio, uint16_t pin, unsigned int frequency,
		long duration) {

	uint32_t ocr = 0;

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	if (duration > 0)
		__toggle_count = 2 * frequency * duration / 1000;
	else
		__toggle_count = -1;

	htim1.Instance = TIM1;

	ocr = 72000000 / frequency / 2 - 1;
	// two prescaler options for 16bit timer
	if (ocr > 0xffff) {
		htim1.Init.Prescaler = 64 - 1;
		ocr = 72000000 / frequency / 2 / 64 - 1;
	} else
		htim1.Init.Prescaler = 1 - 1;

	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = ocr;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}

	// set gpio pin
	__gpio = gpio;
	__pin = pin;

	// start timer
	HAL_TIM_Base_Start_IT(&htim1);
}

void noTone(GPIO_TypeDef *gpio, uint16_t pin) {
	HAL_TIM_Base_Stop_IT(&htim1);
	HAL_TIM_Base_DeInit(&htim1);
	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

