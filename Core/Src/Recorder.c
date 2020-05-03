/*
 * Recorder.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Grzesio
 */

#include "Recorder.h"
#include "StateWaiting.h"
#include "StateRecording.h"
#include "StatePlaying.h"

void startTimer(TIM_HandleTypeDef *timer) {
	HAL_TIM_Base_Start_IT(timer);
}

void setTimer(TIM_HandleTypeDef *timer,
		short prescaler, short period) {
	timer->Instance->PSC = prescaler;
	timer->Instance->ARR = period;
}

void stopTimer(TIM_HandleTypeDef *timer) {
	HAL_TIM_Base_Stop_IT(timer);
}

void startSpeaker(Recorder *recorder) {
	HAL_DAC_Start(recorder->speaker, DAC_CHANNEL_1);
}

void setSpeakerValue(Recorder *recorder, uint32_t value) {
	// mask is needed make sure that errors in code won't break speaker
	HAL_DAC_SetValue(recorder->speaker, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value & 0x000000FF);
}

void stopSpeaker(Recorder *recorder) {
	HAL_DAC_Stop(recorder->speaker, DAC_CHANNEL_1);
}

uint32_t readMicrophoneData(Recorder *recorder) {
	HAL_ADC_Start(recorder->microphone);

	if(HAL_ADC_PollForConversion(recorder->microphone, 10) == HAL_OK) {
		return HAL_ADC_GetValue(recorder->microphone);
	}

	return 0;
}

void changeToStateWaiting(Recorder *recorder) {
	recorder->onStartState = onStartStateWaiting;
	recorder->onUpdateState = onUpdateStateWaiting;
	recorder->onTimerUpdateState = onTimerUpdateStateWaiting;
	recorder->onButtonState = onButtonStateWaiting;

	onStart(recorder);
}

void changeToStateRecording(Recorder *recorder) {
	recorder->onStartState = onStartStateRecording;
	recorder->onUpdateState = onUpdateStateRecording;
	recorder->onTimerUpdateState = onTimerUpdateStateRecording;
	recorder->onButtonState = onButtonStateRecording;

	onStart(recorder);
}

void changeToStatePlaying(Recorder *recorder) {
	recorder->onStartState = onStartStatePlaying;
	recorder->onUpdateState = onUpdateStatePlaying;
	recorder->onTimerUpdateState = onTimerUpdateStatePlaying;
	recorder->onButtonState = onButtonStatePlaying;

	onStart(recorder);
}


void onStart(Recorder *recorder) {
	recorder->onStartState(recorder);
}

void onUpdate(Recorder *recorder) {
	recorder->onUpdateState(recorder);
}

void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *timer) {
	recorder->onTimerUpdateState(recorder, timer);
}

void onButton(Recorder *recorder, uint16_t pin) {
	recorder->onButtonState(recorder, pin);
}
