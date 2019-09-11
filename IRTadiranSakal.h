/*
 * IRTadiranSakal.h
 *
 *  Created on: Aug 9, 2019
 *      Author: arik
 */

#ifndef INCLUDE_IRTADIRANSAKAL_H_
#define INCLUDE_IRTADIRANSAKAL_H_
#include <IR/src/IRsend.h>
#include <IR/src/IRremoteESP8266.h>
#include <stdint.h>
#include <vector>
class IRTadiranSakal {
	public:
		IRTadiranSakal(IRsend* remote);
		virtual ~IRTadiranSakal();
		void setTemp(uint8_t temp);
		void setFan(uint8_t fan);
		void setMode(uint8_t mode);
		void setOn(bool isOn);
		// Send an IR packet with the given parameters.
		bool send(bool power, int mode, int fan, int temperature, int swing,int light);
	private:
		uint8_t code[9];
		IRsend* _remote;

		uint16_t* createBuffer(int repeat =2);
		void updateChecksum();
};

#endif /* INCLUDE_IRTADIRANSAKAL_H_ */
