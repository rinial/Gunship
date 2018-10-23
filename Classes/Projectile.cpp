#include "Projectile.h"
#include "Physics/Physics.h"
#include "Target.h"

USING_NS_CC;

// Constructor
Projectile::Projectile(const cocos2d::Vec2& pos, const float& mass, const float& bounciness) : GameObject(pos, mass, bounciness) { }
// Important for cleaning memory using base class pointer
Projectile::~Projectile() = default;

// Called on overlaps
void Projectile::onOverlap(const PhysContact& contact)
{
	const auto other = contact.getOther(this);
	const auto target = dynamic_cast<Target*>(other);
	if (!target)
		return;

	// Overlaped a Target
	onHitTarget(target, contact.getDirectionFrom(this));
}

// Called on hitting (overlapping) a Target
void Projectile::onHitTarget(Target* target, const Vec2& toTarget)
{
	target->onBeingHit(this, -toTarget);

	// Projectile is destroyed
	destroy();
}
