#pragma once

//--------------------------------------------------------------
//
//
// ADVANCED 3D EXAMPLE
//		ofNode3d, ofCamera, ofEasyCam
//
//
//--------------------------------------------------------------


#include "ofMain.h"
#include "ofxXmlSettings.h"

// Custom objects for this example
#include "Swarm.h"
#include "Grid.h"
#include "OrthoCamera.h"
//#include "HttpForm.h"
//#include "HttpFormManager.h"

#include "ofxGui.h"
#include "ofxOsc.h"

#define HOST "127.0.0.1"
#define PORT_OUT 7110
#define PORT_IN 7120

#define N_CAMERAS 4

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void setupViewports();
		void drawScene(int iCameraDraw);
		void updateMouseRay();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);

		//cameras (all these inherit from ofCamera)
		ofEasyCam camEasyCam;
		orthoCamera camFront;
		orthoCamera camTop;
		orthoCamera camLeft;

		//cameras have parent?
		bool bCamParent;

		//camera pointers
		ofCamera * cameras[N_CAMERAS];
		int iMainCamera;

		//viewports
		ofRectangle viewMain;
		ofRectangle viewGrid[N_CAMERAS];

		//my custom node
		//swarm nodeSwarm;
        vector<moca*> vp_Moca;
        //moca mocaObj;
		grid nodeGrid;
        ofLight light;

		//ray drawn under mouse cursor [start,end]
		ofVec3f ray[2];
    
        ofxOscSender sender;
        ofxOscReceiver receiver;
    
    
        ofTrueTypeFont font;
    ofxXmlSettings XML;
    
    string s_IpPc;
    string s_UserPc;
    string s_PassPc;
    string s_DataSavePath;
    
    int iControlCamId;
    int N_MOCA_CAMERAS;
};


ofVec3f HSV2RGB(ofVec3f a);
ofVec3f RGB2HSV(ofVec3f a);

