/*
 * StatePlaying.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "StatePlaying.h"

void cleanUp(Recorder *recorder) {
	freeList(recorder->soundList);

	stopSpeaker(recorder);
	stopTimer(recorder->soundTimer);
	changeToStateWaiting(recorder);
}

void onStartStatePlaying(Recorder *recorder) {
	setLcdCursor(recorder, 1, 0);
	setLcdText(recorder, "Playing current rec-");
	setLcdCursor(recorder, 2, 0);
	setLcdText(recorder, "-ording ...         ");

	recorder->soundData = 0;
	recorder->currentDataIndex = 0;
	recorder->currentSoundNode = recorder->soundList->head;

	startSpeaker(recorder);
	setTimer(recorder->soundTimer, 1049, 4);
	startTimer(recorder->soundTimer);
}

void onUpdateStatePlaying(Recorder *recorder) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

	// scale 12 bit sound sample to 8 bit
	setSpeakerValue(recorder, recorder->soundData / 16);
}

void onTimerUpdateStatePlaying(Recorder *recorder, TIM_HandleTypeDef *timer) {
	if (timer == recorder->soundTimer) {
		volatile uint64_t *dataIndex = &recorder->currentDataIndex;

		if (*dataIndex >= recorder->soundList->dataSize) {
			recorder->currentSoundNode = recorder->currentSoundNode->next;
			*dataIndex = 0;

			if (recorder->currentSoundNode == NULL) {
				cleanUp(recorder);
				return;
			}
		}

		recorder->soundData = recorder->currentSoundNode->data[*dataIndex];
		++(*dataIndex);
	}
}

void onButtonStatePlaying(Recorder *recorder, uint16_t pin) {
//	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
//		cleanUp(recorder);
//	}
}
