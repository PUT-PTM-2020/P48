/*
 * StatePlayingFile.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "StatePlayingFile.h"

int findTimerSettings(unsigned int sampleRate, unsigned short* prescaler, unsigned short* period) {
	unsigned long psc, arr, temp;
    for (psc = 65535; psc > 0; --psc) {
		temp = sampleRate * psc;

		if (84000000L % temp != 0) {
			continue;
		}
		arr = 84000000L / temp;

		if (arr <= 1) continue;

		if (arr > 65535) return 0;

		*prescaler = (unsigned short) (psc - 1);
		*period = (unsigned short) (arr - 1);
		return 1;
	}

	return 0;
}

void cleanUpFile(Recorder *recorder) {
	stopSpeaker(recorder);
	stopTimer(recorder->soundTimer);

	f_close(&recorder->file);

	changeToStateWaiting(recorder);
}

void onStartStatePlayingFile(Recorder *recorder) {
	FRESULT result;
	if ((result = f_open(&recorder->file, "Nagranie.wav", FA_READ | FA_OPEN_EXISTING)) == FR_OK) {
		setLcdCursor(recorder, 1, 0);
		setLcdText(recorder, "Odtwarzanie         ");
	}
	else {
		setLcdCursor(recorder, 1, 0);

		char* text = "";
		switch (result) {
		case FR_OK: 					{text = "OK                  ";break;}
		case FR_DISK_ERR: 				{text = "FR_DISK_ERR         ";break;}
		case FR_INT_ERR: 				{text = "FR_INT_ERR          ";break;}
		case FR_NOT_READY: 				{text = "FR_NOT_READY        ";break;}
		case FR_NO_FILE: 				{text = "FR_NO_FILE          ";break;}
		case FR_NO_PATH: 				{text = "FR_NO_PATH          ";break;}
		case FR_INVALID_NAME: 			{text = "FR_INVALID_NAME     ";break;}
		case FR_DENIED: 				{text = "FR_DENIED           ";break;}
		case FR_EXIST: 					{text = "FR_EXIST            ";break;}
		case FR_INVALID_OBJECT: 		{text = "FR_INVALID_OBJECT   ";break;}
		case FR_WRITE_PROTECTED:		{text = "FR_WRITE_PROTECTED  ";break;}
		case FR_INVALID_DRIVE: 			{text = "FR_INVALID_DRIVE    ";break;}
		case FR_NOT_ENABLED: 			{text = "FR_NOT_ENABLED      ";break;}
		case FR_NO_FILESYSTEM: 			{text = "FR_NO_FILESYSTEM    ";break;}
		case FR_TIMEOUT: 				{text = "FR_TIMEOUT          ";break;}
		case FR_LOCKED: 				{text = "FR_LOCKED           ";break;}
		case FR_NOT_ENOUGH_CORE: 		{text = "FR_NOT_ENOUGH_CORE  ";break;}
		case FR_TOO_MANY_OPEN_FILES:	{text = "FR_TOO_MANY_OPEN_FIL";break;}

		}

		setLcdText(recorder, text);
		HAL_Delay(2000);
		changeToStateWaiting(recorder);
		return;
	}

	if (!readWavHeader(&recorder->file, &recorder->wavHeader)) {
		setLcdCursor(recorder, 1, 0);
		setLcdText(recorder, "Bledny naglowek WAV ");
		HAL_Delay(2000);

		cleanUpFile(recorder);
		return;
	}

	if (recorder->wavHeader.channelCount != 1
			|| recorder->wavHeader.wavFormat != 1
			|| recorder->wavHeader.bytesPerSample != 1) {
		setLcdCursor(recorder, 1, 0);
		setLcdText(recorder, "Niekompatybilny WAV ");
		HAL_Delay(2000);

		cleanUpFile(recorder);
		return;
	}

	unsigned short prescaler, period;
	if (!findTimerSettings(recorder->wavHeader.sampleRate, &prescaler, &period)) {
		setLcdCursor(recorder, 1, 0);
		setLcdText(recorder, "Nieznany sample rate");
		HAL_Delay(2000);

		cleanUpFile(recorder);
		return;
	}

	recorder->counter = 0;
	recorder->soundData = 0;

	recorder->currentBuffer = 0;
	recorder->bufferReady[0] = 1;
	recorder->bufferReady[1] = 0;

	if (f_read(&recorder->file, recorder->buffer[recorder->currentBuffer], BUFFER_LEN,
			&recorder->remainingBytes[recorder->currentBuffer]) != FR_OK) {

	}

	setTimer(recorder->fileTimer, 1000, 839);
	stopTimer(recorder->fileTimer);

	startSpeaker(recorder);
	//setTimer(recorder->soundTimer, 1049, 4);
	setTimer(recorder->soundTimer, prescaler, period);
	startTimer(recorder->soundTimer);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void onUpdateStatePlayingFile(Recorder *recorder) {

}

void onTimerUpdateStatePlayingFile(Recorder *recorder, TIM_HandleTypeDef *timer) {
	if (timer == recorder->soundTimer) {
		if (recorder->bufferReady[recorder->currentBuffer] == 0) {
			return;
		}

		if (recorder->counter == 0) {
			startTimer(recorder->fileTimer);
		}

		recorder->soundData = (unsigned char) recorder->buffer[recorder->currentBuffer][recorder->counter];
		setSpeakerValue(recorder, recorder->soundData);
		++recorder->counter;

		if (recorder->counter >= BUFFER_LEN) {
			recorder->counter = 0;

			// swap buffers
			recorder->currentBuffer = recorder->currentBuffer ? 0 : 1;
		}
		else if (recorder->counter >= recorder->remainingBytes[recorder->currentBuffer]) {
			cleanUpFile(recorder);
			return;
		}
	}
	if (timer == recorder->fileTimer) {
		stopTimer(recorder->fileTimer);

		int otherBuffer = recorder->currentBuffer ? 0 : 1;

		recorder->bufferReady[otherBuffer] = 0;

		if (f_read(&recorder->file, recorder->buffer[otherBuffer], BUFFER_LEN,
				&recorder->remainingBytes[otherBuffer]) != FR_OK) {

		}

		recorder->bufferReady[otherBuffer] = 1;
	}
}

void onButtonStatePlayingFile(Recorder *recorder) {
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
		cleanUpFile(recorder);
	}
}
