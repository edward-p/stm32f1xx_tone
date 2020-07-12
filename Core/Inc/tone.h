/*
 * tone.h
 *
 *  Created on: Jul 12, 2020
 *      Author: edward
 */

#ifndef INC_TONE_H_
#define INC_TONE_H_
#include "stm32f1xx_hal.h"

extern GPIO_TypeDef *__gpio;
extern uint16_t __pin;
extern long __toggle_count;

void tone(GPIO_TypeDef *gpio, uint16_t pin, unsigned int frequency,
		long duration);
void noTone(GPIO_TypeDef *gpio, uint16_t pin);

#endif /* INC_TONE_H_ */
