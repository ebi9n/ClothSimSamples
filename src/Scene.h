#pragma once
#include <functional>
#include "ofxGui.h"
#include "2DSwingObject.h"

// Scene.h
// 各種サンプルシーンの基底となるクラス.

enum class E_SAMPLE_SCENE
{
	E_SAMPLE_SCENE_PBD_SIMPLE = 0,		// 一番シンプルなサンプル.
	E_SAMPLE_SCRNE_NUM,					// サンプルの総数.
};

class CScene
{
public:
	virtual ~CScene() {};	// 仮想デストラクタ.
	virtual void SetUp() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void KeyPressed(int key) {};
	ofxPanel& GetGUI() { return m_GUI; }
protected:
	ofParameter<bool> m_bShowGui;
	ofxPanel m_GUI;
};

// 起動時のシーン.
class CMenuScene : public CScene
{
public:
	void Draw() override;
	void KeyPressed(int key) override;
	std::function<void(E_SAMPLE_SCENE)> OnSelectSample;

};

class CPBDSimpleScene : public CScene
{
public:
	virtual void Update() override;
	virtual void SetUp() override;
	virtual void Draw() override;
private:
	void InitSample2DSwingObject();
private:
	struct S_PBD_SIMLE_SCENE_PARAM {
		ofParameter<float> fFramePerSecond;
		ofParameter<float> fDampingCoef;
		ofParameter<int> iterations;
		ofParameter<float> fSpringConstant;
		ofParameter<unsigned int> nIterNum;
	};

	std::shared_ptr<CPBD2DSwingObject> m_pPBDSwingObject{nullptr};
	S_PBD_SIMLE_SCENE_PARAM m_sParam;
	ofVec2f m_vOrigin{ofVec2f(0)};
};