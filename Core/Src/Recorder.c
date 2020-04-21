/*
 * Recorder.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Grzesio
 */


#include "Recorder.h"

void startTimerButton(Recorder *recorder) {
	HAL_TIM_Base_Start_IT(recorder->buttonTimer);
}

void startTimerSound(Recorder *recorder) {
	HAL_TIM_Base_Start_IT(recorder->soundTimer);
}

void stopTimerButton(Recorder *recorder) {
	HAL_TIM_Base_Stop_IT(recorder->buttonTimer);
}

void stopTimerSound(Recorder *recorder) {
	HAL_TIM_Base_Stop_IT(recorder->soundTimer);
}

void setTimerButton(Recorder *recorder, short prescaler, short period) {
	recorder->buttonTimer->Instance->PSC = prescaler;
	recorder->buttonTimer->Instance->ARR = period;
}

void setTimerSound(Recorder *recorder, short prescaler, short period) {
	recorder->soundTimer->Instance->PSC = prescaler;
	recorder->soundTimer->Instance->ARR = period;
}

void onStart(Recorder *recorder) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

	recorder->diode = 0;
}

void onUpdate(Recorder *recorder) {
	switch (recorder->diode) {
	case 0: {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	  break;
	}
	case 1: {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	  break;
	}
	case 2: {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	  break;
	}
	case 3: {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	  break;
	}
	}

	if (recorder->diode < 3) ++recorder->diode;
	else recorder->diode = 0;

	HAL_Delay(500);
}

void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *htim) {

}

void onButton(Recorder *recorder, uint16_t pin) {
	// TODO register interrupt
}
