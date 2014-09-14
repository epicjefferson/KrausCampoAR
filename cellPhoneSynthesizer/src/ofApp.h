#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxKinect.h"

#define HOST "localhost"
#define PORT 44000

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		
		ofxCvColorImage 		colorImg;
		ofxCvGrayscaleImage 	grayImg;
		ofxCvGrayscaleImage 	depthImg;
		ofxCvGrayscaleImage 	brightImg;
		ofxCvContourFinder 		contourFinder;
		ofImage					pix;
		ofColor					pixColor;
		ofxKinect				kinect;
		
		int		nearThreshold;
		int		farThreshold;
		int		brightThreshold;
	
		int		blobSize;
		int		numBlobs;
		
		int		videoW;
		int		videoH;
		int		margin;
		
		int		x_pos;
		int		y_pos;
		
		int		hue;
		
		
		ofxOscSender	sender;
	
};
