#ifndef __PHYS_COLLIDER_H__
#define __PHYS_COLLIDER_H__

#include "cocos2d.h" // Just for basic classes like Vec2

// Collision component of a PhysBody
class PhysCollider
{
public:
	// Return position
	const cocos2d::Vec2& getPosition() const { return position_; }

	// Bitmasks
	uint16_t selfMask = 0;  // what it is
	uint16_t hitMask = 0;    // what it can hit (physical collision)
	uint16_t overlapMask = 0; // what it can overlap (just an event)

	// Important for cleaning memory using base class pointer
	virtual ~PhysCollider() = default; 

protected:
	// Constructor is not public so that noone creates PhysCollider directly (only child classes)
	explicit PhysCollider(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, 
		const uint16_t& selfMask = 0, const uint16_t& hitMask = 0, const uint16_t& overlapMask = 0) 
	: selfMask(selfMask), hitMask(hitMask), overlapMask(overlapMask), position_(pos) {}

private:
	// Local position of the collider in PhysBody space
	cocos2d::Vec2 position_;

	// We don't have rotation here for the sake of simplicity
	// It isn't important for the game
	// float rotation_;
};

#endif // __PHYS_COLLIDER_H__
