#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinect.h"
#include "ofxCamaraLucidaCalibration.h"
#include "ofxGui.h"

#include "data_path.h"

class ofApp : public ofBaseApp
{

	public:
		void setup();
		void update();
		void draw();
    void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    cml::ProjectorCameraCalibration calibration;

    ofPixels pix_kinect_rgb;
    ofxKinect kinect;
    int w, h, chan;

    ofxFloatSlider chessboard_brightness;
    ofxToggle chessboard_projected;
    ofxPanel gui;
};
