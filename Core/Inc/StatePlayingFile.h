/*
 * StatePlayingFile.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef INC_STATEPLAYINGFILE_H_
#define INC_STATEPLAYINGFILE_H_

#include "Recorder.h"

void onStartStatePlayingFile(Recorder *recorder);
void onUpdateStatePlayingFile(Recorder *recorder);
void onTimerUpdateStatePlayingFile(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButtonStatePlayingFile(Recorder *recorder);

#endif /* INC_STATEPLAYINGFILE_H_ */
