#ifndef __PHYS_LEFT_RIGHT_MOVEMENT_H__
#define __PHYS_LEFT_RIGHT_MOVEMENT_H__

#include "PhysMovement.h"
#include <functional>

// Forward declarations
class PhysBody;

// Represents movement with constant speed magnitude but changing direction
class PhysLeftRightMovement : PhysMovement
{
public:
	// Evaluates body movement over a period of time
	virtual void move(float dT) override;

	// Constructors
	PhysLeftRightMovement(const cocos2d::Vec2& speed, const float& angularSpeed, const float& curveTime, const float& curveK = 1, const bool& goingDown = true);
	PhysLeftRightMovement(const cocos2d::Vec2& speed, const float& angularSpeed, const float& curveTime, const std::function<float(float, float, float)>& nextAngleFunction, const float& curveK = 1, const bool& goingDown = true);

	// Important for cleaning memory using base class pointer
	virtual ~PhysLeftRightMovement() = default;

protected:
	// Speed of changing angles
	float angularSpeed_ = 0;

	// Coefficient that tells which part of the curve the body is on
	float curveK_ = 1;

	// The time it takes to finish the curve cycle
	float curveTime_;

	// Function that changes anglular acceleration based on deltaTime, curveK_ and angularSpeed_
	// newDeltaAngle nextAngleFunction(dT, curveK, angularSpeed);
	std::function<float(float, float, float)> nextAngleFunction_;

	// True if curveK_ is going down
	bool goingDown_ = true;
};

#endif // __PHYS_LEFT_RIGHT_MOVEMENT_H__
