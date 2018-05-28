#pragma once
#include "ofMain.h"
#include "ofxGui.h"

#define SPRING_CONSTANT 0.1f
#define MAX_VELOCITY 30.0f

#define IP_SEGMENT "192.168.10."
#define IP4 {2,3,4,5,6,7}


// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'
class swarm : public ofNode {
	struct particle {
		ofVec3f position;
		ofVec3f velocity;
		ofColor color;
	};

	public:
		swarm();
		void init(int nParticles, float positionDispersion, float velocityDispersion);
		void customDraw();
		ofLight light;

	protected:
		// we call this update function ourselves
		//  at the beginning of customDraw
		void update();

		// objects
		vector<particle>particles;
};


class moca{
    int i_MocaId;
    int i_Ip4;//IPアドレスの４つ目
    ofVec3f position;
    float f_CameraPan;
    float f_CameraTilt;
    ofEasyCam camMocaCam;
    int daiH,H1,H2,H3;
    ofVec3f positionCam;
    ofVec3f positionCamLook;
    ofPoint scale;
    ofColor colorID;
    ofxPanel gui;
    ofParameter<int> pai_State;
    ofParameter<int> pai_PosX;
    ofParameter<int> pai_PosY;
    ofParameter<int> pai_PosZ;
    
    ofxVec3Slider pa_Pos;
    ofxColorSlider pa_Color;
    
    ofxButton pa_Btn1_Status;
    ofxButton pa_Btn2_RecStart;
    ofxButton pa_Btn3_Shoot;
    ofxButton pa_Btn4_Start;
    ofxButton pa_Btn5_Reboot;
    ofxButton pa_Btn6_Kill;
    ofxButton pa_Btn7_SetPcAddr;
    ofxButton pa_Btn8_DataSend;
    ofxToggle pa_Tog1_Manual;
    bool b_Tog1_Manual;
    float f_Pan;
    float f_Tilt;
    
    //ofParameterGroup parameters;

    void Btn1Pressed();
    void Btn2Pressed();
    void Btn3Pressed();
    void Btn4Pressed();
    void Btn5Pressed();
    void Btn6Pressed();
    void Btn7Pressed();
    void Btn8Pressed();
    void Tog1Pressed();
    void PanChanged();
    void TiltChanged();
    
public:
    moca(int i,int allnum,string s_ip);
    void init(int x,int y,int z);
    void draw(bool flag);//flag 頭だけ表示しないフラグ
    void guidraw();
    
    ofVec3f getPositionCam();
    ofVec3f getPositionLook();
    ofVec3f HSV2RGB(ofVec3f a);
    ofVec3f RGB2HSV(ofVec3f a);

    ofParameter<int> pai_Pan;
    ofParameter<int> pai_Tilt;
    
    bool b_GetStatusSend;
    bool b_SetManualSend;
    bool b_RecStartSend;
    bool b_ShootSend;
    bool b_PanSend;
    bool b_TiltSend;
    bool b_Manual;
    bool b_KillSend;
    bool b_RebootSend;
    bool b_StartSend;
    bool b_SetPcSend;
    bool b_DataSendSend;
    ofParameter<string> ps_Ip;

    
protected:
    // we call this update function ourselves
    //  at the beginning of customDraw
    void update();
    
};