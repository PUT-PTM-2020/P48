/*
 * StateRecordingFile.c
 *
 *  Created on: Apr 30, 2020
 *      Author: Grzesio
 */

#include "StateRecordingFile.h"

void saveFile(Recorder *recorder) {
	stopTimer(recorder->soundTimer);

	if (recorder->counter > 0) {
		f_write(&recorder->file, recorder->buffer[recorder->currentBuffer], recorder->counter,
				&recorder->remainingBytes[recorder->currentBuffer]);

		recorder->wavHeader.fileSize += recorder->counter;
		recorder->wavHeader.dataSize += recorder->counter;
	}

	f_lseek(&recorder->file, 0);
	writeWavHeader(&recorder->file, &recorder->wavHeader);

	f_close(&recorder->file);

//	changeToStateWaiting(recorder);
	changeToStatePlayingFile(recorder);
}

void onStartStateRecordingFile(Recorder *recorder) {
	FRESULT result;
	if ((result = f_open(&recorder->file, "Nagranie.wav", FA_WRITE | FA_CREATE_ALWAYS)) == FR_OK) {
		setLcdCursor(recorder, 1, 0);
		setLcdText(recorder, "Nagrywanie          ");
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

	// header
	recorder->wavHeader.fileType[0] = 'R';
	recorder->wavHeader.fileType[1] = 'I';
	recorder->wavHeader.fileType[2] = 'F';
	recorder->wavHeader.fileType[3] = 'F';

	recorder->wavHeader.fileSize = 36;// wszystko minus 8

	recorder->wavHeader.fileTypeHeader[0] = 'W';
	recorder->wavHeader.fileTypeHeader[1] = 'A';
	recorder->wavHeader.fileTypeHeader[2] = 'V';
	recorder->wavHeader.fileTypeHeader[3] = 'E';
	recorder->wavHeader.chunkFormat[0] = 'f';
	recorder->wavHeader.chunkFormat[1] = 'm';
	recorder->wavHeader.chunkFormat[2] = 't';
	recorder->wavHeader.chunkFormat[3] = ' ';
	recorder->wavHeader.headerLength = 16;// 16

	recorder->wavHeader.wavFormat = 1;// 1 (PCM)
	recorder->wavHeader.channelCount = 1;// 1
	recorder->wavHeader.sampleRate = 16000;// probkowanie
	recorder->wavHeader.sampleRateBytes = 16000;// u nas powinno byc tyle samo co sampleRate
	recorder->wavHeader.bytesPerSample = 1;// u nas 1
	recorder->wavHeader.bitsPerSample = 8;// u nas 8

	recorder->wavHeader.dataTypeHeader[0] = 'd';
	recorder->wavHeader.dataTypeHeader[1] = 'a';
	recorder->wavHeader.dataTypeHeader[2] = 't';
	recorder->wavHeader.dataTypeHeader[3] = 'a';
	recorder->wavHeader.dataSize = 0;

	// state
	recorder->counter = 0;

	recorder->currentBuffer = 0;
	recorder->bufferReady[0] = 1;
	recorder->bufferReady[1] = 1;

	writeWavHeader(&recorder->file, &recorder->wavHeader);

	setTimer(recorder->fileTimer, 1000, 839);
	stopTimer(recorder->fileTimer);

	setTimer(recorder->soundTimer, 1049, 4);
	startTimer(recorder->soundTimer);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void onUpdateStateRecordingFile(Recorder *recorder) {

}

void onTimerUpdateStateRecordingFile(Recorder *recorder, TIM_HandleTypeDef *timer) {
	if (timer == recorder->soundTimer) {
		if (recorder->bufferReady[recorder->currentBuffer] == 0) {
			return;
		}

		recorder->soundData = readMicrophoneData(recorder) / 16;
		recorder->buffer[recorder->currentBuffer][recorder->counter] = (unsigned char) recorder->soundData;
		++recorder->counter;

		if (recorder->counter >= BUFFER_LEN) {
			recorder->counter = 0;

			// swap buffers
			recorder->currentBuffer = recorder->currentBuffer ? 0 : 1;

			// save back-buffer to file
			startTimer(recorder->fileTimer);
		}
	}
	if (timer == recorder->fileTimer) {
		stopTimer(recorder->fileTimer);

		int otherBuffer = recorder->currentBuffer ? 0 : 1;

		recorder->bufferReady[otherBuffer] = 0;

		if (f_write(&recorder->file, recorder->buffer[otherBuffer], BUFFER_LEN,
				&recorder->remainingBytes[otherBuffer]) != FR_OK) {

		}

		recorder->wavHeader.fileSize += recorder->remainingBytes[otherBuffer];
		recorder->wavHeader.dataSize += recorder->remainingBytes[otherBuffer];

		recorder->bufferReady[otherBuffer] = 1;
	}
}

void onButtonStateRecordingFile(Recorder *recorder) {
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
		saveFile(recorder);
	}
}
