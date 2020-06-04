/*
 * StateWaiting.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "StateWaiting.h"

void onStartStateWaiting(Recorder *recorder) {
	setLcdCursor(recorder, 1, 0);
	setLcdText(recorder, "Waiting for an acti-");
	setLcdCursor(recorder, 2, 0);
	setLcdText(recorder, "-on ...             ");
}

void onUpdateStateWaiting(Recorder *recorder) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void onTimerUpdateStateWaiting(Recorder *recorder, TIM_HandleTypeDef *timer) {

}

void onButtonStateWaiting(Recorder *recorder, uint16_t pin) {
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
		changeToStateRecording(recorder);
	}
}
