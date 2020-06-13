/*
 * StatePlaying.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef INC_STATEPLAYING_H_
#define INC_STATEPLAYING_H_

#include "Recorder.h"

void onStartStatePlaying(Recorder *recorder);
void onUpdateStatePlaying(Recorder *recorder);
void onTimerUpdateStatePlaying(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButtonStatePlaying(Recorder *recorder);

#endif /* INC_STATEPLAYING_H_ */
