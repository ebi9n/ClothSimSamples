#pragma once
#include "ofMain.h"
#include <vector>
// 紐オブジェクトのインターフェイス.
class ISwingObject
{
public:
	virtual void Init() = 0;
	virtual void Update(float fDt, const ofVec2f& rParentVelocity) = 0;
	virtual void Term() = 0;
};

// 一つ一つの制御点を表す構造体.
struct S_SWING_CONTROL_POINT
{
	ofVec2f	vLocalPosNow;			// 現在位置.
	ofVec2f vLocalPosPrev;			// 過去の位置.
	ofVec2f vWorldPosNow;			// 現在のW座標系での位置.
	ofVec2f vWorldPosPrev;			// 過去のW座標系での位置.
	ofVec2f vCalcPos;				// 計算用に使う位置.(W座標系)
	ofVec2f vVelocity;				// 速度.
	float	fMass;					// 質量.
	bool	isRoot;					// この制御点が固定点かどうか.
};

struct S_SWING_PARENT_INFO
{
	ofVec2f	vNowWorldPos;			// 今Fでの親のWorld座標での位置.
	ofVec2f	vPrevWorldPos;			// 前F.
	S_SWING_PARENT_INFO();
};

// 距離拘束.
struct S_DISTANCE_CONSTRAINT
{
	S_SWING_CONTROL_POINT* pCtrl1;		// 制御点1.
	S_SWING_CONTROL_POINT* pCtrl2;		// 制御点2.
	float fConstraitLength;				// 距離拘束の長さ.
	float fSpringConstraint;			// バネ定数.

	S_DISTANCE_CONSTRAINT(S_SWING_CONTROL_POINT* _pCtrl1, S_SWING_CONTROL_POINT* _pCtrl2, float _fConstraitLength, float _fSpringConstraint);
};

enum class E_SAMPLE_PATTERN : unsigned int
{
	E_SAMPLE_PATTERN_SIMPLE = 0,	// 最もシンプルなパターン.
};



// 2Dの紐オブジェクトのクラス.
class CPBD2DSwingObject : public ISwingObject
{
public:
	CPBD2DSwingObject();
	static CPBD2DSwingObject* CreateTestPBD2DSwingObject(E_SAMPLE_PATTERN eSamplePattern);
	static float Vector2DCrossProduct(const ofVec2f& vRhs, const ofVec2f& vLhs);

	virtual void Init() override;
	virtual void Update(float fDt, const ofVec2f& rParentMoveVector) override;
	virtual void Term() override;
	const S_SWING_CONTROL_POINT& GetCtrlPoint(const unsigned int nIndex);
	const std::vector<S_SWING_CONTROL_POINT>& GetCtrlPointVector();
	unsigned int GetCtrlPointNum() const;
	void SetParentNowPos(const ofVec2f& vWorldPos);
	void SetParentPrevPos(const ofVec2f& pWorldPos);
	void SetDampingCoef(const float fDampingCoef);
	static float s_fGravity;
	static float s_fDistanceUnit;			// 適当に距離の単位.
	static float s_fMass;					// 適当に距離の単位.
	static float s_fSpringConstraint;					// 適当に距離の単位.
	static unsigned int s_nIterNum;
private:
	void InitSimpleSample();
	void InitControlPoints();
	// Update関連.
		void PrepareForUpdate(float fDt);
		void UpdateVelocity(float fDt);
		void UpdatePosition(float fDt);
		void ApplyDistanceConstraint();
		void ApplyCalc(float fDt);
		void VelocityDamping();
		void PostUpdate();

private:
	std::vector<S_DISTANCE_CONSTRAINT> m_sDistConstraintVector;
	std::vector<S_SWING_CONTROL_POINT> m_sCtrlPointVector;
	std::shared_ptr<S_SWING_PARENT_INFO> m_pParentInfo;
	float m_fDampingCoef;
	unsigned int m_nCtrlPointNum;
	unsigned int m_nDistanceConstraintNum;
	
	static const unsigned int CTRL_NUM = 15;	// これはあとから変更できるようにしたいね….

};
