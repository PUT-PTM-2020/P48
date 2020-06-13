/*
 * StateRecording.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef SRC_STATERECORDING_H_
#define SRC_STATERECORDING_H_

#include "Recorder.h"

void onStartStateRecording(Recorder *recorder);
void onUpdateStateRecording(Recorder *recorder);
void onTimerUpdateStateRecording(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButtonStateRecording(Recorder *recorder);

#endif /* SRC_STATERECORDING_H_ */
