/*
 * WavHeader.c
 *
 *  Created on: Jun 14, 2020
 *      Author: Grzesio
 */

#include "WavHeader.h"

void readText(unsigned char* buffer, unsigned int* from,
		char* text, unsigned int count) {
	for (int i = 0; i < count; ++i) {
		text[i] = buffer[*from];
		++(*from);
	}
}

void readUInt(unsigned char* buffer, unsigned int* from,
		unsigned int* value) {
	// little-endian
	*value = 0;
	for (int i = 0; i < 4; ++i) {
		*value |= buffer[*from] << (i * 8);
		++(*from);
	}
}

void readUShort(unsigned char* buffer, unsigned int* from,
		unsigned short* value) {
	*value = 0;
	for (int i = 0; i < 2; ++i) {
		*value |= buffer[*from] << (i * 8);
		++(*from);
	}
}

void writeText(unsigned char* buffer, unsigned int* from,
		char* text, unsigned int count) {
	for (int i = 0; i < count; ++i) {
		buffer[*from] = (unsigned char) text[i];
		++(*from);
	}
}

void writeUInt(unsigned char* buffer, unsigned int* from,
		unsigned int value) {
	// little-endian
	for (int i = 0; i < 4; ++i) {
		buffer[*from] = (unsigned char) (value & 0xFF);
		value >>= 8;
		++(*from);
	}
}

void writeUShort(unsigned char* buffer, unsigned int* from,
		unsigned short value) {
	// little-endian
	for (int i = 0; i < 2; ++i) {
		buffer[*from] = (unsigned char) (value & 0xFF);
		value >>= 8;
		++(*from);
	}
}

int readWavHeader(FIL* file, WavHeader* wavHeader) {
	unsigned char buffer[44];
	unsigned int bytesRead;

	if (f_read(file, buffer, 44, &bytesRead) != FR_OK
			|| bytesRead != 44) {
		return 0;
	}

	unsigned int nextByte = 0;

	readText(buffer, &nextByte, wavHeader->fileType, 4);
	readUInt(buffer, &nextByte, &wavHeader->fileSize);
	readText(buffer, &nextByte, wavHeader->fileTypeHeader, 4);

	readText(buffer, &nextByte, wavHeader->chunkFormat, 4);
	readUInt(buffer, &nextByte, &wavHeader->headerLength);

	readUShort(buffer, &nextByte, &wavHeader->wavFormat);
	readUShort(buffer, &nextByte, &wavHeader->channelCount);

	readUInt(buffer, &nextByte, &wavHeader->sampleRate);
	readUInt(buffer, &nextByte, &wavHeader->sampleRateBytes);

	readUShort(buffer, &nextByte, &wavHeader->bytesPerSample);
	readUShort(buffer, &nextByte, &wavHeader->bitsPerSample);

	readText(buffer, &nextByte, wavHeader->dataTypeHeader, 4);

	readUInt(buffer, &nextByte, &wavHeader->dataSize);

	return 1;
}

int writeWavHeader(FIL* file, WavHeader* wavHeader) {
	unsigned char buffer[44];
	unsigned int bytesWritten;

	unsigned int nextByte = 0;

	writeText(buffer, &nextByte, wavHeader->fileType, 4);
	writeUInt(buffer, &nextByte, wavHeader->fileSize);
	writeText(buffer, &nextByte, wavHeader->fileTypeHeader, 4);

	writeText(buffer, &nextByte, wavHeader->chunkFormat, 4);
	writeUInt(buffer, &nextByte, wavHeader->headerLength);

	writeUShort(buffer, &nextByte, wavHeader->wavFormat);
	writeUShort(buffer, &nextByte, wavHeader->channelCount);

	writeUInt(buffer, &nextByte, wavHeader->sampleRate);
	writeUInt(buffer, &nextByte, wavHeader->sampleRateBytes);

	writeUShort(buffer, &nextByte, wavHeader->bytesPerSample);
	writeUShort(buffer, &nextByte, wavHeader->bitsPerSample);

	writeText(buffer, &nextByte, wavHeader->dataTypeHeader, 4);

	writeUInt(buffer, &nextByte, wavHeader->dataSize);

	if (f_write(file, buffer, 44, &bytesWritten) != FR_OK
			|| bytesWritten != 44) {
		return 0;
	}

	return 1;
}
