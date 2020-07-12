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
#include "WavHeader.h"

// 32768
#define BUFFER_LEN 32768

typedef volatile struct Recorder {
	// hardware handles
	TIM_HandleTypeDef *soundTimer;
	TIM_HandleTypeDef *fileTimer;

	DAC_HandleTypeDef *speaker;
	ADC_HandleTypeDef *microphone;

	Lcd_HandleTypeDef *lcd;

	// current state
	void (*onStartState)(volatile struct Recorder*);
	void (*onUpdateState)(volatile struct Recorder*);
	void (*onTimerUpdateState)(volatile struct Recorder*, TIM_HandleTypeDef*);
	void (*onButtonState)(volatile struct Recorder*);

	// waiting state variables

	// recording state variables
	List *soundList;
	uint32_t soundData;

	// playing state variables
	Node *currentSoundNode;
	uint64_t currentDataIndex;

	// playing/recording file state variables
	FATFS fatFs;

	FIL file;
	WavHeader wavHeader;

	uint32_t counter;
	unsigned int currentBuffer;
	char buffer[2][BUFFER_LEN];
	unsigned char bufferReady[2];

	unsigned int remainingBytes[2];
} Recorder;

void startTimer(TIM_HandleTypeDef *timer);

void setTimer(TIM_HandleTypeDef *timer, unsigned short prescaler, unsigned short period);

void stopTimer(TIM_HandleTypeDef *timer);

void startSpeaker(Recorder *recorder);
void setSpeakerValue(Recorder *recorder, uint32_t value);
void stopSpeaker(Recorder *recorder);

void setLcdCursor(Recorder *recorder, uint8_t row, uint8_t col);
void setLcdText(Recorder *recorder, char *string);
void setLcdInt(Recorder *recorder, int i);

uint32_t readMicrophoneData(Recorder *recorder);

void changeToStateWaiting(Recorder *recorder);
void changeToStateRecording(Recorder *recorder);
void changeToStateRecordingFile(Recorder *recorder);
void changeToStatePlaying(Recorder *recorder);
void changeToStatePlayingFile(Recorder *recorder);

void onStart(Recorder *recorder);
void onUpdate(Recorder *recorder);
void onTimerUpdate(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButton(Recorder *recorder);

#endif /* INC_RECORDER_H_ */
