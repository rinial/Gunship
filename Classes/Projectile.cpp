#include "Projectile.h"
#include "Physics/Physics.h"
#include "Target.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// Constructor
Projectile::Projectile(const cocos2d::Vec2& pos, const float& mass, const float& bounciness) : GameObject(pos, mass, bounciness) { }
// Important for cleaning memory using base class pointer
Projectile::~Projectile() = default;

// Called on hits
void Projectile::onHit(const PhysContact& contact)
{
	GameObject::onHit(contact);

	// TODO move to LaserBall
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_BOUNCE_SOUND_EFFECT);
}
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
	// TODO move to LaserBall
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_HIT_SOUND_EFFECT);

	target->onBeingHit(this, -toTarget);

	// Projectile is destroyed
	// TODO pool instead
	destroy();
}
