#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
	cout << "WIDTH: " << width << " HEIGHT: " << height << endl;
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	mySerial.setup(0, baud); //open the first device
	mySerial.startContinuousRead();
	ofAddListener(mySerial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	message = "";
	setupMedia();
	setupAnimations();
}

//--------------------------------------------------------------
void ofApp::setupMedia(){
	// IMAGES
	background.load("images/background.png");
	text1.load("images/lorem1.png");
	text2.load("images/lorem2.png");
	text3.load("images/lorem3.png");
	text4.load("images/lorem4.png");
	text5.load("images/lorem5.png");
	text6.load("images/lorem6.png");
	text7.load("images/lorem7.png");
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE ANIMATIONS GO HERE
	 */
	// NOTHING TO SEE HERE SO FAR
	/**
	 * ALL THE IMAGE ANIMATIONS GO HERE
	 */
	// personOneAnimation = ImageAnimation(
	// 	personOne,
	// 	glm::vec2(fixedWidth/2.0, fixedHeight - 120),
	// 	glm::vec2(800, 184),
	// 	300
	// );
	/**
	 * ALL THE TEXT TYPING GO HERE
	 */
	// personOneText = TextTyping(
	// 	"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
	// 	glm::vec2(150, 780),
	// 	ofColor(255),
	// 	7,
	// 	150
	// );
	/**
	 * ALL THE TEXT FADING GO HERE
	 */
	texts.push_back(text1);
	texts.push_back(text2);
	texts.push_back(text3);
	texts.push_back(text4);
	texts.push_back(text5);
	texts.push_back(text6);
	texts.push_back(text7);
	manifestoFading = TextFading(texts);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// EUROPE
	handMarkers[0] = HandMarker(glm::vec2(60, fixedHeight - 100), red, true);
	// SOUTH AMERICA
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth/2.0 + 40, fixedHeight - 115), red, false);
	// NORTH AMERICA
	handMarkers[2] = HandMarker(glm::vec2(fixedWidth/3.0 * 2.0 + 60, fixedHeight - 185), red, false);
	// ASIA
	handMarkers[3] = HandMarker(glm::vec2(60, fixedHeight/2.0 + 80), red, true);
}

//--------------------------------------------------------------
void ofApp::update(){
	updateSerials();
	incrementer++;
	if (fabs(universalCounter - TWO_PI) < 0.0001) {
		universalCounter = 0.0f;
	} else {
		universalCounter = (universalCounter + PI/100.0);
	}
	for (int i = 0; i < handMarkers.size(); i++) {
		handMarkers[i].update(universalCounter);
	}
}

//--------------------------------------------------------------
void ofApp::updateSerials(){
	// Loop through the serial inputs
	for (int i = 0; i < serialInput.size(); i++) {
		// If we have something going on with the serial input...
		if (serialInput[i] == 1) {
			// Increase the threshold number,
			touchThreshold[i]++;
			// and once that reaches 10 we can run the animation
			if (touchThreshold[i] >= 50) {
				// Reset the threshold
				touchThreshold[i] = 0;
				checkShouldRunAnimations(i);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::onNewMessage(string & message){
	vector<string> input = ofSplitString(message, ",");
	serialInput.clear();
	for (int i = 0; i < input.size() - 1; i++) {
		serialInput.push_back(ofToInt(input[i]));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawBackground();
	drawAnimations();
	drawHandMarkers();
}

//--------------------------------------------------------------
void ofApp::drawBackground(){
	ofPushStyle();
	ofBackground(0);
	ofSetColor(255);
	// Draw up and down lines
	for (int i = 0; i < 2; i++) {
		ofDrawRectangle(fixedWidth/3.0 * (i + 1) - 1, 0, 2, fixedHeight);
		float divConst = 5.0;
		if (i == 0) {
			ofDrawRectangle(0, fixedHeight/divConst, fixedWidth, 2);
		} else {
			ofDrawRectangle(0, fixedHeight - fixedHeight/divConst, fixedWidth, 2);
		}
	}
	ofPopStyle();
	// background.draw(0, 0, fixedWidth, fixedHeight);
	manifestoFading.update(incrementer);
	manifestoFading.draw();
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	handMarkers[0].draw();
	handMarkers[1].draw();
	handMarkers[2].draw();
	handMarkers[3].draw();
}

//--------------------------------------------------------------
void ofApp::drawAnimations(){
	bool runningAnyAnimations = false;
	// Run through the `shouldRunAnimation` array, and run any of the animations that we should
	for (int i = 0; i < shouldRunAnimation.size(); i++) {
		if (shouldRunAnimation[i]) {
			// We are in fact running an animation
			runningAnyAnimations = true;
			// Increase the animation counter,
			// which is what we're using to run the animation for
			// a set amount of time
			animationCounter[i]++;
			// Run the fucking animation
			runAnimation(i);
		}
	}
	// If we are running any animations, increment
	// the universal animation counter
	// and check if we should stop running all animations
	if (runningAnyAnimations) {
		universalAnimationCounter++;
		if (universalAnimationCounter >= universalAnimationCounterMax) {
			universalAnimationCounter = 0;
			for (int i = 0; i < shouldRunAnimation.size(); i++) {
				shouldRunAnimation[i] = false;
				animationCounter[i] = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Keys 1 through 9 are 49 through 57
	// so use this to determine the index of the animation we should run
	if (key >= 49 && key <= 57) {
		int keyIndex = key - 49;
		// Simulate increasing the threshold number,
		touchThreshold[keyIndex] = 50;
		// and once that reaches 50 we can run the animation
		if (touchThreshold[keyIndex] >= 50) {
			// Reset the threshold
			touchThreshold[keyIndex] = 0;
			checkShouldRunAnimations(keyIndex);
		}
	} else if (key == 32) {
		for (int i = 0; i < shouldRunAnimation.size(); i++) {
			shouldRunAnimation[i] = false;
			animationCounter[i] = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::checkShouldRunAnimations(int index){
	bool shouldRunOne = false;
	// EVERY BUTTON SHOULD RUN ANIMATIONS, NO CHECKS
	if (index == 0) { // EUROPE
		shouldRunOne = true;
	} else if (index == 1) { // SOUTH AMERICA
		shouldRunOne = true;
	} else if (index == 2) { // NORTH AMERICA
		shouldRunOne = true;
	} else if (index == 3) { // ASIA
		shouldRunOne = true;
	}
	if (shouldRunOne) {
		shouldRunAnimation[index] = true;
		universalAnimationCounter = 0;
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		// EUROPE PRESSED
		// PEOPLE SHOW UP
		case 0:
			ofPushStyle();
			ofPopStyle();
			break;
		// SOUTH AMERICA PRESSED
		// PEOPLE SHOW UP
		case 1:
			ofPushStyle();
			ofPopStyle();
			break;
		// NORTH AMERICA PRESSED
		// PEOPLE SHOW UP
		case 2:
			ofPushStyle();
			ofPopStyle();
			break;
		// ASIA PRESSED
		// PEOPLE SHOW UP
		case 3:
			ofPushStyle();
			ofPopStyle();
			break;
		default:
			// Do nothing, this is meaningless
			break;
	}
}
