#include "Swarm.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'


moca::moca(int i,int allnum,string s_ip){
    i_MocaId = i;
    //int i_Ip4s[10]=IP4;
    vector<string> vs_buf = ofSplitString(s_ip, ".");
    i_Ip4 = stoi(vs_buf[3]);
    gui.setup();
    /*parameters.setName("Moca"+ofToString(i));
    parameters.add(pai_State.set("State",0));
    parameters.add(pai_PosX.set("Pos X",0));
    parameters.add(pai_PosY.set("Pos Y",0));
    parameters.add(pai_PosZ.set("Pos Z",0));*/
    gui.setName("Moca"+ofToString(i));
    /*gui.add(pai_State.set("State",0));
    gui.add(pai_PosX.set("Pos X",0));
    gui.add(pai_PosY.set("Pos Y",0));
    gui.add(pai_PosZ.set("Pos Z",0));*/
    ofVec3f hsv = ofVec3f(1.0*i/allnum,1,1);
    ofVec3f rgb = HSV2RGB(hsv)*255.0;
    ofColor col = ofColor((int)rgb[0],(int)rgb[1],(int)rgb[2]);
    colorID = col;
    //pa_Color.minimize();
    gui.add(pa_Color.setup("color", col, ofColor(0, 0), ofColor(255, 255)));
    //gui.add(ps_Ip.set(IP_SEGMENT+ofToString(i_Ip4)));
    gui.add(ps_Ip.set(s_ip));

    gui.add(pa_Pos.setup("center", ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5,0), ofVec3f(0, 0,0), ofVec3f(ofGetWidth(), ofGetHeight(),0)));
    gui.minimizeAll();
    gui.add(pa_Btn1_Status.setup("Get Status"));
    pa_Btn1_Status.addListener(this, &moca::Btn1Pressed);
    gui.add(pa_Btn2_RecStart.setup("Rec Start"));
    pa_Btn2_RecStart.addListener(this, &moca::Btn2Pressed);
    gui.add(pa_Btn3_Shoot.setup("Shoot"));
    pa_Btn3_Shoot.addListener(this, &moca::Btn3Pressed);

    gui.add(pa_Btn4_Start.setup("Moca Start"));
    pa_Btn4_Start.addListener(this, &moca::Btn4Pressed);
    gui.add(pa_Btn6_Kill.setup("Kill"));
    pa_Btn6_Kill.addListener(this, &moca::Btn6Pressed);
    
    
    if(0){
        gui.add(pa_Btn5_Reboot.setup("Reboot"));
        pa_Btn5_Reboot.addListener(this, &moca::Btn5Pressed);
        gui.add(pa_Btn7_SetPcAddr.setup("Set PC Addr"));
        pa_Btn7_SetPcAddr.addListener(this, &moca::Btn7Pressed);
        gui.add(pa_Btn8_DataSend.setup("Data Send"));
        pa_Btn8_DataSend.addListener(this, &moca::Btn8Pressed);
    }
    
    ofxButton pa_Btn4_Start;
    ofxButton pa_Btn5_Reboot;
    ofxButton pa_Btn6_Kill;

    
    
    gui.add(pa_Tog1_Manual.setup("Manual Mode",false));
    
    gui.add(pai_Pan.set("Pan Angle",180.0,0.0,360.0));
    gui.add(pai_Tilt.set("Tilt Angle",0.0,-14.0,25.0));
    f_Pan=pai_Pan;
    f_Tilt=pai_Tilt;
    
    gui.setPosition(ofGetScreenWidth()-(i%3+1)*gui.getWidth(),ofGetScreenHeight()-((int)(i/3.0)+1)*gui.getHeight());
    //parameters.add(renderer1.parameters);
    //parameters.add(renderer2.parameters);
    

}

void moca::init(int x,int y, int z){
    position.x = x;
    position.y = y;
    position.z = z;
    f_CameraPan=0;
    f_CameraTilt=0;
    daiH = 128.0;
    H1 = 5.0;
    H2 = 80.0;
    H3 = 317-daiH-H1-H2;
    positionCam=ofVec3f(0, daiH+H3+H2/2, 0);
    //positionCam=ofVec3f(0, daiH+H3+H2*2+H1, 0);
    positionCamLook=ofVec3f(1, 0, 0);
    camMocaCam.setPosition(positionCam);
    camMocaCam.lookAt(positionCam+positionCamLook);
    float f_KakudoH=36.9;
    float f_KakudoV=27;
    float f_ScreenL=10.0;
    float f_ScreenW=f_ScreenL*tan(f_KakudoH/2/90*(PI/2))*2;
    float f_ScreenH=f_ScreenL*tan(f_KakudoV/2/90*(PI/2))*2;
    //camMocaCam.setScale(16.0/4, 9.0/4, 10);
    camMocaCam.setScale(f_ScreenW, f_ScreenH, f_ScreenL);
    scale = ofVec3f(0.1,0.1,0.1);
}

void moca::guidraw(){
    ofSetColor(255);
    //ofScale(0.5, 0.5);
    gui.draw();
    //ofScale(2,2);
}

