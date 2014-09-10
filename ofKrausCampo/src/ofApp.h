#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 44000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		ofVideoGrabber 			vidGrabber;
		ofxCvColorImage 		colorImg;
		ofxCvGrayscaleImage 	grayImg;
		ofxCvContourFinder 		contourFinder;
		ofImage					pix;

		int threshold;
		int blobSize;
		int numBlobs;
		bool bLearnBackground;
	
		int videoW;
		int videoH;
		int margin;
	
		int x_pos;
		int y_pos;
		
		ofxOscSender			sender;

};
