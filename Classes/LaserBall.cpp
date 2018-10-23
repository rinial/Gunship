#include "LaserBall.h"
#include "Physics/Physics.h"
#include "Target.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// Constructors
LaserBall::LaserBall(const cocos2d::Vec2& pos, const cocos2d::Vec2& speed) : LaserBall(pos, std::make_unique<PhysMovement>(speed)) {}
LaserBall::LaserBall(const cocos2d::Vec2& pos, std::unique_ptr<PhysMovement> movement) : Projectile(pos, LASER_BALL_MASS, LASER_BALL_BOUNCINESS)
{
	laserBall_ = Sprite::create();
	laserBall_->initWithFile(LASER_BALL_SPRITE);
	laserBall_->setPosition(0, 0);

	rootNode_->addChild(laserBall_);

	addCollider(std::make_unique<PhysCircleCollider>(laserBall_->getContentSize().width / 2, LASER_BALL_BITMASKS));
	setMovement(std::move(movement));
}
// Important for cleaning memory using base class pointer
LaserBall::~LaserBall() = default;

// Called on hits
void LaserBall::onHit(const PhysContact& contact)
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_BOUNCE_SOUND_EFFECT);

	Projectile::onHit(contact);
}

// Called on hitting (overlapping) a Target
void LaserBall::onHitTarget(Target* target, const Vec2& toTarget)
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_HIT_SOUND_EFFECT);

	// TODO pool somehow?
	Projectile::onHitTarget(target, toTarget);
}