void moca::draw(bool flag){
    update();
    ofPushStyle();
    // Render light as white sphere
    //ofDrawSphere(light.getPosition(), 2.0);
    ofScale(scale);
    ofTranslate(position);
    /*************
       |H1
       |
       |H2(black)
       |
       |
       |H3
       |
     |
     |daiH
     |
    **************/
    ofSetColor(200, 200, 200);
    ofDrawCylinder(0, daiH/2,0, 119.0/2, daiH);
    ofDrawCylinder(0, daiH+H3/2,0, 95.0/2, H3);
    if(flag==false){
        ofSetColor(colorID);
        ofDrawCylinder(0, daiH+H3+H2+H1/2,0, 95.0/2, H1);
        ofSetColor(30, 30, 30);
        ofDrawCylinder(0, daiH+H3+H2/2,0, 95.0/2, H2);
    }
    //ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    //ofDrawBitmapString(" light", particles[0].position);
    ofSetColor(0,0,255);
    ofDrawBitmapString("test Moca", ofVec3f(0,daiH+(H3+H2+H1)*2,0));
    ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    ofDisableLighting();

    ofSetColor(colorID);
    ofMatrix4x4 inverseCameraMatrix;
    inverseCameraMatrix.makeInvertOf(camMocaCam.getModelViewProjectionMatrix());
    ofMultMatrix( inverseCameraMatrix );
    ofPoint scale2(0.9,0.9,0.9);
    ofScale(scale2);
    ofNoFill();
    // i.e. a box -1, -1, -1 to +1, +1, +1
    ofDrawBox(0, 0, 0, 2.0f);
    ofEnableLighting();
    
    ofPopMatrix();

    ofTranslate(-position);
    ofScale(1/scale);
    ofPopStyle();
}

void moca::update(){
    f_CameraPan=ofGetElapsedTimef()*0.5;
    f_CameraPan=f_Pan*PI/180;
    f_CameraTilt=f_Tilt*PI/180;
    positionCamLook=ofVec3f(cos(f_CameraPan),tan(f_CameraTilt),sin(f_CameraPan));
    camMocaCam.setPosition(positionCam);
    camMocaCam.lookAt(positionCam+positionCamLook);
    colorID = pa_Color;
    gui.setHeaderBackgroundColor(colorID);
    //gui.setPosition(ofGetScreenWidth()-(i_MocaId%3+1)*gui.getWidth(),ofGetScreenHeight()-((int)(i_MocaId/3.0)+1)*gui.getHeight());
    if(pa_Tog1_Manual!=b_Tog1_Manual)Tog1Pressed();
    if(pai_Pan!=f_Pan)PanChanged();
    if(pai_Tilt!=f_Tilt)TiltChanged();
    b_Tog1_Manual=pa_Tog1_Manual;
    f_Pan=pai_Pan;
    f_Tilt=pai_Tilt;
}

