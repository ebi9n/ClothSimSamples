#include "2DSwingObject.h"

// コンストラクタ.
S_DISTANCE_CONSTRAINT::S_DISTANCE_CONSTRAINT(S_SWING_CONTROL_POINT* _pCtrl1, S_SWING_CONTROL_POINT* _pCtrl2, float _fConstraitLength, float _fSpringConstraint)
	: pCtrl1(_pCtrl1)
	, pCtrl2(_pCtrl2)
	, fConstraitLength(_fConstraitLength)
	, fSpringConstraint(_fSpringConstraint)
{ }

S_SWING_PARENT_INFO::S_SWING_PARENT_INFO()
	: vNowWorldPos(ofVec2f(0.f, 0.f))
	, vPrevWorldPos(ofVec2f(0.f, 0.f))
{}

float CPBD2DSwingObject::s_fGravity = 98.f;
float CPBD2DSwingObject::s_fDistanceUnit = 15.f;
float CPBD2DSwingObject::s_fMass = 1.f;
float CPBD2DSwingObject::s_fSpringConstraint = 0.5f;
unsigned int CPBD2DSwingObject::s_nIterNum = 10;
// コンストラクタ.
CPBD2DSwingObject::CPBD2DSwingObject()
	: m_sDistConstraintVector()
	, m_sCtrlPointVector()
	, m_pParentInfo()
	, m_fDampingCoef(0.f)
	, m_nCtrlPointNum(0)
	, m_nDistanceConstraintNum(0)
{
}

// 初期化.
CPBD2DSwingObject* CPBD2DSwingObject::CreateTestPBD2DSwingObject(E_SAMPLE_PATTERN eSamplePattern)
{
	CPBD2DSwingObject* pSwingObj = new CPBD2DSwingObject;
	switch (eSamplePattern)
	{
	case E_SAMPLE_PATTERN::E_SAMPLE_PATTERN_SIMPLE:
		pSwingObj->InitSimpleSample();
		break;
	default:
		break;
	}
	return pSwingObj;
}

float CPBD2DSwingObject::Vector2DCrossProduct(const ofVec2f& vRhs, const ofVec2f& vLhs)
{
	return vRhs.x * vLhs.y - vRhs.y * vRhs.x;
}
// 制御点を取得.
const S_SWING_CONTROL_POINT& CPBD2DSwingObject::GetCtrlPoint(const unsigned int nIndex)
{
	return m_sCtrlPointVector[nIndex];
}

unsigned int CPBD2DSwingObject::GetCtrlPointNum() const
{
	return m_nCtrlPointNum;
}

const std::vector<S_SWING_CONTROL_POINT>& CPBD2DSwingObject::GetCtrlPointVector()
{
	return m_sCtrlPointVector;
}

void CPBD2DSwingObject::SetParentNowPos(const ofVec2f& vWorldPos)
{
	m_pParentInfo->vNowWorldPos = vWorldPos;
}
void CPBD2DSwingObject::SetParentPrevPos(const ofVec2f& vWorldPos)
{
	m_pParentInfo->vPrevWorldPos = vWorldPos;
}

void CPBD2DSwingObject::SetDampingCoef(const float fDampingCoef)
{
	m_fDampingCoef = fDampingCoef;
}
// 初期化.(正直Createを返すのでいらない...)
void CPBD2DSwingObject::Init()
{
}

void CPBD2DSwingObject::Update(const float fDt, const ofVec2f& rParentMoveVector)
{
	
	// 更新の前準備.
	{
		PrepareForUpdate(fDt);
	}
	// 外力計算.
	{
		UpdateVelocity(fDt);
	}

	// 速度ダンピング.
	VelocityDamping();
	// 位置の更新.
	{
		UpdatePosition(fDt);
	}


	// 拘束条件の適用.
	for (auto i = 0; i < s_nIterNum; ++i)
	{
		ApplyDistanceConstraint();
	}
	// 拘束条件の結果の適用.
	{
		ApplyCalc(fDt);
	}

	PostUpdate();
}

