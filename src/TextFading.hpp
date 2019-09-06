//
//  TextFading.hpp
//  Text Scrolling Class
//
//

#pragma once
#include "ofMain.h"

class TextFading
{
public:

	TextFading();
	TextFading(vector<ofImage> _texts);

	void setup();
	void update(int _inc);
	void draw();

	float quadEaseOut(float t);

	vector<ofImage> texts;
	int inc;
	int textIncrementer = 0;

	int incMax = 3000;
	int fadeOutStart = 2000;
	int fadeInStart = 2666;

	float fixedWidth = 1920.0;
};
