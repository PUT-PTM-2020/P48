/*
 * Recorder.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Grzesio
 */

#ifndef INC_RECORDER_H_
#define INC_RECORDER_H_

#include "main.h"

typedef struct Recorder {
	TIM_HandleTypeDef *soundTimer;
	TIM_HandleTypeDef *buttonTimer;

	char fileBuffer[256];
	int diode;
} Recorder;

void startTimerButton(Recorder *recorder);
void startTimerSound(Recorder *recorder);

void stopTimerSound(Recorder *recorder);
void stopTimerSoundButton(Recorder *recorder);

void setTimerSound(Recorder *recorder, short prescaler, short period);
void setTimerSoundButton(Recorder *recorder, short prescaler, short period);


void onStart(Recorder *recorder);
void onUpdate(Recorder *recorder);
void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *htim);
void onButton(Recorder *recorder, uint16_t pin);

#endif /* INC_RECORDER_H_ */