void CPBD2DSwingObject::Term()
{
}

// 最もシンプルなサンプル.
void CPBD2DSwingObject::InitSimpleSample()
{
	const ofVec2f vZero = ofVec2f();
	// 制御点を斜めに並べる.
	for (unsigned int i = 0; i < CTRL_NUM; ++i) {
		S_SWING_CONTROL_POINT sCtrl;
		sCtrl.fMass = s_fMass;
		sCtrl.isRoot = (i == 0) ? true : false;
		const float fPos = s_fDistanceUnit * i;
		
		sCtrl.vLocalPosNow = ofVec2f(fPos, fPos);
		sCtrl.vCalcPos = sCtrl.vLocalPosNow;
		sCtrl.vLocalPosPrev = sCtrl.vLocalPosNow;

		sCtrl.vWorldPosNow = vZero;
		sCtrl.vWorldPosPrev = vZero;
		sCtrl.vVelocity = vZero;

		m_sCtrlPointVector.push_back(sCtrl);
	}
	
	// 距離拘束値をセット.
	const unsigned int nDistConstraintNum = CTRL_NUM - 1;
	for (unsigned int i = 0; i < nDistConstraintNum; ++i) {
		// 距離しきい値を計算.
		S_SWING_CONTROL_POINT& rCtrl1 = m_sCtrlPointVector[i];
		S_SWING_CONTROL_POINT& rCtrl2 = m_sCtrlPointVector[i+1];

		const ofVec2f vDiff = rCtrl1.vLocalPosNow - rCtrl2.vLocalPosNow;
		const float fDistance = vDiff.length();

		S_DISTANCE_CONSTRAINT sDistConstraint(&rCtrl1, &rCtrl2, fDistance, s_fSpringConstraint);
		m_sDistConstraintVector.push_back(sDistConstraint);
	}
	
	m_nCtrlPointNum = CTRL_NUM;
	m_nDistanceConstraintNum = nDistConstraintNum;
	m_pParentInfo = std::shared_ptr<S_SWING_PARENT_INFO>(new S_SWING_PARENT_INFO);
	
}


// Rootだけ現在位置でLocal To World.
void CPBD2DSwingObject::PrepareForUpdate(float fDt)
{
	const ofVec2f& rPrevWorldPos = m_pParentInfo->vPrevWorldPos;
	const ofVec2f& rNowWorldPos = m_pParentInfo->vNowWorldPos;
	for (auto& rCtrl : m_sCtrlPointVector) {
		
		rCtrl.vLocalPosPrev = rCtrl.vLocalPosNow;

		// Rootだけ現在位置に更新.
		if (rCtrl.isRoot) {
			rCtrl.vWorldPosNow = rCtrl.vLocalPosPrev + rNowWorldPos;
		}
		// 他は1F前の位置にとどまらせる
		else {
			rCtrl.vWorldPosNow = rCtrl.vLocalPosPrev + rPrevWorldPos;
		}

		rCtrl.vWorldPosPrev = rCtrl.vLocalPosPrev + rPrevWorldPos;
		rCtrl.vCalcPos = rCtrl.vWorldPosNow;
	}
}

// World To Local.
void CPBD2DSwingObject::PostUpdate()
{
	const ofVec2f& rPrevWorldPos = m_pParentInfo->vPrevWorldPos;
	const ofVec2f& rNowWorldPos = m_pParentInfo->vNowWorldPos;
	for (auto& rCtrl : m_sCtrlPointVector) {
		rCtrl.vLocalPosNow  = rCtrl.vWorldPosNow - rNowWorldPos;
		rCtrl.vLocalPosPrev = rCtrl.vWorldPosPrev - rPrevWorldPos;
	}
}

// 外力の影響による速度を計算.
void CPBD2DSwingObject::UpdateVelocity(float fDt)
{
	for (auto& rCtrl : m_sCtrlPointVector) {
		if (rCtrl.isRoot) {
			rCtrl.vVelocity = ofVec2f(0.f);
		}
		rCtrl.vVelocity.y += s_fGravity * fDt;
	}
}

