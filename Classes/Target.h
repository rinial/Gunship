#ifndef __TARGET_H__
#define __TARGET_H__

#include "GameObject.h"
#include "cocos2d.h"

// A general class for targets
// It lacks functionality at this stage of the project but may be needed more later
class Target : public GameObject
{

public:
	// Constructor
	explicit Target(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, const float& mass = 1, const float& bounciness = 1);
	// Important for cleaning memory using base class pointer
	virtual ~Target();

	// We don't override onOverlap
	// The idea is to let Projectile kill target, not to let them both decide what happens

	// Called on being hit (overlaped) by a Projectile
	virtual void onBeingHit(class Projectile* projectile, const cocos2d::Vec2& toProjectile);
};

#endif // __TARGET_H__
