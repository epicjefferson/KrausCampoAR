#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	
	sender.setup(HOST, PORT);
	
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	kinect.setCameraTiltAngle(0);
	
	videoW = 400;
	videoH = 300;
	margin = 30;
	
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImg.allocate(kinect.width, kinect.height);
	depthImg.allocate(kinect.width, kinect.height);
	brightImg.allocate(kinect.width, kinect.height);
	
	nearThreshold = 215;
	farThreshold = 195;
	brightThreshold = 135;
	
	blobSize = 1000;
	numBlobs = 1;
	
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(0, 0, 0);
	
	kinect.update();
	
	colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
	grayImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
	depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	brightImg = colorImg;

	
	unsigned char * depths = depthImg.getPixels();
	unsigned char * brights = brightImg.getPixels();
	unsigned char * grays = grayImg.getPixels();

	int numPixels = grayImg.getWidth() * grayImg.getHeight();
	for (int i = 0; i < numPixels; i++) {
		if (brights[i] > brightThreshold && depths[i] < nearThreshold && depths[i] > farThreshold) {
			grays[i] = 255;
		} else {
			grays[i] = 0;
		}
	}
	
	grayImg.flagImageChanged();
	
	contourFinder.findContours(grayImg, 20, blobSize, numBlobs, false);
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetColor((255, 255, 255));
	
	//	initialize OSC
	ofxOscMessage m;
	
	//	send OSC message for number of blobs
	m.setAddress("/blobs/count");
	m.addIntArg(numBlobs);
	sender.sendMessage(m);
	m.clear();
	
	//	copy colorImg (which is of type ofOpenCv) to pix (which is type ofImage)
	pix.setFromPixels(colorImg.getPixels(), kinect.width, kinect.height, OF_IMAGE_COLOR);
	
	//	draw color images onto screen
	pix.draw(margin, margin, videoW, videoH);
	
	//	draw thresholded image
	grayImg.draw(2 * margin + videoW, margin, videoW, videoH);
	//	draw blobs
	contourFinder.draw(2 * margin + videoW, margin, videoW, videoH);

	//	for each blob
	for (int i = 0; i < contourFinder.nBlobs; i++) {

		//	find their center point
		x_pos = contourFinder.blobs[i].centroid.x;
		y_pos = contourFinder.blobs[i].centroid.y;

		//	send OSC messages for center point
		m.setAddress("/blobs/" + ofToString(i) + "/pos/x");
		m.addIntArg(x_pos);
		sender.sendMessage(m);
		m.clear();
		m.setAddress("/blobs/" + ofToString(i) + "/pos/y");
		m.addIntArg(y_pos);
		sender.sendMessage(m);
		m.clear();
		
		pixColor = pix.getColor(x_pos, y_pos);
		hue = pixColor.getHue();

		//	send OSC messages for color values
		m.setAddress("/blobs/" + ofToString(i) + "/color/hue");
		m.addIntArg(hue);
		sender.sendMessage(m);
		m.clear();
		m.setAddress("/blobs/" + ofToString(i) + "/color/r");
		m.addIntArg(pixColor[0]);
		sender.sendMessage(m);
		m.clear();
		m.setAddress("/blobs/" + ofToString(i) + "/color/g");
		m.addIntArg(pixColor[1]);
		sender.sendMessage(m);
		m.clear();
		m.setAddress("/blobs/" + ofToString(i) + "/color/b");
		m.addIntArg(pixColor[2]);
		sender.sendMessage(m);
		m.clear();


		//	display position of blob center
		string pos_str = "x: " + ofToString(x_pos) + " y: " + ofToString(y_pos);
		ofDrawBitmapString(pos_str, 2 * margin + videoW, 2 * margin + videoH + margin * i);

		//	display pixel color of blob center
		ofSetColor(ofColor::fromHsb(hue, 255, 255));
		ofDrawBitmapString(ofToString(hue), margin, 2 * margin + videoH + margin * i);

	}

	//	draw all information to screen as text
	ofSetColor((100, 100, 100));
	
	string far_thresh_str = "Far Threshold: " + ofToString(farThreshold) + ". (press: +/-)";
	ofDrawBitmapString(far_thresh_str, 2 * margin + videoW, (3 + numBlobs) * margin + videoH);
	
	string near_thresh_str = "Near Threshold: " + ofToString(nearThreshold) + ". (press: 9/0)";
	ofDrawBitmapString(near_thresh_str, 2 * margin + videoW, (4 + numBlobs) * margin + videoH);
	
	string bright_thresh_str = "Bright Threshold: " + ofToString(brightThreshold) + ". (press: [/])";
	ofDrawBitmapString(bright_thresh_str, 2 * margin + videoW, (5 + numBlobs) * margin + videoH);

	string size_str = "Blob Size: " + ofToString(blobSize) + ". (press: ;/')";
	ofDrawBitmapString(size_str, 2 * margin + videoW, (6 + numBlobs) * margin + videoH);

	string num_str = "# of Blobs: " + ofToString(numBlobs) + ". (press: ,/.)";
	ofDrawBitmapString(num_str, 2 * margin + videoW, (7 + numBlobs) * margin + videoH);

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	switch (key) {
		case '=':
			farThreshold++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '-':
			farThreshold--;
			if (farThreshold < 0) farThreshold = 0;
			break;
		case '0':
			nearThreshold++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '9':
			nearThreshold--;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case ']':
			brightThreshold++;
			if (brightThreshold > 255) brightThreshold = 255;
			break;
		case '[':
			brightThreshold--;
			if (brightThreshold < 0) brightThreshold = 0;
			break;
		case '\'':
			blobSize += 1000;
			break;
		case ';':
			blobSize -= 1000;
			break;
		case '.':
			numBlobs++;
			break;
		case ',':
			numBlobs--;
			if (numBlobs < 0) numBlobs = 0;
			break;
			
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0);
	kinect.close();
	
}
