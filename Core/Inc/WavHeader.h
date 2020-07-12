/*
 * WavHeader.h
 *
 *  Created on: 14 cze 2020
 *      Author: Grzesio
 */

#ifndef INC_WAVHEADER_H_
#define INC_WAVHEADER_H_

#include "ff.h"

typedef volatile struct {
	char fileType[4];// "RIFF"
	unsigned int fileSize;// wszystko minus 8

	char fileTypeHeader[4];// "WAVE"
	char chunkFormat[4];// "fmt "
	unsigned int headerLength;// 16

	unsigned short wavFormat;// 1 (PCM)
	unsigned short channelCount;// 1
	unsigned int sampleRate;// probkowanie
	unsigned int sampleRateBytes;// u nas powinno byc tyle samo co sampleRate
	unsigned short bytesPerSample;// u nas 1
	unsigned short bitsPerSample;// u nas 8

	char dataTypeHeader[4];// "data"
	unsigned int dataSize;

} WavHeader;

int readWavHeader(FIL* file, WavHeader* wavHeader);
int writeWavHeader(FIL* file, WavHeader* wavHeader);

#endif /* INC_WAVHEADER_H_ */
