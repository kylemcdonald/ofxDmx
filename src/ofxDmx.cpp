#include "ofxDmx.h"
#include "ofMain.h"

#define DMX_PRO_HEADER_SIZE 4
#define DMX_PRO_START_MSG 0x7E
#define DMX_START_CODE 0
#define DMX_START_CODE_SIZE 1
#define DMX_PRO_SEND_PACKET 6 // "periodically send a DMX packet" mode
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
		unsigned int dataSize = levels.size() + DMX_START_CODE_SIZE;
		unsigned int packetSize = DMX_PRO_HEADER_SIZE + dataSize + DMX_PRO_END_SIZE;
		vector<unsigned char> packet(packetSize);
		
		// header
		packet[0] = DMX_PRO_START_MSG;
		packet[1] = DMX_PRO_SEND_PACKET;
		packet[2] = dataSize & 0xff; // data length lsb
		packet[3] = (dataSize >> 8) & 0xff; // data length msb
		
		// data
		packet[4] = DMX_START_CODE; // first data byte
		copy(levels.begin(), levels.end(), packet.begin() + 5);
		
		// end
		packet[packetSize - 1] = DMX_PRO_END_MSG;
		
		serial.writeBytes(&packet[0], packetSize);
		
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

void ofxDmx::setLevel(unsigned int channel, unsigned char level) {
	if(badChannel(channel)) {
		return;
	}
	channel--; // convert from 1-initial to 0-initial
	if(level != levels[channel]) {
		levels[channel] = level;
		needsUpdate = true;
	}
}

void ofxDmx::clear() {
	for (int i = 0; i < levels.size(); i++) {
		levels[i] = 0;
	}
}

unsigned char ofxDmx::getLevel(unsigned int channel) {
	if(badChannel(channel)) {
		return 0;
	}
	channel--; // convert from 1-initial to 0-initial
	return levels[channel];
}
