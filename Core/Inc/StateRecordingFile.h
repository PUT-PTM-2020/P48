/*
 * StateRecordingFile.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef INC_STATERECORDINGFILE_H_
#define INC_STATERECORDINGFILE_H_

#include "Recorder.h"

void onStartStateRecordingFile(Recorder *recorder);
void onUpdateStateRecordingFile(Recorder *recorder);
void onTimerUpdateStateRecordingFile(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButtonStateRecordingFile(Recorder *recorder);

#endif /* INC_STATERECORDINGFILE_H_ */
