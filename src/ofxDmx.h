// TODO: support input, and perhaps other devices.

#pragma once

#include <ofSerial.h> 

class ofxDmx {
public:
	ofxDmx();
	~ofxDmx();
	
	// connect to the serial port. valid number of channels is 24-512. performance
	// is directly related to the number of channels, so use the minimum required.
	bool connect(int device = 0, unsigned int channels = 24);
	bool connect(string device, unsigned int channels = 24);
	void activateMk2(unsigned char key0 = 0xC8, unsigned char key1 = 0xD0, unsigned char key2 = 0x88, unsigned char key3 = 0xAD);
	void disconnect();
	
	void setLevel(unsigned int channel, unsigned char level, unsigned int universe = 1);
	void clear();
	unsigned char getLevel(unsigned int channel);	
	void update(bool force = false); // send a packet to the dmx controller
	
	void setChannels(unsigned int channels = 24); // change the number of channels
	bool isConnected();
	
private:	
	int connected;
	int universes;
	vector<unsigned char> levels;
	vector<unsigned char> levels2;	// 2nd universe, only for MK2
	ofSerial serial;
	bool needsUpdate;
	
	bool badChannel(unsigned int channel);
};
