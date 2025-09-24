#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "Scene.h"
#include "2DSwingObject.h"
#include <memory>

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key) override;

    // GUI
    ofxPanel gui;
    ofParameter<float> fFramePerSecond;
    ofParameter<float> m_fDampingCoef;
    ofParameter<int> iterations;
    ofParameter<bool> showGui;
    ofParameter<E_SAMPLE_PATTERN> m_eSampleType;
    ofParameter<float> m_fSpringConstant;
    ofParameter<unsigned int> m_nIterNum;

    void InitSample2DSwingObject();

private: 
    std::shared_ptr<CScene> m_pCurrentScene;
    std::shared_ptr<CMenuScene> m_pMenuScene;
    std::shared_ptr<CPBDSimpleScene> m_pPBDSimpleScene;
    std::shared_ptr<CPBD2DSwingObject> m_pPBDSwingObject;
    ofVec2f m_vOrigin;


};
