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
	void disconnect();
	
	void setLevel(unsigned int channel, unsigned char level);
	void clear();
	unsigned char getLevel(unsigned int channel);	
	void update(); // send a packet to the dmx controller
	
	void setChannels(unsigned int channels = 24); // change the number of channels
	bool isConnected();
	
private:	
	int connected;
	vector<unsigned char> levels;
	ofSerial serial;
};

