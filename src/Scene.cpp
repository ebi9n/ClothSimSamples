#include "Scene.h"
#include "ofMain.h"

/// <summary>
/// 選択するサンプルのメニュー.
/// </summary>
void CMenuScene::Draw()
{
	// 適当に背景色を指定.
	const int nBackGroundRGBArray[3]
	{
		128,128,128
	};

	ofBackground(nBackGroundRGBArray[0], nBackGroundRGBArray[1], nBackGroundRGBArray[2]);
	ofDrawBitmapString("Press 1 Access To Most Simple Sample.",100,100);
}

void CMenuScene::KeyPressed(int key)
{
	if (key == '1' && OnSelectSample) {
		OnSelectSample(E_SAMPLE_SCENE::E_SAMPLE_SCENE_PBD_SIMPLE);
	}
}

// 初期化.
void CPBDSimpleScene::SetUp()
{
	// モジュールの初期化.
	{
		InitSample2DSwingObject();
		m_vOrigin.set(ofVec2f(500.f, 150.f));
	}

	// GUIの初期化.
	{
		m_bShowGui = true;
		S_PBD_SIMLE_SCENE_PARAM& rParam = m_sParam;
		m_GUI.setup("PBD Settings");
		m_GUI.add(rParam.fFramePerSecond.set("FramePerSecond", 60.f, 12.f, 120.f));
		m_GUI.add(rParam.iterations.set("Iterations", 10, 1, 20));
		m_GUI.add(rParam.fSpringConstant.set("Spring Const.", 0.5f, 0.f, 1.5f));
		m_GUI.add(rParam.nIterNum.set("PBD Iter Num.", 10, 1, 100));
		m_GUI.add(rParam.fDampingCoef.set("Damping Coef.", 0.001f, 0.0f, 1.f));
	}
}

void CPBDSimpleScene::Update()
{
	// パラメータ更新.
	S_PBD_SIMLE_SCENE_PARAM& rParam = m_sParam;
	{
		m_pPBDSwingObject->SetDampingCoef(rParam.fDampingCoef);
		CPBD2DSwingObject::s_fSpringConstraint = rParam.fSpringConstant;
		CPBD2DSwingObject::s_nIterNum = rParam.nIterNum;
	}

	const float fFramePerSec = rParam.fFramePerSecond;
	// 固定点の更新.
	{
		m_pPBDSwingObject->SetParentPrevPos(m_vOrigin);
		    
		static float count = 0;
		count += 2.f / fFramePerSec;
		m_vOrigin.x = 500.f + 50.f * sinf(count);
		m_vOrigin.y = 100.f + 50.f * cosf(count);

		m_pPBDSwingObject->SetParentNowPos(m_vOrigin);

	}

	// Swingオブジェクトの更新.
	m_pPBDSwingObject->Update(1.f/ fFramePerSec, ofVec2f(0.f, 0.f));
}

void CPBDSimpleScene::Draw()
{
	// 適当に背景色を指定.
	const int nBackGroundRGBArray[3]
	{
		255,255,255
	};

	ofBackground(nBackGroundRGBArray[0], nBackGroundRGBArray[1], nBackGroundRGBArray[2]);
	ofSetColor(0);
	
	if (m_bShowGui) {
		m_GUI.draw();
	}
	
	const std::vector<S_SWING_CONTROL_POINT>& rCtrlPointVector = m_pPBDSwingObject->GetCtrlPointVector();
	for (auto& ctrl : rCtrlPointVector) {
		ofVec2f vDrawPos = m_vOrigin + ctrl.vLocalPosNow;
		ofDrawCircle(vDrawPos, 8.f);
	}

}

void CPBDSimpleScene::InitSample2DSwingObject()
{
	// 本来ならシーン遷移を管理すべきだから, ここで破棄すべきだよな.
	m_pPBDSwingObject = nullptr;

	// オブジェクトを生成し, スマートptrにセット,
	m_pPBDSwingObject = std::shared_ptr<CPBD2DSwingObject>(CPBD2DSwingObject::CreateTestPBD2DSwingObject(E_SAMPLE_PATTERN::E_SAMPLE_PATTERN_SIMPLE));

}