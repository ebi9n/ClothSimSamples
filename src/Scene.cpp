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
	ofDrawBitmapString("Test Test",100,100);
}

void CMenuScene::KeyPressed(int key)
{
	if (key == '1' && OnSelectSample) {
		OnSelectSample(E_SAMPLE_SCENE::E_SAMPLE_SCENE_PBD_SIMPLE);
	}
}

void CPBDSimpleScene::Draw()
{
	// 適当に背景色を指定.
	const int nBackGroundRGBArray[3]
	{
		255,255,255
	};

	ofBackground(nBackGroundRGBArray[0], nBackGroundRGBArray[1], nBackGroundRGBArray[2]);
}