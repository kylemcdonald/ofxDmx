#pragma once

#include "ofMain.h"

#include "ofxCurvesTool.h"
#include "ofxDmx.h"
#include "ofxAutoControlPanel.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxCurvesTool curvesTool;
	ofxDmx dmx;
	ofImage img;
	ofxAutoControlPanel panel;
	string port;
	int modules, channelsPerModule;
};
