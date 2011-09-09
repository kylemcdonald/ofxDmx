#include "ofxDmx.h"
#include "ofMain.h"

#define DMX_PRO_HEADER_SIZE 4
#define DMX_PRO_START_MSG 0x7E
#define DMX_PRO_SEND_PACKET 6 // periodically send a DMX packet at this rate
#define DMX_PRO_END_SIZE 1
#define DMX_PRO_END_MSG 0xE7

ofxDmx::ofxDmx()
:connected(false)
,needsUpdate(false) {
}

ofxDmx::~ofxDmx() {
	serial.close();
	connected = false;
}

bool ofxDmx::connect(int device, unsigned int channels) {
	serial.enumerateDevices();
	serial.setVerbose(false);
	connected = serial.setup(device, 57600); 
	setChannels(channels);
	return connected;
}

bool ofxDmx::connect(string device, unsigned int channels) {
	serial.enumerateDevices();
	serial.setVerbose(false);
	connected = serial.setup(device.c_str(), 57600);
	setChannels(channels);
	return connected;
}

bool ofxDmx::isConnected() {
	return connected;
}

void ofxDmx::disconnect() {
	serial.close();
}

void ofxDmx::setChannels(unsigned int channels) {
	levels.resize(ofClamp(channels, 24, 512));
}

void ofxDmx::update(bool force) {
	if(needsUpdate || force) {
		needsUpdate = false;
		unsigned int n = levels.size();
		unsigned int packetSize = DMX_PRO_HEADER_SIZE + n + DMX_PRO_END_SIZE;
		vector<unsigned char> packet(packetSize);
		
		// header
		packet[0] = DMX_PRO_START_MSG;
		packet[1] = DMX_PRO_SEND_PACKET;
		packet[2] = n & 255; // data length lsb
		packet[3] = (n >> 8) & 255; // data length msb
		
		// levels
		copy(levels.begin(), levels.end(), packet.begin() + 4);
		
		// end
		packet[packetSize - 1] = DMX_PRO_END_MSG;
		
		serial.writeBytes(&packet[0], packetSize);
	}
}

void ofxDmx::setLevel(unsigned int channel, unsigned char level) {
	if(level != levels[channel]) {
		needsUpdate = true;
	}
	levels[channel] = level;
}

void ofxDmx::clear() {
	for (int i = 0; i < levels.size(); i++) {
		levels[i] = 0;
	}
}

unsigned char ofxDmx::getLevel(unsigned int channel) {
	return levels[channel];
}
