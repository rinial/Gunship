#include "PhysLeftRightMovement.h"

USING_NS_CC;

// Evaluates body movement over a period of time
void PhysLeftRightMovement::move(const float dT)
{
	// First move in line a bit
	PhysMovement::move(dT);

	// rotate speed
	speed_ = speed_.rotateByAngle(Vec2::ZERO, nextAngleFunction_(dT, curveK_, angularSpeed_));

	// change K
	if (goingDown_)
	{
		curveK_ -= (dT / curveTime_) * 4;
		if (curveK_ <= -1)
		{
			curveK_ = -1;
			goingDown_ = false;
		}
	}
	else
	{
		curveK_ += (dT / curveTime_) * 4;
		if (curveK_ >= 1)
		{
			curveK_ = 1;
			goingDown_ = true;
		}
	}
}

// Constructors
PhysLeftRightMovement::PhysLeftRightMovement(const cocos2d::Vec2& speed, const float& angularSpeed, const float& curveTime, const float& curveK, const bool& goingDown) : PhysLeftRightMovement(speed, angularSpeed, curveTime,
// Default angle changing function
[](float dT, float curveK, float angularSpeed)
{
	if (curveK > 0)
		return angularSpeed * dT;
	if (curveK < 0)
		return -angularSpeed * dT;
}, 
curveK, goingDown) { }

PhysLeftRightMovement::PhysLeftRightMovement(const cocos2d::Vec2& speed, const float& angularSpeed, const float& curveTime, const std::function<float(float, float, float)>& nextAngleFunction, const float& curveK, const bool& goingDown) : PhysMovement(speed)
{
	if (curveTime <= 0)
		throw std::invalid_argument("curveTime should be > 0");
	if (curveK < -1 || curveK > 1)
		throw std::invalid_argument("curveK should be in [-1, 1]");

	angularSpeed_ = angularSpeed;
	curveTime_ = curveTime;
	curveK_ = curveK;
	nextAngleFunction_ = nextAngleFunction;
	goingDown_ = goingDown;
}
