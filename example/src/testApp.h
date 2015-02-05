#pragma once

#include "ofMain.h"
#include "ofxDmx.h"
#include "ofxGui.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofxDmx dmx;
	int level;
    
    ofxPanel panel;
    ofParameter<int> chan1;
    ofParameter<int> chan2;
    ofParameter<int> chan3;
    ofParameter<bool> autoCycle;
};
