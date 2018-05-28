#include "ofApp.h"

//--------------------------------------------------------------
//
//
// ADVANCED 3D EXAMPLE
//		ofNode3d, ofCamera, ofEasyCam
//
//
//--------------------------------------------------------------

//
// SUGGESTED EXERCISES
//
// 0. Run and understand the example
//
// 1. Change number of particles in the swarm.
// 2. Change the dynamic properties of the swarm (speed, orbit radius)
// 3. Change the near and far clipping planes of camEasyCam
//
// 4. Add another camera to the existing 4.
//		Have all parts of the example working with all 5 cameras.
//
// 6. Create your own custom node class and add an instance of it
//		to the scene.
//
// 7. Understand how the 'frustrum preview' works
//



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	ofEnableDepthTest();
    font.load( OF_TTF_SANS,9,true,true);
    
    //system();//python OSC server起動
    system("python /Users/masarumizuochi/programming/python/Moca/MocaMaster.py &");
    
    //OSC
    
    sender.setup(HOST, PORT_OUT);
    receiver.setup(PORT_IN);
    
    
    //OSC send test
    //--
	// Setup cameras

	iMainCamera = 0;
	bCamParent = false;

	// user camera
	//camEasyCam.setTarget(nodeSwarm);
    //camEasyCam.setTarget(mocaObj);
	camEasyCam.setDistance(300);
	cameras[0] = &camEasyCam;


	// front
	camFront.scale = 10;
    cameras[1] = &camFront;

	// top
	camTop.scale = 20;
	//camTop.tilt(-90);
	cameras[2] = &camTop;

	// left
	camLeft.scale = 20;
	//camLeft.pan(-90);
	cameras[3] = &camLeft;

	//
	//--



	//--
	// Define viewports

	setupViewports();

	//
	//--

    ofSetSmoothLighting(true);
    light.setDiffuseColor( ofFloatColor(.85, .85, .85) );
    light.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    

    
    //XML Settings
    if( XML.loadFile("MocaSettings.xml") ){
    //if( XML.loadFile("mySettings.xml") ){
            printf("MocaSettings.xml loaded!\n");
    }else{
        printf("unable to load MocaSettings.xml check data/ folder\n");
    }
    
    //read the colors from XML
    //if the settings file doesn't exist we assigns default values (170, 190, 240)
    //red		= XML.getValue("BACKGROUND:COLOR:RED", 170);
    //green	= XML.getValue("BACKGROUND:COLOR:GREEN", 190);
    //blue	= XML.getValue("BACKGROUND:COLOR:BLUE", 240);
    s_IpPc = XML.getValue("IP_PC", "1.1.1.1");
    s_UserPc = XML.getValue("USER_PC", "pc_user");
    s_PassPc = XML.getValue("PASS_PC", "pc_pass");
    s_DataSavePath = XML.getValue("DATA_SAVE_PATH", "/Users/");
    cout <<"PC IP: " << s_IpPc << endl;
    cout <<"PC User: " << s_UserPc << endl;
    cout <<"PC Pass: " << s_PassPc << endl;
    cout <<"PC Data Path: " << s_DataSavePath << endl;
    
    XML.pushTag("Mocas");
    N_MOCA_CAMERAS = XML.getNumTags("Moca");
    //N_MOCA_CAMERAS = XML.getNumTags("STROKE:PT");
    printf("Setup %d Moca data\n",N_MOCA_CAMERAS);
	//--
	// Setup swarm

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	//nodeSwarm.init(100, 50, 10);
    vector<ofVec3f> vi_MocaPos;
    for(int i=0;i<N_MOCA_CAMERAS;i++){
        ofVec3f buf;
        buf[0]=XML.getValue("Moca:X", 0,i);
        buf[2]=XML.getValue("Moca:Y", 0,i);
        buf[1]=XML.getValue("Moca:Z", 0,i);
        vi_MocaPos.push_back(buf);
    }
    string s_IpSegment = XML.getValue("IP_SEGMENT", "192.168.0.");
    /*int i_MocaPos[N_MOCA_CAMERAS][3]={
        {400,0,400},
        {400,0,-400},
        {600,0,00},
        {-600,0,0},
        {-400,0,400},
        {-400,0,-400}};*/
    for(int i=0;i<N_MOCA_CAMERAS;i++){
        moca *p_moca = new moca(i,N_MOCA_CAMERAS,s_IpSegment+ofToString(XML.getValue("Moca:IP4", 0,i)));
        p_moca->init(vi_MocaPos[i][0],vi_MocaPos[i][1],vi_MocaPos[i][2]);
        vp_Moca.push_back(p_moca);
    }
	//
	//--
}