void moca::Btn1Pressed(){
    b_GetStatusSend = true;
    printf("[GUI]Btn1Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn2Pressed(){
    b_RecStartSend = true;
    printf("[GUI]Btn2Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn3Pressed(){
    b_ShootSend = true;
    printf("[GUI]Btn3Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn4Pressed(){
    b_StartSend = true;
    printf("[GUI]Btn4Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn5Pressed(){
    b_RebootSend = true;
    printf("[GUI]Btn5Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn6Pressed(){
    b_KillSend = true;
    printf("[GUI]Btn6Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn7Pressed(){
    b_SetPcSend = true;
    printf("[GUI]Btn7Pressed IP4:%d \n",i_Ip4);
}
void moca::Btn8Pressed(){
    b_DataSendSend = true;
    printf("[GUI]Btn8Pressed IP4:%d \n",i_Ip4);
}

void moca::PanChanged(){
    b_PanSend = true;
    printf("[GUI]Pan %d IP4:%d \n",pai_Pan.get(),i_Ip4);
}
void moca::TiltChanged(){
    b_TiltSend = true;
    printf("[GUI]Tilt %d IP4:%d \n",pai_Tilt.get(),i_Ip4);
}



void moca::Tog1Pressed(){
    b_SetManualSend = true;
    b_Manual = pa_Tog1_Manual;
    printf("[GUI]Tog1 Manual Mode %d Pressed IP4:%d \n",b_Manual,i_Ip4);
}


ofVec3f moca::getPositionCam(){
    //return ((position+positionCam+positionCamLook*300)*scale);
    return ((position+positionCam)*scale);
}
ofVec3f moca::getPositionLook(){
    //return ((position+positionCam+positionCamLook*310)*scale);
    return ((position+positionCam+positionCamLook)*scale);
}

swarm::swarm(){
	light.setAmbientColor(ofColor(0, 0, 0));
}

void swarm::init(int nParticles, float positionDispersion, float velocityDispersion){

	// Check if we've already initialised
	if(particles.size() != 0){
		// clear out old data
		ofLogWarning("swarm") <<  "Swarm: Already initialised";

		particles.clear();
	}

	ofSeedRandom();
	//
	ofVec3f position, velocity;
	ofColor color;
	for(int i = 0; i < nParticles; i++){
		position.x = (ofRandom(1.0f) - 0.5f)  * positionDispersion;
		position.y = (ofRandom(1.0f) - 0.5f)  * positionDispersion;
		position.z = (ofRandom(1.0f) - 0.5f)  * positionDispersion;

		velocity.x = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;
		velocity.y = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;
		velocity.z = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;

		color.r = ofRandom(255.0f);
		color.g = ofRandom(255.0f);
		color.b = 150.0f;
		color.a = 255.0f;

		particle newParticle;
		newParticle.position = position;
		newParticle.velocity = velocity;
		newParticle.color = color;

		// add our new particle to the vector
		particles.push_back(newParticle);
	}

}

void swarm::customDraw(){
	// We run the update ourselves manually. ofNode does
	//  not do this for us.
	update();



	//--
	// Draw particles

	// We use the position of the first
	//  particle as the position of the
	//  light.
	ofPushStyle();
	light.enable();
	light.setPosition(particles[0].position);

	for(unsigned int i = 0; i < particles.size(); i++){
		ofPushStyle();
		ofSetColor(particles[i].color);

		ofDrawSphere(particles[i].position, 1.0);

		ofPopStyle();
	}

	light.disable();
	ofDisableLighting();

	//
	//--



	// Render light as white sphere
	ofSetColor(255, 255, 255);
	ofDrawSphere(light.getPosition(), 2.0);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(" light", particles[0].position);
	ofPopStyle();
}

void swarm::update(){

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();

	// Update positions, velocities
	for(unsigned int i = 0; i < particles.size(); i++){

		// -----------
		//
		//	MOTION MATHS
		//
		//		'Simple Harmonic Motion' + a little extra
		// ----
		//

		// [1] apply velocity to postion
		//  (i.e. integrate velocity)
		//
		//  v = dx / dt (*)
		//  x = x + dx [every frame]
		//
		// therefore
		//  x = x + v * dt (*)
		//

		// (velcotity is taken from previous frame)
		particles[i].position += particles[i].velocity * dt;



		// [2] apply spring force to velocity
		//  (i.e. integrate acceleration)
		//
		//  a = -k * x (this is the shm restoring force, aka spring force)
		//  a = dv / dt
		//
		// therefore from (*)s above
		//  (v = v + dv)
		//
		//  v = v + (dt * a)
		//  v = v + (dt * -k * x)
		//
		particles[i].velocity += -SPRING_CONSTANT * particles[i].position * dt;


		// [3] to get a super simple kind of 'flocking' behaviour
		//  we add a second spring force to velocity relative
		//  to the position of the light
		// NOTICE: THIS ISN'T REAL FLOCKING!
		particles[i].velocity += -SPRING_CONSTANT * (particles[i].position - light.getPosition()) * dt;


		// [4] Force a maximum velocity
		if(particles[i].velocity.length() > MAX_VELOCITY){
			particles[i].velocity /= particles[i].velocity.length() * MAX_VELOCITY;
		}

		//
		// -----------

	}
}

ofVec3f moca::HSV2RGB(ofVec3f a){
    const float h = a[0];
    const float s = a[1];
    const float v = a[2];
    const float hueF = h * 6.0f;
    const int hueI = static_cast<int>(hueF);
    const float fr = hueF - hueI;
    const float m = v * (1.0f-s);
    const float n = v * (1.0f-s*fr);
    const float p = v * (1.0f-s*(1.0f-fr));
    ofVec3f o;
    switch(hueI)
    {
        case 0: o[0] = v; o[1] = p; o[2] = m; break;
        case 1: o[0] = n; o[1] = v; o[2] = m; break;
        case 2: o[0] = m; o[1] = v; o[2] = p; break;
        case 3: o[0] = m; o[1] = n; o[2] = v; break;
        case 4: o[0] = p; o[1] = m; o[2] = v; break;
        default: o[0] = v; o[1] = m; o[2] = n; break;
    }
    return o;
}

ofVec3f moca::RGB2HSV(ofVec3f a){
    const float min = std::min(std::min(a[0],a[1]),a[2]);
    const float max = std::max(std::max(a[0],a[1]),a[2]);
    
    ofVec3f hsv=ofVec3f(0.0f,0.0f,max);
    
    const float delta = max - min;
    
    if(delta!=0.0f)
    {
        hsv[1] = delta / max;
        
        if(a[0]==max)
        {
            hsv[0] = (a[1]-a[2]) / delta;
        }
        else if(a[1]==max)
        {
            hsv[0] = 2.0f + (a[2]-a[0]) / delta;
        }
        else
        {
            hsv[0] = 4.0f + (a[0]-a[1]) / delta;
        }
        
        hsv[0] /= 6.0f;
        
        if(hsv[0]<0.0f)
        {
            hsv[0] += 1.0f;
        }
    }
    
    return hsv;
    
}
