/*
 * IRTadiran.cpp
 *
 *  Created on: May 24, 2018
 *      Author: arik
 */

#include "IRTadiran.h"
#include <SmingCore/SmingCore.h>
//int calculateChecksum() {
//	int sum = 0;
//	for (int i = 0; i < words.size() - 1; i++)
//		sum += words.get(i).getNumber();
//	int factor = 0xf * (3 + temp / 8) + fan * 0xf;
//}
IRTadiran::IRTadiran(IRsend* remote) :
		_remote(remote) {
	code[0] = 0x1;
	code[1] = 0x14; //fan
	code[2] = 0x30; //temp
	code[3] = 0;
	code[4] = 0;
	code[5] = 0x30; //on
	code[6] = 0;
	code[7] = 0xb; //checksum
}

IRTadiran::~IRTadiran() {
}

uint16_t* IRTadiran::createBuffer() {
	uint16_t* buff = new uint16_t[264];
	int i = 0, repeat = 2;
	for (int l = 0; l < repeat; l++) {
		buff[i++] = 8000;
		buff[i++] = 4000;
		Serial.printf("index %d\n", i);
		for (int j = 0; j < 8; j++) {
			for (int mask = 1; mask < 256; mask <<= 1) {

				buff[i++] = mask & code[j] ? 1618 : 545;
				buff[i++] = mask & code[j] ? 545 : 1618;
			}
		}

		Serial.printf("index after data %d\n", i);
		if (l < repeat - 1) {
			buff[i++] = 1618;
			buff[i++] = 31000;
			Serial.printf("index gap %d\n", i);
		}

		Serial.printf("index after gap %d\n", i);
	}
	buff[i++] = 1618;
	buff[i++] = 1618;
	return buff;
}

void IRTadiran::updateChecksum() {
	int sum = 0;
	for (int i = 0; i < 7; i++)
		sum += code[i];
	int temp = code[2] / 2;
	int fan = (code[1] & 0xf0) >> 4;
	bool swing = (code[6] & 0xc0)!=0;
	code[7] = sum - (0xf * (3 + temp / 8) + (fan) * 0xf + (swing? 0xb4 : 0 ));
}

bool IRTadiran::send(bool power, int mode, int fan, int temperature,
bool swing) {
	if (power) {
		code[2] = 2 * temperature;
		code[1] = ((1 + fan) << 4) | (mode & 0xf);
		code[6] = (swing ? (code[6] | 0xc0) : (~(0xc0) & code[6]));
		updateChecksum();
	}
	else {
		code[0] = 0x1;
		code[1] = 0x14;
		code[2] = 0x30;
		code[6] = code[4] = code[3] = 0x0;
		code[5] = 0xc0;
		code[7] = 0x15;
	}
	Serial.printf("\n----data\n");
	for(int i=0;i<8;i++){
		Serial.printf("0x%08x\n", code[i]);
	}
	uint16_t* buff = createBuffer();
	_remote->sendRaw(buff, 264, 38);
	delete (buff);
	return true;
}

void IRTadiran::setTemp(uint8_t temp) {
	code[2] = 2 * temp;
	updateChecksum();
}

void IRTadiran::setFan(uint8_t fan) {
	code[1] = ((1 + fan) << 4) | (code[1] & 0xf);
	updateChecksum();
}

void IRTadiran::setMode(uint8_t mode) {
	code[1] = (code[1] & 0xf0) | (mode & 0xf);
	updateChecksum();
}

void IRTadiran::setOn(bool isOn) {
	code[5] = ((isOn ? 0x30 : 0xc0)) | (code[5] & 0xf);
	updateChecksum();
}
