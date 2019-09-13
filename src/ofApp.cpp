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
	mySerial.startContinuousRead(false);
	ofAddListener(mySerial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	message = "";
	setupMedia();
	setupAnimations();
}

//--------------------------------------------------------------
void ofApp::setupMedia(){
	// IMAGES
	background.load("images/background.png");
	text1.load("images/manifesto-1.png");
	text2.load("images/manifesto-2.png");
	text3.load("images/manifesto-3.png");
	text4.load("images/manifesto-4.png");
	text5.load("images/manifesto-5.png");
	text6.load("images/manifesto-6.png");
	text7.load("images/manifesto-7.png");
	text8.load("images/manifesto-8.png");
	text9.load("images/manifesto-9.png");
	text10.load("images/manifesto-10.png");
	text11.load("images/manifesto-11.png");
	peopleEurope.load("images/people-europe.png");
	peopleNorthAmerica.load("images/people-northamerica.png");
	peopleSouthAmerica.load("images/people-southamerica.png");
	dotsEurope.load("images/dots-europe.png");
	dotsNorthAmerica.load("images/dots-northamerica.png");
	dotsSouthAmerica.load("images/dots-southamerica.png");
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
	pplEuropeAnimation = ImageAnimation(
		peopleEurope,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
	pplSouthAmericaAnimation = ImageAnimation(
		peopleSouthAmerica,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
	pplNorthAmericaAnimation = ImageAnimation(
		peopleNorthAmerica,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
	dotsEuropeAnimation = ImageAnimation(
		dotsEurope,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
	dotsSouthAmericaAnimation = ImageAnimation(
		dotsSouthAmerica,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
	dotsNorthAmericaAnimation = ImageAnimation(
		dotsNorthAmerica,
		glm::vec2(fixedWidth/2.0, fixedHeight/2.0),
		glm::vec2(fixedWidth, fixedHeight),
		250
	);
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
	texts.push_back(text8);
	texts.push_back(text9);
	texts.push_back(text10);
	texts.push_back(text11);
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
	cout << "THE MESSAGE " << message << endl;
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
	}
	if (shouldRunOne) {
		// Reset all animations (if not already running this one)
		// then run this one
		if (!shouldRunAnimation[index] == true) {
			for (int i = 0; i < shouldRunAnimation.size(); i++) {
				shouldRunAnimation[i] = false;
				animationCounter[i] = 0;
			}
		}
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
			pplEuropeAnimation.update(animationCounter[0]);
			pplEuropeAnimation.draw();
			dotsEuropeAnimation.update(animationCounter[0]);
			dotsEuropeAnimation.draw();
			ofPopStyle();
			break;
		// SOUTH AMERICA PRESSED
		// PEOPLE SHOW UP
		case 1:
			ofPushStyle();
			pplSouthAmericaAnimation.update(animationCounter[1]);
			pplSouthAmericaAnimation.draw();
			dotsSouthAmericaAnimation.update(animationCounter[1]);
			dotsSouthAmericaAnimation.draw();
			ofPopStyle();
			break;
		// NORTH AMERICA PRESSED
		// PEOPLE SHOW UP
		case 2:
			ofPushStyle();
			pplNorthAmericaAnimation.update(animationCounter[2]);
			pplNorthAmericaAnimation.draw();
			dotsNorthAmericaAnimation.update(animationCounter[2]);
			dotsNorthAmericaAnimation.draw();
			ofPopStyle();
			break;
		default:
			// Do nothing, this is meaningless
			break;
	}
}
