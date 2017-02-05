#include "ofApp.h"

void ofApp::setup() {

	dmx.connect("tty.usbserial-EN143965"); // use the name
	//dmx.connect(0); // or use a number
    
    chan1.set("Channel 1", 120, 0, 255);
    chan2.set("Channel 2", 120, 0, 255);
    chan3.set("Channel 3", 120, 0, 255);
    autoCycle.set("AutoCycle", false);
    panel.setName("Panel");
    panel.setup();
    panel.add(chan1);
    panel.add(chan2);
    panel.add(chan3);
    panel.add(autoCycle);
}

void ofApp::update() {

	// use the time to generate a level
    if (autoCycle) {
        chan1 = 127 + 127 * sin(2 * ofGetElapsedTimef());
        chan2 = 127 + 127 * sin(-2 * ofGetElapsedTimef());
        chan3 = 127 + 127 * sin(1.5 * ofGetElapsedTimef());
    }

	dmx.setLevel(1, chan1);
	dmx.setLevel(2, chan2);
    dmx.setLevel(3, chan3);
	dmx.update();
}

void ofApp::draw() {

	ofSetColor(chan1);
	ofDrawRectangle(0, 0, ofGetWidth() / 3, ofGetHeight());
	
	ofSetColor(chan2);
	ofDrawRectangle(ofGetWidth() / 3, 0, ofGetWidth() / 3, ofGetHeight());
    
    ofSetColor(chan3);
    ofDrawRectangle(2*(ofGetWidth() / 3), 0, ofGetWidth() / 3, ofGetHeight());
    
    panel.draw();
}
