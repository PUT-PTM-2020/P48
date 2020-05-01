/*
 * StateWaiting.h
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#ifndef INC_STATEWAITING_H_
#define INC_STATEWAITING_H_

#include "Recorder.h"

void onStartStateWaiting(Recorder *recorder);
void onUpdateStateWaiting(Recorder *recorder);
void onTimerUpdateStateWaiting(Recorder *recorder, TIM_HandleTypeDef *timer);
void onButtonStateWaiting(Recorder *recorder, uint16_t pin);

#endif /* INC_STATEWAITING_H_ */