//--------------------------------------------------------------
void ofApp::setupViewports(){
	//call here whenever we resize the window


	//--
	// Define viewports

	float xOffset = ofGetWidth() / 3;
	float yOffset = ofGetHeight() / N_CAMERAS;

	viewMain.x = xOffset;
	viewMain.y = 0;
	viewMain.width = xOffset * 2;
	viewMain.height = ofGetHeight();

	for(int i = 0; i < N_CAMERAS; i++){

		viewGrid[i].x = 0;
		viewGrid[i].y = yOffset * i;
		viewGrid[i].width = xOffset;
		viewGrid[i].height = yOffset;
	}

	//
	//--
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    /*ofxOscMessage m;
    m.setAddress("/moca/init");
    m.addStringArg("test");
    sender.sendMessage(m);*/

    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        if(m.getAddress()=="/moca/init"){
            string buf = m.getArgAsString(0);
            cout << "[OF]Receive Message:" << buf << endl;
        }
    }
    
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_GetStatusSend){
            vp_Moca[i]->b_GetStatusSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/status");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_SetManualSend){
            vp_Moca[i]->b_SetManualSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/manual");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            m.addIntArg(vp_Moca[i]->b_Manual);
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_RecStartSend){
            vp_Moca[i]->b_RecStartSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/rec");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            m.addIntArg(1);
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_ShootSend){
            vp_Moca[i]->b_ShootSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/shoot");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            m.addIntArg(0);
            sender.sendMessage(m);
        }
    }

    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_PanSend){
            vp_Moca[i]->b_PanSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/pan");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            int buf = vp_Moca[i]->pai_Pan;
            string test;
            test =ofToString(buf);
            m.addStringArg(test);
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_TiltSend){
            vp_Moca[i]->b_TiltSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/tilt");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            int buf = vp_Moca[i]->pai_Tilt;
            string test;
            test =ofToString(buf);
            m.addStringArg(test);
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_StartSend){
            vp_Moca[i]->b_StartSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/start");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_RebootSend){
            vp_Moca[i]->b_RebootSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/reboot");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_KillSend){
            vp_Moca[i]->b_KillSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/kill");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_SetPcSend){
            vp_Moca[i]->b_SetPcSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/setpc");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            m.addStringArg(s_IpPc);
            m.addStringArg(s_UserPc);
            m.addStringArg(s_PassPc);
            m.addStringArg(s_DataSavePath);
            sender.sendMessage(m);
        }
    }
    for(int i=0;i<vp_Moca.size();i++){
        if(vp_Moca[i]->b_DataSendSend){
            vp_Moca[i]->b_DataSendSend = false;
            ofxOscMessage m;
            m.setAddress("/moca/datasend");
            //m.addStringArg("192.168.10.2");
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
            sender.sendMessage(m);
        }
    }
    //if((int)(ofGetElapsedTimef())%2==0){
    //}
    
    
    light.setPosition(cos(ofGetElapsedTimef()*.5)*ofGetWidth(), sin(ofGetElapsedTimef()*.5)*ofGetHeight(), 500);
    //MocaCameraPos
    camFront.setPosition(vp_Moca[0]->getPositionCam());
    camFront.lookAt(vp_Moca[0]->getPositionLook());
    camTop.setPosition(vp_Moca[1]->getPositionCam());
    camTop.lookAt(vp_Moca[1]->getPositionLook());
    camLeft.setPosition(vp_Moca[2]->getPositionCam());
    camLeft.lookAt(vp_Moca[2]->getPositionLook());

    //camEasyCam.setPosition(100,200, 0);
    
}


