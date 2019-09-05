#pragma once

#include "ofMain.h"
#include "Constants.hpp"
#include "ofxSimpleSerial.h"
#include "DashedLine.hpp"
#include "DottedLine.hpp"
#include "HandMarker.hpp"
#include "ImageAnimation.hpp"
#include "TextTyping.hpp"
#include "TextFading.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void setupMedia();
        void setupAnimations();
		void update();
		void updateSerials();
		void draw();
		void drawBackground();
		void drawHandMarkers();
		void drawAnimations();
		void onNewMessage(string & message);
		void keyPressed(int key);
        void checkShouldRunAnimations(int index);
		void runAnimation(int animationNum);

		ofxSimpleSerial	mySerial;
		int baud = 57600;
        ofColor red = ofColor(220, 20, 30);
    	int serial1 = 0;
    	int serial2 = 0;

    	float width;
    	float height;

        float fixedWidth = 1920.0;
        float fixedHeight = 1080.0;

        int incrementer = 0;
    	float universalCounter = 0.0;

    	string message;

    	vector<int> serialInput;

    	std::array<int, 4> touchThreshold = {{0, 0, 0, 0}};

    	std::array<bool, 4> shouldRunAnimation = {{false, false, false, false}};
    	std::array<int, 4> animationCounter = {{0, 0, 0, 0}};
    	std::array<int, 4> animationCounterMax = {{5000, 5000, 5000, 5000}};

    	// All the hand markers declared here
    	std::array<HandMarker, 4> handMarkers;

        // All images declared here
        ofImage background;
        ofImage text1, text2, text3, text4, text5, text6, text7;
        // ofImage personName;

        // All image animations
        // ImageAnimation personNameAnimation;

        // All Text Typing animations
        // TextTyping personNameText;

        // All Text Fading animation stuff
		vector<ofImage> texts;
        TextFading manifestoFading;
};
