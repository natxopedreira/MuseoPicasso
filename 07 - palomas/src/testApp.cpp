#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    
    
    width = 640;
	height = 480;
    
	video.initGrabber(width, height);
	//video.setDesiredFrameRate(30);

	videoColorCvImage.allocate(width, height);
	videoGrayscaleCvImage.allocate(width, height);
	videoPrevFrameImage.allocate(width, height);
	videoDiffImage.allocate(width, height);
    
    area.bEditMode = false;
    area.objName = "track";
    area.loadSettings();
    
    pidginLoop.loadMovie("paloma_loop.mov");
    pidginLoop.play();
    
    pidginFly.loadMovie("paloma_vuela.mov");
    
    photoViewer.loadMovie("03.mov");
    
    
    
    threshold = 80;     // threshold de la diferencia
    nToFly = 3.0;       // valor para disparar los p‡jaros
    
    bDebug = false;
    bToFly = false;
    
    
    ////------ GUI
    mapInMin = 0;
    mapInMax = 5000.0;
    mapOutMin = 0.3;
    mapOutMax = 3.0;
    
    horaInicio = 12.0;
    horaFin = 18.0;
    
    areaSupIzda = area.getTopLeft();
    areaInfDcha = area.getBottomRight();
    
    diffMov.allocate(width, height, OF_IMAGE_COLOR);
    videoGui.allocate(width, height, OF_IMAGE_COLOR);
    videoAreaGui.allocate(width, height, GL_RGB);
    
    aplicacionEnCurso = APP_STATE_HUNTER;
    
    initGui();

    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    switch (aplicacionEnCurso) {
        case APP_STATE_HUNTER:
            updateStateHunter();
            break;
            
        case APP_STATE_PHOVIEWER:
            photoViewer.update();
            break;
        default:
            break;
    }
    


    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    
    
    
    
    
    
    
    
    
    
    /// --  comprobamos que aplicacion queremos segun la hora
    ////

    

    if(ofGetHours() > (int)horaInicio && ofGetHours() < (int)horaFin && aplicacionEnCurso == APP_STATE_HUNTER)
    {
        //// es la hora del pasafotos
        
        //-- comprueba si hay algo en play;
        if(pidginLoop.isPlaying()) pidginLoop.stop();
        if(pidginFly.isPlaying()) pidginFly.stop();
        
        if(!photoViewer.isPlaying()) photoViewer.play();
        
        aplicacionEnCurso = APP_STATE_PHOVIEWER;
        
        cout << "APP_STATE_PHOVIEWER" << endl;
        
        
    }else if((ofGetHours() < (int)horaInicio || ofGetHours() > (int)horaFin) && aplicacionEnCurso == APP_STATE_PHOVIEWER){
        
        //// es la hora del dovehunter
        aplicacionEnCurso = APP_STATE_HUNTER;
        if(photoViewer.isPlaying()) photoViewer.stop();
        if(!pidginLoop.isPlaying()) pidginLoop.play();
        
        cout << "APP_STATE_HUNTER" << endl;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    
    switch (aplicacionEnCurso) {
        case APP_STATE_HUNTER:
            
           
            
            ofSetColor(255);
            if (pidginFly.isPlaying()){
                pidginFly.draw(0,0);
            } else {
                pidginLoop.draw(0,0);
            }

            
            
            
            break;
            
        case APP_STATE_PHOVIEWER:
            photoViewer.draw(0, 0);
            break;
        default:
            break;
    }

}


//--------------------------------------------------------------
void testApp::updateStateHunter(){
    ///--- update para cuando esta el cazapalomas
    video.update();
	
	if (video.isFrameNew()){
		videoColorCvImage.setFromPixels(video.getPixels(), width, height);
		videoGrayscaleCvImage = videoColorCvImage;
		
		videoDiffImage.absDiff(videoGrayscaleCvImage, videoPrevFrameImage);
		videoDiffImage.threshold(threshold);
		
		videoPrevFrameImage = videoGrayscaleCvImage;
        
		nChange = ofMap(videoDiffImage.countNonZeroInRegion(area.x, area.y, area.width, area.height), mapInMin,mapInMax, mapOutMin, mapOutMax);
        
        if (!pidginFly.isPlaying() && nChange >= nToFly){
            pidginFly.play();
            //cout << "vuelaaaaa" << endl;
        }
        
        if(bDebug){
            /// si estamos en debug actualizamos la imagen del gui
            
            diffMov.setFromPixels(videoDiffImage.getPixelsRef());
            
            ofPixels pxRef;
            videoAreaGui.readToPixels(pxRef);
            
            videoGui.setFromPixels(pxRef);
            videoGui.update();
            
        }
	}
    
    ofPushStyle();
    videoAreaGui.begin();
    videoColorCvImage.draw(0, 0);
    ofSetColor(255, 10, 10,100);
    ofRect(areaSupIzda.x, areaSupIzda.y, areaInfDcha.x, areaInfDcha.y);
    videoAreaGui.end();
    ofPopStyle();
    
    
    pidginLoop.update();
    pidginFly.update();
    
    if (pidginFly.getPosition() > 0.9){
        pidginFly.setPosition(0.0);
        pidginFly.stop();
    }

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
        area.bEditMode = bDebug;
        gui->setVisible(bDebug);
       
    } else if ( key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::initGui(){

    gui = new ofxUISuperCanvas("PIKATXU'S DOVE DETECTOR", 100, 100, 340, 900 ,OFX_UI_FONT_MEDIUM);
    gui->setColorBack(ofColor(90,90,90));
    
    gui->addSpacer();
    gui->addRangeSlider("rango horas pasafotos", 0, 24, &horaInicio, &horaFin);
    gui->addSpacer();
    
    gui->addSlider("threshold", .0, 255.0, &threshold);
    gui->addSlider("valor para volar", .0, 10.0, &nToFly);
    
    gui->addSpacer();
    gui->addSlider("mapInMin", .0, 1000.0, &mapInMin);
    gui->addSlider("mapInMax", .0, 10000.0, &mapInMax);
    gui->addSlider("mapOutMin", .0, 10.0, &mapOutMin);
    gui->addSlider("mapOutMax", .0, 10.0, &mapOutMax);
    
    gui->addSpacer();
    gui->addImage("movimiento", &diffMov,160,120);
    gui->addWidgetRight(new ofxUIImage(0, 0, 160, 120, &videoGui, "camara"));

    
    gui->addSpacer();
    gui->add2DPad("sup_izda", ofPoint(0,640), ofPoint(0,480), &areaSupIzda,160,120);
    gui->addWidgetRight(new ofxUI2DPad("inf_dcha", ofPoint(0,640), ofPoint(0,480), &areaInfDcha, 160, 120));
    
    gui->addSpacer();
    gui->addLabel("movimiento");
    gui->addValuePlotter("movimiento", 340, mapOutMin, mapOutMax, &nChange, 30);
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);

    gui->setVisible(false);
    
    gui->loadSettings("GUI/guiSettings.xml");
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    
    if(name == "sup_izda"){
        
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        area.init(pad->getScaledValue().x, pad->getScaledValue().y, areaInfDcha.x , areaInfDcha.y);
    }else if (name == "inf_dcha"){
        
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        area.init(areaSupIzda.x, areaSupIzda.y,pad->getScaledValue().x, pad->getScaledValue().y);
        
    }
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void testApp::exit()
{
    video.close();
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}
