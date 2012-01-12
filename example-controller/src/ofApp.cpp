#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	redCurve.setup(256);
	greenCurve.setup(256);
	blueCurve.setup(256);
	
	ofxXmlSettings xml("dmx.xml");
	port = xml.getValue("port", "");
	modules = xml.getValue("modules", 10);
	channelsPerModule = xml.getValue("channelsPerModule", 4);
	
	panel.setup(256, 740);
	panel.setPosition(4, 4);
	panel.addPanel("settings 0");
	panel.addToggle("saveCurves", false);
	panel.addToggle("loadCurves", false);
	panel.addSlider("red", 1, 0, 1);
	panel.addSlider("green", 1, 0, 1);
	panel.addSlider("blue", 1, 0, 1);
	for(int module = 1; module <= modules; module++) {
		string label = "mod" + ofToString(module);
		panel.addSlider(label, 0, 0, 255, true);
	}
	panel.loadSettings("settings.xml");
	
	panel.setValueB("loadCurves", true);
	
	dmx.connect(port, modules * channelsPerModule);
	dmx.update(true); // black on startup
}

void ofApp::exit() {
	dmx.clear();
	dmx.update(true); // black on shutdown
}

void ofApp::update() {
	if(panel.getValueB("saveCurves")) {
		redCurve.save("redCurve.yml");
		greenCurve.save("greenCurve.yml");
		blueCurve.save("blueCurve.yml");
		panel.setValueB("saveCurves", false);
	}
	if(panel.getValueB("loadCurves")) {
		redCurve.load("redCurve.yml");
		greenCurve.load("greenCurve.yml");
		blueCurve.load("blueCurve.yml");
		panel.setValueB("loadCurves", false);
	}
	
	float red = panel.getValueF("red");
	float green = panel.getValueF("green");
	float blue = panel.getValueF("blue");
	int channel = 1;
	for(int module = 1; module <= modules; module++) {
		string label = "mod" + ofToString(module);
		int cur = panel.getValueI(label);
		dmx.setLevel(channel++, redCurve[cur * red]);
		dmx.setLevel(channel++, greenCurve[cur * green]);
		dmx.setLevel(channel++, blueCurve[cur * blue]);
		channel++;
	}
	if(dmx.isConnected()) {
		dmx.update();
	} else {
		panel.msg = "Could not connect to port " + port;
	}
}

void ofApp::draw() {
	ofBackground(0);
	ofPushMatrix();
	ofTranslate(256 + 8, 4);
	redCurve.draw(0, 0);
	greenCurve.draw(0, 256);
	blueCurve.draw(0, 512);
	
	ofTranslate(256, 0);
	int channel = 1;
	for(int module = 1; module <= modules; module++) {
		string label = "mod" + ofToString(module);
		int rc = channel++;
		int gc = channel++;
		int bc = channel++;
		int ac = channel++;
		int r = dmx.getLevel(rc);
		int g = dmx.getLevel(gc);
		int b = dmx.getLevel(bc);
		ofSetColor(r, g, b);
		ofFill();
		ofRect(4, module * 16 + 6, 14, 14);
		ofSetColor(255);
		ofNoFill();
		ofRect(4, module * 16 + 6, 14, 14);
		string rs = ofToString(rc) + ":" + ofToString(r);
		string gs = ofToString(gc) + ":" + ofToString(g);
		string bs = ofToString(bc) + ":" + ofToString(b);
		string text = label + " (" + rs + ", " + gs + ", " + bs + ")";
		ofDrawBitmapString(text, 24, module * 16 + 16);
	}
	
	ofPopMatrix();
}

void ofApp::keyPressed(int key) {
}