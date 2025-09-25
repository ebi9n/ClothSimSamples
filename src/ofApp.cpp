#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(60);
    m_pMenuScene = std::make_shared<CMenuScene>();

    // コールバックを登録.
    m_pMenuScene->OnSelectSample = [this](E_SAMPLE_SCENE eSampleScene) {
        if (eSampleScene == E_SAMPLE_SCENE::E_SAMPLE_SCENE_PBD_SIMPLE) {
            m_pCurrentScene = m_pPBDSimpleScene;
            
        }

        m_pCurrentScene->SetUp();
    };

    m_pPBDSimpleScene = std::make_shared<CPBDSimpleScene>();
    m_pCurrentScene = m_pMenuScene;
}

void ofApp::update() {
    
    m_pCurrentScene->Update();

    //m_pPBDSwingObject->SetDampingCoef(m_fDampingCoef);
    //CPBD2DSwingObject::s_fSpringConstraint = m_fSpringConstant;
    //CPBD2DSwingObject::s_nIterNum = m_nIterNum;
    //// 固定点の更新.
    //{
    //    m_pPBDSwingObject->SetParentPrevPos(m_vOrigin);
    //    
    //    static float count = 0;
    //    count += 2.f / fFramePerSecond;
    //    m_vOrigin.x = 500.f + 50.f * sinf(count);
    //    m_vOrigin.y = 100.f + 50.f * cosf(count);

    //    m_pPBDSwingObject->SetParentNowPos(m_vOrigin);

    //}

    //// Swingオブジェクトの更新.
    //m_pPBDSwingObject->Update(1.f/ fFramePerSecond, ofVec2f(0.f, 0.f));
}


void ofApp::draw() {
    m_pCurrentScene->Draw();
    //ofBackground(255);
    //ofSetColor(0);

    //if (showGui) gui.draw();

    //const std::vector<S_SWING_CONTROL_POINT>& rCtrlPointVector = m_pPBDSwingObject->GetCtrlPointVector();
    //for (auto& ctrl : rCtrlPointVector) {
    //    ofVec2f vDrawPos = m_vOrigin + ctrl.vLocalPosNow;
    //    ofDrawCircle(vDrawPos, 8.f);
    //}

}

void ofApp::keyPressed(int key)
{
    m_pCurrentScene->KeyPressed(key);
}


void ofApp::InitSample2DSwingObject()
{
    //m_pPBDSwingObject = nullptr;

    //// オブジェクトを生成しスマートポインタにセット.
    //m_pPBDSwingObject = std::shared_ptr<CPBD2DSwingObject>(CPBD2DSwingObject::CreateTestPBD2DSwingObject(eSampleType));

}