//--------------------------------------------------------------
void ofApp::draw(){
	//--
	// Highlight background of selected camera

	ofDisableDepthTest();
	ofPushStyle();
	ofSetColor(100, 100, 100);
	ofDrawRectangle(viewGrid[iMainCamera]);
	ofPopStyle();
	ofEnableDepthTest();

	//
	//--



	//--
	// Draw all viewports

	// draw main viewport
	cameras[iMainCamera]->begin(viewMain);
	drawScene(iMainCamera);
    
    //ofDrawBitmapString("test Mocavaaaaa",viewMain.x + 20,30);
    //ofDrawBitmapString("test Mocavaaaaafdafafafffa",ofVec3f(0,100,100));
    //font.drawString("frame: ",20,20);二次元にしか書けない

	// calculate mouse ray whilst this camera is active
	updateMouseRay();

	cameras[iMainCamera]->end();

	// draw side viewports
	for(int i = 0; i < N_CAMERAS; i++){

		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
        ofDrawBitmapString("test Mocavaaaaa",viewMain.x + 20,30);
        font.drawString("frame: ",20,20);
		cameras[i]->end();
	}

	//
	//--



	//--
	// Draw annotations (text, gui, etc)

	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything

	// draw some labels
    ofSetColor(255,0,0);
    //ofDrawBitmapString("test Mocavaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaf¥nfafadfffdafadfadfadfadfadfaf¥nfadfffffffffffffffffffffffffffffffffffffffffffdafa¥nraraadsfdsafasdf",viewMain.x + 20,30);
    //ofDrawBitmapString("test Mocavaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaf¥nfafadfffdafadfadfadfadfadfaf¥nfadfffffffffffffffffffffffffffffffffffffffffffdafa¥nraraadsfdsafasdf", ofVec3f(30,30,0));
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
	ofDrawBitmapString("Camera selected: " + ofToString(iMainCamera + 1), viewMain.x + 20, 50);
	ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
	ofDrawBitmapString("Press 'p' to toggle parents on OrthoCamera's", viewMain.x + 20, 90);

	ofDrawBitmapString("EasyCam",   viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("Front",     viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Top",       viewGrid[2].x + 20, viewGrid[2].y + 30);
	ofDrawBitmapString("Left",      viewGrid[3].x + 20, viewGrid[3].y + 30);

    

    
	// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for(int i = 0; i < N_CAMERAS; i++){
		ofDrawRectangle(viewGrid[i]);
	}
	//
	ofDrawRectangle(viewMain);
    for(int i=0;i<vp_Moca.size();i++){
        vp_Moca[i]->guidraw();
    }

    
	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();

	//
    
	//--
}

void ofApp::drawScene(int iCameraDraw){

	//nodeSwarm.draw();
    ofEnableLighting();
    light.enable();

    nodeGrid.draw();

    for(int i=0;i<vp_Moca.size();i++){
        bool CameraThrough = false;
        if(iCameraDraw==1 or iCameraDraw==2 or iCameraDraw==3 ){
            if(i==(iCameraDraw-1))CameraThrough=true;
        }
        vp_Moca[i]->draw(CameraThrough);
    }
    ofDisableLighting();

/*    ofSetColor(0, 0, 255);
    ofDrawCylinder(0, 0,0, 119.0/2, 100);
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("test Mocavaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaf¥nfafadfffdafadfadfadfadfadfaf¥nfadfffffffffffffffffffffffffffffffffffffffffffdafa¥nraraadsfdsafasdf",0,30,0);*/

	//--
	// Draw frustum preview for ofEasyCam camera

	// This code draws our camera in
	//	the scene (reddy/pink lines)
	//
	// The pyramid-like shape defined
	//	by the cameras view is called
	//	a 'frustum'.
	//
	// Often we refer to the volume
	//	which can be seen by the
	//	camera as 'the view frustum'.


	// First check if we're already drawing the view through the easycam
	// If so, don't draw the frustum preview.
	if(iCameraDraw != 0){

		ofPushStyle();
		ofPushMatrix();

		//--
		// Create transform for box->frustum

		// In 'camera space' the bounds of
		//  the view are defined by a box
		//  with bounds -1->1 in each axis
		//
		// To convert from camera to world
		//  space, we multiply by the inverse
		//  camera matrix of the camera, i.e
		//  inverse of the ViewProjection
		//  matrix.
		//
		// By applying this transformation
		//  our box in camera space is
		//  transformed into a frustum in
		//  world space.
		//

		// The camera's matricies are dependant on
		//  the aspect ratio of the viewport.
		//  (Which is why we use the viewport as
		//  an argument when we begin the camera.
		//
		// If this camera is fullscreen we'll use
		//   viewMain, else we'll use viewGrid[0]
		ofRectangle boundsToUse;
		if(iMainCamera == 0){
			boundsToUse = viewMain;
		}
		else{
			boundsToUse = viewGrid[0];
		}

		// Now lets get the inverse ViewProjection
		//  for the camera
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf(camEasyCam.getModelViewProjectionMatrix(boundsToUse));

		// By default, we can say
		//	'we are drawing in world space'
		//
		// The camera matrix performs
		//	world->camera
		//
		// The inverse camera matrix performs
		//	camera->world
		//
		// Our box is in camera space, if we
		//	want to draw that into world space
		//	we have to apply the camera->world
		//	transformation.
		//

		// This syntax is a little messy.
		// What it's saying is, send the data
		//  from the inverseCameraMatrix object
		//  to OpenGL, and apply that matrix to
		//  the current OpenGL transform
		ofMultMatrix( inverseCameraMatrix );

		//
		//--



		//--
		// Draw box in camera space
		// (i.e. frustum in world space)

		ofNoFill();
		// i.e. a box -1, -1, -1 to +1, +1, +1
		ofDrawBox(0, 0, 0, 2.0f);
		//
		//--

		ofPopStyle();
		ofPopMatrix();
	}

	//
	//--



	//--
	// Draw mouse ray

	// Draw the ray if ofEasyCam is in main view,
	//  and we're not currently drawing in that view
	if(iMainCamera == 0 && iCameraDraw != 0){
		ofPushStyle();
		ofSetColor(100, 100, 255);
		ofDrawLine(ray[0], ray[1]);
		ofPopStyle();
	}

	//
	//--
}

//--------------------------------------------------------------
void ofApp::updateMouseRay(){
	// Define ray in screen space
	ray[0] = ofVec3f(ofGetMouseX(), ofGetMouseY(), -1);
	ray[1] = ofVec3f(ofGetMouseX(), ofGetMouseY(), 1);

	// Transform ray into world space
	ray[0] = cameras[iMainCamera]->screenToWorld(ray[0], viewMain);
	ray[1] = cameras[iMainCamera]->screenToWorld(ray[1], viewMain);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(key >= '1' && key <= '4'){
		iMainCamera = key - '1';
	}

	if(key == 'f'){
		ofToggleFullscreen();
	}
    if(key == 't'){
        cout << "[OF] test osc send" << endl;
        ofxOscMessage m;
        m.setAddress("/moca/init");
        m.addStringArg("test");
        sender.sendMessage(m);
    }
    if(key == 'q'){
        cout << "[OF] quit osc send" << endl;
        ofxOscMessage m;
        m.setAddress("/moca/quit");
        m.addStringArg("test");
        sender.sendMessage(m);
        sleep(1);
        OF_EXIT_APP(0);
    }

	if(key == 'p'){
		if(bCamParent){
			camFront.clearParent();
			camTop.clearParent();
			camLeft.clearParent();

			bCamParent = false;
		}
		else{
			//camFront.setParent(nodeSwarm.light);
			//camTop.setParent(nodeSwarm.light);
			//camLeft.setParent(nodeSwarm.light);

			bCamParent = true;
		}
	}
    
    if(key == 's'){
        ofxOscMessage m;
        m.setAddress("/moca/ping");
        m.addIntArg(vp_Moca.size());
        for(int i=0;i<vp_Moca.size();i++){
            m.addStringArg(ofToString(vp_Moca[i]->ps_Ip));
        }
        sender.sendMessage(m);
    }
    if(key== '-'){
        iControlCamId = (iControlCamId+N_MOCA_CAMERAS-1)%N_MOCA_CAMERAS;
    }
    if(key== '='){
        iControlCamId = (iControlCamId+1)%N_MOCA_CAMERAS;
    }
    if(key== OF_KEY_UP){
        (vp_Moca[iControlCamId]->pai_Tilt)+=2;
    }
    if(key== OF_KEY_DOWN){
        (vp_Moca[iControlCamId]->pai_Tilt)-=2;
    }
    if(key== OF_KEY_RIGHT){
        (vp_Moca[iControlCamId]->pai_Pan)+=5;
    }
    if(key== OF_KEY_LEFT){
        (vp_Moca[iControlCamId]->pai_Pan)-=5;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	setupViewports();
}

ofVec3f HSV2RGB(ofVec3f a){
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

ofVec3f RGB2HSV(ofVec3f a){
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


