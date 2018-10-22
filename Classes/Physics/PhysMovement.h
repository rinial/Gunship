#ifndef __PHYS_MOVEMENT_H__
#define __PHYS_MOVEMENT_H__

#include "cocos2d.h" // Just for basic things like Vec2

// Forward declarations
class PhysBody;
class PhysContact;

// Basic class that represents some form of movement of PhysBody
// Every movement has speed. Children specify how it changes
class PhysMovement
{
public:
	// Set the body that will be changed by this movement_
	// Should only be called from PhysBody directly when adding movement_
	void setBody(PhysBody* body);
	// Return body
	PhysBody* getBody() const { return body_; }

	// Get speed
	// We don't have a public setter, cause movement is what controls speed
	const cocos2d::Vec2& getSpeed() const { return speed_; }

	// Called from PhysBody on hits as it can affect movement
	virtual void onHit(const PhysContact& contact);

	// Evaluates body movement over a period of time
	virtual void move(float dT);

	// Stops the body. It may still move later
	virtual void stop() { newSpeed_ = cocos2d::Vec2::ZERO; }

	// Constructor
	explicit PhysMovement(const cocos2d::Vec2& speed = cocos2d::Vec2::ZERO) : newSpeed_(speed) {}

	// Important for cleaning memory using base class pointer
	virtual ~PhysMovement() = default;

protected:
	// New speed that should be changed when speed needs to change
	cocos2d::Vec2 newSpeed_;
private:
	// Actual speed of the PhysBody, changed before move()
	cocos2d::Vec2 speed_;

	// Only set directly from PhysBody upon adding new movement_
	PhysBody* body_ = nullptr;
};

#endif // __PHYS_MOVEMENT_H__
