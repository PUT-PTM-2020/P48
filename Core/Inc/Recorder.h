/*
 * Recorder.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Grzesio
 */

#ifndef INC_RECORDER_H_
#define INC_RECORDER_H_

#include "main.h"
#include "ff.h"
#include "lcd.h"
#include "List.h"

#define FA_READ          0x01
#define FA_WRITE         0x02
#define FA_OPEN_EXISTING 0x00
#define FA_CREATE_NEW    0x04
#define FA_CREATE_ALWAYS 0x08
#define FA_OPEN_ALWAYS   0x10
#define FA_OPEN_APPEND   0x30

typedef volatile struct Recorder {
	// hardware handles
	TIM_HandleTypeDef *soundTimer;

	DAC_HandleTypeDef *speaker;
	ADC_HandleTypeDef *microphone;

	Lcd_HandleTypeDef *lcd;

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

void setLcdCursor(Recorder *recorder, uint8_t row, uint8_t col);
void setLcdText(Recorder *recorder, char *string);

uint32_t readMicrophoneData(Recorder *recorder);

void changeToStateWaiting(Recorder *recorder);
void changeToStateRecording(Recorder *recorder);
void changeToStatePlaying(Recorder *recorder);

void onStart(Recorder *recorder);
void onUpdate(Recorder *recorder);
void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButton(Recorder *recorder, uint16_t pin);

#endif /* INC_RECORDER_H_ */
