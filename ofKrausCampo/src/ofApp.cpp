#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	
	sender.setup(HOST, PORT);

	videoW = 531;
	videoH = 330;
	margin = 30;
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(videoW, videoH);

	colorImg.allocate(videoW, videoH);
	grayImg.allocate(videoW, videoH);

	bLearnBackground = true;
	threshold = 200;
	blobSize = (videoW * videoH) / 8;
	numBlobs = 1;

}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(0, 0, 0);

	bool bNewFrame = false;

	vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();

	if (bNewFrame == true) {
		colorImg.setFromPixels(vidGrabber.getPixels(), videoW, videoH);
	}

	grayImg = colorImg;

	grayImg.threshold(threshold);

	contourFinder.findContours(grayImg, 20, blobSize, numBlobs, false);

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor((255, 255, 255));
	
	//	draw color and gray images onto screen
	colorImg.draw(margin, margin);
	grayImg.draw(2 * margin + videoW, margin);


	//	copy colorImg (which is of type ofOpenCv) to pix (which is type ofImage)
	pix.setFromPixels(colorImg.getPixels(), videoW, videoH, OF_IMAGE_COLOR);
	
	//	initialize OSC
	ofxOscMessage m;

	//	send OSC message for number of blobs
	m.setAddress("/blobs/count");
	m.addIntArg(numBlobs);
	sender.sendMessage(m);
	m.clear();

	//	for each blob
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		
		//	draw blobs
		contourFinder.blobs[i].draw(2 * margin + videoW, margin);

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
		
		//	get pixel color from color image
		ofColor pixColor = pix.getColor(x_pos, y_pos);
		
		//	send OSC messages for color values
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
		ofSetColor(pixColor);
		ofDrawBitmapString(ofToString(pixColor), margin, 2 * margin + videoH + margin * i);

	}

	//	draw all information to screen as text
	ofSetColor((100, 100, 100));
	string thresh_str = "Threshold: " + ofToString(threshold) + ". (press: +/-)";
	ofDrawBitmapString(thresh_str, 2 * margin + videoW, (3 + numBlobs) * margin + videoH);
	
	string size_str = "Blob Size: " + ofToString(blobSize) + ". (press: [/])";
	ofDrawBitmapString(size_str, 2 * margin + videoW, (4 + numBlobs) * margin + videoH);
	
	string num_str = "# of Blobs: " + ofToString(numBlobs) + ". (press: ;/')";
	ofDrawBitmapString(num_str, 2 * margin + videoW, (5 + numBlobs) * margin + videoH);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
		case '=':
			threshold++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold--;
			if (threshold < 0) threshold = 0;
			break;
		case ']':
			blobSize += 1000;
			break;
		case '[':
			blobSize -= 1000;
			break;
		case '\'':
			numBlobs++;
			break;
		case ';':
			numBlobs--;
			if (numBlobs < 0) numBlobs = 0;
			break;

	}
}
