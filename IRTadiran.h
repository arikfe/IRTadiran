/*
 * IRTadiran.h
 *
 *  Created on: May 24, 2018
 *      Author: arik
 */

#ifndef INCLUDE_IRTADIRAN_H_
#define INCLUDE_IRTADIRAN_H_
#include <IR/src/IRsend.h>
#include <IR/src/IRremoteESP8266.h>
#include <stdint.h>
#include <vector>

#define HEADER_MARK 8000
#define HEADER_SPACE 4200
#define ONE_MARK 1618
#define ONE_SPACE 545
#define ZERO_MARK 545
#define ZERO_SPACE 1618


class IRTadiran {
	public:

		virtual ~IRTadiran();

		// Initialize
		IRTadiran(IRsend* remote);
		void setTemp(uint8_t temp);
		void setFan(uint8_t fan);
		void setMode(uint8_t mode);
		void setOn(bool isOn);
		// Send an IR packet with the given parameters.
		bool send(bool power, int mode, int fan,
				int temperature, bool swing);

	private:
		uint8_t code[8];
		IRsend* _remote;

		uint16_t* createBuffer();
		void updateChecksum();
};

#endif /* INCLUDE_IRTADIRAN_H_ */
