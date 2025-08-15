#pragma once
#include <functional>
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
	void Draw() override;
};