// 位置の更新.
void CPBD2DSwingObject::UpdatePosition(float fDt)
{
	for (auto& rCtrl : m_sCtrlPointVector) {
		if (rCtrl.isRoot) {
			rCtrl.vCalcPos = rCtrl.vWorldPosNow;
			continue;
		}
		rCtrl.vCalcPos = rCtrl.vWorldPosNow + (rCtrl.vVelocity * fDt);
	}
}

// 拘束条件の適用.
void CPBD2DSwingObject::ApplyDistanceConstraint()
{
	for (auto& rContraint : m_sDistConstraintVector) {
		const float fCtrl1MassInv = 1.f/ rContraint.pCtrl1->fMass;
		const float fCtrl2MassInv = 1.f/ rContraint.pCtrl2->fMass;
		
		const ofVec2f vDiff = rContraint.pCtrl1->vCalcPos - rContraint.pCtrl2->vCalcPos;
		const float fDiff = vDiff.length();
		const float fCommonCoef = s_fSpringConstraint * (fDiff - rContraint.fConstraitLength) / (fCtrl1MassInv + fCtrl2MassInv) / fDiff;
		
		const ofVec2f vChangeCalcPos1 = -fCommonCoef * fCtrl1MassInv * vDiff;
		const ofVec2f vChangeCalcPos2 =  fCommonCoef * fCtrl2MassInv * vDiff;

		rContraint.pCtrl1->vCalcPos += rContraint.pCtrl1->isRoot ? ofVec2f(0.f, 0.f) : vChangeCalcPos1;
		rContraint.pCtrl2->vCalcPos += rContraint.pCtrl2->isRoot ? ofVec2f(0.f, 0.f) : vChangeCalcPos2;
	}
}

void CPBD2DSwingObject::ApplyCalc(float fDt)
{
	for (auto& rCtrl : m_sCtrlPointVector) {
		if (rCtrl.isRoot) {
			continue;
		}
		rCtrl.vVelocity = (rCtrl.vCalcPos - rCtrl.vWorldPosNow) / fDt;
		rCtrl.vWorldPosNow = rCtrl.vCalcPos;
	}
}

// 
void CPBD2DSwingObject::VelocityDamping()
{
	// 重心位置と, 重心速度を計算.
	ofVec2f vPosCenter = ofVec2f(0.f);
	ofVec2f vVelCenter = ofVec2f(0.f);
	float fTotalMass = 0;
	for (auto& rCtrl : m_sCtrlPointVector) {
		vPosCenter += rCtrl.fMass * rCtrl.vWorldPosNow;
		vVelCenter += rCtrl.fMass * rCtrl.vVelocity;
		fTotalMass += rCtrl.fMass;
	}
	vPosCenter /= fTotalMass;
	vVelCenter /= fTotalMass;

	// 角速度を計算.
	float fMomentOfInertia = 0;
	float fAngularMomentum = 0;
	// todo. 相対位置ベクトルは計算する.
	for (auto& rCtrl : m_sCtrlPointVector) {
		ofVec2f vRelativePos = rCtrl.vWorldPosNow - vPosCenter;
		fAngularMomentum += rCtrl.fMass * Vector2DCrossProduct(vRelativePos, rCtrl.vVelocity);
		fMomentOfInertia += rCtrl.fMass * vRelativePos.lengthSquared();
	}
	const float fAngularVelocity = fAngularMomentum / fMomentOfInertia;

	// ダンピングを行う.
	const float kDamping = m_fDampingCoef;
	for (auto& rCtrl : m_sCtrlPointVector) {

		ofVec2f& rVelocity = rCtrl.vVelocity;
		ofVec2f vRelativePos = rCtrl.vWorldPosNow - vPosCenter;
		ofVec2f vDiff = -rVelocity + vVelCenter + ofVec2f(fAngularVelocity * vRelativePos.y, -fAngularVelocity * vRelativePos.x);
		rVelocity += kDamping * vDiff;
	}
}
void CPBD2DSwingObject::InitControlPoints()
{

}