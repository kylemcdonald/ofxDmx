#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 512, 256, OF_WINDOW);
	ofRunApp(new ofApp());
}
