#include "LaserBall.h"
#include "Physics/Physics.h"
#include "Target.h"
#include "Definitions.h"

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

// Set the color of laser ball
void LaserBall::setColor(const Color3B& color)
{
	laserBall_->setColor(color);
}

// Constructors
LaserBall::LaserBall(const Vec2& pos, const Vec2& speed) : LaserBall(pos, std::make_unique<PhysMovement>(speed)) {}
LaserBall::LaserBall(const Vec2& pos, std::unique_ptr<PhysMovement> movement) : Projectile(pos, LASER_BALL_MASS, LASER_BALL_BOUNCINESS)
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

// Check lifetime and destroy if it's too long
void LaserBall::step(const float dT)
{
	Projectile::step(dT);

	if (getLifeTime() > LASER_BALL_LIFE_TIME)
		// destroy();
		setActive(false);
	else
		laserBall_->setOpacity(std::pow((LASER_BALL_LIFE_TIME - getLifeTime()) / LASER_BALL_LIFE_TIME, 0.3) * LASER_BALL_START_OPACITY);
}

// Called on hitting (overlapping) a Target
void LaserBall::onHitTarget(Target* target, const Vec2& toTarget)
{
	// Play sound
	SimpleAudioEngine::getInstance()->playEffect(LASER_HIT_SOUND_EFFECT);

	target->onBeingHit(this, -toTarget);
	setActive(false); // event will be send and gunship will handle it to pool the laserball
	//destroy();
}
