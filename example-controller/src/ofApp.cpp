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
	int pages = 0;
	for(int i = 0; i < modules; i++) {
		if(i > 0 && i %  10 == 0) {
			panel.addPanel("settings " + ofToString(pages++));
		}
		string label = "mod" + ofToString(i);
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
	for(int i = 0; i < modules; i++) {
		string label = "mod" + ofToString(i);
		int cur = panel.getValueI(label);
		int j = i * channelsPerModule;
		dmx.setLevel(j + 0, curvesTool[cur * red]);
		dmx.setLevel(j + 1, curvesTool[cur * green]);
		dmx.setLevel(j + 2, curvesTool[cur * blue]);
	}
	if(dmx.isConnected()) {
		dmx.update();
	} else {
		panel.msg = "Could not connect to port " + port;
	}
	
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
	int channel = 0;
	for(int i = 0; i < modules; i++) {
		string label = "mod" + ofToString(i);
		int rc = channel++;
		int gc = channel++;
		int bc = channel++;
		int ac = channel++;
		int r = dmx.getLevel(rc);
		int g = dmx.getLevel(gc);
		int b = dmx.getLevel(bc);
		ofSetColor(r, g, b);
		ofFill();
		ofRect(4, i * 16 + 6, 14, 14);
		ofSetColor(255);
		ofNoFill();
		ofRect(4, i * 16 + 6, 14, 14);
		string rs = ofToString(rc+1) + ":" + ofToString(r);
		string gs = ofToString(gc+1) + ":" + ofToString(g);
		string bs = ofToString(bc+1) + ":" + ofToString(b);
		string text = label + " (" + rs + ", " + gs + ", " + bs + ")";
		ofDrawBitmapString(text, 24, i * 16 + 16);
	}
	
	ofPopMatrix();
}

void ofApp::keyPressed(int key) {
}