#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);

    // サンプルパターンを0にセット.
    eSampleType.set(E_SAMPLE_PATTERN::E_SAMPLE_PATTERN_SIMPLE);
    InitSample2DSwingObject();

    // GUI初期化
    gui.setup("PBD Settings");
    gui.add(fFramePerSecond.set("FramePerSecond", 60.f, 12.f, 120.f));
    gui.add(iterations.set("Iterations", 10, 1, 20));
    gui.add(m_fSpringConstant.set("Spring Const.", 0.5f,0.f,1.5f));
    gui.add(m_nIterNum.set("PBD Iter Num.", 10, 1, 100));
    gui.add(m_fDampingCoef.set("Damping Coef.", 0.001f, 0.0f, 1.f));

    m_vOrigin.set(ofVec2f(500.f, 150.f));

}

void ofApp::update() {
    
    m_pPBDSwingObject->SetDampingCoef(m_fDampingCoef);
    CPBD2DSwingObject::s_fSpringConstraint = m_fSpringConstant;
    CPBD2DSwingObject::s_nIterNum = m_nIterNum;
    // 固定点の更新.
    {
        m_pPBDSwingObject->SetParentPrevPos(m_vOrigin);
        
        static float count = 0;
        count += 2.f / fFramePerSecond;
        m_vOrigin.x = 500.f + 50.f * sinf(count);
        m_vOrigin.y = 100.f + 50.f * cosf(count);

        m_pPBDSwingObject->SetParentNowPos(m_vOrigin);

    }

    // Swingオブジェクトの更新.
    m_pPBDSwingObject->Update(1.f/ fFramePerSecond, ofVec2f(0.f, 0.f));
}


void ofApp::draw() {
    ofBackground(255);
    ofSetColor(0);

    if (showGui) gui.draw();

    const std::vector<S_SWING_CONTROL_POINT>& rCtrlPointVector = m_pPBDSwingObject->GetCtrlPointVector();
    for (auto& ctrl : rCtrlPointVector) {
        ofVec2f vDrawPos = m_vOrigin + ctrl.vLocalPosNow;
        ofDrawCircle(vDrawPos, 8.f);
    }

}

void ofApp::InitSample2DSwingObject()
{
    m_pPBDSwingObject = nullptr;

    // オブジェクトを生成しスマートポインタにセット.
    m_pPBDSwingObject = std::shared_ptr<CPBD2DSwingObject>(CPBD2DSwingObject::CreateTestPBD2DSwingObject(eSampleType));

}