/*
 * Recorder.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Grzesio
 */

#ifndef INC_RECORDER_H_
#define INC_RECORDER_H_

#include "main.h"

#include "List.h"

typedef volatile struct Recorder {
	// hardware handles
	TIM_HandleTypeDef *soundTimer;

	DAC_HandleTypeDef *speaker;
	ADC_HandleTypeDef *microphone;

	// current state
	void (*onStartState)(volatile struct Recorder*);
	void (*onUpdateState)(volatile struct Recorder*);
	void (*onTimerUpdateState)(volatile struct Recorder*, TIM_HandleTypeDef*);
	void (*onButtonState)(volatile struct Recorder*, uint16_t);

	// waiting state variables

	// recording state variables
	List *soundList;
	uint32_t soundData;

	// playing state variables
	Node *currentSoundNode;
	uint64_t currentDataIndex;
} Recorder;

void startTimer(TIM_HandleTypeDef *timer);

void setTimer(TIM_HandleTypeDef *timer, short prescaler, short period);

void stopTimer(TIM_HandleTypeDef *timer);

void startSpeaker(Recorder *recorder);
void setSpeakerValue(Recorder *recorder, uint32_t value);
void stopSpeaker(Recorder *recorder);

uint32_t readMicrophoneData(Recorder *recorder);

void changeToStateWaiting(Recorder *recorder);
void changeToStateRecording(Recorder *recorder);
void changeToStatePlaying(Recorder *recorder);

void onStart(Recorder *recorder);
void onUpdate(Recorder *recorder);
void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButton(Recorder *recorder, uint16_t pin);

#endif /* INC_RECORDER_H_ */
