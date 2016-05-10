#pragma once

#include "ofMain.h"

#include "ofxDmx.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();
	
	ofxDmx dmx;
	ofxPanel panel;
	string port;
	int modules, channelsPerModule;
    ofParameter<bool> moduleNum[11];
    
    ofParameter<float> red[11], green[11], blue[11];
    ofParameter<bool> load, save;
};
