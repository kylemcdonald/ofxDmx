#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	curvesTool.setup(256);
	img.allocate(256, 256, OF_IMAGE_GRAYSCALE);
	
	ofxXmlSettings xml("dmx.xml");
	port = xml.getValue("port", "");
	modules = xml.getValue("modules", 10);
	channelsPerModule = xml.getValue("channelsPerModule", 4);
	dmx.connect(port, modules * channelsPerModule);
	
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
}

void ofApp::update() {
	if(curvesTool.isLutNew()) {
		for(int x = 0; x < 256; x++) {
			for(int y = 0; y < 256; y++) {
				img.setColor(x, y, ofColor(curvesTool[x]));
			}
		}
		img.update();
	}
	
	float red = panel.getValueF("red");
	float green = panel.getValueF("green");
	float blue = panel.getValueF("blue");
	int channel = 1;
	for(int module = 1; module <= modules; module++) {
		string label = "mod" + ofToString(module);
		int cur = panel.getValueI(label);
		dmx.setLevel(channel++, curvesTool[cur * red]);
		dmx.setLevel(channel++, curvesTool[cur * green]);
		dmx.setLevel(channel++, curvesTool[cur * blue]);
		channel++;
	}
	//if(dmx.isConnected()) {
		dmx.update();
	/*} else {
		panel.msg = "Could not connect to port " + port;
	}*/
	
	if(panel.getValueB("saveCurves")) {
		curvesTool.save("curves.yml");
		panel.setValueB("saveCurves", false);
	}
	if(panel.getValueB("loadCurves")) {
		curvesTool.load("curves.yml");
		panel.setValueB("loadCurves", false);
	}
}

void ofApp::draw() {
	ofBackground(0);
	ofPushMatrix();
	ofTranslate(256 + 8, 4);
	curvesTool.draw(0);
	img.draw(256, 0);
	
	ofTranslate(0, 256);
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