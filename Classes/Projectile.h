#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "GameObject.h"
#include "cocos2d.h"

// A general class for projectiles
// It lacks functionality at this stage of the project but may be needed more later
class Projectile : public GameObject
{
public:
	// Constructor
	explicit Projectile(const cocos2d::Vec2& pos = cocos2d::Vec2::ZERO, const float& mass = 1, const float& bounciness = 1);
	// Important for cleaning memory using base class pointer
	virtual ~Projectile();

	// Called on hits
	virtual void onHit(const PhysContact& contact) override;
	// Called on overlaps
	virtual void onOverlap(const PhysContact& contact) override;

protected:
	// Called on hitting (overlapping) a Target
	virtual void onHitTarget(class Target* target, const cocos2d::Vec2& toTarget);
};

#endif // __PROJECTILE_H__
