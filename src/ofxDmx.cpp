#include "ofxDmx.h"
#include "ofMain.h"

#define DMX_PRO_HEADER_SIZE 4
#define DMX_PRO_START_MSG 0x7E
#define DMX_START_CODE 0
#define DMX_START_CODE_SIZE 1
#define DMX_PRO_SEND_PACKET 6 // "periodically send a DMX packet" mode
#define DMX_PRO_END_SIZE 1
#define DMX_PRO_END_MSG 0xE7

// Enttec DMX USB PRO MK2 specific
#define DMX_PRO_SEND_PACKET2 0xA9 // send to universe #2
#define ENTTEC_PRO_ENABLE_API2 0x0D
#define ENTTEC_PRO_PORT_ASS_REQ 0xCB

ofxDmx::ofxDmx()
:connected(false)
,needsUpdate(false) {
	universes = 1;
}

ofxDmx::~ofxDmx() {
	serial.close();
	connected = false;
}

bool ofxDmx::connect(int device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device, 57600); 
	setChannels(channels);
	return connected;
}

bool ofxDmx::connect(string device, unsigned int channels) {
	serial.listDevices();
	connected = serial.setup(device.c_str(), 57600);
	setChannels(channels);
	return connected;
}

bool ofxDmx::isConnected() {
	return connected;
}

void ofxDmx::disconnect() {
	serial.close();
    connected = false;
}

void ofxDmx::setChannels(unsigned int channels) {
	levels.resize(ofClamp(channels, 24, 512));
	levels2.resize(ofClamp(channels, 24, 512));
}

/* 	
	You can use your own API key, but don't have to! 

	ATTENTION: this won't throw an error, if no MK2 is plugged in.
	You'll just have to wait and see if your 2nd universe lights 
	turn on to know if the activation worked. Or watch out for:
	LED on device blinks RED if 2nd universe setLevel msg was rejected.
*/
void ofxDmx::activateMk2(unsigned char key0, unsigned char key1, unsigned char key2, unsigned char key3) {

	// step 1: set API -key
	unsigned int dataSize = 4;
	unsigned int packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
	vector<unsigned char> packet(packetSize);
	
	// header
	packet[0] = DMX_PRO_START_MSG;
	packet[1] = ENTTEC_PRO_ENABLE_API2;
	packet[2] = dataSize & 0xff; // data length lsb
	packet[3] = (dataSize >> 8) & 0xff; // data length msb
	
	// data = API key, LSB at lowest address
	packet[4] = key3;
	packet[5] = key2;
	packet[6] = key1;
	packet[7] = key0;
	
	// end
	packet[packetSize - 1] = DMX_PRO_END_MSG;
	
	serial.writeBytes(&packet[0], packetSize);

	ofSleepMillis(200);
	cout << "MK2: activated with API key" << endl;




	// step 2, enable both ports
	dataSize = 2;
	packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
	vector<unsigned char> packet2(packetSize);
	
	// header
	packet2[0] = DMX_PRO_START_MSG;
	packet2[1] = ENTTEC_PRO_PORT_ASS_REQ;
	packet2[2] = dataSize & 0xff; // data length lsb
	packet2[3] = (dataSize >> 8) & 0xff; // data length msb
	
	// data
	packet2[4] = 1;
	packet2[5] = 1;
	
	// end
	packet2[packetSize - 1] = DMX_PRO_END_MSG;
	
	serial.writeBytes(&packet2[0], packetSize);

	ofSleepMillis(200);
	cout << "MK2: enabled both DMX ports" << endl;

	universes = 2;

}

void ofxDmx::update(bool force) {
	if(needsUpdate || force) {
		needsUpdate = false;

		for (unsigned int i=0; i<universes; i++) {

			unsigned int dataSize = levels.size() + DMX_START_CODE_SIZE;
			unsigned int packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
			vector<unsigned char> packet(packetSize);
			
			// header
			packet[0] = DMX_PRO_START_MSG;
			if (i==0) packet[1] = DMX_PRO_SEND_PACKET;
			else packet[1] = DMX_PRO_SEND_PACKET2;
			packet[2] = dataSize & 0xff; // data length lsb
			packet[3] = (dataSize >> 8) & 0xff; // data length msb
			
			// data
			packet[4] = DMX_START_CODE; // first data byte
			if (i==0) copy(levels.begin(), levels.end(), packet.begin() + 5);
			else copy(levels2.begin(), levels2.end(), packet.begin() + 5);

			// end
			packet[packetSize - 1] = DMX_PRO_END_MSG;
			
			serial.writeBytes(&packet[0], packetSize);
		}
		
#ifdef OFXDMX_SPEW
		cout << "@" << ofGetSystemTime() << endl;
		for(int i = 0; i < packetSize; i++) {
			cout << setw(2) << hex << (int) packet[i];
			if((i + 1) % 8 == 0) {
				cout << endl;
			}
		}
#endif
	}
}

bool ofxDmx::badChannel(unsigned int channel) {
	if(channel > levels.size()) {
		ofLogError() << "Channel " + ofToString(channel) + " is out of bounds. Only " + ofToString(levels.size()) + " channels are available.";
		return true;
	}
	if(channel == 0) {
		ofLogError() << "Channel 0 does not exist. DMX channels start at 1.";
		return true;
	}
	return false;
}

void ofxDmx::setLevel(unsigned int channel, unsigned char level, unsigned int universe) {
	if(badChannel(channel)) {
		return;
	}
	channel--; // convert from 1-initial to 0-initial
	if (universe == 1) {
		if(level != levels[channel]) {
			levels[channel] = level;
			needsUpdate = true;
		}
	} else if (universe == 2) {
		if(level != levels2[channel]) {
			levels2[channel] = level;
			needsUpdate = true;
		}
	}
}

void ofxDmx::clear() {
	for (int i = 0; i < levels.size(); i++) {
		levels[i] = 0;
	}
	for (int i = 0; i < levels2.size(); i++) {
		levels2[i] = 0;
	}
}

unsigned char ofxDmx::getLevel(unsigned int channel) {
	if(badChannel(channel)) {
		return 0;
	}
	channel--; // convert from 1-initial to 0-initial
	return levels[channel];
}
