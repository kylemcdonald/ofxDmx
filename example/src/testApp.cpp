#include "testApp.h"

void testApp::setup() {
	dmx.connect("tty.usbserial-ENS8KBA6"); // use the name
	//dmx.connect(0); // or use a number
}

void testApp::update() {
	// use the time to generate a level
	level = ofMap(sin(ofGetElapsedTimef() * 10), -1, 1, 0, 255);
	
	// pulses channels 1 and 2
	dmx.setLevel(1, level);
	dmx.setLevel(2, 255 - level);
	dmx.update();
}

void testApp::draw() {
	ofSetColor(level);
	ofRect(0, 0, ofGetWidth() / 2, ofGetHeight());
	
	ofSetColor(255 - level);
	ofRect(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight());
}
