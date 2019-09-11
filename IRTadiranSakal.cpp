/*
 * IRTadiranSakal.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: arik
 */

#include "IRTadiranSakal.h"

#include <SmingCore/SmingCore.h>
IRTadiranSakal::IRTadiranSakal(IRsend* remote) :
		_remote(remote) {

	code[0] = 0x19; // fan | mode
	code[1] = 0x9; //temp
	code[2] = 0x60; // light
	code[3] = 0x50;
	code[4] = 2;	//swing
	code[5] = 0; //on
	code[6] = 0;
	code[7] = 0x80; //checksum
	code[8] = 6;
}

IRTadiranSakal::~IRTadiranSakal() {
	// TODO Auto-generated destructor stub
}

void IRTadiranSakal::setTemp(uint8_t temp) {
	code[1] = ((code[1] & 0xf0) | (temp - 0x10));
	updateChecksum();
}

void IRTadiranSakal::setFan(uint8_t fan) {
	code[0] = ((code[0] & 0xf) | (fan << 4));
	updateChecksum();
}

void IRTadiranSakal::setMode(uint8_t mode) {
	int val = 0;
	switch (mode) {
		case 1:
			val = 0xc;
			break;
		case 2:
			val = 0xa;
			break;
		case 3:
			val = 9;
			break;
		case 8:
			val = 0xb;
			break;
		default:
			val = mode;
	}
	code[0] = ((code[0] & 0xf0) | (val));
	updateChecksum();
}

void IRTadiranSakal::setOn(bool isOn) {
	if (!isOn) {
			code[0] = 0x11;
			code[1] = 0xa;
			code[2] = 0x20;
			code[3] = 0x50;
			code[4] = 0x12;
			code[5] = 0;
			code[6] = 2;
			code[7] = 0x80;
			code[8] = 4;

		}
//	updateChecksum();
}

bool IRTadiranSakal::send(bool power, int mode, int fan, int temperature,
		int swing, int light) {
	code[1] = temperature - 0x10;
	setMode(mode);
	setFan(fan);
	code[2] = light ? 0x60 : 0x40;
	code[4] = swing > 0 ? swing * 8 + 0x12 : 2;
	setOn(power);
	uint16_t* buff = createBuffer(1);
	int buffSise = 2 * 69 + 1;
	Serial.printf("buffer created\n");
	_remote->sendRaw(buff, buffSise, 38);
	return true;
}

uint16_t* IRTadiranSakal::createBuffer(int repeat) {

	int i = 0;
	uint16_t* buff = new uint16_t[repeat * 69 * 2 + 1];
	for (int l = 0; l < repeat; l++) {
		int count = 0;
		buff[i++] = 9000;
		buff[i++] = 4400;
		for (int j = 0; j < 8; j++) {
			for (int mask = 1; mask < 256; mask <<= 1) {

				buff[i++] = 590;
				buff[i++] = mask & code[j] ? 1594 : 590;
				if (count == 34) {
					buff[i++] = 590;
					buff[i++] = 19700;
				}
				count++;
			}
		}
		for (int mask = 1; mask < 8; mask <<= 1) {

			buff[i++] = 590;
			buff[i++] = mask & code[8] ? 1594:  590;
		}
		if (l < repeat - 1) {
			buff[i++] = 590;
			buff[i++] = 39976;
		}

	}
	buff[i++] = 590;
	Serial.printf("index End %d\n", i);

	return buff;
}

void IRTadiranSakal::updateChecksum() {
	int sum = 0;
	for (int i = 0; i < 8; i++)
		sum += code[i];
	sum -= code[4];
	sum += 2;
	int t = sum & 15;
	code[7] = (t & 1) ? 0x80 : 0;
	code[8] = (t / 2);
	code[8] = code[8] ^ (1 << 2);
//	code[8] = code[8]^ (1 << 2);
}
