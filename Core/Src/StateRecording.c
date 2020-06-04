/*
 * StateRecording.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "StateRecording.h"
#include "List.h"

void onStartStateRecording(Recorder *recorder) {
	setLcdCursor(recorder, 1, 0);
	setLcdText(recorder, "Recording with micr-");
	setLcdCursor(recorder, 2, 0);
	setLcdText(recorder, "-ophone ...         ");

	recorder->soundData = 0;
	recorder->soundList = createList(800);// 50 ms at 16k Hz

	setTimer(recorder->soundTimer, 1049, 4);
	startTimer(recorder->soundTimer);
}

void onUpdateStateRecording(Recorder *recorder) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

	recorder->soundData = readMicrophoneData(recorder);
}

void onTimerUpdateStateRecording(Recorder *recorder, TIM_HandleTypeDef *timer) {
	if (timer == recorder->soundTimer) {
		addToList(recorder->soundList, recorder->soundData);
	}
}

void onButtonStateRecording(Recorder *recorder, uint16_t pin) {
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
		stopTimer(recorder->soundTimer);

		List *list = recorder->soundList;
		for (uint64_t i = list->lastFreeIndex; i < list->dataSize; ++i) {
			list->last->data[i] = 0;
		}

		changeToStatePlaying(recorder);
	}
}
